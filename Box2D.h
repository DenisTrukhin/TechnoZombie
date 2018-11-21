#pragma once
#include "Point2D.h"
using DirectionsArray = std::unique_ptr<std::array<bool, 8>>;
using SatArray = std::unique_ptr<std::array<float, 4>>;

short top = 0;
short right = 1;
short bottom = 2;
short left = 3;
float other_box_sector = 200.f;

class Box2D
{
public:
	Box2D() = default;
	Box2D(const Point2D& upperVertex,const Point2D& lowerVertex)
		: _uppVertex(upperVertex), _lowVertex(lowerVertex)
	{};

	Box2D(float x1, float y1, float x2, float y2)
		: _uppVertex(x1, y1), _lowVertex(x2, y2)
	{};

	Box2D(const Box2D& that)
		: _uppVertex(that._uppVertex), _lowVertex(that._lowVertex)
	{};
	
	Box2D& operator= (const Box2D& that)
	{
		if (&that == this) return *this;
		_uppVertex = that._uppVertex;
		_lowVertex = that._lowVertex;
		return *this;
	} 

	bool operator == (const Box2D& that) const
	{
		return equalWithEps(getUppVertex().x(), that.getUppVertex().x()) &&
			equalWithEps(getUppVertex().y(), that.getUppVertex().y()) &&
			equalWithEps(getLowVertex().x(), that.getLowVertex().x()) &&
			equalWithEps(getLowVertex().y(), that.getLowVertex().y());
	}

	bool operator != (const Box2D& that) const
	{
		return !operator==(that);
	}

	Box2D& operator * (float num)
	{
		float lengthX = _lowVertex.x() - _uppVertex.x();
		float lengthY = _lowVertex.y() - _uppVertex.y();
		_lowVertex = Point2D(_lowVertex.x() + lengthX * (num - 1.f) / 2.f,
			_lowVertex.y() + lengthY * (num - 1.f) / 2.f);
		_uppVertex = Point2D(_uppVertex.x() - lengthX * (num - 1.f) / 2.f,
			_uppVertex.y() - lengthY * (num - 1.f) / 2.f);
		return *this;
	}
	
	Box2D& operator / (float num)
	{
		if (num < eps)
		{
			//need to add an exception
			return *this;
		}
		float lengthX = _lowVertex.x() - _uppVertex.x();
		float lengthY = _lowVertex.y() - _uppVertex.y();
		_lowVertex = Point2D(_lowVertex.x() - lengthX * (1.f - 1.f / num) / 2.f,
			_lowVertex.y() - lengthY * (1.f - 1.f / num) / 2.f);
		_uppVertex = Point2D(_uppVertex.x() + lengthX * (1.f - 1.f / num) / 2.f,
			_uppVertex.y() + lengthY * (1.f - 1.f / num) / 2.f);
		return *this;
	}

	Box2D& operator += (const Point2D& point)
	{
		_lowVertex += point;
		_uppVertex += point;
		return *this;
	}
	
	SatArray isBoxCrossesBox(const Box2D& that) const
	{
		SatArray side_of_barrier = std::make_unique<std::array<float, 4>>();
		Segment x1(_uppVertex.x(), _lowVertex.x());
		Segment y1(_uppVertex.y(), _lowVertex.y());
		Segment x2(that._uppVertex.x(), that._lowVertex.x());
		Segment y2(that._uppVertex.y(), that._lowVertex.y());
		float cLX, cLY;
		if ((cLX = commonLength(x1, x2)) > 0.f && (cLY = commonLength(y1, y2)) > 0.f)
		{
			if (cLX < cLY)
			{
				if (x1.max() >= x2.min() && x1.max() < x2.max())
					side_of_barrier->at(right) = cLX;
				else 
					side_of_barrier->at(left) = cLX;
			}
			else
			{
				if (y1.max() >= y2.min() && y1.max() < y2.max())
					side_of_barrier->at(bottom) = cLY;
				else
					side_of_barrier->at(top) = cLY;
			}
		}
		return side_of_barrier;
	}



	bool isBoxCrossesBoxBool(const Box2D& that) const
	{
		if (_lowVertex.x() < that._uppVertex.x()) // левее
			return false;
		if (_uppVertex.x() > that._lowVertex.x()) // правее
			return false;
		if (_lowVertex.y() < that._uppVertex.y()) // выше
			return false;
		if (_uppVertex.y() > that._lowVertex.y()) // ниже
			return false;
		return true;
	}

