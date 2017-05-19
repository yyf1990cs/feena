#pragma once
#include <math.h>

enum
{
	CENTI_PIXEL = 100, //1像素 = 100厘像素
	GRAVITY_ACC = 98000,//98000
	PI_DEGREE = 180,
	JUMP_RISE_SPEED = 45000,
	JUMP_BACK_SPEED = 10000,
}; 


float const INVALID_RATIO = 999.0f;
double const INVALID_ANGLE = 999.0f;
float const LANDABLE_RATIO = PI / 3;

inline sf::Vector2f cp2p(sf::Vector2i v)
{
	return sf::Vector2f(float(v.x) / CENTI_PIXEL, float(v.y) / CENTI_PIXEL);
}
inline sf::Vector2f cp2p(sf::Vector2u v)
{
	return sf::Vector2f(float(v.x) / CENTI_PIXEL, float(v.y) / CENTI_PIXEL);
}
inline sf::Vector2i p2cp(sf::Vector2f v)
{
	return sf::Vector2i(int(v.x * CENTI_PIXEL), int(v.y * CENTI_PIXEL));
}

struct ConvexPolygon;
struct Edge;
struct Point : public sf::Vector2i
{
	Point(const sf::Vector2i& inputPoint, uint8 intputIndex)
		:sf::Vector2i(inputPoint)
		, index(intputIndex)
	{

	}

	const uint8 index;
	const Point* prev;
	const Point* next;
	const Edge* fromOf;
	const Edge* toOf;
	const ConvexPolygon* parent;
};


struct Edge
{
	Edge(const Point& p1, const Point& p2)
		:from(&p1)
		, to(&p2)
		, index(p1.index)
		, m_angle(INVALID_ANGLE)
	{
	}


	const uint8 index;
	const Point* from;
	const Point* to;
	const Edge* prev;
	const Edge* next;
	const ConvexPolygon* parent;

	
	double getAngle() const
	{
		if (m_angle != INVALID_ANGLE)
			return m_angle;
		m_angle = atan2(to->y - from->y, to->x - from->x);
		return m_angle;
	}

	float getRatio() const
	{
		if (to->x == from->x)
			return to->y > from->y ? INVALID_RATIO : -INVALID_RATIO;
		return float(to->y - from->y) / float(to->x - from->x);
	}

private:
	mutable double m_angle;
};


inline bool angleEqual(double angle1, double angle2)
{
	double offset = angle1 - angle2;
	if (offset <= 0.01f && offset >= -0.01f)
		return true;
	angle2 = angle2 <= 0 ? angle2 + PI : angle2 - PI;
	offset = angle1 - angle2;
	if (offset <= 0.01f && offset >= -0.01f)
		return true;
	return false;
}


std::ostream& operator<<(std::ostream& stream, const Point& point);
std::ostream& operator<<(std::ostream& stream, const sf::Vector2i& point);
std::ostream& operator<<(std::ostream& stream, const Edge& edge);

//必须以顺时针定义各个顶点
struct ConvexPolygon
{
	ConvexPolygon(const std::vector < sf::Vector2i >& input)
	{
		uint8 pCount = input.size();
		points.reserve(pCount);
		edges.reserve(pCount);
		for (uint8 index = 0; index < pCount; ++index)
		{
			const sf::Vector2i& v = input[index];
			points.emplace_back(v, index);
		}

		for (uint8 index = 0; index < pCount; ++index)
		{
			Point& p = points[index];
			Point* prevP = nullptr;
			Point* nextP = nullptr;
			p.parent = this;
			if (index == 0)
			{
				prevP = &points[pCount - 1];
				nextP = &points[index + 1];
			}
			else if (index == pCount - 1)
			{
				prevP = &points[index - 1];
				nextP = &points[0];
			}
			else
			{
				prevP = &points[index - 1];
				nextP = &points[index + 1];
			}

			p.prev = prevP;
			p.next = nextP;
			edges.emplace_back(p, *nextP);
			p.fromOf = &edges.back();
			nextP->toOf = &edges.back();
		}

		for (uint8 index = 0; index < pCount; ++index)
		{
			Edge& e = edges[index];
			e.parent = this;
			if (index == 0)
			{
				e.prev = &edges[pCount - 1];
				e.next = &edges[index + 1];
			}
			else if (index == pCount - 1)
			{
				e.prev = &edges[index - 1];
				e.next = &edges[0];
			}
			else
			{
				e.prev = &edges[index - 1];
				e.next = &edges[index + 1];
			}

		}
	}


