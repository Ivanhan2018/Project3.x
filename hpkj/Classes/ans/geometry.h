/*
** 实现 cocos2dx 的一些简单扩展
** author: hyw
** date: 2012.08.01
*/

#ifndef __COCOS2D_EX_H__
#define __COCOS2D_EX_H__

#include "cocos2d.h"
#include "AnsGeomethry.h"

using namespace cocos2d;

// -----------------------------------------------------------------------
// CCGeometry
// -----------------------------------------------------------------------
template<typename T>
class CCVector2 : public ans::TVector2<T>
{
public:
	inline CCVector2() 
		: ans::TVector2<float>()
	{}
	inline CCVector2(float x, float y)
		: ans::TVector2<float>(x, y)
	{}
	inline CCVector2(const Vec2 &point)
		: ans::TVector2<float>(point.x, point.y)
	{}
	inline CCVector2(const cocos2d::Size &size)
		: ans::TVector2<float>(size.width, size.height)
	{}

	// -----------------------------------------------
	inline CCVector2 operator+(const CCVector2 &v2) const { return CCVector2(this->x + v2.x, this->y + v2.y); }
	inline CCVector2 operator-(const CCVector2 &v2) const { return CCVector2(this->x - v2.x, this->y - v2.y); }
	inline CCVector2 operator*(const CCVector2 &v2) const { return CCVector2(this->x * v2.x, this->y * v2.y); }
	inline CCVector2 operator/(const CCVector2 &v2) const { return CCVector2(this->x / v2.x, this->y / v2.y); }
	inline CCVector2& operator+=(const CCVector2 &v2) { this->x += v2.x; this->y += v2.y; return *this; }
	inline CCVector2& operator-=(const CCVector2 &v2) { this->x -= v2.x; this->y -= v2.y; return *this; }
	inline CCVector2& operator*=(const CCVector2 &v2) { this->x *= v2.x; this->y *= v2.y; return *this; }
	inline CCVector2& operator/=(const CCVector2 &v2) { this->x /= v2.x; this->y /= v2.y; return *this; }

	template<typename ST>
	inline CCVector2 operator*(ST scale) const { return CCVector2(this->x * scale, this->y * scale); }
	template<typename ST>
	inline CCVector2 operator/(ST scale) const { return CCVector2(this->x / scale, this->y / scale); }
	template<typename ST>
	inline CCVector2& operator*=(ST scale) { this->x *= scale; this->y *= scale; return *this; }
	template<typename ST>
	inline CCVector2& operator/=(ST scale) { this->x /= scale; this->y /= scale; return *this; }

	// -----------------------------------------------
	inline CCVector2& operator=(const Vec2 &point) { this->x = point.x; this->y = point.y; return *this;}
	inline CCVector2& operator=(const cocos2d::Size &size) { this->x = size.width; this->y = size.height; return *this;}
	inline operator Vec2() const { return Vec2(this->x, this->y); }
	inline operator cocos2d::Size() const { return cocos2d::Size(this->x, this->y); }

	inline CCVector2 operator+(const Vec2 &point) const { return CCVector2(this->x + point.x, this->y + point.y);}
	inline CCVector2 operator-(const Vec2 &point) const { return CCVector2(this->x - point.x, this->y - point.y);}
	inline CCVector2 operator*(const Vec2 &point) const { return CCVector2(this->x * point.x, this->y * point.y);}
	inline CCVector2 operator/(const Vec2 &point) const { return CCVector2(this->x / point.x, this->y / point.y);}
	inline CCVector2& operator+=(const Vec2 &point) { this->x += point.x; this->y += point.y; return *this; }
	inline CCVector2& operator-=(const Vec2 &point) { this->x -= point.x; this->y -= point.y; return *this; }
	inline CCVector2& operator*=(const Vec2 &point) { this->x *= point.x; this->y *= point.y; return *this; }
	inline CCVector2& operator/=(const Vec2 &point) { this->x /= point.x; this->y /= point.y; return *this; }

	inline CCVector2 operator+(const cocos2d::Size &size) const { return CCVector2(this->x + size.width, this->y + size.height);}
	inline CCVector2 operator-(const cocos2d::Size &size) const { return CCVector2(this->x - size.width, this->y - size.height);}
	inline CCVector2 operator*(const cocos2d::Size &size) const { return CCVector2(this->x * size.width, this->y * size.height);}
	inline CCVector2 operator/(const cocos2d::Size &size) const { return CCVector2(this->x / size.width, this->y / size.height);}
	inline CCVector2& operator+=(const cocos2d::Size &size) { this->x += size.width; this->y += size.height; return *this; }
	inline CCVector2& operator-=(const cocos2d::Size &size) { this->x -= size.width; this->y -= size.height; return *this; }
	inline CCVector2& operator*=(const cocos2d::Size &size) { this->x *= size.width; this->y *= size.height; return *this; }
	inline CCVector2& operator/=(const cocos2d::Size &size) { this->x /= size.width; this->y /= size.height; return *this; }
};

