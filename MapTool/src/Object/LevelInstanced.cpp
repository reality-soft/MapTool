#include "LevelInstanced.h"
#include "ResourceMgr.h"

LevelInstanced::LevelInstanced()
{
}

LevelInstanced::~LevelInstanced()
{
}

void LevelInstanced::Init(string stmesh_id, string vs_id, string ps_id)
{
	mesh = shared_ptr<StaticMesh>(RESOURCE->UseResource<StaticMesh>(stmesh_id));
	vs = shared_ptr<VertexShader>(RESOURCE->UseResource<VertexShader>(vs_id));
	surface.shader_id = ps_id;
	
	CreateInstanceBuffer();
	DX11APP->GetDeviceContext()->UpdateSubresource(instancing.buffer.Get(), 0, 0, &instancing.data, 0, 0);
}

void LevelInstanced::Frame()
{
	for (auto world : instancing_map)
	{
		instancing.data.instance_transform[world.first] = XMMatrixTranspose(world.second);
	}
	DX11APP->GetDeviceContext()->UpdateSubresource(instancing.buffer.Get(), 0, 0, &instancing.data, 0, 0);
	DX11APP->GetDeviceContext()->GSSetShader(nullptr, 0, 0);
	DX11APP->GetDeviceContext()->VSSetConstantBuffers(1, 1, instancing.buffer.GetAddressOf());
	DX11APP->GetDeviceContext()->IASetInputLayout(vs.get()->InputLayoyt());
	DX11APP->GetDeviceContext()->VSSetShader(vs.get()->Get(), 0, 0);
}

void LevelInstanced::Render()
{
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	surface.SetSurface();

	UINT total_count = 0;
	for (auto mesh : mesh.get()->meshes)
	{
		total_count += mesh.indices.size();
	}
	for (auto mesh : mesh.get()->meshes)
	{
		DX11APP->GetDeviceContext()->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		DX11APP->GetDeviceContext()->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		DX11APP->GetDeviceContext()->DrawIndexedInstanced(mesh.indices.size(), instancing_map.size(), 0, 0, 0);
	}
}

void LevelInstanced::Release()
{
}

void LevelInstanced::SetTRS(XMVECTOR& t, XMVECTOR r, XMVECTOR s)
{
	XMMATRIX matrix = XMMatrixIdentity();
	matrix *= XMMatrixTranslationFromVector(t);
	matrix *= XMMatrixRotationRollPitchYawFromVector(r);
	matrix *= XMMatrixScalingFromVector(t);

	world_transform = matrix;
}

void LevelInstanced::CreateSurfaceMaterial(const vector<string>& texture_id)
{
	KGCA41B::Material material;
	material.texture_id = texture_id;

	surface.materials.push_back(material);

	surface.CreateSurface();
}

void LevelInstanced::CreateInstanceBuffer()
{
	HRESULT hr;

	// Create a constant buffer for the instance data
	D3D11_SUBRESOURCE_DATA subdata;
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&subdata, sizeof(subdata));

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(CbInstance::Data);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	subdata.pSysMem = &instancing.data;

	hr = DX11APP->GetDevice()->CreateBuffer(&desc, NULL, instancing.buffer.GetAddressOf());
}

void LevelInstanced::AddNewInstance(XMMATRIX& transform)
{
	if (instancing_map.size() > 128)
		return;
	instancing_map.insert(make_pair(instance_count++, transform));
}

void LevelInstanced::TransformationInstance(UINT index, XMVECTOR& _transform)
{
	XMMATRIX transform = XMMatrixIdentity();
	transform = transform * XMMatrixTranslationFromVector(_transform);
	instancing_map.find(index)->second = transform;
	XMVECTOR translate;
	XMVECTOR scale;
	XMVECTOR roatation;
	XMMatrixDecompose(&scale, &roatation, &translate, transform);

	ImGui::Text("Movement");
	ImGui::Text(to_string(translate.m128_f32[0]).c_str());
	ImGui::Text(to_string(translate.m128_f32[1]).c_str());
	ImGui::Text(to_string(translate.m128_f32[2]).c_str());
}
