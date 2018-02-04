#pragma once

#define SIMD_SSE


#if defined(SIMD_SSE) // SSE

	#include <xmmintrin.h>
	using VectorData = __m128;

#elif defined(SIMD_NONE) // NONE

	using VectorData = float[4];

#else

	#error You must define one of simd instruction type

#endif


class VectorBase
{
public:
	static VectorBase Zero();

	VectorBase();
	VectorBase(float x, float y, float z, float w);
	~VectorBase();

	//void Swap(Vector& other); // TODO: 구현하기

	VectorBase& operator= (const VectorBase& rhs);

	VectorBase& operator= (float rhs);

	bool operator== (const VectorBase& rhs) const; // TODO: 개별 Vector구현으로

	bool operator!= (const VectorBase& rhs) const; // TODO: 개별 Vector구현으로

	VectorBase operator+ (const VectorBase& rhs) const;
	VectorBase operator- (const VectorBase& rhs) const;
	VectorBase operator* (float rhs) const;
	VectorBase operator* (const VectorBase& rhs) const;
	VectorBase operator/ (float rhs) const;
	VectorBase operator/ (const VectorBase& rhs) const;

	const VectorBase& operator+() const;
	VectorBase operator-() const;

	VectorBase& operator+= (const VectorBase& rhs);
	VectorBase& operator-= (const VectorBase& rhs);
	VectorBase& operator*= (float rhs);
	VectorBase& operator*= (const VectorBase& rhs);
	VectorBase& operator/= (float rhs);
	VectorBase& operator/= (const VectorBase& rhs);

protected:
	friend class Vector2;
	friend class Vector3;
	friend class Vector4;

	VectorData mData;

	friend VectorBase operator* (float lhs, const VectorBase& rhs);
	friend VectorBase operator/ (float lhs, const VectorBase& rhs);
};

VectorBase operator* (float lhs, const VectorBase& rhs);
VectorBase operator/ (float lhs, const VectorBase& rhs);

class Vector2 : public VectorBase
{
public:
	Vector2();
	Vector2(float x, float y);

	Vector2(const VectorBase& vec);
	Vector2& operator=(const VectorBase& vec);

	operator Vector3() const;
	operator Vector4() const;

	void GetFloat2(float* float2);

	VectorBase Length() const;
	VectorBase SquareLength() const;

	VectorBase Dot(const Vector2& rhs) const;
	static VectorBase Dot(const Vector2& lhs, const Vector2& rhs);

	void Normalize();
	VectorBase Normalized() const;

	VectorBase Cross(const Vector2& rhs) const;
	static VectorBase Cross(const Vector2& lhs, const Vector2& rhs);

private:
	friend class Vector3;
	friend class Vector4;
	Vector2(const VectorData vData);
};

class Vector3 : public VectorBase
{
public:
	Vector3();
	Vector3(float x, float y, float z);

	Vector3(const VectorBase& vec);
	Vector3& operator=(const VectorBase& vec);

	operator Vector2() const;
	operator Vector4() const;

	void GetFloat3(float* float3);

	VectorBase Length() const;
	VectorBase SquareLength() const;

	VectorBase Dot(const Vector3& rhs) const;
	static VectorBase Dot(const Vector3& lhs, const Vector3& rhs);

	void Normalize();
	VectorBase Normalized() const;

	VectorBase Cross(const Vector3& rhs) const;
	static VectorBase Cross(const Vector3& lhs, const Vector3& rhs);

private:
	friend class Vector2;
	friend class Vector4;
	Vector3(const VectorData vData);
};

class Vector4 : public VectorBase
{
public:
	Vector4();
	Vector4(float x, float y, float z, float w);

	Vector4(const VectorBase& vec);
	Vector4& operator=(const VectorBase& vec);

	operator Vector2() const;
	operator Vector3() const;

	void GetFloat4(float* float4);

	VectorBase Length() const;
	VectorBase SquareLength() const;

	VectorBase Dot(const Vector4& rhs) const;
	static VectorBase Dot(const Vector4& lhs, const Vector4& rhs);

	void Normalize();
	VectorBase Normalized() const;

	VectorBase Cross(const Vector4& rhs) const;
	static VectorBase Cross(const Vector4& lhs, const Vector4& rhs);

private:
	friend class Vector2;
	friend class Vector3;
	Vector4(const VectorData vData);
};

// Include inline function definition

#ifndef VECTOR_IMPLIMENTATION

	#if defined(SIMD_SSE) // SSE

		#include "Vector/VectorSSE.h"

	#elif defined(SIMD_NONE) // NONE

		#include "Vector/VectorArray.h"

	#endif

#endif // !VECTOR_IMPLIMENTATION