typedef CCVector2<float> Vector2;
typedef ans::TVector3<float> Vector3;
typedef ans::TVector4<float> Vector4;

#define vv2(x, y) Vector2((x), (y))
#define vv3(x, y, z) Vector3((x), (y), (z))
#define vv4(x, y, z, w) Vector4((x), (y), (z), (w))

// -------------------------------------------------------------
// Vec2
// -------------------------------------------------------------
// Vec2 + Vec2
inline Vector2 operator+(const Vec2 &p1, const Vec2 &p2) { return vv2(p1.x + p2.x, p1.y + p2.y); }

// Vec2 - Vec2
inline Vector2 operator-(const Vec2 &p1, const Vec2 &p2) { return vv2(p1.x - p2.x, p1.y - p2.y); }

// Vec2 * Vec2
inline Vector2 operator*(const Vec2 &point1, const Vec2 &point2) { return vv2(point1.x * point2.x, point1.y * point2.y); }

// Vec2 * scale
inline Vector2 operator*(const Vec2 &point, float scale) { return vv2(point.x * scale, point.y * scale); }

// scale * Vec2
inline Vector2 operator*(float scale, const Vec2 &point) { return vv2(point.x * scale, point.y * scale); }

// Vec2 / Vec2
inline Vector2 operator/(const Vec2 &point1, const Vec2 &point2) { return vv2(point1.x / point2.x, point1.y / point2.y);}

// Vec2 / scale
inline Vector2 operator/(const Vec2 &point, float scale) { return vv2(point.x / scale, point.y / scale);}

// Vec2 += Vec2
inline Vec2& operator+=(Vec2 &p1, const Vec2 &p2) { p1.x += p2.x; p1.y += p2.y; return p1; }

// Vec2 -= Vec2
inline Vec2& operator-=(Vec2 &p1, const Vec2 &p2) { p1.x -= p2.x; p1.y -= p2.y; return p1; }

// Vec2 *= scale
inline Vec2& operator*=(Vec2 &point, float scale) { point.x *= scale; point.y *= scale; return point; }

// Vec2 /= scale
inline Vec2& operator/=(Vec2 &point, float scale) { point.x /= scale; point.y /= scale; return point; }


// ---------------------------------------------------
// Vec2 + cocos2d::Size
inline Vector2 operator+(const Vec2 &point, const cocos2d::Size &size) { return vv2(point.x + size.width, point.y + size.height); }

// Vec2 - cocos2d::Size
inline Vector2 operator-(const Vec2 &point, const cocos2d::Size &size) { return vv2(point.x - size.width, point.y - size.height); }

// Vec2 * cocos2d::Size
inline Vector2 operator*(const Vec2 &point, const cocos2d::Size &size) { return vv2(point.x * size.width, point.y * size.height); }

// Vec2 / cocos2d::Size
inline Vector2 operator/(const Vec2 &point, const cocos2d::Size &size) { return vv2(point.x / size.width, point.y / size.height); }

// Vec2 += cocos2d::Size
inline Vec2& operator+=(Vec2 &point, const cocos2d::Size &size) { point.x += size.width, point.y += size.height; return point;}

// Vec2 -= cocos2d::Size
inline Vec2& operator-=(Vec2 &point, const cocos2d::Size &size) { point.x -= size.width, point.y -= size.height; return point;}

// Vec2 *= cocos2d::Size
inline Vec2& operator*=(Vec2 &point, const cocos2d::Size &size) { point.x *= size.width, point.y *= size.height; return point;}

// Vec2 /= cocos2d::Size
inline Vec2& operator/=(Vec2 &point, const cocos2d::Size &size) { point.x /= size.width, point.y /= size.height; return point;}


// -------------------------------------------------------------
// cocos2d::Size
// -------------------------------------------------------------
// cocos2d::Size + cocos2d::Size
inline Vector2 operator+(const cocos2d::Size &p1, const cocos2d::Size &p2) { return vv2(p1.width + p2.width, p1.height + p2.height); }

// cocos2d::Size - cocos2d::Size
inline Vector2 operator-(const cocos2d::Size &p1, const cocos2d::Size &p2) { return vv2(p1.width - p2.width, p1.height - p2.height); }

