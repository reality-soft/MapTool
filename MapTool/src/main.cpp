#include "MapTool.h"

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ENGINE->OnInit(hinstance, L"MapTool", { 1280, 720 });

	MapTool map_tool;

	ENGINE->Run(&map_tool);

	ENGINE->OnRelease();

	return 0;
}