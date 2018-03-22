#pragma once

#include <stdint.h>

#include <memory>
#include <utility>
// TODO: Release일 때 전부 RCast로
#define DCast(T) dynamic_cast<T>

#define RCast(T) reinterpret_cast<T>

#define SCast(T) static_cast<T>

#define CCast(T) const_cast<T>

template <typename T>
using SPtr = std::shared_ptr<T>;

template <typename T>
using UPtr = std::unique_ptr<T>;

template <typename T>
using WPtr = std::weak_ptr<T>;

#define DPCast(T) std::dynamic_pointer_cast<T>

#define SPCast(T) std::static_pointer_cast<T>

#define CPCast(T) std::const_pointer_cast<T>

// TODO: 다른 곳으로 옮기기
enum class MouseButtonType
{
	Left, Right, Middle
};

#include <string>
using String = std::string;
using WString = std::wstring;
#include "String.h"

#include <vector>
template <typename T>
using Vector = std::vector<T>;

template <typename Type, size_t Size>
using Array = std::array<Type, Size>;

#include <unordered_map>
template <typename Key, typename Value>
using HashMap = std::unordered_map<Key, Value>;

#define SET_ENUM_AS_FLAGS(type) \
	inline type operator |(type a, type b)\
	{\
		return static_cast<type>(static_cast<int>(a) | static_cast<int>(b));\
	}\
	inline type operator &(type a, type b)\
	{\
		return static_cast<type>(static_cast<int>(a) & static_cast<int>(b));\
	}\
	inline type& operator |=(type& a, type b)\
	{\
		return a = a | b;\
	}\
	inline type& operator &=(type& a, type b)\
	{\
		return a = a & b;\
	}