// cocos2d::Size * scale
inline Vector2 operator*(const cocos2d::Size &point, float scale) { return vv2(point.width * scale, point.height * scale); }

// scale * cocos2d::Size
inline Vector2 operator*(float scale, const cocos2d::Size &point) { return vv2(point.width * scale, point.height * scale); }

// cocos2d::Size / scale
inline Vector2 operator/(const cocos2d::Size &point, float scale) { return vv2(point.width / scale, point.height / scale);}

// cocos2d::Size += cocos2d::Size
inline cocos2d::Size& operator+=(cocos2d::Size &p1, const cocos2d::Size &p2) { p1.width += p2.width; p1.height += p2.height; return p1; }

// cocos2d::Size -= cocos2d::Size
inline cocos2d::Size& operator-=(cocos2d::Size &p1, const cocos2d::Size &p2) { p1.width -= p2.width; p1.height -= p2.height; return p1; }

// cocos2d::Size *= scale
inline cocos2d::Size& operator*=(cocos2d::Size &point, float scale) { point.width *= scale; point.height *= scale; return point; }

// cocos2d::Size /= scale
inline cocos2d::Size& operator/=(cocos2d::Size &point, float scale) { point.width /= scale; point.height /= scale; return point; }


// -------------------------------------------------------------
// Vector2 & Vec2 & cocos2d::Size
// -------------------------------------------------------------
// Vec2 + Vector2
inline Vector2 operator+(const Vec2 &point, const Vector2 &v2) { return Vector2(v2.x + point.x, v2.y + point.y);}

// Vec2 - Vector2
inline Vector2 operator-(const Vec2 &point, const Vector2 &v2) { return Vector2(point.x - v2.x, point.y - v2.y);}

// Vec2 * Vector2
inline Vector2 operator*(const Vec2 &point, const Vector2 &v2) { return Vector2(v2.x * point.x, v2.y * point.y);}

// Vec2 / Vector2
inline Vector2 operator/(const Vec2 &point, const Vector2 &v2) { return Vector2(v2.x / point.x, v2.y / point.y);}

// ---------------------------------------------------
// cocos2d::Size + Vector2
inline Vector2 operator+(const cocos2d::Size &size, const Vector2 &v2) { return Vector2(v2.x + size.width, v2.y + size.height);}

// cocos2d::Size - Vector2
inline Vector2 operator-(const cocos2d::Size &size, const Vector2 &v2) { return Vector2(size.width - v2.x, size.height - v2.y);}

// cocos2d::Size * Vector2
inline Vector2 operator*(const cocos2d::Size &size, const Vector2 &v2) { return Vector2(v2.x * size.width, v2.y * size.height);}

// cocos2d::Size / Vector2
inline Vector2 operator/(const cocos2d::Size &size, const Vector2 &v2) { return Vector2(v2.x / size.width, v2.y / size.height);}


// -------------------------------------------------------------
// Rect
// -------------------------------------------------------------
// Rect + Vec2
inline Rect operator+(const Rect &rect, const Vec2 &point){return Rect(rect.origin.x + point.x, rect.origin.y + point.y, rect.size.width, rect.size.height);}

// Rect - Vec2
inline Rect operator-(const Rect &rect, const Vec2 &point){return Rect(rect.origin.x - point.x, rect.origin.y - point.y, rect.size.width, rect.size.height);}

// Rect * scale
inline Rect operator*(const Rect &rect, float scale) { return Rect(rect.origin.x, rect.origin.y, rect.size.width * scale, rect.size.height * scale);}

// scale * Rect
inline Rect operator*(float scale, const Rect &rect) { return Rect(rect.origin.x, rect.origin.y, rect.size.width * scale, rect.size.height * scale);}

// Rect / scale
inline Rect operator/(const Rect &rect, float scale) { return Rect(rect.origin.x, rect.origin.y, rect.size.width / scale, rect.size.height / scale);}

// Rect += Vec2
inline Rect& operator+=(Rect &rect, const Vec2 &point) { rect.origin.x += point.x; rect.origin.y += point.y; return rect;}

// Rect -= Vec2
inline Rect& operator-=(Rect &rect, const Vec2 &point) { rect.origin.x -= point.x; rect.origin.y -= point.y; return rect;}

// Rect *= scale
inline Rect& operator*=(Rect &rect, float scale) { rect.size *= scale; return rect; }

// Rect /= sacle
inline Rect& operator/=(Rect &rect, float scale) { rect.size /= scale; return rect; }


#endif