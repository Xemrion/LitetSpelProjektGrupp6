#define NOMINMAX
#include "Graphics.h"
#include "game.h"
#include "KeyboardInput.h"
#include "MouseInput.h"
#include <chrono>
#include <d3d11.h>
#include <Windows.h>

KeyboardInput keyboard;
MouseInput mouse;
Sounds gameSounds;
Game* game;
Graphics graphics;

double dt;

bool playerMove;
bool highlight;

int xMus = 0;
float powerCoolDown = 0.0;
bool gameEnd = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || keyboard.IsAutoRepeatEnabled()) { //Does it one time unless auto-repeat is enabled.
			keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	case(WM_MOUSEMOVE):
		const POINTS pointMove = MAKEPOINTS(lParam);
		if (pointMove.x >= 0 && pointMove.x < 1280 && pointMove.y >= 0 && pointMove.y < 720) {
			mouse.OnMouseMove(pointMove.x, pointMove.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else {
			//If either left or right button is pressed while leaving the window, keep track of position
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON)) {
				mouse.OnMouseMove(pointMove.x, pointMove.y);
			}
			else {
				//If left of right button is not pressed while leaving, don't track position
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	case(WM_MOUSEWHEEL):
		const POINTS pointWheel = MAKEPOINTS(lParam);
		mouse.OnWheelDelta(pointWheel.x, pointWheel.y, GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case(WM_LBUTTONDOWN):
		const POINTS pointLDown = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pointLDown.x, pointLDown.y);
		break;
	case(WM_LBUTTONUP):
		const POINTS pointLUp = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pointLUp.x, pointLUp.y);
		break;
	case(WM_RBUTTONDOWN):
		const POINTS pointRDown = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pointRDown.x, pointRDown.y);
		break;
	case(WM_RBUTTONUP):
		const POINTS pointRUp = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pointRUp.x, pointRUp.y);
		break;
	case(WM_MBUTTONDOWN):
		const POINTS pointMDown = MAKEPOINTS(lParam);
		mouse.OnMiddlePressed(pointMDown.x, pointMDown.y);
		break;
	case(WM_MBUTTONUP):
		const POINTS pointMUP = MAKEPOINTS(lParam);
		mouse.OnMiddleReleased(pointMUP.x, pointMUP.y);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND InitWindow(HINSTANCE hInstance, int width, int height)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = "Game Project";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	
	HWND handle = CreateWindowEx(
		0,
		"Game Project",
		"Game Project",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

void mouseFunc() 
{
	if (game->state == GameState::LevelState) 
	{
		if (mouse.LeftIsPressed())
		{
			game->leftButtonDown = true;
			game->mousePos = graphics.windowToWorldCoord(glm::vec2(mouse.GetXPos(), mouse.GetYPos()));
		}
		else
			game->leftButtonDown = false;
	}
	else 
	{
		if (mouse.LeftIsPressed())
		{
			if (mouse.GetXPos() >= 720 && mouse.GetXPos() <= 1080 && mouse.GetYPos() > 270 && mouse.GetYPos() < 620) {
				gameSounds.StopMenuMusic();
				gameSounds.PlayMenuClickSound();
				//gameSounds.StartGameMusic();
				game->state = GameState::LevelState;
			}
			else if (mouse.GetXPos() < 560 && mouse.GetXPos() >= 200 && mouse.GetYPos() > 270 && mouse.GetYPos() < 620)
			{
				gameSounds.PlayMenuBackSound();
				gameEnd = true;
			}
		}
		if ((mouse.GetXPos() >= 720 && mouse.GetXPos() <= 1080 && mouse.GetYPos() > 270 && mouse.GetYPos() < 620)) {
			if (highlight != true) {
				gameSounds.PlayMenuHighlightSound();
				game->MenuYes.color = yellow;
				highlight = true;
			}
		}
		else if((mouse.GetXPos() < 560 && mouse.GetXPos() >= 200 && mouse.GetYPos() > 270 && mouse.GetYPos() < 620)) {
			if (highlight != true) {
				gameSounds.PlayMenuHighlightSound();
				game->MenuNo.color = yellow;
				highlight = true;
			}
		}
		else {
			highlight = false;
			game->MenuNo.color = red;
			game->MenuYes.color = green;
		}

	}

}

void keyboardFunc()
{
	//Movement
	if (game->state == GameState::LevelState) {
		if (game->level.player.knockBack == false)
		{
			if (keyboard.KeyIsPressed('D') || keyboard.KeyIsPressed(VK_RIGHT))
			{
				game->keys[Game::Keys::right] = true;
			}
			if (keyboard.KeyIsPressed('A') || keyboard.KeyIsPressed(VK_LEFT))
			{
				game->keys[Game::Keys::left] = true;
			}
			if ((game->keys[Game::Keys::left] == true || game->keys[Game::Keys::right] == true) && !game->level.player.isStuck) {
				if (playerMove != true) {
					gameSounds.StartPlayerMoveLoop();
					playerMove = true;
				}
			}
			if (game->keys[Game::Keys::left] != true && game->keys[Game::Keys::right] != true || game->level.player.isStuck) {
				if (playerMove != false) {
					gameSounds.StopPlayerMoveLoop();
					playerMove = false;
				}
			}
			if (keyboard.KeyIsPressed('W') || keyboard.KeyIsPressed(VK_UP))
			{
				game->keys[Game::Keys::up] = true;
			}
			if (keyboard.KeyIsPressed('S') || keyboard.KeyIsPressed(VK_DOWN))
			{
				game->keys[Game::Keys::down] = true;
			}
			if (keyboard.KeyIsPressed('R')) {
				game->level.player.recallBlobs();
			}
			if (keyboard.KeyIsPressed('P')) {
				graphics.createShaders();
			}
			if (keyboard.KeyIsPressed('K')) {
				game->level.player.lifeCharges = 0;
			}
		} 
		else if ((game->level.player.levelCompleted == true || game->level.player.knockBack == true) && playerMove != false) {
			gameSounds.StopPlayerMoveLoop();
			playerMove = false;
		}
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	game = new Game();
	HWND wndHandle = InitWindow(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT);
	MSG msg = { 0 };
	HRESULT hr = graphics.init(wndHandle, true);
	if (FAILED(hr)) return 2;
	//if (!gameSounds.InitializeSound(wndHandle)) return 3; //Sounds failed
	game->gameSounds = &gameSounds;

	ShowWindow(wndHandle, nCmdShow);
	//gameSounds.StartMenuMusic();

	//game->init();
	//graphics.setStaticBoxes(game->level.staticBoxes);
	//graphics.setCameraPos((game->level.player.pos + vec3(0, 20, -150)), false);
	//game.update(0.00001);
	//graphics.setMovingBoxes(game.level.movingBoxes);
	//graphics.setMetaballs(game.level.spheres);
	//graphics.setLasers(game.level.laserGraphics);
	//graphics.castPlayerShadow(game.level.player.pos);
	//graphics.swapBuffer();

	//while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	bool gameLoaded = false;
	game->state = GameState::MenuState;
	game->menuLoad();

	auto prevFrameTime = std::chrono::steady_clock::now();
	while (WM_QUIT != msg.message && gameEnd == false)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (game->state == GameState::LevelState && !gameLoaded)
			{
				game->gameSounds = &gameSounds;
				game->init();
				graphics.setStaticBoxes(game->level.staticBoxes);
				graphics.setCameraPos((game->level.player.pos + vec3(0, 20, -150)), false);
				gameLoaded = true;
			}
			if (game->level.player.lifeCharges <= 0 && game->playerExist == true) {
				delete game;
				game = new Game;
				graphics.setCameraPos(vec3(0,0,-150), false);
				game->state = GameState::MenuState;
				gameLoaded = false;
				game->menuLoad();
			}
			auto currentFrameTime = std::chrono::steady_clock::now();
			dt = (double)std::chrono::duration_cast<std::chrono::microseconds>(currentFrameTime - prevFrameTime).count() / 1000000;
			prevFrameTime = currentFrameTime;
			
			char title[64];
			_itoa_s(1/dt, title, 64, 10);
			SetWindowTextA(wndHandle, title);

			if (!game->level.player.levelCompleted)
			{
				keyboardFunc();
				mouseFunc();
			}
			else 
			{
				//game.animateColor(graphics);
				//game.animateVictory(game.playerSphere);
			}
			game->update(dt);
			graphics.setCameraPos(game->getCameraPos(), game->getCameraPan());
			graphics.setMovingBoxes(game->level.movingBoxes);
			graphics.setMetaballs(game->level.spheres);
			graphics.setLasers(game->level.laserGraphics);
			graphics.castPlayerShadow(game->level.player.pos);
			graphics.swapBuffer();
			powerCoolDown -= (float)dt;
		}
	}
	delete game;
	gameSounds.Shutdown();
	return 0;
}
