#include "Math.h"

namespace cube
{
	float Math::Rad2Deg(float rad)
	{
		return rad * 180.0f / Pi;
	}

	float Math::Deg2Rad(float deg)
	{
		return deg * Pi / 180.0f;
	}

	float Math::Sin(float rad) // TODO: ���� ����
	{
		return PrecisionSin(rad);
	}

	float Math::Cos(float rad) // TODO: ���� ����
	{
		return PrecisionCos(rad);
	}

	float Math::Tan(float rad) // TODO: ���� ����
	{
		return PrecisionTan(rad);
	}

	float Math::PrecisionSin(float rad)
	{
		return std::sinf(rad);
	}

	float Math::PrecisionCos(float rad)
	{
		return std::cosf(rad);
	}

	float Math::PrecisionTan(float rad)
	{
		return std::tanf(rad);
	}
}