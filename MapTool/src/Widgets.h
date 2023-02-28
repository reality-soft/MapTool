#pragma once
#include "Engine_include.h"
#include "LevelEditor.h"

#define GWNAME(gwclass) typeid(gwclass).name()
#define NOT(a) a = !a
using namespace KGCA41B;

enum class MsgType
{
	NONE,
	OW_LEVEL_EDITOR,
	OW_RES_VIEWER,
	OW_INSTANCED_FOLIAGE,
	OPT_WIREFRAME,
};

class GwMainMenu : public KGCA41B::GuiWidget
{
public:
	virtual void Update() override;
	virtual void Render() override;
	
	MsgType msg_ = MsgType::NONE;
};

class GwLevelEditor : public KGCA41B::GuiWidget
{
public:
	virtual void Update() override;
	virtual void Render() override;
	
	void Gernate(Level* new_level);
public:
	// regenerating
	int cell_count_ = 4;
	int cell_distance_ = 1;
	int uv_scale_ = 10;

	// sculpting
	bool brush_on_ = true;
	float brush_size_ = 100;

	LevelEditor* editing_level = nullptr;
};

class GwResViewer : public KGCA41B::GuiWidget
{
};

class GwPorperty : public KGCA41B::GuiWidget
{
public:
	XMMATRIX world_transform_;
	string mouse_pos_text;
public:
	virtual void Update() override;
	virtual void Render() override;
};

