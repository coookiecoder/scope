#pragma once

#include "Vector.tpp"

namespace cookie {
    template<typename Type>
    class alignas(16) Matrix4D {
        private:
            Type data[4][4] = {};

        public:
            Matrix4D();
            Matrix4D(Type data[4][4]);
            Matrix4D(Type value);
            ~Matrix4D();

            Type get(int x, int y);

            void set(int x, int y, Type value);

            Type* operator[](int row);
            const Type* operator[] (int row) const;

            Matrix4D<Type> operator+(const Matrix4D<Type>& other) const;
            Matrix4D<Type> operator-(const Matrix4D<Type>& other) const;
            Matrix4D<Type> operator*(const Matrix4D<Type>& other) const;
    };

    template <typename Type>
    Matrix4D<Type>::Matrix4D() = default;

    template <typename Type>
    Matrix4D<Type>::Matrix4D(Type data[4][4]) {
        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
                this->data[x][y] = data[x][y];
    }

    template <typename Type>
    Matrix4D<Type>::Matrix4D(Type value) {
        if (value != Type()) {
            this->data[0][0] = value;
            this->data[1][1] = value;
            this->data[2][2] = value;
            this->data[3][3] = value;
        }
    }

    template <typename Type>
    Matrix4D<Type>::~Matrix4D() = default;

    template <typename Type>
    Type Matrix4D<Type>::get(int x, int y) {
        return this->data[x][y];
    }

    template <typename Type>
    void Matrix4D<Type>::set(int x, int y, Type value) {
        this->data[x][y] = value;
    }

    template <typename Type>
    Type* Matrix4D<Type>::operator[](int row) {
        return this->data[row];
    }

    template <typename Type>
    const Type* Matrix4D<Type>::operator[](int row) const {
        return this->data[row];
    }

    template <typename Type>
    Matrix4D<Type> Matrix4D<Type>::operator+(const Matrix4D<Type>& other) const {
        Matrix4D<Type> result(*this);

        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
                result[x][y] += other[x][y];

        return result;
    }

    template <typename Type>
    Matrix4D<Type> Matrix4D<Type>::operator-(const Matrix4D<Type>& other) const {
        Matrix4D<Type> result(*this);

        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
                result[x][y] -= other[x][y];

        return result;
    }

    template <typename Type>
    Matrix4D<Type> Matrix4D<Type>::operator*(const Matrix4D<Type>& other) const {
        Matrix4D<Type> result;

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                result[x][y] = static_cast<Type>(0);
                for (int k = 0; k < 4; ++k) {
                    result[x][y] += (*this)[x][k] * other[k][y];
                }
            }
        }

        return result;
    }
}

namespace cookie {
    template<typename Type>
    Matrix4D<Type> rotate(const Matrix4D<Type>& m, const float angle, Vector3D<Type> axis) {
        axis = normalize(axis);
        const float c = std::cos(angle);
        const float s = std::sin(angle);
        const float t = 1.0f - c;

        const float x = axis.x;
        const float y = axis.y;
        const float z = axis.z;

        Matrix4D rot(1.0f);
        rot[0][0] = t*x*x + c;
        rot[0][1] = t*x*y + s*z;
        rot[0][2] = t*x*z - s*y;

        rot[1][0] = t*x*y - s*z;
        rot[1][1] = t*y*y + c;
        rot[1][2] = t*y*z + s*x;

        rot[2][0] = t*x*z + s*y;
        rot[2][1] = t*y*z - s*x;
        rot[2][2] = t*z*z + c;

        return m * rot;
    }

    template<typename Type>
    Matrix4D<Type> translate(const Matrix4D<Type>& mat, const Vector3D<Type>& vec) {
        Matrix4D<Type> result = mat;

        result[3][0] += vec.x;
        result[3][1] += vec.y;
        result[3][2] += vec.z;

        return result;
    }

    template<typename Type>
    Matrix4D<Type> lookAt(const Vector3D<Type>& eye, const Vector3D<Type>& center, const Vector3D<Type>& up) {
        const cookie::Vector3D<float> f = cookie::normalize(cookie::subtract(center, eye));
        const cookie::Vector3D<float> s = cookie::normalize(cookie::cross(f, up));
        const cookie::Vector3D<float> u = cookie::cross(s, f);

        Matrix4D<float> result(1.0f);
        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;

        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;

        result[0][2] = -f.x;
        result[1][2] = -f.y;
        result[2][2] = -f.z;

        result[3][0] = -dot(s, eye);
        result[3][1] = -dot(u, eye);
        result[3][2] = dot(f, eye);

        return result;
    }

    template<typename Type>
    Matrix4D<Type> perspective(const Type fov, const Type aspect, const Type near, const Type far) {
        const float tanHalfFov = std::tan(fov / 2.0f);
        Matrix4D<float> result(0.0f);

        result[0][0] = 1.0f / (aspect * tanHalfFov);
        result[1][1] = 1.0f / tanHalfFov;
        result[2][2] = -(far + near) / (far - near);
        result[2][3] = -1.0f;
        result[3][2] = -(2.0f * far * near) / (far - near);

        return result;
    }
}
