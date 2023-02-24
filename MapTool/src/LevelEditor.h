#pragma once
#include "Engine_include.h"
#include "FileTransfer.h"

using namespace KGCA41B;

class LevelEditor : public Level
{
public:
	LevelEditor() {}
	~LevelEditor() {}

public:
	bool ExportToFile(string filename);

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
	void Regenerate(UINT num_row, UINT num_col, int cell_distance, int uv_scale);
	void ResetHeightField();

	// Edit Options
	float sculpting_brush_ = 100.0f;

	// Default Export Directory
	string export_dir = "../../Contents/BinaryPackage/";


private:
	bool CreateTempBuffer(ID3D11Buffer** _buffer);
};

