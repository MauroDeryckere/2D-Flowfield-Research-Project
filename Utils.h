/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EVector2.h: Vector2D struct
/*=============================================================================*/
#ifndef UTILS
#define	UTILS

#include <iostream>
#include "math.h"

namespace utils
{
	constexpr auto PI_d{ 3.14159265358979323846 };
	constexpr auto PI_f{ 3.14159265358979323846f };

	template<typename T, typename = std::enable_if<std::is_pod<T>::value>>
	constexpr auto Square(T v)
	{
		return v * v;
	}

	template<typename T, typename = std::enable_if<std::is_pod<T>::value>>
	constexpr bool AreEqual(T a, T b, float precision = FLT_EPSILON)
	{
		if (abs(a - b) > precision)
			return false;
		return true;
	}

	constexpr float ToRadians(const float angle)
	{
		return angle * (static_cast<float>(PI_d) / 180.f);
	}
	constexpr float ToDegrees(const float angle)
	{
		return angle * (180.f / static_cast<float>(PI_d));
	}

#define ZeroVector2f Vector2f()
#define UnitVector2f Vector2f(1.f,1.f)

	//Point 2i
	struct Point2i
	{
		int x;
		int y;
	};

	//Recti
	struct Recti
	{
		int x;
		int y;
		int width;
		int height;
	};

	//Vector 2f
	struct Vector2f
	{
		//=== Datamembers ===
		float x = 0.0f;
		float y = 0.0f;

		//=== Constructors ===
		Vector2f() = default;
		Vector2f(float _x, float _y) :x(_x), y(_y) {};

//		//=== Vector Conversions Functions ===
//#ifdef USE_BOX2D
//		explicit Vector2f(const b2Vec2& v) : x(v.x), y(v.y) {};
//		Vector2f& operator=(const b2Vec2& v) { x = v.x; y = v.y; return *this; }
//		operator b2Vec2() const
//		{
//			return { x, y };
//		};
//#endif

		//=== Arithmetic Operators ===
		inline auto operator-(const Vector2f& v) const
		{
			return Vector2f(x - v.x, y - v.y);
		}
		inline auto operator-() const
		{
			return Vector2f(-x, -y);
		}
		inline auto operator*(float scale) const
		{
			return Vector2f(x * scale, y * scale);
		}
		inline auto operator/(float scale) const
		{
			const auto revScale = 1.0f / scale;
			return Vector2f(x * revScale, y * revScale);
		}

		//=== Compound Assignment Operators === //auto& for type deduction
		inline auto& operator+=(const Vector2f& v)
		{
			x += v.x; y += v.y; return *this;
		}
		inline auto& operator-=(const Vector2f& v)
		{
			x -= v.x; y -= v.y; return *this;
		}
		inline auto& operator*=(float scale)
		{
			x *= scale; y *= scale; return *this;
		}
		inline auto& operator/=(float scale)
		{
			const auto revScale = 1.0f / scale;
			x *= revScale; y *= revScale; return *this;
		}



		//=== Relational Operators ===
		inline auto operator==(const Vector2f& v) const /*Check if both components are equal*/
		{
			return AreEqual(x, v.x) && AreEqual(y, v.y);
		}
		inline auto operator!=(const Vector2f& v) const /*Check if one or both components are NOT equal*/
		{
			return !(*this == v);
		}

		//=== Member Access Operators ===
		inline float operator[](unsigned int i) const
		{
			return ((i == 0) ? x : y);
			//if (i >= 0 && i < 2)
			//	return ((i == 0) ? x : y);
			//throw; /*TODO : specify error thrown;*/
		}
		inline float& operator[](unsigned int i)
		{
			return ((i == 0) ? x : y);
			//if (i >= 0 && i < 2)
			//	return ((i == 0) ? x : y);
			//throw; /*TODO : specify error thrown;*/
		}

		//=== Internal Vector Functions ===
		inline auto Dot(const Vector2f& v) const
		{
			return x * v.x + y * v.y;
		}

		inline auto Cross(const Vector2f& v) const
		{
			return x * v.y - y * v.x;
		}

		inline auto GetAbs() const
		{
			return Vector2f(abs(x), abs(y));
		}

		inline auto MagnitudeSquared() const
		{
			return x * x + y * y;
		}

		inline auto Magnitude() const
		{
			return sqrtf(MagnitudeSquared());
		}

		inline float Normalize()
		{
			auto m = Magnitude();
			if (AreEqual(m, 0.f))
			{
				*this = ZeroVector2f;
				return 0.f;
			}

			auto invM = 1.f / m;
			x *= invM;
			y *= invM;

			return m;
		}

