#pragma once

#include "String.h"
#include "format.h"

#define SIMD_SSE


#if defined(SIMD_SSE) // SSE

#include <xmmintrin.h>
namespace cube
{
	using VectorData = __m128;
} // namespace cube

#elif defined(SIMD_NONE) // NONE

namespace cube
{
	using VectorData = float[4];
} // namespace cube

#else

	#error You must define one of simd instruction type

#endif

namespace cube
{
	struct Float2
	{
		float x;
		float y;
	};
	// Float2 formatting
	inline void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Float2& f2)
	{
		f.writer().write("({:.3f}, {:.3f})", f2.x, f2.y);
	}
	inline void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Float2& f2)
	{
		f.writer().write(L"({:.3f}, {:.3f})", f2.x, f2.y);
	}
	inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short*& format_str, const Float2& f2)
	{
		f.writer().write((const unsigned short*)u"({:.3f}, {:.3f})", f2.x, f2.y);
	}
	inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t*& format_str, const Float2& f2)
	{
		f.writer().write(u"({:.3f}, {:.3f})", f2.x, f2.y);
	}
	inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t*& format_str, const Float2& f2)
	{
		f.writer().write(U"({:.3f}, {:.3f})", f2.x, f2.y);
	}

	struct Float3
	{
		float x;
		float y;
		float z;
	};
	// Float3 formatting
	inline void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Float3& f3)
	{
		f.writer().write("({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}
	inline void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Float3& f3)
	{
		f.writer().write(L"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}
	inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short*& format_str, const Float3& f3)
	{
		f.writer().write((const unsigned short*)u"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}
	inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t*& format_str, const Float3& f3)
	{
		f.writer().write(u"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}
	inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t*& format_str, const Float3& f3)
	{
		f.writer().write(U"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}

	struct Float4
	{
		float x;
		float y;
		float z;
		float w;
	};
	// Float4 formatting
	inline void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Float4& f4)
	{
		f.writer().write("({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}
	inline void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Float4& f4)
	{
		f.writer().write(L"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}
	inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short*& format_str, const Float4& f4)
	{
		f.writer().write((const unsigned short*)u"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}
	inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t*& format_str, const Float4& f4)
	{
		f.writer().write(u"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}
	inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t*& format_str, const Float4& f4)
	{
		f.writer().write(U"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}

	// Forward declarations
	class VectorBase;
	class Vector4;
	class Matrix;
	class MatrixUtility;
	VectorBase operator*(const Vector4& lhs, const Matrix& rhs);

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
		friend class Matrix;
		friend class MatrixUtility;

		VectorData mData;

		friend VectorBase operator* (float lhs, const VectorBase& rhs);
		friend VectorBase operator/ (float lhs, const VectorBase& rhs);
		friend VectorBase operator* (const Vector4& lhs, const Matrix& rhs);
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

		Float2 GetFloat2() const;

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

	// Vector2 formatting
	inline void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Vector2& vec2)
	{
		Float2 f2 = vec2.GetFloat2();
		f.writer().write("({:.3f}, {:.3f})", f2.x, f2.y);
	}
	inline void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Vector2& vec2)
	{
		Float2 f2 = vec2.GetFloat2();
		f.writer().write(L"({:.3f}, {:.3f})", f2.x, f2.y);
	}
	inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short*& format_str, const Vector2& vec2)
	{
		Float2 f2 = vec2.GetFloat2();
		f.writer().write((const unsigned short*)u"({:.3f}, {:.3f})", f2.x, f2.y);
	}
	inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t*& format_str, const Vector2& vec2)
	{
		Float2 f2 = vec2.GetFloat2();
		f.writer().write(u"({:.3f}, {:.3f})", f2.x, f2.y);
	}
	inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t*& format_str, const Vector2& vec2)
	{
		Float2 f2 = vec2.GetFloat2();
		f.writer().write(U"({:.3f}, {:.3f})", f2.x, f2.y);
	}

	class Vector3 : public VectorBase
	{
	public:
		Vector3();
		Vector3(float x, float y, float z);

		Vector3(const VectorBase& vec);
		Vector3& operator=(const VectorBase& vec);

		operator Vector2() const;
		operator Vector4() const;

		Float3 GetFloat3() const;

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

	// Vector3 formatting
	inline void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Vector3& vec3)
	{
		Float3 f3 = vec3.GetFloat3();
		f.writer().write("({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}
	inline void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Vector3& vec3)
	{
		Float3 f3 = vec3.GetFloat3();
		f.writer().write(L"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}
	inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short*& format_str, const Vector3& vec3)
	{
		Float3 f3 = vec3.GetFloat3();
		f.writer().write((const unsigned short*)u"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}
	inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t*& format_str, const Vector3& vec3)
	{
		Float3 f3 = vec3.GetFloat3();
		f.writer().write(u"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}
	inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t*& format_str, const Vector3& vec3)
	{
		Float3 f3 = vec3.GetFloat3();
		f.writer().write(U"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
	}

	class Vector4 : public VectorBase
	{
	public:
		Vector4();
		Vector4(float x, float y, float z, float w);

		Vector4(const VectorBase& vec);
		Vector4& operator=(const VectorBase& vec);

		operator Vector2() const;
		operator Vector3() const;

		Float4 GetFloat4() const;

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

	// Vector4 formatting
	inline void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Vector4& vec4)
	{
		Float4 f4 = vec4.GetFloat4();
		f.writer().write("({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}
	inline void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Vector4& vec4)
	{
		Float4 f4 = vec4.GetFloat4();
		f.writer().write(L"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}
	inline void format_arg(fmt::BasicFormatter<unsigned short>& f, const unsigned short*& format_str, const Vector4& vec4)
	{
		Float4 f4 = vec4.GetFloat4();
		f.writer().write((const unsigned short*)u"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}
	inline void format_arg(fmt::BasicFormatter<char16_t>& f, const char16_t*& format_str, const Vector4& vec4)
	{
		Float4 f4 = vec4.GetFloat4();
		f.writer().write(u"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}
	inline void format_arg(fmt::BasicFormatter<char32_t>& f, const char32_t*& format_str, const Vector4& vec4)
	{
		Float4 f4 = vec4.GetFloat4();
		f.writer().write(U"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
	}
} // namespace cube

// Include inline function definition

#ifndef VECTOR_IMPLEMENTATION

	#if defined(SIMD_SSE) // SSE

		#include "Vector/VectorSSE.h"

	#elif defined(SIMD_NONE) // NONE

		#include "Vector/VectorArray.h"

	#endif

#endif // !VECTOR_IMPLEMENTATION
