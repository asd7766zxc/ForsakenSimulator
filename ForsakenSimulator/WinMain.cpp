#include "App.h"
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		return App{}.GO();
	}
	catch (const ExceptionHelper& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Ò»°ãÀýÍâ î›r", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}