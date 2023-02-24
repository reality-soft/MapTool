#include "LevelEditor.h"
#include "PickingMgr.h"

void LevelEditor::Regenerate(UINT num_row, UINT num_col, int cell_distance, int uv_scale)
{
	level_mesh_.vertices.clear();
	level_mesh_.vertex_buffer.Get()->Release();
	level_mesh_.vertex_buffer.ReleaseAndGetAddressOf();

	level_mesh_.indices.clear();
	level_mesh_.index_buffer.Get()->Release();
	level_mesh_.index_buffer.ReleaseAndGetAddressOf();

	so_buffer_.Get()->Release();
	so_buffer_.ReleaseAndGetAddressOf();

	hit_circle_.buffer.Get()->Release();
	hit_circle_.buffer.ReleaseAndGetAddressOf();

	edit_option_.buffer.Get()->Release();
	edit_option_.buffer.ReleaseAndGetAddressOf();

	height_list_.clear();

	height_field_body_->removeCollider(height_field_collider_);
	PHYSICS->GetPhysicsWorld()->destroyCollisionBody(height_field_body_);

	height_field_shape_ = nullptr;
	height_field_collider_ = nullptr;
	height_field_body_ = nullptr;

	CreateLevel(num_row, num_row, cell_distance, uv_scale);
	CreateHeightField(-(int)num_row, num_row);
}

bool LevelEditor::ExportToFile(string filename)
{
	FileTransfer file_transfer(export_dir + filename, WRITE);

	// Single Datas;
	file_transfer.WriteBinary<UINT>(&num_row_vertex_, 1);
	file_transfer.WriteBinary<UINT>(&num_col_vertex_, 1);
	file_transfer.WriteBinary<int>(&cell_distance_, 1);
	file_transfer.WriteBinary<int>(&uv_scale_, 1);

	// Arrays
	file_transfer.WriteBinary<Vertex>(level_mesh_.vertices.data(), level_mesh_.vertices.size());
	file_transfer.WriteBinary<UINT>(level_mesh_.indices.data(), level_mesh_.indices.size());
	file_transfer.WriteBinary<string>(texture_id.data(), texture_id.size());

	return true;
}

bool LevelEditor::CreateEditSOStage()
{
	// SO Buffer
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA subdata;
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&subdata, sizeof(subdata));

	desc.ByteWidth = sizeof(StreamVertex) * level_mesh_.indices.size();
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_STREAM_OUTPUT;

	HRESULT hr = DX11APP->GetDevice()->CreateBuffer(&desc, nullptr, so_buffer_.GetAddressOf());
	if (FAILED(hr))
		return false;

	//  Hit Circle
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&subdata, sizeof(subdata));

	desc.ByteWidth = sizeof(CbHitCircle::Data);

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	subdata.pSysMem = &hit_circle_.data;

	hr = DX11APP->GetDevice()->CreateBuffer(&desc, &subdata, hit_circle_.buffer.GetAddressOf());
	if (FAILED(hr))
		return false;

	// ConstantBuffer : EditOption
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&subdata, sizeof(subdata));

	desc.ByteWidth = sizeof(CbEditOption::Data);

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	subdata.pSysMem = &edit_option_.data;

	hr = DX11APP->GetDevice()->CreateBuffer(&desc, &subdata, edit_option_.buffer.GetAddressOf());
	if (FAILED(hr))
		return false;
}

void LevelEditor::SetEditSOStage()
{
	// Set VS Cb : HitCircle
	DX11APP->GetDeviceContext()->VSSetConstantBuffers(1, 1, hit_circle_.buffer.GetAddressOf());

	// Set GS Cb : EditOption
	DX11APP->GetDeviceContext()->GSSetConstantBuffers(0, 1, edit_option_.buffer.GetAddressOf());

	// Set Shader : SO Gs
	GeometryShader* gs = RESOURCE->UseResource<GeometryShader>(gs_id_);
	if (gs != nullptr)
		DX11APP->GetDeviceContext()->GSSetShader(gs->Get(), 0, 0);

	// Stream Output Stage
	UINT so_offset = 0;
	DX11APP->GetDeviceContext()->SOSetTargets(1, so_buffer_.GetAddressOf(), &so_offset);	
}

void LevelEditor::GetEditSOStage()
{
	DX11APP->GetDeviceContext()->GSSetShader(NULL, NULL, 0);
	ID3D11Buffer* temp_buffer = nullptr;
	CreateTempBuffer(&temp_buffer);
	DX11APP->GetDeviceContext()->CopyResource(temp_buffer, so_buffer_.Get());

	D3D11_MAPPED_SUBRESOURCE so_mapped_res;
	DX11APP->GetDeviceContext()->Map(temp_buffer, 0, D3D11_MAP_READ, 0, &so_mapped_res);
	StreamVertex* vertices = reinterpret_cast<StreamVertex*>(so_mapped_res.pData);
	for (int i = 0; i < so_mapped_res.RowPitch / sizeof(StreamVertex); ++i)
	{
		if (i < level_mesh_.indices.size())
		{
			level_mesh_.vertices[level_mesh_.indices[i]].p = vertices[i].o;
			level_mesh_.vertices[level_mesh_.indices[i]].n = vertices[i].n;
		}
	}
	DX11APP->GetDeviceContext()->Unmap(temp_buffer, 0);
	temp_buffer->Release();
	temp_buffer = nullptr;

	DX11APP->GetDeviceContext()->UpdateSubresource(level_mesh_.vertex_buffer.Get(), 0, nullptr, level_mesh_.vertices.data(), 0, 0);
}

void LevelEditor::LevelEdit()
{
	if (PICKING->current_body == height_field_body_)
	{
		hit_circle_.data.hitpoint = PICKING->current_point;
		hit_circle_.data.is_hit = true;
		hit_circle_.data.circle_radius = sculpting_brush_;

		DX11APP->GetDeviceContext()->UpdateSubresource(hit_circle_.buffer.Get(), 0, nullptr, &hit_circle_.data, 0, 0);
	}

	if (DINPUT->GetMouseWheel())
	{
		edit_option_.data.altitude.x += DINPUT->GetMouseWheel() * 0.1;
		DX11APP->GetDeviceContext()->UpdateSubresource(edit_option_.buffer.Get(), 0, nullptr, &edit_option_.data, 0, 0);
	}

	if (DINPUT->GetMouseButton().x)
	{
		GetEditSOStage();
	}	
}

bool LevelEditor::CreateTempBuffer(ID3D11Buffer** _buffer)
{
	ID3D11Buffer* edit_buffer = nullptr;
	D3D11_BUFFER_DESC desc;
	so_buffer_.Get()->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	if (FAILED(DX11APP->GetDevice()->CreateBuffer(&desc, nullptr, _buffer)))
		return false;

	return true;
}

void LevelEditor::ResetHeightField()
{
	height_list_.clear();
	GetHeightList();

	height_field_body_->removeCollider(height_field_collider_);
	PHYSICS->GetPhysicsWorld()->destroyCollisionBody(height_field_body_);

	height_field_shape_ = nullptr;
	height_field_collider_ = nullptr;
	height_field_body_ = nullptr;

	float heightest = 0;
	float lowest = 0;
	for (int i = 0; i < height_list_.size(); ++i)
	{
		if (heightest < height_list_[i])
			heightest = height_list_[i];

		else if (lowest > height_list_[i])
			lowest = height_list_[i];
	}

	CreateHeightField(lowest, heightest);
}
