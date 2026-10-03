#include <array>
#include <cmath>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <numbers>
#include <ranges>

// Reference: GLSL plasma https://x.com/XorDev/status/1894123951401378051

using u8 = uint8_t;
using u16 = uint16_t;
using u64 = uint64_t;

using i32 = int32_t;
using i64 = int64_t;

constexpr double pi{ std::numbers::pi_v<float> };
constexpr double two_pi{ 2.0 * std::numbers::pi };

// Constexpr math
constexpr float _fabs(float f) {
    return (f > 0) ? f : -f;
}

constexpr double _reduce(double x) {
    const double k = x / two_pi;
    const i64 multiple = static_cast<i64>(k + (k >= 0 ? 0.5 : -0.5));
    return x - multiple * two_pi;
}

constexpr float _sinf(float xf) {
    const double x = _reduce(xf);
    const double x2 = x * x;
    double term = x, sum = x;
    for (i32 n{ 1 }; n <= 4; ++n) {
        term *= -x2 / ((2 * n) * (2 * n + 1));
        sum += term;
    }
    return static_cast<float>(sum);
}

constexpr float _cosf(float x) {
    return _sinf(x + static_cast<float>(std::numbers::pi / 2));
}

constexpr float _expf(float xf) {
    if (xf > 88.f) return 3.4e38f;
    if (xf < -87.f) return 0.f;
    constexpr double ln2{ std::numbers::ln2 };
    const double x = xf;
    const double kf = x / ln2;
    const i64 k = static_cast<i64>(kf + (kf >= 0 ? 0.5 : -0.5));
    const double r = x - k * ln2; // |r| <= ~0.35
    double term = 1, sum = 1;
    for (int n = 1; n <= 8; ++n) {
        term *= r / n;
        sum += term;
    }
    for (long long i = 0; i < k; ++i) sum *= 2;
    for (long long i = 0; i > k; --i) sum /= 2;
    return static_cast<float>(sum);
}

constexpr float _tanhf(float x) {
    const float a = _fabs(x);
    if (a > 9.f) return x > 0 ? 1.f : -1.f;
    const float e = _expf(-2.f * a);
    const float t = (1.f - e) / (1.f + e);
    return x > 0 ? t : -t;
}


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
        _sinf(vec.x),
        _sinf(vec.y),
        _sinf(vec.z),
        _sinf(vec.w)
    };
}

constexpr Vec4 exp(const Vec4& vec) {
    return {
        _expf(vec.x),
        _expf(vec.y),
        _expf(vec.z),
        _expf(vec.w)
    };
}

constexpr Vec4 tanh(const Vec4& vec) {
    return {
        _tanhf(vec.x),
        _tanhf(vec.y),
        _tanhf(vec.z),
        _tanhf(vec.w)
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

constexpr Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs) {
    Vec2 result{lhs};
    return result += rhs;
}

constexpr Vec2 operator+(const Vec2 &lhs, const float rhs) {
    Vec2 result{lhs};
    return result += rhs;
}

constexpr Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs) {
    Vec2 result{lhs};
    return result -= rhs;
}

constexpr Vec2 operator*(const Vec2 &lhs, const Vec2 &rhs) {
    Vec2 result{lhs};
    return result *= rhs;
}

constexpr Vec2 operator*(const Vec2 &lhs, const float rhs) {
    Vec2 result{lhs};
    return result *= rhs;
}

constexpr Vec2 operator/(const Vec2 &lhs, const float rhs) {
    Vec2 result{lhs};
    return result /= rhs;
}

constexpr Vec2 cos(const Vec2 &vec) {
    return {
        _cosf(vec.x),
        _cosf(vec.y)
    };
}

template<typename T, typename IndexType = std::size_t>
concept MutableIndexed = requires(T container, IndexType i, typename T::value_type val) {
    container[i];
    container[i] = val;
};

template<u64 Width, u64 Height>
consteval auto construct_frame(MutableIndexed auto& pixels, u64 timestep, u16 n_frames) {
    Vec2 r{ static_cast<float>(Width), static_cast<float>(Height) };
    u64 idx{ };
    const float t = (static_cast<float>(timestep) / n_frames) * 2 * pi;
    for (u64 y{ 0 }; y < Height; ++y) {
        for (u64 x{ 0 }; x < Width; ++x) {
            Vec2 FC{ static_cast<float>(x), static_cast<float>(y) };
            Vec2 p{ (FC * 2.0f - r) / r.y };
            Vec2 l{ };
            Vec2 i{ };
            Vec2 v{ p * (l += 4.f - 4.f * _fabs(.7f - p.dot(p))) };
            Vec4 o{ };
            for (; i.y++ < 8.; o += (sin(v.xyyx()) + 1.) * _fabs(v.x - v.y)) {
                v += cos(v.yx() * i.y + i + t) / i.y + .7;
            }
            o = tanh(5. * exp(l.x - 4. - Vec4(-1, 1, 2, 0) * p.y) / o);
            pixels[idx++] = static_cast<u8>(o.x * 255);
            pixels[idx++] = static_cast<u8>(o.y * 255);
            pixels[idx++] = static_cast<u8>(o.z * 255);
        }
    }
    return pixels;
}

template<u64 Width, u64 Height, u64 Channels, u16 N_Frames>
consteval auto generate_frames(u16 scale) {
    std::array<std::array<u8, Width * Height * Channels>, N_Frames> frames{ };
    for (u16 t{ 0 }; t < N_Frames; ++t) {
        construct_frame<Width, Height>(frames[t], t, N_Frames);
    }
    return frames;
}

i32 main() {
    constexpr u16 scale{ 10 };
    constexpr u16 n_frames{ scale * 2 };
    constexpr u64 width{ 16 * scale };
    constexpr u64 height{ 9 * scale };
    constexpr std::array<u8, width * height * 3> pixels{ };
    const std::string ppm_header{ std::format("P6 {} {} 255\n", width, height) };
    static constexpr auto frames{ generate_frames<width, height, 3, n_frames>(scale) };
    for (u64 t{ 0 }; t < n_frames; ++t) {
        const auto& frame{ frames[t] };
        auto ppm_filepath{
            std::filesystem::path(std::format("out_{:02d}.ppm", t))
        };
        auto ppm_file{
            std::ofstream(ppm_filepath, std::ios::binary)
        };
        if (!ppm_file) {
            std::println(stderr, "Failed to open {}", ppm_filepath.string());
            return 1;
        }
        ppm_file.write(ppm_header.c_str(), ppm_header.size());
        ppm_file.write(reinterpret_cast<const char*>(frame.data()), frame.size());
        std::println("File generated: {}", ppm_filepath.string());
    }

    return 0;
}
