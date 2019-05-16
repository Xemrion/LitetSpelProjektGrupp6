#pragma once

#include <queue>

//Exampel if(mouseVariable.ReadEvent().GetType() == MouseInput::Event::Type::RightPress) för högerklick

class MouseInput {
	friend class RenderWindow;
	friend class Graphics;
public:
	class Event {
	public:
		enum class Type {
			LeftPress,
			RightPress,
			LeftRelease,
			RightRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid,
			MiddlePress,
			MiddleRelease
		};
	private:
		Type typeOfInput;
		bool leftPressed,
			rightPressed,
			middlePressed;
		int xPos,
			yPos;
	public:
		Event() noexcept :
			typeOfInput(Type::Invalid),
			leftPressed(false),
			rightPressed(false),
			middlePressed(false),
			xPos(0),
			yPos(0)
		{}
		Event(Type type, const MouseInput& mouse) noexcept :
			typeOfInput(type),
			leftPressed(mouse.leftIsPressed),
			rightPressed(mouse.rightIsPressed),
			middlePressed(mouse.middleIsPressed),
			xPos(mouse.x),
			yPos(mouse.y)
		{}
		bool isValid() const noexcept {
			return typeOfInput != Type::Invalid;
		}
		Type GetType() const noexcept {
			return typeOfInput;
		}
		std::pair<int, int> GetPos() const noexcept {
			return{ xPos, yPos };
		}
		int GetXPos() const noexcept {
			return xPos;
		}
		int GetYPos() const noexcept {
			return yPos;
		}
		bool LeftIsPressed() const noexcept {
			return leftPressed;
		}
		bool RightIsPressed() const noexcept {
			return rightPressed;
		}

	};
public:
	MouseInput() = default;
	MouseInput(const MouseInput&) = delete;
	MouseInput& operator=(const MouseInput&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetXPos() const noexcept;
	int GetYPos() const noexcept;
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool MiddleIsPressed() const noexcept;
	MouseInput::Event ReadEvent() noexcept;
	bool IsEmpty() const noexcept;
	void Flush() noexcept;
	//Windows message functions
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnMiddlePressed(int x, int y) noexcept;
	void OnMiddleReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
private: //Variables
	static constexpr unsigned int bufferSize = 16u;
	int x,
		y;
	bool leftIsPressed = false,
		rightIsPressed = false,
		middleIsPressed = false,
		isInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<MouseInput::Event> buffer;
};
