#pragma once

#include "serialization/json.h"

#include <cmath>
#include <iostream>

using json = nlohmann::json;

template<typename T>
struct Vector3D
{
	T x;
	T y;
	T z;

	Vector3D<T>() : x(), y(), z() {}

	Vector3D<T>(T x, T y, T z) : x(x), y(y), z(z) {}

	bool operator==(const Vector3D<T>& vec)
	{
		return x == vec.x && y == vec.y && z == vec.z;
	}

	Vector3D<T>& operator=(const Vector3D<T>& vec)
	{
		x = vec.x, y = vec.y, z = vec.z;
		return *this;
	}

	Vector3D<T>& operator*(const Vector3D<T>& vec)
	{
		x *= vec.x, y *= vec.y, z *= vec.z;
		return *this;
	}

	Vector3D<T>& operator*(T scale)
	{
		x *= scale, y *= scale, z *= scale;
		return *this;
	}

	Vector3D<T>& operator+(const Vector3D<T>& vec)
	{
		x += vec.x, y += vec.y, z += vec.z;
		return *this;
	}

	T norma()
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	Vector3D<T>& normalize()
	{
		T n = norma();
		x += x / n, y += y / n, z += z / n;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector3D<T>& vec);
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector3D<T>& vec)
{
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

typedef Vector3D<float>  Vectorf3D;
typedef Vector3D<double> Vectord3D;

template<typename T>
struct Quaternion
{
	T x;
	T y;
	T z;
	T w;

	Quaternion<T>() : x(), y(), z(), w() {}

	Quaternion<T>(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

	Quaternion<T>(T roll, T pitch, T yaw)
	{
		T t0 = std::cos(yaw * 0.5);
		T t1 = std::sin(yaw * 0.5);
		T t2 = std::cos(roll * 0.5);
		T t3 = std::sin(roll * 0.5);
		T t4 = std::cos(pitch * 0.5);
		T t5 = std::sin(pitch * 0.5);

		w = t0 * t2 * t4 + t1 * t3 * t5;
		x = t0 * t3 * t4 - t1 * t2 * t5;
		y = t0 * t2 * t5 + t1 * t3 * t4;
		z = t1 * t2 * t4 - t0 * t3 * t5;
	}

	bool operator==(const Quaternion<T>& q) const
	{
		return w == q.w && x == q.x && y == q.y && z == q.z;
	}

	Quaternion<T>& operator=(const Quaternion<T>& q)
	{
		w = q.w, x = q.x, y = q.y, z = q.z;
		return *this;
	}

	Quaternion<T>& operator*(const Quaternion<T>& q)
	{
		x = x * q.w + y * q.z - z * q.y + w * q.x;
		y = -x * q.z + y * q.w + z * q.x + w * q.y;
		z = x * q.y - y * q.x + z * q.w + w * q.z;
		w = -x * q.x - y * q.y - z * q.z + w * q.w;
		return *this;
	}

	Quaternion<T>& operator*(T scale)
	{
		w *= scale, x *= scale, y *= scale, z *= scale;
		return *this;
	}

	Quaternion<T>& operator+(const Quaternion<T>& q)
	{
		w += q.w, x += q.x, y += q.y, z += q.z;
		return *this;
	}

	T norma()
	{
		return std::sqrt(w * w + x * x + y * y + z * z);
	}

	Quaternion<T>& normalize()
	{
		T n = norma();
		w += w / n, x += x / n, y += y / n, z += z / n;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const Quaternion<T>& q);

	Vector3D<T> toEulerianAngle()
	{
		Vector3D<T> vec;
		T ysqr = y * y;

		// roll (x-axis rotation)
		T t0 = 2.0 * (w * x + y * z);
		T t1 = 1.0 - 2.0 * (x * x + ysqr);
		vec.x = std::atan2(t0, t1);

		// pitch (y-axis rotation)
		double t2 = 2.0 * (w * y - z * x);
		t2 = t2 > 1.0 ? 1.0 : t2;
		t2 = t2 < -1.0 ? -1.0 : t2;
		vec.y = std::asin(t2);

		// yaw (z-axis rotation)
		double t3 = 2.0 * (w * z + x * y);
		double t4 = 1.0 - 2.0 * (ysqr + z * z);
		vec.z = std::atan2(t3, t4);

		return vec;
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Quaternion<T>& q)
{
	os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
	return os;
}

typedef Quaternion<float>  Quatf;
typedef Quaternion<double> QuatD;

struct Position
{
	Vectorf3D m_pos;
	Vectorf3D m_rot;

	Position() : m_pos(), m_rot() {}
};

void to_json(json& j, const Vectorf3D& m);

void from_json(const json& j, Vectorf3D& m);

void to_json(json& j, const Position& m);

void from_json(const json& j, Position& m);