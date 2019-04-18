#include"KeyboardInput.h"

bool KeyboardInput::KeyIsPressed(unsigned char inputCode) const noexcept {
	return keystates[inputCode];
}
KeyboardInput::Event KeyboardInput::ReadKey() noexcept {
	if (keybuffer.size() > 0u) {
		KeyboardInput::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	else {
		return KeyboardInput::Event();
	}
}
bool KeyboardInput::IsQueueEmpty() const noexcept {
	return keybuffer.empty();
}
void KeyboardInput::ClearQueue() noexcept {
	this->keybuffer = std::queue<KeyboardInput::Event>();
}

char KeyboardInput::ReadChar() noexcept {
	if (charbuffer.size() > 0u) {
		unsigned char c = charbuffer.front();
		charbuffer.pop();
		return c;
	}
	else {
		return 0;
	}
}
bool KeyboardInput::CharIsEmpty() const noexcept {
	return charbuffer.empty();
}
void KeyboardInput::ClearChar() noexcept {
	this->charbuffer = std::queue<char>();
}
void KeyboardInput::Flush() noexcept {
	this->ClearChar();
	this->ClearQueue();
}

void KeyboardInput::EnableAutoRepeat() noexcept {
	this->autoRepeatEnabled = true;
}
void KeyboardInput::DisableAutoRepeat() noexcept {
	this->autoRepeatEnabled = false;
}
bool KeyboardInput::IsAutoRepeatEnabled() const noexcept {
	return this->autoRepeatEnabled;
}

void KeyboardInput::OnKeyPressed(unsigned char inputCode) noexcept {
	keystates[inputCode] = true;
	keybuffer.push(KeyboardInput::Event(KeyboardInput::Event::Type::Pressed, inputCode));
	TrimBuffer(keybuffer);
}
void KeyboardInput::OnKeyReleased(unsigned char inputCode) noexcept {
	keystates[inputCode] = false;
	keybuffer.push(KeyboardInput::Event(KeyboardInput::Event::Type::Released, inputCode));
	TrimBuffer(keybuffer);
}
void KeyboardInput::OnChar(char character) noexcept {
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}
void KeyboardInput::ClearState() noexcept {
	keystates.reset();
}
template<typename T>
static void KeyboardInput::TrimBuffer(std::queue<T>& buffer) noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}