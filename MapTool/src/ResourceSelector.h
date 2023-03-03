#pragma once
#include "Engine_include.h"

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

