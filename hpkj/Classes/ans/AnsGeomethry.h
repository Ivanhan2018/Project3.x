/*
** ...
** author: hyw
** date: 2012.08.22
*/

#ifndef __ANS_GEOMETHRY_H__
#define __ANS_GEOMETHRY_H__


namespace ans{

// -----------------------------------------------------------------------
// TVector2
// -----------------------------------------------------------------------
template<typename T>
class TVector2 
{
private:
	typedef TVector2<T> MyType;

public:
	inline TVector2()
		: x ((T)0), y((T)0)	
	{}

	inline TVector2(T _x, T _y)
		: x(_x), y(_y)
	{}

	template<typename TT>
	inline TVector2(TT _x, TT _y)
		: x((T)_x), y((T)_y)
	{}

	inline void reset(const MyType &v2)
	{
		x = v2.x; y = v2.y;
	}

	template<typename TT>
	inline void reset(TT _x, TT _y)
	{
		x = (T)_x; y = (T)_y;
	}

	inline bool operator==(const MyType &v2) const { return x == v2.x && y == v2.y; }
	inline MyType operator+(const MyType &v2) const { return MyType(x + v2.x, y + v2.y); }
	inline MyType operator-(const MyType &v2) const	{ return MyType(x - v2.x, y - v2.y); }
	inline MyType operator*(const MyType &v2) const { return MyType(x * v2.x, y * v2.y); }
	inline MyType operator/(const MyType &v2) const { return MyType(x / v2.x, y / v2.y); }
	inline MyType& operator+=(const MyType &v2) { x += v2.x; y += v2.y; return *this; }
	inline MyType& operator-=(const MyType &v2) { x -= v2.x; y -= v2.y; return *this; }
	inline MyType& operator*=(const MyType &v2) { x *= v2.x; y *= v2.y; return *this; }
	inline MyType& operator/=(const MyType &v2) { x /= v2.x; y /= v2.y; return *this; }

	template<typename ST>
	inline MyType operator*(ST scale) const { return MyType(x * scale, y * scale); }
	template<typename ST>
	inline MyType operator/(ST scale) const { return MyType(x / scale, y / scale); }
	template<typename ST>
	inline MyType& operator*=(ST scale) { x *= scale; y *= scale; return *this; }
	template<typename ST>
	inline MyType& operator/=(ST scale) { x /= scale; y /= scale; return *this; }

public:
	T x, y;
};


// -----------------------------------------------------------------------
// TVector3
// -----------------------------------------------------------------------
template<typename T>
class TVector3
{
private:
	typedef TVector3<T> MyType;

public:
	inline TVector3()
		: x((T)0), y((T)0), z((T)0)
	{}

	inline TVector3(T _x, T _y, T _z)
		: x(_x), y(_y), z(_z)
	{}

	template<typename TT>
	inline TVector3(TT _x, TT _y, TT _z)
		: x((T)_x), y((T)_y), z((T)_z)
	{}

	inline void reset(const MyType &v3)
	{
		x = v3.x; y = v3.y; z = v3.z;
	}

	template<typename TT>
	inline void reset(TT _x, TT _y, TT _z)
	{
		x = (T)_x; y = (T)_y; z = (T)_z;
	}

	inline bool operator==(const MyType &v3) const { return x == v3.x && y == v3.y && z == v3.z; }
	inline MyType operator+(const MyType &v3) const { return MyType(x + v3.x, y + v3.y, z + v3.z); }
	inline MyType operator-(const MyType &v3) const	{ return MyType(x - v3.x, y - v3.y, z - v3.z); }
	inline MyType operator*(const MyType &v3) const { return MyType(x * v3.x, y * v3.y, z * v3.z); }
	inline MyType operator/(const MyType &v3) const { return MyType(x / v3.x, y / v3.y, z / v3.z); }
	inline MyType& operator+=(const MyType &v3) { x += v3.x; y += v3.y; z += v3.z; return *this; }
	inline MyType& operator-=(const MyType &v3) { x -= v3.x; y -= v3.y; z -= v3.z; return *this; }
	inline MyType& operator*=(const MyType &v3) { x *= v3.x; y *= v3.y; z *= v3.z; return *this; }
	inline MyType& operator/=(const MyType &v3) { x /= v3.x; y /= v3.y; z /= v3.z; return *this; }

	template<typename ST>
	inline MyType operator*(ST scale) const { return MyType(x * scale, y * scale, z * scale); }
	template<typename ST>
	inline MyType operator/(ST scale) const { return MyType(x / scale, y / scale, z * scale); }
	template<typename ST>
	inline MyType& operator*=(ST scale) { x *= scale; y *= scale; z *= scale; return *this; }
	template<typename ST>
	inline MyType& operator/=(ST scale) { x /= scale; y /= scale; z *= scale; return *this; }

public:
	T x, y, z;
};


// -----------------------------------------------------------------------
// TVector4
// -----------------------------------------------------------------------
template<typename T>
class TVector4
{
private:
	typedef TVector4<T> MyType;

public:
	inline TVector4()
		: x((T)0), y((T)0), z((T)0), w((T)0)
	{}

	inline TVector4(T _x, T _y, T _z, T _w)
		: x(_x), y(_y), z(_z), w(_w)
	{}

	template<typename TT>
	inline TVector4(TT _x, TT _y, TT _z, TT _w)
		: x((T)_x), y((T)_y), z((T)_z), w((T)_w)
	{}

	inline void reset(const MyType &v4)
	{
		x = v4.x; y = v4.y; z = v4.z; z = v4.w;
	}

	template<typename TT>
	inline void reset(TT _x, TT _y, TT _z, TT _w)
	{
		x = (T)_x; y = (T)_y; z = (T)_z; w = (T)_w;
	}

	inline bool operator==(const MyType &v4) const { return x == v4.x && y == v4.y && z == v4.z && w == v4.w; }
	inline MyType operator+(const MyType &v4) const { return MyType(x + v4.x, y + v4.y, z + v4.z, w + v4.w); }
	inline MyType operator-(const MyType &v4) const	{ return MyType(x - v4.x, y - v4.y, z - v4.z, w - v4.w); }
	inline MyType operator*(const MyType &v4) const { return MyType(x * v4.x, y * v4.y, z * v4.z, w * v4.w); }
	inline MyType operator/(const MyType &v4) const { return MyType(x / v4.x, y / v4.y, z / v4.z, w / v4.w); }
	inline MyType& operator+=(const MyType &v4) { x += v4.x; y += v4.y; z += v4.z; w += v4.w; return *this; }
	inline MyType& operator-=(const MyType &v4) { x -= v4.x; y -= v4.y; z -= v4.z; w -= v4.w; return *this; }
	inline MyType& operator*=(const MyType &v4) { x *= v4.x; y *= v4.y; z *= v4.z; w *= v4.w; return *this; }
	inline MyType& operator/=(const MyType &v4) { x /= v4.x; y /= v4.y; z /= v4.z; w /= v4.w; return *this; }

	template<typename ST>
	inline MyType operator*(ST scale) const { return MyType(x * scale, y * scale, z * scale, w * scale); }
	template<typename ST>
	inline MyType operator/(ST scale) const { return MyType(x / scale, y / scale, z * scale, w / scale); }
	template<typename ST>
	inline MyType& operator*=(ST scale) { x *= scale; y *= scale; z *= scale; w *= scale; return *this; }
	template<typename ST>
	inline MyType& operator/=(ST scale) { x /= scale; y /= scale; z *= scale; w /= scale; return *this; }

public:
	T x, y, z, w;
};


}

#endif