#include "Engine3D.h"
LRESULT CALLBACK RenderingWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	short hiWParam = HIWORD(wParam);
	short loWParam = LOWORD(wParam);
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		CloseWindow(hWnd);
		PostQuitMessage(0);
		break;
	case WM_MOUSEWHEEL: 
		
		if (hiWParam > 0)
		{
			if (RenderingWindow::CtrlIsPressed(loWParam)) //if ctrl is pressed
			{
				engine3D->camera.Move(MOVE_FRONT);
			}
			else
			{
				engine3D->camera.pitch -= engine3D->camera.rotationSpeed;
			}
		}
		else
		{
			if (RenderingWindow::CtrlIsPressed(loWParam)) //if ctrl is pressed
			{
				engine3D->camera.Move(MOVE_BACK);
			}
			else
			{
				engine3D->camera.pitch += engine3D->camera.rotationSpeed;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		if (RenderingWindow::CtrlIsPressed(loWParam))
			engine3D->camera.Move(MOVE_LEFT);
		else
			engine3D->camera.yaw -= engine3D->camera.rotationSpeed;
		break;
	case WM_RBUTTONDOWN:
		if (RenderingWindow::CtrlIsPressed(loWParam))
			engine3D->camera.Move(MOVE_RIGHT);
		else
			engine3D->camera.yaw += engine3D->camera.rotationSpeed;
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	return 0;
}

LRESULT CALLBACK RenderingWindow::MsgRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RenderingWindow *app = NULL;
	if (msg == WM_CREATE)
	{
		app = (RenderingWindow*)((LPCREATESTRUCT)(lParam))->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else
	{
		app = (RenderingWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}
	return app->WndProc(hWnd, msg, wParam, lParam);
}

RenderingWindow::RenderingWindow(int width, int height, LPCSTR title)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = MsgRouter;
	wc.lpszClassName = "Engine3DWindow";
	wc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wc);
	windowHandle = CreateWindowEx(NULL, wc.lpszClassName, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, NULL, this);

	ShowWindow(windowHandle, SW_SHOWNORMAL);
	UpdateWindow(windowHandle);
}

RenderingWindow::~RenderingWindow()
{}

RET_VAL RenderingWindow::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		//Render
		if (engine3D)
		{
			engine3D->Render();
		}

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return RET_OK;
}

HWND RenderingWindow::GetWindowHandle()
{
	return windowHandle;
}

RET_VAL RenderingWindow::SetEngine3DPointer(Engine3D* enginePointer)
{
	engine3D = enginePointer;
	return RET_OK;
}

bool RenderingWindow::CtrlIsPressed(short lowWord)
{
	if ((lowWord & MK_CONTROL) == MK_CONTROL)
		return true;
	else
		return false;
}