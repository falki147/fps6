#include <Math/Vector.hpp>

Vector<3> VectorBase<3>::cross(const Vector<3>& other) {
	return Vector<3>(
		data[1] * other.data[2] - data[2] * other.data[1],
		data[2] * other.data[0] - data[0] * other.data[2],
		data[0] * other.data[1] - data[1] * other.data[0]
	);
}
