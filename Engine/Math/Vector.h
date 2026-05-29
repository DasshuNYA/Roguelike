// @file Vector.h

#pragma once

#include <cmath>
#include <utility>

namespace Engine
{
template <typename T>
struct Vector2D
{
    T x = static_cast<T>(0);
    T y = static_cast<T>(0);

    constexpr Vector2D() = default;

    constexpr Vector2D(T newX, T newY) : x(newX), y(newY) {}

    float GetLength() const
    {
        return std::sqrt(static_cast<float>(x * x + y * y));
    }

    T DotProduct(const Vector2D<T>& vector) const { return x * vector.x + y * vector.y; }
};

using Vector2Df = Vector2D<float>;
using Vector2Di = Vector2D<int>;
using Position = Vector2Df;

template <typename T>
Vector2D<T> operator+(const Vector2D<T>& left, const Vector2D<T>& right)
{
    return {left.x + right.x, left.y + right.y};
}

template <typename T>
Vector2D<T> operator-(const Vector2D<T>& left, const Vector2D<T>& right)
{
    return {left.x - right.x, left.y - right.y};
}

template <typename T>
Vector2D<T> operator-(const Vector2D<T>& left)
{
    return {-left.x, -left.y};
}

template <typename T>
Vector2D<T> operator*(const Vector2D<T>& left, const Vector2D<T>& right)
{
    return {left.x * right.x, left.y * right.y};
}

template <typename T>
Vector2D<T> operator*(const T scalar, const Vector2D<T>& right)
{
    return {scalar * right.x, scalar * right.y};
}

template <typename T>
Vector2D<T> operator*(const Vector2D<T>& left, const T scalar)
{
    return {scalar * left.x, scalar * left.y};
}

template <typename T>
bool operator==(const Vector2D<T>& left, const Vector2D<T>& right)
{
    return left.x == right.x && left.y == right.y;
}

template <typename T>
bool operator!=(const Vector2D<T>& left, const Vector2D<T>& right)
{
    return left.x != right.x || left.y != right.y;
}

// This complex template allows us to convert any vector type to any other
// vector type (like our Vector2D to SFML's Vector and vice versa)
template <typename U, typename V>
U Convert(const V& v)
{
    using XType = decltype(std::declval<U>().x);
    using YType = decltype(std::declval<U>().y);

    return {static_cast<XType>(v.x), static_cast<YType>(v.y)};
}
}  // namespace Engine
