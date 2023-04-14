#include "MapTool.h"

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ENGINE->OnInit(hinstance, L"MapTool", R1920x1080, false);

	SCENE_MGR->SetScene<MapTool>();

	ENGINE->Run();

	ENGINE->OnRelease();

	return 0;
}