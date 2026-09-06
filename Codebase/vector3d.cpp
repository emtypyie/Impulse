#include "Vector3d.hpp"

//  Basic Operators (Addition & Subtraction)
Vector3d Vector3d::operator+(const Vector3d& other) const {
    return Vector3d(x + other.x, y + other.y, z + other.z);
}

Vector3d Vector3d::operator-(const Vector3d& other) const {
    return Vector3d(x - other.x, y - other.y, z - other.z);
}

// 2. Scaling Operators (Multiplication & Division)
Vector3d Vector3d::operator*(double scalar) const {    // Scalar multiplication Operator
    return Vector3d(x * scalar, y * scalar, z * scalar);
}

Vector3d Vector3d::operator/(double scalar) const {   // Scalar division Operator
    if (scalar == 0.0) {
        // Prevent division by zero crash; return an empty vector
        return Vector3d(0.0, 0.0, 0.0);
    }
    return Vector3d(x / scalar, y / scalar, z / scalar);
}

// 3. In-place Compound Operators (Modifies the active vector directly)
Vector3d& Vector3d::operator+=(const Vector3d& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3d& Vector3d::operator-=(const Vector3d& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

// 4. Vector Geometry Calculations
double Vector3d::lengthSquared() const {
    return (x * x) + (y * y) + (z * z);
}

double Vector3d::length() const {
    return std::sqrt(lengthSquared());
}

Vector3d Vector3d::normalized() const {
    double len = length();
    if (len > 0.0) {
        return *this / len; // Uses our overloaded division operator
    }
    return Vector3d(0.0, 0.0, 0.0);
}

// 5. Utility Console Debugging
void Vector3d::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")\n";
}

double Vector3d::dot(const Vector3d& other) const {
    return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3d Vector3d::cross(const Vector3d& other) const {
    return Vector3d(
        (y * other.z) - (z * other.y),
        (z * other.x) - (x * other.z),
        (x * other.y) - (y * other.x)
    );
}

