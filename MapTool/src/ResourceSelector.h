#pragma once
#include "Engine_include.h"

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

class ResourceSelector : public KGCA41B::GuiWidget
{
public:
	void Active();

public:
	void Init();
	void Release();
public:
	virtual void Update() override;
	virtual void Render() override;

private:
	map<string, string> res_id_map;
};

