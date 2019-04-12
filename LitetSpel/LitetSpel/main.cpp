#include <Windows.h>
#include <chrono>
#include <d3d11.h>
#include "Graphics.h"
#include "game.h"
#include "KeyboardInput.h"

KeyboardInput keyboard;
Game game;

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
			double dt = (double)std::chrono::duration_cast<std::chrono::microseconds>(currentFrameTime - prevFrameTime).count() / 1000000;
			prevFrameTime = currentFrameTime;

			//Movement
			if (keyboard.KeyIsPressed('D'))
			{
				//game.currentLevel.player.move(dt, glm::vec3(1.0f, 0.0f, 0.0f));
				game.keys[1] = true;
			}
			if (keyboard.KeyIsPressed('A'))
			{
				//game.currentLevel.player.move(dt, glm::vec3(-1.0f, 0.0f, 0.0f));
				game.keys[0] = true;
			}
			if (keyboard.KeyIsPressed('W'))
			{
				//game.currentLevel.player.move(dt, glm::vec3(-1.0f, 0.0f, 0.0f));
				game.keys[2] = true;
			}
			if (keyboard.KeyIsPressed('S'))
			{
				//game.currentLevel.player.move(dt, glm::vec3(-1.0f, 0.0f, 0.0f));
				game.keys[3] = true;
			}

			game.update(dt);
			//graphics.queueBoxes(game.currentLevel.boxes);
			graphics.queueMetaballs(game.currentLevel.spheres);
			graphics.swapBuffer();

		}
	}
}
