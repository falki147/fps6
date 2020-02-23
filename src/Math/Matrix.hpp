#pragma once

#include <algorithm>

template<unsigned int dim> struct Matrix;
template<unsigned int dim> struct Vector;

template<unsigned int dim> struct MatrixBase {
	float data[dim * dim];
};

template<> struct MatrixBase<4> {
	static Matrix<4> createPerspective(float fovy, float aspect, float zNear, float zFar);
	static Matrix<4> createPerspectiveCenter(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix<4> createLookAt(Vector<3> eye, Vector<3> target, Vector<3> up);
	static Matrix<4> createTranslation(Vector<3> trans);
	static Matrix<4> createScalation(Vector<3> trans);
	static Matrix<4> createRotationZ(float dir);
	static Matrix<4> createRotationZ(float sin, float cos);
	static Matrix<4> createOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);

	float data[16];
};

template<unsigned int dim> struct Matrix: public MatrixBase<dim> {
	Matrix() { }

	template<
		typename ...Args,
		typename std::enable_if<dim * dim == sizeof...(Args), int>::type = 0
	> Matrix(Args... args) {
		float argsArray[] = { float(args)... };

		for (auto i = 0u; i < dim * dim; ++i)
			this->data[i] = argsArray[i];
	}

	Matrix<dim> transposed() {
		Matrix<dim> ret;

		for (auto i = 0u; i < dim; ++i)
			for (auto j = 0u; j < dim; ++j)
				ret.data[i * dim + j] = this->data[i + j * dim];

		return ret;
	}

	Matrix<dim> operator*(const Matrix<dim>& right) {
		Matrix<dim> ret;

		for (auto j = 0u; j < dim; ++j) { // left row index
			for (auto i = 0u; i < dim; ++i) { // right column index
				auto sum = 0.0f;

				for (auto k = 0u; k < dim; ++k)
					sum += this->data[j * dim + k] * right.data[i + k * dim];

				ret.data[j * dim + i] = sum;
			}
		}

		return ret;
	}

	static Matrix<dim> identity() {
		Matrix<dim> ret;

		for (auto& val : ret.data)
			val = 0.0f;

		for (auto i = 0u; i < dim * dim; i += dim + 1)
			ret.data[i] = 1.0f;

		return ret;
	}
};

typedef Matrix<3> mat3;
typedef Matrix<4> mat4;
