#include <Windows.h>
#include "Engine/FrameWork/Framework.h"
#include "Engine/FrameWork/MyGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Framework* gameloop = new MyGame();

	gameloop->Execute();

	delete gameloop;

	return 0;
}