	DirectionsArray isPointInSector(const Box2D& that) const
	{
		DirectionsArray side_of_other_box = std::make_unique<std::array<bool, 8>>();
		for (size_t i = 0; i < 4; ++i)
		{
			if (i == top)
			{
				Point2D leftVertex = { this->getCenter().x() - other_box_sector, this->getCenter().y() - other_box_sector };
				Point2D rightVertex = { this->getCenter().x() + other_box_sector, this->getCenter().y() - other_box_sector };
				float commonSquare = abs((leftVertex.x() - this->getCenter().x()) * (rightVertex.y() - this->getCenter().y()) -
					(rightVertex.x() - this->getCenter().x()) * (leftVertex.y() - this->getCenter().y())) / 2.0;
				float square1 = abs((leftVertex.x() - that.getCenter().x()) * (rightVertex.y() - that.getCenter().y()) -
					(rightVertex.x() - that.getCenter().x()) * (leftVertex.y() - that.getCenter().y())) / 2.0;
				float square2 = abs((rightVertex.x() - that.getCenter().x()) * (this->getCenter().y() - that.getCenter().y()) - 
					(this->getCenter().x() - that.getCenter().x()) * (rightVertex.y() - that.getCenter().y())) / 2.0;
				float square3 = abs((this->getCenter().x() - that.getCenter().x()) * (leftVertex.y() - that.getCenter().y()) -
					(leftVertex.x() - that.getCenter().x()) * (this->getCenter().y() - that.getCenter().y())) / 2.0;
				if (abs(commonSquare - (square1 + square2 + square3)) <= 0.01f)
					side_of_other_box->at(top) = true;
			}
			if (i == right)
			{
				Point2D uppVertex = { this->getCenter().x() + other_box_sector, this->getCenter().y() - other_box_sector };
				Point2D lowVertex = { this->getCenter().x() + other_box_sector, this->getCenter().y() + other_box_sector };
				float commonSquare = abs((uppVertex.x() - this->getCenter().x()) * (lowVertex.y() - this->getCenter().y()) -
					(lowVertex.x() - this->getCenter().x()) * (uppVertex.y() - this->getCenter().y())) / 2.0;
				float square1 = abs((uppVertex.x() - that.getCenter().x()) * (lowVertex.y() - that.getCenter().y()) -
					(lowVertex.x() - that.getCenter().x()) * (uppVertex.y() - that.getCenter().y())) / 2.0;
				float square2 = abs((lowVertex.x() - that.getCenter().x()) * (this->getCenter().y() - that.getCenter().y()) -
					(this->getCenter().x() - that.getCenter().x()) * (lowVertex.y() - that.getCenter().y())) / 2.0;
				float square3 = abs((this->getCenter().x() - that.getCenter().x()) * (uppVertex.y() - that.getCenter().y()) -
					(uppVertex.x() - that.getCenter().x()) * (this->getCenter().y() - that.getCenter().y())) / 2.0;
				if (abs(commonSquare - (square1 + square2 + square3)) <= 0.01f)
					side_of_other_box->at(right) = true;
			}
			if (i == bottom)
			{
				Point2D leftVertex = { this->getCenter().x() - other_box_sector, this->getCenter().y() + other_box_sector };
				Point2D rightVertex = { this->getCenter().x() + other_box_sector, this->getCenter().y() + other_box_sector };
				float commonSquare = abs((rightVertex.x() - this->getCenter().x()) * (leftVertex.y() - this->getCenter().y()) -
					(leftVertex.x() - this->getCenter().x()) * (rightVertex.y() - this->getCenter().y())) / 2.0;
				float square1 = abs((rightVertex.x() - that.getCenter().x()) * (leftVertex.y() - that.getCenter().y()) -
					(leftVertex.x() - that.getCenter().x()) * (rightVertex.y() - that.getCenter().y())) / 2.0;
				float square2 = abs((leftVertex.x() - that.getCenter().x()) * (this->getCenter().y() - that.getCenter().y()) -
					(this->getCenter().x() - that.getCenter().x()) * (leftVertex.y() - that.getCenter().y())) / 2.0;
				float square3 = abs((this->getCenter().x() - that.getCenter().x()) * (rightVertex.y() - that.getCenter().y()) -
					(rightVertex.x() - that.getCenter().x()) * (this->getCenter().y() - that.getCenter().y())) / 2.0;
				if (abs(commonSquare - (square1 + square2 + square3)) <= 0.01f)
					side_of_other_box->at(bottom) = true;
			}
			if (i == left)
			{
				Point2D uppVertex = { this->getCenter().x() - other_box_sector, this->getCenter().y() - other_box_sector };
				Point2D lowVertex = { this->getCenter().x() - other_box_sector, this->getCenter().y() + other_box_sector };
				float commonSquare = abs((lowVertex.x() - this->getCenter().x()) * (uppVertex.y() - this->getCenter().y()) -
					(uppVertex.x() - this->getCenter().x()) * (lowVertex.y() - this->getCenter().y())) / 2.0;
				float square1 = abs((lowVertex.x() - that.getCenter().x()) * (uppVertex.y() - that.getCenter().y()) -
					(uppVertex.x() - that.getCenter().x()) * (lowVertex.y() - that.getCenter().y())) / 2.0;
				float square2 = abs((uppVertex.x() - that.getCenter().x()) * (this->getCenter().y() - that.getCenter().y()) -
					(this->getCenter().x() - that.getCenter().x()) * (uppVertex.y() - that.getCenter().y())) / 2.0;
				float square3 = abs((this->getCenter().x() - that.getCenter().x()) * (lowVertex.y() - that.getCenter().y()) -
					(lowVertex.x() - that.getCenter().x()) * (this->getCenter().y() - that.getCenter().y())) / 2.0;
				if (abs(commonSquare - (square1 + square2 + square3)) <= 0.01f)
					side_of_other_box->at(left) = true;
			}
		}
		return side_of_other_box;
	}

