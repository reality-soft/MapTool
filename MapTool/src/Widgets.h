#pragma once
#include "Engine_include.h"

#define GWNAME(gwclass) typeid(gwclass).name()
using namespace KGCA41B;

enum class MsgType
{
	NONE,
	OW_RES_VIEWER,
};

class GwMainMenu : public KGCA41B::GuiWidget
{
public:
	virtual void Update() override;
	virtual void Render() override;

	MsgType msg_ = MsgType::NONE;
};

class GwResViewer : public KGCA41B::GuiWidget
{
public:
	virtual void Update() override;
	virtual void Render() override;

private:
	map<string, string> res_id_map;
};

class GwPorperty : public KGCA41B::GuiWidget
{
public:
	XMMATRIX world_transform_;
	POINT mouse_pos;

	XMVECTOR camera_pos;
	XMVECTOR ray_start;
	XMVECTOR ray_end;

	string mouse_pos_text;

	string camera_pos_text;
	string ray_start_text;
	string ray_end_text;

public:
	virtual void Update() override;
	virtual void Render() override;
};

