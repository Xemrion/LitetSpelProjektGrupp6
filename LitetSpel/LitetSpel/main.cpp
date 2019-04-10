#include <Windows.h>
#include <chrono>
#include <d3d11.h>
#include "Graphics.h"
#include "game.h"

Game game;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (lParam)
		case VK_LEFT: {
			game.keys[0] = true;
		} break;
		case VK_RIGHT: {
			game.keys[1] = true;
		} break;
		case VK_UP: {
			game.keys[2] = true;
		} break;
		case VK_DOWN: {
			game.keys[3] = true;
		} break;
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
			game.update(dt);
			graphics.queueBoxes(game.currentLevel.boxes);
			graphics.queueMetaballs(game.currentLevel.spheres);
			graphics.swapBuffer();
		}
	}
}
