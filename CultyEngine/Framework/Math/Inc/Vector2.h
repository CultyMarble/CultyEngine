#pragma once

namespace CultyEngine::MathC
{
    struct Vector2
    {
        union
        {
            struct { float x, y; };
            std::array<float, 2> v;// same as float[2]
        };

        constexpr Vector2() noexcept : Vector2(0.0f) {}
        explicit constexpr Vector2(float f) noexcept : Vector2(f, f) {}
        constexpr Vector2(float x, float y) noexcept : x{ x }, y{ y } {}

        // commonly used vectors
        const static Vector2 Zero;
        const static Vector2 One;
        const static Vector2 XAxis;
        const static Vector2 YAxis;

        constexpr Vector2 operator-() const { return { -x, -y }; }
        constexpr Vector2 operator+(Vector2 b) const { return { x + b.x, y + b.y }; }
        constexpr Vector2 operator-(Vector2 b) const { return { x - b.x, y - b.y }; }
        constexpr Vector2 operator*(float s) const { return { x * s, y * s }; }
        constexpr Vector2 operator/(float s) const { return { x / s, y / s }; }

        constexpr Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
        constexpr Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
        constexpr Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
        constexpr Vector2& operator/=(float s) { x /= s; y /= s; return *this; }

        // Inside the Vector2 struct
        constexpr bool operator==(const Vector2& other) const
        {
            return x == other.x && y == other.y;
        }

        constexpr bool operator!=(const Vector2& other) const
        {
            return !(*this == other);
        }


        // Utility functions
        float LengthSquared() const { return x * x + y * y; }
        float Length() const { return std::sqrt(LengthSquared()); }

        Vector2 Normalize() const
        {
            float len = Length();
            return (len > 0.0f) ? Vector2(x / len, y / len) : Vector2(0.0f, 0.0f);
        }

        static float Dot(const Vector2& a, const Vector2& b)
        {
            return a.x * b.x + a.y * b.y;
        }
    };
}