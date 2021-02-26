#include "vector.hpp"
#include <cmath>

double magnitude(const Vector2& u)
{
    return std::sqrt(std::pow(u.x(), 2) + std::pow(u.y(), 2));
}

Vector2 scale(const Vector2& u, double alpha)
{
    return {u.x() / alpha, u.y() / alpha};
}

Vector2 normalize(const Vector2& u)
{
    double mag = magnitude(u);
    return scale(u, mag);
}

Vector2 add(const Vector2& u, const Vector2& v)
{
    return {u.x() + v.x(), u.y() + v.y()};
}

Vector2 subtract(const Vector2& u, const Vector2& v)
{
    return {u.x() - v.x(), u.y() - v.y()};
}

double magnitude(const Vector3& u)
{
    return std::sqrt(std::pow(u.x(), 2) + std::pow(u.y(), 2) + std::pow(u.z(), 2));
}

Vector3 scale(const Vector3& u, double alpha)
{
    return {u.x() / alpha, u.y() / alpha, u.z() / alpha};
}

Vector3 normalize(const Vector3& u)
{
    double mag = magnitude(u);
    return scale(u, mag);
}

Vector3 add(const Vector3& u, const Vector3& v)
{
    return {u.x() + v.x(), u.y() + v.y(), u.z() + v.z()};
}

Vector3 subtract(const Vector3& u, const Vector3& v)
{
    return {u.x() - v.z(), u.y() - v.y(), u.z() - v.z()};
}