	// S=1/2[(x1-x3)(y2-y3)-(x2-x3)(y2-y3)] 

	bool isBoxNear(const Box2D& that) const
	{
		return distanceToOtherBox(that) <= 200.f;
	}

	float otherBoxCenterWidth(const Box2D& that) const
	{
		float thisCenterX = this->getCenter().x();
		float thatCenterX = that.getCenter().x();
		return thatCenterX - thisCenterX;
	}

	float otherBoxCenterHeight(const Box2D& that) const
	{
		float thisCenterY = this->getCenter().y();
		float thatCenterY = that.getCenter().y();
		return  thatCenterY - thisCenterY;
	}

	float distanceToOtherBox(const Box2D& that) const
	{
		float width = std::max(this->getCenter().x(), that.getCenter().x()) - 
			std::min(this->getCenter().x(), that.getCenter().x());
		float height = std::max(this->getCenter().y(), that.getCenter().y()) -
			std::min(this->getCenter().y(), that.getCenter().y());
		return sqrt(width * width + height * height);
	}

	const Point2D getCenter() const
	{
		return Point2D((_lowVertex.x() + _uppVertex.x()) / 2.f, 
			(_lowVertex.y() + _uppVertex.y()) / 2.f);
	}

	friend std::ostream& operator << (std::ostream& os, Box2D box)
	{
		os << "Box2D {" << box._uppVertex << ";" << box._uppVertex << "}";
		return os;
	}

	float width()
	{
		return _lowVertex.x() - _uppVertex.x();
	}

	float height()
	{
		return _lowVertex.y() - _uppVertex.y();
	}

	void setUppVertex(const Point2D& point) { _uppVertex = point; }
	void setLowVertex(const Point2D& point) { _lowVertex = point; }
	const Point2D& getUppVertex() const { return _uppVertex; }
	const Point2D& getLowVertex() const { return _lowVertex; }

	friend sf::Packet& operator >> (sf::Packet& packet, Box2D& box)
	{
		return packet >> box._lowVertex >> box._uppVertex;
	}

	friend sf::Packet& operator << (sf::Packet& packet, const Box2D& box)
	{
		return packet << box._lowVertex << box._uppVertex;
	}
private:
	void orderOfPoints()
	{
		if (_lowVertex.x() == _uppVertex.x() || _lowVertex.y() == _uppVertex.y())
		{
			//exception
			_lowVertex = _uppVertex = Point2D();
		}
		else
		{
			_lowVertex.setX(std::max(_lowVertex.x(), _uppVertex.x()));
			_lowVertex.setY(std::max(_lowVertex.y(), _uppVertex.y()));
			_uppVertex.setX(std::min(_lowVertex.x(), _uppVertex.x()));
			_uppVertex.setY(std::min(_lowVertex.y(), _uppVertex.y()));
		}
	}
	bool equalWithEps(float v1, float v2) const
	{
		return fabs(v1 - v2) < eps;
	}
	Point2D _uppVertex;
	Point2D _lowVertex;
};