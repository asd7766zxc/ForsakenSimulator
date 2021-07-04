#include "App.h"
#include <sstream>
#include <iomanip>
App::App():wnd(800,600,"YEE"){}
int App::GO()
{
	
	while (true)
	{
		//if ecode has somethin ,exit 
		if (const auto ecode = Window::ProcessMessge())
		{
			return *ecode;
		}
		DoFrame();
	}
}
void App::DoFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed:" << t;
	wnd.SetTitle(oss.str());
}