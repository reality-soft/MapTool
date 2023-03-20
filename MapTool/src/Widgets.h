#pragma once
#include "Engine_include.h"
#include "LevelEditor.h"

#define RGB_TO_FLOAT(x, y, z) ImVec4(x / 255.f, y / 255.f, z / 255.f, 1.f)
#define LISTBOX_COLOR RGB_TO_FLOAT(11, 15, 13)
#define STM_COLOR RGB_TO_FLOAT(93, 155, 155)
#define SKM_COLOR RGB_TO_FLOAT(73, 126, 118)
#define SKT_COLOR RGB_TO_FLOAT(179, 40, 33)
#define ANM_COLOR RGB_TO_FLOAT(244, 70, 017)
#define VS_COLOR  RGB_TO_FLOAT(201, 60, 32)
#define PS_COLOR  RGB_TO_FLOAT(222, 76, 138)
#define TEX_COLOR RGB_TO_FLOAT(63, 136, 143)
#define SOUND_COLOR RGB_TO_FLOAT(151, 79, 102)

#define GWNAME(gwclass) typeid(gwclass).name()
#define NOT(a) a = !a
using namespace reality;

enum class MsgType
{
	NONE,
	OW_LEVEL_EDITOR,
	OW_GUIDE_LINE,
	OW_RES_VIEWER,
	OPT_WIREFRAME,
};

class GwMainMenu : public reality::GuiWidget
{
public:
	virtual void Update() override;
	virtual void Render() override;
	
	MsgType msg_ = MsgType::NONE;
};


class GwPorperty : public reality::GuiWidget
{
public:
	int visible_leaves;
	XMFLOAT4 collide_pos;
	XMFLOAT3 camera;

public:
	string fps;
	string visible_leaf;
	string collision;
	string camera_pos;

public:
	virtual void Update() override;
	virtual void Render() override;
};

class GwLevelEditor : public reality::GuiWidget
{
public:
	virtual void Update() override;
	virtual void Render() override;

public:
	// resize
	int max_lod = 3;
	int cell_scale = 8;
	int uv_scale_ = 8;
	int row_col_blocks = 8;

	// sculpting
	char tex_id_buffer[128] = {0,};
	bool brush_on_ = true;
	bool paint_on = true;
	float brush_size_ = 100;

	// Object
	char mesh_id_buffer[128];

	LevelEditor* editing_level = nullptr;
	Level* loaded_level = nullptr;
	InstanceData* selected_instance = nullptr;

	char dir_buffer[128] = "../../Contents/BinaryPackage/Levels/";
	char save_buffer[128] = {0,};
	char load_buffer[128] = {0,};

private:
	void CwEditTerrain();
	void CwObjectControl();

	Texture* DragDropImage(string& name);
};
