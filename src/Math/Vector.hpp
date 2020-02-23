#pragma once

#include <algorithm>
#include <math.h>

template<unsigned int dim> struct Vector;

template<unsigned int dim> struct VectorBase {
	float data[dim];
};

template<> struct VectorBase<2> {
	union {
		float data[2];

		struct {
			float x, y;
		};
	};
};

template<> struct VectorBase<3> {
	Vector<3> cross(const Vector<3>& other);

	union {
		float data[3];

		struct {
			float x, y, z;
		};
	};
};

template<unsigned int dim> struct Vector: VectorBase<dim> {
	Vector() { }

	template<
		typename ...Args,
		typename std::enable_if<dim == sizeof...(Args), int>::type = 0
	> Vector(Args... args) {
		float argsArray[] = { float(args)... };

		for (auto i = 0u; i < dim; ++i)
			this->data[i] = argsArray[i];
	}

	Vector<dim> operator-() const {
		Vector<dim> ret;

		for (auto i = 0u; i < dim; ++i)
			ret.data[i] = -this->data[i];

		return ret;
	}

	Vector<dim> operator+(const Vector<dim>& other) const {
		Vector<dim> ret;

		for (auto i = 0u; i < dim; ++i)
			ret.data[i] = this->data[i] + other.data[i];

		return ret;
	}

	Vector<dim> operator-(const Vector<dim>& other) const {
		Vector<dim> ret;

		for (auto i = 0u; i < dim; ++i)
			ret.data[i] = this->data[i] - other.data[i];

		return ret;
	}

	float operator*(const Vector<dim>& other) const {
		auto sum = 0.0f;

		for (auto i = 0u; i < dim; ++i)
			sum += this->data[i] * other.data[i];

		return sum;
	}

	Vector<dim> operator*(float scalar) const {
		Vector<dim> ret;

		for (auto i = 0u; i < dim; ++i)
			ret.data[i] = this->data[i] * scalar;

		return ret;
	}

	Vector<dim> operator/(float scalar) const {
		Vector<dim> ret;

		for (auto i = 0u; i < dim; ++i)
			ret.data[i] = this->data[i] / scalar;

		return ret;
	}

	float length() const {
		auto sum = 0.0f;

		for (auto i = 0u; i < dim; ++i)
			sum += this->data[i] * this->data[i];

		return std::sqrt(sum);
	}

	float lengthSquared() const {
		auto sum = 0.0f;

		for (auto i = 0u; i < dim; ++i)
			sum += this->data[i] * this->data[i];

		return sum;
	}

	Vector<dim> normalized() const {
		auto len = length();

		if (len == 0)
			return zero();

		return *this / length();
	}

	static Vector<dim> zero() {
		Vector<dim> out;

		for (auto& val : out.data)
			val = 0.0f;

		return out;
	}
};

typedef Vector<2> vec2;
typedef Vector<3> vec3;
typedef Vector<4> vec4;
