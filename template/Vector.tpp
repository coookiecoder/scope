#pragma once

#include <iostream>

template <class Type>
class Vector3D {
private:
    Type x = 0;
    Type y = 0;
    Type z = 0;

public:
    Vector3D();
    Vector3D(Type x, Type y, Type z);
    ~Vector3D();

    Type getX() const;
    Type getY() const;
    Type getZ() const;

    void setX(Type x);
    void setY(Type y);
    void setZ(Type z);
};

template <class Type>
std::ostream &operator<<(std::ostream &os, Vector3D<Type>& vector);
template <class Type>
std::istream &operator>>(std::istream &is, Vector3D<Type>& vector);

template <class Type>
class Vector2D {
private:
    Type x = 0;
    Type y = 0;

public:
    Vector2D();
    Vector2D(Type x, Type y);
    ~Vector2D();

    Type getX() const;
    Type getY() const;

    void setX(Type x);
    void setY(Type y);
};

template <class Type>
std::ostream &operator<<(std::ostream &os, Vector2D<Type>& vector);
template <class Type>
std::istream &operator>>(std::istream &is, Vector2D<Type>& vector);

template <class Type>
Vector3D<Type>::Vector3D() = default;

template <class Type>
Vector3D<Type>::Vector3D(const Type x, const Type y, const Type z) : x(x), y(y), z(z) {

}

template <class Type>
Vector3D<Type>::~Vector3D() = default;

template <class Type>
Type Vector3D<Type>::getX() const {
    return x;
}

template <class Type>
Type Vector3D<Type>::getY() const {
    return y;
}

template <class Type>
Type Vector3D<Type>::getZ() const {
    return z;
}

template <class Type>
void Vector3D<Type>::setX(const Type x) {
    this->x = x;
}

template <class Type>
void Vector3D<Type>::setY(const Type y) {
    this->y = y;
}

template <class Type>
void Vector3D<Type>::setZ(const Type z) {
    this->z = z;
}

template <class Type>
std::ostream& operator<<(std::ostream& os, Vector3D<Type>& vector) {
    os << "[" << vector.getX() << "," << vector.getY() << "," << vector.getZ() << "]";
    return os;
}


template <class Type>
std::istream& operator>>(std::istream& is, Vector3D<Type>& vector) {
    int x, y, z;
    is >> x >> y >> z;

    vector.setX(x);
    vector.setY(y);
    vector.setZ(z);

    return is;
}


template <class Type>
Vector2D<Type>::Vector2D() = default;

template <class Type>
Vector2D<Type>::Vector2D(const Type x, const Type y) : x(x), y(y) {

}

template <class Type>
Vector2D<Type>::~Vector2D() = default;


template <class Type>
Type Vector2D<Type>::getX() const {
    return x;
}

template <class Type>
Type Vector2D<Type>::getY() const {
    return y;
}

template <class Type>
void Vector2D<Type>::setX(const Type x) {
    this->x = x;
}

template <class Type>
void Vector2D<Type>::setY(const Type y) {
    this->y = y;
}

template <class Type>
std::ostream& operator<<(std::ostream& os, Vector2D<Type>& vector) {
    os << "[" << vector.getX() << "," << vector.getY() << "]";
    return os;
}


template <class Type>
std::istream& operator>>(std::istream& is, Vector2D<Type>& vector) {
    int x, y;
    is >> x >> y;

    vector.setX(x);
    vector.setY(y);

    return is;
}