	inline uint32 size() const
	{
		return points.size();
	}

	std::vector<const Point> points;
	std::vector<const Edge> edges;
};

struct AABB
{
	AABB()
		:lowerBound(0, 0)
		, upperBound(0,0)
	{
	}

	sf::Vector2u getSize() const
	{
		return sf::Vector2u(uint32(upperBound.x - lowerBound.x), uint32(upperBound.y - lowerBound.y));
	}

	void operator+=(const sf::Vector2i& v)
	{
		lowerBound += v;
		upperBound += v;
	}
	void operator-=(const sf::Vector2i& v)
	{
		lowerBound -= v;
		upperBound -= v;
	}
	
	AABB operator+(const sf::Vector2i& v) const
	{
		AABB tmp = *this;
		tmp.lowerBound += v;
		tmp.lowerBound += v;
		return tmp;
	}
	AABB operator-(const sf::Vector2i& v) const
	{
		AABB tmp = *this;
		tmp.lowerBound -= v;
		tmp.upperBound -= v;
		return tmp;
	}

	bool operator^(const AABB& b) const
	{
		if (lowerBound.x > b.upperBound.x)
			return false;
		if (lowerBound.y > b.upperBound.y)
			return false;
		if (upperBound.x < b.lowerBound.x)
			return false;
		if (upperBound.y < b.lowerBound.y)
			return false;
		return true;
	}

	sf::Vector2i lowerBound;//AABB的左上坐标（厘像素）
	sf::Vector2i upperBound;//AABB的右下坐标（厘像素）
};


sf::Vector2i getVerticalSegment(const sf::Vector2i& v);

uint64 inline getP2PDistanceSqrt(const sf::Vector2i& a, const sf::Vector2i& b)
{
	return ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

uint64 inline getP2SDistance(const sf::Vector2i& a, const sf::Vector2i& b1, const sf::Vector2i& b2)
{
	return ((a.x - b1.x)*(b2.y - b1.y) - (b2.x - b1.x)*(a.y - b1.y)) / (uint32)sqrt((b2.x - b1.x)*(b2.x - b1.x) + (b2.y - b1.y)*(b2.y - b1.y));
}

inline bool isRectCross(const sf::Vector2i& a, const sf::Vector2i& b, const sf::Vector2i& c, const sf::Vector2i& d)
{
	if (_min(a.x, b.x) > _max(c.x, d.x))
		return false;
	if (_min(a.y, b.y) > _max(c.y, d.y))
		return false;
	if (_max(a.x, b.x) < _min(c.x, d.x))
		return false;
	if (_max(a.y, b.y) < _min(c.y, d.y))
		return false;
}

inline bool isSegmentCross(const sf::Vector2i& a, const sf::Vector2i& b, const sf::Vector2i& c, const sf::Vector2i& d, sf::Vector2i& crossPoint)
{
	//先计算2个线段组成的矩形是否相交，排除一部分
	if (_min(a.x, b.x) > _max(c.x, d.x))
		return false;
	if (_min(a.y, b.y) > _max(c.y, d.y))
		return false;
	if (_max(a.x, b.x) < _min(c.x, d.x))
		return false;
	if (_max(a.y, b.y) < _min(c.y, d.y))
		return false;

	// 三角形abc 面积的2倍  
	int64 area_abc = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
	// 三角形abd 面积的2倍  
	int64 area_abd = (a.x - d.x) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);
	// 面积符号相同则两点在线段同侧,不相交 (对点在线段上的情况,本例当作不相交处理);  
	if ((area_abc > 0 && area_abd > 0) || (area_abc < 0 && area_abd < 0) || area_abc == 0 || area_abd == 0) { //
		return false;
	}

	// 三角形cda 面积的2倍  
	int64 area_cda = (c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
	// 三角形cdb 面积的2倍  
	// 注意: 这里有一个小优化.不需要再用公式计算面积,而是通过已知的三个面积加减得出.  
	int64 area_cdb = area_cda + area_abc - area_abd;
	if ((area_cda > 0 && area_cdb > 0) || (area_cda < 0 && area_cdb < 0) || area_cda == 0 || area_cda == 0) {//
		return false;
	}

	//计算交点坐标  
	double t = (double)area_cda / double(area_abd - area_abc);
	int64 dx = (int64)round(t*(double)(b.x - a.x));
	int64 dy = (int64)round(t*(double)(b.y - a.y));
	crossPoint.x = a.x + int32(dx);
	crossPoint.y = a.y + int32(dy);
	return true;
}