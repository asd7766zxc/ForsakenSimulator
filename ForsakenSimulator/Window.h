#pragma once
#include "asdWin.h"
#include "ExceptionHelper.h"
#include "Keyboard.h"
class Window
{
public:
	class Exception : public ExceptionHelper {
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private : 
	//Ωy“ª“ï¥∞
	class WindowClass
	{
	public :
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		static constexpr const char* wndClassName = "ASD Direct3D Engine Window";
		static WindowClass wndclass;
		HINSTANCE hInst;
	};

public:
	Window(int width, int height, const char* name) ;
	~Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
		Keyboard kb;
private:
	int width;
	int height;
	HWND hWnd;
};

//error exception helper macro 
#define CHWND_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)
#define CHWND_LAST_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,GetLastError())

