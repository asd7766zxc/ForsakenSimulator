#include "Window.h"
#include <sstream>
#include "resource.h"
Window::WindowClass Window::WindowClass::wndclass;
Window::WindowClass::WindowClass() noexcept :
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC; //阻止視窗進行GDI+ 渲染
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,32,32,0)); //Load icon form resources
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));;
	RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}
const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}
HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return wndclass.hInst;
};
Window::Window(int width, int height, const char* name) :width(width),height(height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) //由於視窗的長寬會受到視窗工具列的影響 因此需要做調整
	{
		throw AWND_LAST_EXCEPT();
	}
	

	hWnd = CreateWindow(WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::GetInstance(), this);

	if (hWnd == nullptr)
	{
		throw AWND_LAST_EXCEPT();
	}
	ShowWindow(hWnd,SW_SHOWDEFAULT);
}
Window::~Window()
{
	DestroyWindow(hWnd);
}
void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		throw AWND_LAST_EXCEPT();
	}
}
std::optional<int> Window::ProcessMessge() noexcept
{
	//msg pump
	MSG msg;
	//while queue has messages ,remove them and diapatch them (without bllock )
	while (PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}
		//process msg ususally 
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//rebound the ptr to window 
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	//clear keys when window loss focus
	case WM_KILLFOCUS:
		kb.ClearState();
		break;

	/*Handle Keyboard*/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: //SYS Key handle
		if(!(lParam & 0x40000000 || kb.AutoRepeatIsEnabled()))
		{ 
			kb.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kb.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kb.OnChar(static_cast<unsigned char>(wParam));
		break;
	/*Handle Keyboard*/
	/*Handle Mouse*/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width &&pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/*Handle Mouse*/

	}

	
	return DefWindowProc(hWnd, msg, wParam, lParam);

}

//Window Exception
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:ExceptionHelper(line,file),hr(hr){}
const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl<<"[Error Code] "<<GetErrorCode()<<"[Descrption] "<<GetErrorString()<<std::endl<<GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const char* Window::Exception::GetType() const noexcept
{
	return "Window Exception";
}
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,nullptr,hr,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),reinterpret_cast<LPSTR>(&pMsgBuf),0,nullptr );
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}
HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}
std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}