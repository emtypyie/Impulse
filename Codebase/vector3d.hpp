#pragma once
#include <cmath>
#include <iostream>

class Vector3d {
public:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vector3d() = default;
    Vector3d(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector3d operator+(const Vector3d& other) const;
    Vector3d operator-(const Vector3d& other) const;
    Vector3d operator*(double scalar) const;
    Vector3d operator/(double scalar) const;

    Vector3d& operator+=(const Vector3d& other);
    Vector3d& operator-=(const Vector3d& other);

    double lengthSquared() const;
    double length() const;
    Vector3d normalized() const;
    
    void print() const;
        // Geometric Multiplications
    double dot(const Vector3d& other) const;
    Vector3d cross(const Vector3d& other) const;

};
