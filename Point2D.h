#pragma once
#include "Segment.h"

float const eps = 1e-5f;

class Point2D
{
public:
	Point2D() {};

	Point2D(float x, float y)
		: _x(x), _y(y)
	{};

	Point2D(const Point2D& that)
		: _x(that._x), _y(that._y)
	{};

	const Point2D& operator = (const Point2D& that)
	{
		if (&that == this) { return *this; }
		_x = that._x;
		_y = that._y;
		return *this;
	}

	Point2D operator + (const Point2D& that) const
	{
		return { _x + that._x, _y + that._y };
	}

	Point2D operator + (float num) const
	{
		return{ _x + num, _y + num };
	}

	Point2D operator - (const Point2D& that) const
	{
		return {_x - that._x, _y - that._y};
	}

	Point2D operator- (float num) const
	{
		return {_x - num, _y - num};
	}

	Point2D operator - () const
	{
		return {-_x, -_y};
	}

	Point2D operator * (float num) const
	{
		return {_x * num, _y * num};
	}

	Point2D operator / (float num) const
	{
		return {_x / num, _y / num};
	}

	Point2D& operator += (const Point2D& that)
	{
		_x += that._x;
		_y += that._y;
		return *this;
	}

	Point2D& operator -= (const Point2D& that)
	{
		_x -= that._x;
		_y -= that._y;
		return *this;
	}

	Point2D& operator *= (float num)
	{
		_x *= num;
		_y *= num;
		return *this;
	}

	Point2D& operator /= (float num)
	{
		_x /= num;
		_y /= num;
		return *this;
	}

	bool operator == (const Point2D& that) const
	{
		return equalWithEps(_x, that._x) && equalWithEps(_y, that._y);
	}

	bool operator != (const Point2D& that) const
	{
		return !operator==(that);
	}

	bool operator < (const Point2D& that)
	{
		if (_x != that._x) return _x < that._x;
		return _y < that._y;
	}

	void normalize()
	{
		float length = this->length();
		if (!equalWithEps(0.f, length))
		{
			_x /= length;
			_y /= length;
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Point2D& that)
	{
		os << "Point2D(" << that._x << ';' << that._y << ")";
		return os;
	}
	void setX(float x) {_x = x;}
	void setY(float y) { _y = y; }
	float x() const { return _x; }
	float y() const { return _y; } 

	friend sf::Packet& operator >> (sf::Packet& packet, Point2D& point)
	{
		return packet >> point._x >> point._y;
	}
	friend sf::Packet& operator<< (sf::Packet& packet, const Point2D& point)
	{
		return packet << point._x << point._y;
	}
private:
	bool equalWithEps(float v1, float v2) const
	{
		return fabs(v1 - v2) < eps;
	}
	float length() const
	{
		return sqrt(_x * _x + _y * _y);
	}
	float _x;
	float _y;
};