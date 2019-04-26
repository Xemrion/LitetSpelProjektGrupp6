#include <Windows.h>
#include <chrono>
#include <d3d11.h>
#include "Graphics.h"
#include "game.h"
#include "KeyboardInput.h"
#include "MouseInput.h"

KeyboardInput keyboard;
MouseInput mouse;
Game game;

double dt;

int xMus = 0;
float powerCoolDown = 0.0;

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
	if (mouse.LeftIsPressed())
	{
		game.leftButtonDown = true;
		game.mousePos = glm::vec3(mouse.GetXPos(), mouse.GetYPos(), 0);
	}
	else
		game.leftButtonDown = false;
}

void keyboardFunc()
{
	//Movement
	if (keyboard.KeyIsPressed('D'))
	{
		game.keys[1] = true;
	}
	if (keyboard.KeyIsPressed('A'))
	{
		game.keys[0] = true;
	}
	if (keyboard.KeyIsPressed('W'))
	{
		game.keys[2] = true;
	}
	if (keyboard.KeyIsPressed('S'))
	{
		game.keys[3] = true;
	}
	if (keyboard.KeyIsPressed('B')) 
	{
		if (powerCoolDown <= 0) 
		{
			if (game.currentLevel.player.status == PlayerStatus::None)
			{
				game.currentLevel.player.status = PlayerStatus::Bouncy;
			}
			else
			{
				game.currentLevel.player.status = PlayerStatus::None;
			}
			powerCoolDown = 0.2f;
		}
	}
	if (keyboard.KeyIsPressed('H'))
	{
		if (powerCoolDown <= 0)
		{
			if (game.currentLevel.player.status == PlayerStatus::None)
			{
				game.currentLevel.player.status = PlayerStatus::Heavy;
			}
			else
			{
				game.currentLevel.player.status = PlayerStatus::None;
			}
			powerCoolDown = 0.2f;
		}
	}
	if (keyboard.KeyIsPressed('Y'))
	{
		if (powerCoolDown <= 0)
		{
			if (game.currentLevel.player.status == PlayerStatus::None)
			{
				game.currentLevel.player.status = PlayerStatus::Sticky;
			}
			else
			{
				game.currentLevel.player.status = PlayerStatus::None;
			}
			powerCoolDown = 0.2f;
		}
	}
	if (keyboard.KeyIsPressed('R'))
		game.currentLevel.player.recallBlobs();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	HWND wndHandle = InitWindow(hInstance, 1280, 720);
	MSG msg = { 0 };
	Graphics graphics;
	HRESULT hr = graphics.init(wndHandle, true);
	if (FAILED(hr)) return 2;
	game.init();
	ShowWindow(wndHandle, nCmdShow);
	
	auto prevFrameTime = std::chrono::steady_clock::now();
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			auto currentFrameTime = std::chrono::steady_clock::now();
			dt = (double)std::chrono::duration_cast<std::chrono::microseconds>(currentFrameTime - prevFrameTime).count() / 1000000;
			prevFrameTime = currentFrameTime;
			
			char title[64];
			//_itoa_s(1/dt, title, 64, 10);
			-_itoa_s(game.currentLevel.player.jumpCooldown, title, 64, 10);
			SetWindowTextA(wndHandle, title);
			
			keyboardFunc();
			mouseFunc();

			game.update(dt);
			graphics.setCameraPos(glm::vec3(game.playerSphere.centerRadius) + glm::vec3(0.0, 20.0, -100.0));
			graphics.setBoxes(game.currentLevel.boxes);
			graphics.setMetaballs(game.currentLevel.spheres);
			graphics.swapBuffer();

			powerCoolDown -= (float)dt;
		}
		
	}

	return 0;
}
