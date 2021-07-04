#pragma once
#include "Window.h"
#include "Timer.h"
class App
{
public: 
	App();
	//main frame / message loop
	int GO();
private :
	void DoFrame();
private:
	Window wnd;
	Timer timer;
};