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
	string mouse_pos_text;

	XMVECTOR camera_pos;
	XMFLOAT2 ndc_pos;
	string camera_pos_text;

	XMVECTOR ray_hitpoint_;
	string ray_hitpoint_text_;


public:
	virtual void Update() override;
	virtual void Render() override;
};

