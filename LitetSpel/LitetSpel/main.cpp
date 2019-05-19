#define NOMINMAX

#include "Globals.h"

#include <d3d11.h>
#include <Windows.h>
#include <chrono>

#include "Graphics.h"
#include "game.h"
#include "KeyboardInput.h"
#include "MouseInput.h"

// TODO: refactor into the class hierarchy!
KeyboardInput  keyboard;
MouseInput     mouse;
Graphics       graphics;
Game           game( keyboard, mouse, graphics );

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
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

HWND InitWindow(HINSTANCE hInstance, int width, int height) {
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    using Clock = std::chrono::high_resolution_clock;

	HWND wndHandle = InitWindow(hInstance, 1280, 720);
	MSG msg = { 0 };
	HRESULT hr = graphics.init(wndHandle, true);
	if (FAILED(hr)) return 2;

	bool gameLoaded = false;
	// game.menuLoad(); // TODO
	ShowWindow(wndHandle, nCmdShow);
	
    double dt_s { .0 };
	auto prevFrameTime = Clock::now(),
         currFrameTime = Clock::now();
    auto elapsedTime   = prevFrameTime - currFrameTime;

    unsigned long frame_n = 0; // temp debug thing; feel free to remove

    bool gameEnd = false; // TODO: refactor into game?
	while  (WM_QUIT != msg.message and !gameEnd ) {
		if ( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {// TODO
            // if (game.state == GameState::LevelState && !gameLoaded) {
            //     game.init();
            //     graphics.setStaticBoxes(game.level.staticBoxes);
            //     gameLoaded = true;
            // }

			currFrameTime = Clock::now();
			elapsedTime   = currFrameTime - prevFrameTime;
			prevFrameTime = currFrameTime;
            dt_s = std::chrono::duration<double>(elapsedTime).count();
			
            (void)++frame_n; // temp debug thing; feel free to remove

			char title[64];
            snprintf( title, sizeof(title)/sizeof(char), "%5.1f FPS (%5.1fms/frame)", 1.0/dt_s, dt_s*1000 );
			SetWindowTextA(wndHandle, title);

            // TODO
            // if (!game.level.player.levelCompleted) 
            // {
            //     keyboardFunc();
            //     mouseFunc();
            // }
            // else  {
            //     //game.animateColor(graphics);
            //     //game.animateVictory(game.playerSphere);
            // }

			game.update( dt_s );
			graphics.setCameraPos( game.getLevel().getPlayer().getPosition() + glm::vec3(.0f, 20.0f, -100.0f));
			graphics.setMovingBoxes( game.getLevel().getBoxes() ); // TODO: not moving boxes?
			graphics.setMetaballs( game.getLevel().getSpheres() );
            graphics.castPlayerShadow( game.getLevel().getPlayer().getPosition() );
			graphics.swapBuffer();
		}
	}
	return 0;
}
