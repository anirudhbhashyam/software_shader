#include <cmath>
#include <filesystem>
#include <fstream>
#include <print>

// GLSL plasma https://x.com/XorDev/status/1894123951401378051

struct Vec4 {
    public:
        Vec4() = default;
        Vec4(const float x, const float y, const float z, const float w)
            : x(x), y(y), z(z), w(w) {}
        Vec4 &operator+=(const Vec4 &vec) {
            this->x += vec.x;
            this->y += vec.y;
            this->z += vec.z;
            this->w += vec.w;
            return *this;
        }

        Vec4 &operator+=(const float scale) {
            this->x += scale;
            this->y += scale;
            this->z += scale;
            this->w += scale;
            return *this;
        }

        Vec4 &operator-=(const float scale) {
            this->x -= scale;
            this->y -= scale;
            this->z -= scale;
            this->w -= scale;
            return *this;
        }

        Vec4 &operator*=(const Vec4 &vec) {
            this->x *= vec.x;
            this->y *= vec.y;
            this->z *= vec.z;
            this->w *= vec.w;
            return *this;
        }

        Vec4 &operator*=(const float scale) {
            this->x *= scale;
            this->y *= scale;
            this->z *= scale;
            this->w *= scale;
            return *this;
        }

        Vec4 &operator/=(const Vec4 &vec) {
            this->x /= vec.x;
            this->y /= vec.y;
            this->z /= vec.z;
            this->w /= vec.w;
            return *this;
        }

    public:
        float x{ }, y{ }, z{ }, w{ };
};

inline Vec4 operator+(const Vec4 &lhs, const Vec4 &rhs) {
    Vec4 result{lhs};
    return result += rhs;
}

inline Vec4 operator+(const Vec4 &lhs, const float scale) {
    Vec4 result{lhs};
    return result += scale;
}

inline Vec4 operator-(const Vec4 &lhs, const float scale) {
    Vec4 result{lhs};
    return result -= scale;
}

inline Vec4 operator-(const float scale, const Vec4 &vec) {
    return {
        scale - vec.x,
        scale - vec.y,
        scale - vec.z,
        scale - vec.w
    };
}

inline Vec4 operator*(const Vec4 &lhs, const float scale) {
    Vec4 result{lhs};
    return result *= scale;
}

inline Vec4 operator*(const float scale, const Vec4 &vec) {
    Vec4 result{vec};
    return result *= scale;
}

inline Vec4 operator/(const Vec4 &lhs, const Vec4 &rhs) {
    Vec4 result{lhs};
    return result /= rhs;
}

inline Vec4 sin(const Vec4 &vec) {
    return {
        std::sinf(vec.x),
        std::sinf(vec.y),
        std::sinf(vec.z),
        std::sinf(vec.w)
    };
}

inline Vec4 exp(const Vec4& vec) {
    return {
        std::expf(vec.x),
        std::expf(vec.y),
        std::expf(vec.z),
        std::expf(vec.w)
    };
}

inline Vec4 tanh(const Vec4& vec) {
    return {
        std::tanhf(vec.x),
        std::tanhf(vec.y),
        std::tanhf(vec.z),
        std::tanhf(vec.w)
    };
}

struct Vec2 {
    public:
        Vec2() = default;
        Vec2(const float x, const float y) : x(x), y(y) {}

        Vec2 &operator+=(const Vec2 &vec) {
            this->x += vec.x;
            this->y += vec.y;
            return *this;
        }

        Vec2 &operator+=(const float scale) {
            this->x += scale;
            this->y += scale;
            return *this;
        }

        Vec2 &operator-=(const Vec2 &vec) {
            this->x -= vec.x;
            this->y -= vec.y;
            return *this;
        }

        Vec2 &operator*=(const Vec2 &vec) {
            this->x *= vec.x;
            this->y *= vec.y;
            return *this;
        }

        Vec2 &operator*=(const float scale) {
            this->x *= scale;
            this->y *= scale;
            return *this;
        }

        Vec2 &operator/=(const float scale) {
            this->x /= scale;
            this->y /= scale;
            return *this;
        }

        Vec2 yx() const { return {y, x}; }

        Vec4 xyyx() const { return {x, y, y, x}; }

    public:
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

inline float dot(const Vec2 &lhs, const Vec2 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

int32_t main() {
    constexpr uint16_t scale{60};
    constexpr uint16_t width{16 * scale};
    constexpr uint16_t height{9 * scale};
    constexpr double pi{3.1415926535};
    for (int32_t i = 0; i < scale; ++i) {
        auto ppm_filepath{std::filesystem::path(std::format("out_{:02d}.ppm", i))};
        auto ppm_file{std::ofstream(ppm_filepath, std::ios::binary)};

        if (!ppm_file) continue;

        std::string ppm_header = std::format("P6 {} {} 255\n", width, height);
        ppm_file.write(ppm_header.c_str(), ppm_header.size());

        const float t = (static_cast<float>(i) / 240) * 2 * pi;

        Vec2 r{static_cast<float>(width), static_cast<float>(height)};
        for (int32_t y = 0; y < height; ++y) {
            for (int32_t x = 0; x < width; ++x) {
                Vec2 FC{static_cast<float>(x), static_cast<float>(y)};
                Vec2 p{ (FC * 2.0f - r) / r.y };
                Vec2 l{ };
                Vec2 i{ };
                Vec2 v{ p * (l += 4.f - 4.f * std::abs(.7f - dot(p, p))) };
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
        ppm_file.close();
        std::println("File generated: {}", ppm_filepath.string());
    }
    return 0;
}
