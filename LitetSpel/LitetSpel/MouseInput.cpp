#include"MouseInput.h"
#include<Windows.h>

std::pair<int, int> MouseInput::GetPos() const noexcept {
	return{ x, y };
}
int MouseInput::GetXPos() const noexcept {
	return x;
}
int MouseInput::GetYPos() const noexcept {
	return y;
}
bool MouseInput::IsInWindow() const noexcept {
	return isInWindow;
}
bool MouseInput::LeftIsPressed() const noexcept {
	return leftIsPressed;
}
bool MouseInput::RightIsPressed() const noexcept {
	return rightIsPressed;
}
bool MouseInput::MiddleIsPressed() const noexcept {
	return middleIsPressed;
}
MouseInput::Event MouseInput::ReadEvent() noexcept {
	if (buffer.size() > 0u) {
		MouseInput::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else {
		return MouseInput::Event();
	}
}
bool MouseInput::IsEmpty() const noexcept {
	return buffer.empty();
}
void MouseInput::Flush() noexcept {
	buffer = std::queue<MouseInput::Event>();
}

//Window message functions
void MouseInput::OnMouseMove(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::Move, *this));
	TrimBuffer();

}
void MouseInput::OnMouseLeave() noexcept {
	this->isInWindow = false;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::Leave, *this));
	TrimBuffer();
}
void MouseInput::OnMouseEnter() noexcept {
	this->isInWindow = true;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::Leave, *this));
	TrimBuffer();
}
void MouseInput::OnLeftPressed(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::LeftPress, *this));
	TrimBuffer();
}
void MouseInput::OnLeftReleased(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::LeftRelease, *this));
	TrimBuffer();
}
void MouseInput::OnRightPressed(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::RightPress, *this));
	TrimBuffer();
}
void MouseInput::OnRightReleased(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::RightRelease, *this));
	TrimBuffer();
}
void MouseInput::OnMiddlePressed(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::MiddlePress, *this));
	TrimBuffer();
}
void MouseInput::OnMiddleReleased(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::MiddleRelease, *this));
	TrimBuffer();
}
void MouseInput::OnWheelUp(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::WheelUp, *this));
	TrimBuffer();
}
void MouseInput::OnWheelDown(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	buffer.push(MouseInput::Event(MouseInput::Event::Type::WheelDown, *this));
	TrimBuffer();
}
void MouseInput::TrimBuffer() noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
void MouseInput::OnWheelDelta(int x, int y, int delta) noexcept {
	wheelDeltaCarry += delta;
	while (wheelDeltaCarry >= WHEEL_DELTA) {
		wheelDeltaCarry -= WHEEL_DELTA;
		this->OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA) {
		wheelDeltaCarry += WHEEL_DELTA;
		this->OnWheelDown(x, y);
	}
}