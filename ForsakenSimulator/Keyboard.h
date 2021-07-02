#pragma once
#include <queue>
#include <bitset>
class Keyboard
{
	friend class Window;
public:
	class  Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invaild
		};
	private:
		Type type;
		unsigned char code;
	public :
		Event() :
			type(Type::Invaild), code(0u) {}
		Event(Type type,unsigned char code) noexcept:
			type(type),code(code){}
		bool isPress() const noexcept
		{
			return type == Type::Press;
		}
		bool isRelease() const noexcept
		{
			return type == Type::Release;
		}
		bool isVaild() const noexcept
		{
			return type != Type::Invaild;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator = (const Keyboard&) = delete;
	//key event
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept; //Clear Key Queue
	//char event 
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;//Clear Queue
	//autorepeat constrol
	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool AutoRepeatIsEnabled() const noexcept;
private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept; // clear bitset
	template<typename T> static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};