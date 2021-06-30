#include <Windows.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
			PostQuitMessage(777);
			break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "F");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "YEE");
		}
		break;
	case WM_CHAR:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	const auto pClassname = "Forsake";
	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC; //阻止視窗進行GDI+ 渲染
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr; //預設
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassname;
	wc.hIcon = nullptr;
	RegisterClassEx(&wc);

	//建立視窗												//視窗Style
	HWND hWnd = CreateWindowEx(0, pClassname, "Yee Window", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX , 100, 100, 640, 480, nullptr, nullptr, hInstance, nullptr);
	//展示最純的視窗
	ShowWindow(hWnd,SW_SHOW);
	//message pump
	MSG msg; //msg structure 
	//Get message hWnd set to null mean catch all msg from the same thread 
	BOOL gResult; 
	while(gResult = GetMessage(&msg,nullptr,0,0) > 0 )	
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}