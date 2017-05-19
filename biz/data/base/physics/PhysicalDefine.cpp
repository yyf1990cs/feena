#include "stdafx.h"
#include "PhysicalDefine.h"

sf::Vector2i getVerticalSegment(const sf::Vector2i& v)
{
	sf::Vector2i normal;
	normal.y = v.x;
	normal.x = -v.x * v.x / v.y;

	return normal;
}


std::ostream& operator<<(std::ostream& stream, const Point& point)
{
	stream << "(" << point.x << "," << point.y << ")";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const sf::Vector2i& point)
{
	stream << "(" << point.x << "," << point.y << ")";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Edge& edge)
{
	stream << "(" << edge.from->x << "," << edge.from->y << ") -> (" << edge.to->x << "," << edge.to->y << ")";
	return stream;
}

