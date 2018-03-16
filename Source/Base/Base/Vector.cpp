#include "Vector.h"

// Vector2 formatting
void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Vector2& vec2)
{
	float vec2F[2];
	vec2.GetFloat2(vec2F);
	f.writer().write("({:.3f}, {:.3f})", vec2F[0], vec2F[1]);
}
void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Vector2& vec2)
{
	float vec2F[2];
	vec2.GetFloat2(vec2F);
	f.writer().write(L"({:.3f}, {:.3f})", vec2F[0], vec2F[1]);
}

// Vector3 formatting
void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Vector3& vec3)
{
	float vec3F[3];
	vec3.GetFloat3(vec3F);
	f.writer().write("({:.3f}, {:.3f}, {:.3f})", vec3F[0], vec3F[1], vec3F[2]);
}
void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Vector3& vec3)
{
	float vec3F[3];
	vec3.GetFloat3(vec3F);
	f.writer().write(L"({:.3f}, {:.3f}, {:.3f})", vec3F[0], vec3F[1], vec3F[2]);
}

// Vector4 formatting
void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Vector4& vec4)
{
	float vec4F[4];
	vec4.GetFloat4(vec4F);
	f.writer().write("({:.3f}, {:.3f}, {:.3f}, {:.3f})", vec4F[0], vec4F[1], vec4F[2], vec4F[3]);
}
void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Vector4& vec4)
{
	float vec4F[4];
	vec4.GetFloat4(vec4F);
	f.writer().write(L"({:.3f}, {:.3f}, {:.3f}, {:.3f})", vec4F[0], vec4F[1], vec4F[2], vec4F[3]);
}
