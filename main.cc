#include <cmath>
#include <filesystem>
#include <fstream>
#include <print>

// Reference: GLSL plasma https://x.com/XorDev/status/1894123951401378051

using u16 = uint16_t;

struct Vec4 {
    constexpr Vec4() = default;
    constexpr Vec4(const float x, const float y, const float z, const float w)
        : x(x), y(y), z(z), w(w)
    {}

    constexpr Vec4 &operator+=(const Vec4 &vec) {
        this->x += vec.x;
        this->y += vec.y;
        this->z += vec.z;
        this->w += vec.w;
        return *this;
    }

    constexpr Vec4 &operator+=(const float scale) {
        this->x += scale;
        this->y += scale;
        this->z += scale;
        this->w += scale;
        return *this;
    }

    constexpr Vec4 &operator-=(const float scale) {
        this->x -= scale;
        this->y -= scale;
        this->z -= scale;
        this->w -= scale;
        return *this;
    }

    constexpr Vec4 &operator*=(const Vec4 &vec) {
        this->x *= vec.x;
        this->y *= vec.y;
        this->z *= vec.z;
        this->w *= vec.w;
        return *this;
    }

    constexpr Vec4 &operator*=(const float scale) {
        this->x *= scale;
        this->y *= scale;
        this->z *= scale;
        this->w *= scale;
        return *this;
    }

    constexpr Vec4 &operator/=(const Vec4 &vec) {
        this->x /= vec.x;
        this->y /= vec.y;
        this->z /= vec.z;
        this->w /= vec.w;
        return *this;
    }

    float x{ }, y{ }, z{ }, w{ };
};

constexpr Vec4 operator+(const Vec4 &lhs, const Vec4 &rhs) {
    Vec4 result{lhs};
    return result += rhs;
}

constexpr Vec4 operator+(const Vec4 &lhs, const float scale) {
    Vec4 result{lhs};
    return result += scale;
}

constexpr Vec4 operator-(const Vec4 &lhs, const float scale) {
    Vec4 result{lhs};
    return result -= scale;
}

constexpr Vec4 operator-(const float scale, const Vec4 &vec) {
    return {
        scale - vec.x,
        scale - vec.y,
        scale - vec.z,
        scale - vec.w
    };
}

constexpr Vec4 operator*(const Vec4 &lhs, const float scale) {
    Vec4 result{lhs};
    return result *= scale;
}

constexpr Vec4 operator*(const float scale, const Vec4 &vec) {
    Vec4 result{vec};
    return result *= scale;
}

constexpr Vec4 operator/(const Vec4 &lhs, const Vec4 &rhs) {
    Vec4 result{lhs};
    return result /= rhs;
}

constexpr Vec4 sin(const Vec4 &vec) {
    return {
        std::sinf(vec.x),
        std::sinf(vec.y),
        std::sinf(vec.z),
        std::sinf(vec.w)
    };
}

constexpr Vec4 exp(const Vec4& vec) {
    return {
        std::expf(vec.x),
        std::expf(vec.y),
        std::expf(vec.z),
        std::expf(vec.w)
    };
}

constexpr Vec4 tanh(const Vec4& vec) {
    return {
        std::tanhf(vec.x),
        std::tanhf(vec.y),
        std::tanhf(vec.z),
        std::tanhf(vec.w)
    };
}

struct Vec2 {
    constexpr Vec2() = default;
    constexpr Vec2(const float x, const float y)
        : x(x), y(y)
    {}

    constexpr Vec2 &operator+=(const Vec2 &vec) {
        this->x += vec.x;
        this->y += vec.y;
        return *this;
    }

    constexpr Vec2 &operator+=(const float scale) {
        this->x += scale;
        this->y += scale;
        return *this;
    }

    constexpr Vec2 &operator-=(const Vec2 &vec) {
        this->x -= vec.x;
        this->y -= vec.y;
        return *this;
    }

    constexpr Vec2 &operator*=(const Vec2 &vec) {
        this->x *= vec.x;
        this->y *= vec.y;
        return *this;
    }

    constexpr Vec2 &operator*=(const float scale) {
        this->x *= scale;
        this->y *= scale;
        return *this;
    }

    constexpr Vec2 &operator/=(const float scale) {
        this->x /= scale;
        this->y /= scale;
        return *this;
    }

    constexpr float dot(const Vec2 &vec) const {
        return this->x * vec.x + this->y * vec.y;
    }

    constexpr Vec2 yx() const { return {y, x}; }

    constexpr Vec4 xyyx() const { return {x, y, y, x}; }

    float x{ }, y{ };
};

inline Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs) {
    Vec2 result{lhs};
    return result += rhs;
}

inline Vec2 operator+(const Vec2 &lhs, const float rhs) {
    Vec2 result{lhs};
    return result += rhs;
}

inline Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs) {
    Vec2 result{lhs};
    return result -= rhs;
}

inline Vec2 operator*(const Vec2 &lhs, const Vec2 &rhs) {
    Vec2 result{lhs};
    return result *= rhs;
}

inline Vec2 operator*(const Vec2 &lhs, const float rhs) {
    Vec2 result{lhs};
    return result *= rhs;
}

inline Vec2 operator/(const Vec2 &lhs, const float rhs) {
    Vec2 result{lhs};
    return result /= rhs;
}

inline Vec2 cos(const Vec2 &vec) {
    return {
        std::cosf(vec.x),
        std::cosf(vec.y)
    };
}

int32_t main() {
    constexpr u16 scale{ 120 };
    constexpr u16 width{ 16 * scale };
    constexpr u16 height{ 9 * scale };
    constexpr double pi{ 3.1415926535 };
    const std::string ppm_header{ std::format("P6 {} {} 255\n", width, height) };
    for (u16 timestep = 0; timestep < scale * 2; ++timestep) {
        auto ppm_filepath{
            std::filesystem::path(std::format("out_{:02d}.ppm", timestep))
        };
        auto ppm_file{
            std::ofstream(ppm_filepath, std::ios::binary)
        };

        if (!ppm_file) continue;

        ppm_file.write(ppm_header.c_str(), ppm_header.size());

        const float t = (static_cast<float>(timestep) / 240) * 2 * pi;

        Vec2 r{ static_cast<float>(width), static_cast<float>(height) };
        for (u16 y{ 0 }; y < height; ++y) {
            for (u16 x{ 0 }; x < width; ++x) {
                Vec2 FC{ static_cast<float>(x), static_cast<float>(y) };
                Vec2 p{ (FC * 2.0f - r) / r.y };
                Vec2 l{ };
                Vec2 i{ };
                Vec2 v{ p * (l += 4.f - 4.f * std::abs(.7f - p.dot(p))) };
                Vec4 o{ };
                for (; i.y++ < 8.; o += (sin(v.xyyx()) + 1.) * std::abs(v.x - v.y)) {
                    v += cos(v.yx() * i.y + i + t) / i.y + .7;
                }
                o = tanh(5. * exp(l.x - 4. - Vec4(-1, 1, 2, 0) * p.y) / o);
                ppm_file.put(static_cast<uint8_t>(o.x * 255));
                ppm_file.put(static_cast<uint8_t>(o.y * 255));
                ppm_file.put(static_cast<uint8_t>(o.z * 255));
            }
        }
        std::println("File generated: {}", ppm_filepath.string());
    }
    return 0;
}
