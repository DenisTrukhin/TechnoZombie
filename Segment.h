#pragma once
#include <memory>
#include <array>
#include <vector>
#include <iostream>

class Segment
{
public:
	Segment() = default;
	Segment(float min, float max)
	{
		_a = std::min(min, max);
		_b = std::max(min, max);
	};

	float min()
	{
		return std::min(_a, _b);
	}

	float max()
	{
		return std::max(_a, _b);
	}

	float length()
	{
		return std::max(_a, _b) - std::min(_a, _b);
	}

	float a() { return _a; }
	float b() { return _b; }
	float& setA() { return _a; }
	float& setB() { return _b; }

private:
	float _a;
	float _b;
};

float commonLength(Segment s1, Segment s2)
{
	if (s1.max() < s2.min() || s2.max() < s1.min())
		return 0.f;
	else if (s1.min() >= s2.min() && s1.max() <= s2.max())
		return s1.length();
	else if (s2.min() >= s1.min() && s2.max() <= s1.max())
		return s2.length();
	else if (s1.max() >= s2.min() && s2.min() >= s1.min())
		return s1.max() - s2.min();
	else
		return s2.max() - s1.min();
}