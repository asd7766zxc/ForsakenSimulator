#include "Window.h"
#include <sstream>
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		Window wnd(480, 600, "For Test");
		MSG msg;
		BOOL gResult;
		int i = 0;
		while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			while (!wnd.mouse.isEmpty())
			{
				const auto e = wnd.mouse.Read();
				switch (e.GetType())
				{
				case  Mouse::Event::Type::WheelUp:
				{
					i++;
					std::ostringstream oss;
					oss << "Up" << i;
					wnd.SetTitle(oss.str());
				}
					break;
				case Mouse::Event::Type::WheelDown:
				{
					i--;
					std::ostringstream oss;
					oss << "Down" << i;
					wnd.SetTitle(oss.str());
				}
				break;
				}
			}
		}
		if (gResult == -1)
		{
			return -1;
		}
		return msg.wParam;
	}
	catch (const ExceptionHelper& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Ò»°ãÀýÍâ î›r", MB_OK | MB_ICONEXCLAMATION);
	}
}