		inline Vector2f GetNormalized() const /*! Returns a normalized copy of this vector. This vector does not change.*/
		{
			auto v = Vector2f(*this);
			v.Normalize();
			return v;
		}

		inline auto DistanceSquared(const Vector2f& v) const
		{
			return Square(v.x - x) + Square(v.y - y);
		}

		inline auto Distance(const Vector2f& v) const
		{
			return sqrtf(DistanceSquared(v));
		}

		inline auto Clamp(float max) const
		{
			auto scale = max / Magnitude();
			scale = scale < 1.f ? scale : 1.f;
			return *this * scale;
		}
	};

	//=== Global Vector Operators ===
#pragma region GlobalVectorOperators
	inline auto operator+(const Vector2f& v, const Vector2f& v2)
	{
		return Vector2f(v.x + v2.x, v.y + v2.y);
	}

	inline auto operator* (float s, const Vector2f& v)
	{
		return Vector2f(s * v.x, s * v.y);
	}

	inline auto operator*(const Vector2f& a, const Vector2f& b)
	{
		return Vector2f(a.x * b.x, a.y * b.y);
	}

	inline auto operator/ (float s, const Vector2f& v)
	{
		const auto revScale = 1.0f / s;
		return Vector2f(revScale * v.x, revScale * v.y);
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector2f& rhs)
	{
		os << "(" << rhs.x << ", " << rhs.y << " )";
		return os;
	}
#pragma endregion //GlobalVectorOperators

	//=== Global Vector Functions ===
#pragma region GlobalVectorFunctions
	inline auto Dot(const Vector2f& v1, const Vector2f& v2)
	{
		return v1.Dot(v2);
	}

	inline auto Cross(const Vector2f& v1, const Vector2f& v2)
	{
		return v1.Cross(v2);
	}

	inline auto GetAbs(const Vector2f& v)
	{
		return v.GetAbs();
	}

	inline void Abs(Vector2f& v) /*! Make absolute Vector2f of this Vector2f */
	{
		v = v.GetAbs();
	}

	inline void Normalize(Vector2f& v)
	{
		v.Normalize();
	}

	inline auto GetNormalized(const Vector2f& v)
	{
		return v.GetNormalized();
	}

	inline auto DistanceSquared(const Vector2f& v1, const Vector2f& v2)
	{
		return v1.DistanceSquared(v2);
	}

	inline auto Distance(const Vector2f& v1, const Vector2f& v2)
	{
		return v1.Distance(v2);
	}

	inline auto Clamp(const Vector2f& v, float max)
	{
		auto scale = max / v.Magnitude();
		scale = scale < 1.f ? scale : 1.f;
		return v * scale;
	}
#pragma endregion //GlobalVectorFunctions

#pragma region ExtraFunctions
	///*! Random Vector2f */
	//inline Vector2f randomVector2(float max = 1.f)
	//{
	//	return{ randomBinomial(max),randomBinomial(max) };
	//}
	//inline Vector2f randomVector2(float min, float max)
	//{
	//	return{ randomFloat(min, max),randomFloat(min, max) };
	//}

	/*Creates a normalized vector from an angle in radians.  */
	inline Vector2f OrientationToVector(float orientation)
	{
		return Vector2f(cos(orientation), sin(orientation));
	}

	/*Calculates the orientation angle from a vector*/
	inline float VectorToOrientation(const Vector2f& vector)
	{
		return atan2f(vector.y, vector.x);
	}

	/*! Get Angle Between 2 vectors*/
	inline float AngleBetween(const Vector2f& v1, const Vector2f& v2) {
		float x = v1.Dot(v2);
		float y = v1.Cross(v2);
		return atan2(y, x);
	}

	inline bool IsPointInRect(const utils::Point2i& point, const utils::Recti rect)
	{
		if (point.x < rect.x || point.x >= rect.x + rect.width
		||  point.y > rect.y || point.y <= rect.y - rect.height)
		{
			return false;
		}
		return true;
	};

	inline bool IsOverlapping(const utils::Recti& r1, const utils::Recti& r2)
	{
		if (r1.x > r2.x + r2.width 
		|| r1.x + r1.width < r2.x
		|| r1.y <+ r2.y - r2.height 
		|| r1.y - r1.height > r2.y)
		{
			return false;
		}

		return true;
	};

#pragma endregion //ExtraFunctions
}
#endif