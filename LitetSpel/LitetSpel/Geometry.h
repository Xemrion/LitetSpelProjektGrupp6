#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class Sphere {
public:
	Vector4 centerRadius;
};

class Box {
public:
	Vector4 center;
	Vector4 halfLengths;
};
