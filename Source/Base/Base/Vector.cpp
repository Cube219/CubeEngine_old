#include "Vector.h"

// Float2 formatting
void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Float2& f2)
{
	f.writer().write("({:.3f}, {:.3f})", f2.x, f2.y);
}

void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Float2& f2)
{
	f.writer().write(L"({:.3f}, {:.3f})", f2.x, f2.y);
}

// Float3 formatting
void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Float3& f3)
{
	f.writer().write("({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
}

void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Float3& f3)
{
	f.writer().write(L"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
}

// Float4 formatting
void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Float4& f4)
{
	f.writer().write("({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
}

void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Float4& f4)
{
	f.writer().write(L"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
}

// Vector2 formatting
void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Vector2& vec2)
{
	Float2 f2 = vec2.GetFloat2();
	f.writer().write("({:.3f}, {:.3f})", f2.x, f2.y);
}
void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Vector2& vec2)
{
	Float2 f2 = vec2.GetFloat2();
	f.writer().write(L"({:.3f}, {:.3f})", f2.x, f2.y);
}

// Vector3 formatting
void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Vector3& vec3)
{
	Float3 f3 = vec3.GetFloat3();
	f.writer().write("({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
}
void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Vector3& vec3)
{
	Float3 f3 = vec3.GetFloat3();
	f.writer().write(L"({:.3f}, {:.3f}, {:.3f})", f3.x, f3.y, f3.z);
}

// Vector4 formatting
void format_arg(fmt::BasicFormatter<char>& f, const char*& format_str, const Vector4& vec4)
{
	Float4 f4 = vec4.GetFloat4();
	f.writer().write("({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
}
void format_arg(fmt::BasicFormatter<wchar_t>& f, const wchar_t*& format_str, const Vector4& vec4)
{
	Float4 f4 = vec4.GetFloat4();
	f.writer().write(L"({:.3f}, {:.3f}, {:.3f}, {:.3f})", f4.x, f4.y, f4.z, f4.w);
}
