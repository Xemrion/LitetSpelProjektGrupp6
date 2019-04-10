#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H
#include<bitset>
#include<queue>


class KeyboardInput {

public:
	class Event {
	public:
		enum class Type {
			Pressed,
			Released,
			Invalid
		};
	private:
		Type typeOfInput;
		unsigned char code;
	public:
		Event() :
			typeOfInput(Type::Invalid),
			code(0u)
		{}
		Event(Type type, unsigned char code) noexcept :
			typeOfInput(type),
			code(code)
		{}
		bool IsPressed() const noexcept {
			return typeOfInput == Type::Pressed;
		}
		bool IsReleased() const noexcept {
			return typeOfInput == Type::Released;
		}
		bool IsValid() const noexcept {
			return typeOfInput != Type::Invalid;
		}
		unsigned char GetCode() const noexcept {
			return code;
		}
	};
	KeyboardInput() = default;
	KeyboardInput(const KeyboardInput&) = delete;
	KeyboardInput& operator=(const KeyboardInput&) = delete;

	bool KeyIsPressed(unsigned char inputCode) const noexcept;
	Event ReadKey() noexcept;
	bool IsQueueEmpty() const noexcept;
	void ClearQueue() noexcept;

	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void ClearChar() noexcept;
	void Flush() noexcept;

	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool IsAutoRepeatEnabled() const noexcept;
public:
	void OnKeyPressed(unsigned char inputCode) noexcept;
	void OnKeyReleased(unsigned char inputCode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u; //No key exceeds 1 Byte.
	static constexpr unsigned int bufferSize = 16u;
	bool autoRepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};


#endif // !KEYBOARDINPUT_H