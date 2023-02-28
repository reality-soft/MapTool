#pragma once
#include "Engine_include.h"
#include "FileTransfer.h"

using namespace KGCA41B;

struct StreamVertex
{
	XMFLOAT4 p;
	XMFLOAT3 o;
	XMFLOAT3 n;
	XMFLOAT4 c;
	XMFLOAT2 t;
};

struct CbHitCircle
{
	CbHitCircle() = default;
	CbHitCircle(const CbHitCircle& other)
	{
		data = other.data;
		other.buffer.CopyTo(buffer.GetAddressOf());
	}
	struct Data
	{
		bool is_hit = false;
		float circle_radius = 0.0f;
		XMVECTOR hitpoint = { 0, 0, 0, 0 };
	} data;

	ComPtr<ID3D11Buffer> buffer;
};

struct CbEditOption
{
	CbEditOption() = default;
	CbEditOption(const CbEditOption& other)
	{
		data = other.data;
		other.buffer.CopyTo(buffer.GetAddressOf());
	}
	struct Data
	{
		XMINT4 altitude = { 0, 0, 0, 0 };

	} data;
	ComPtr<ID3D11Buffer> buffer;
};

class LevelEditor : public Level
{
public:
	LevelEditor() {}
	~LevelEditor() {}

public:
	bool ExportToFile(string filename);
	bool CopyFromSavedLevel(Level* saved_level);

	// Edit-SO-Stage	
	bool CreateEditSOStage();
	void SetEditSOStage();
	void GetEditSOStage();

	string gs_id_;
	ComPtr<ID3D11Buffer> so_buffer_;
	CbHitCircle hit_circle_;
	CbEditOption edit_option_;

	// Physics
	void LevelEdit();
	void ResetHeightField();

	// Edit Options
	float sculpting_brush_ = 10.0f;

	// Default Export Directory
	string export_dir = "../../Contents/BinaryPackage/";


private:
	bool CreateTempBuffer(ID3D11Buffer** _buffer);
};

