#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>

Matrix<4> MatrixBase<4>::createPerspective(float fovy, float aspect, float zNear, float zFar) {
	auto yMax = zNear * std::tan(0.5f * fovy);
	auto yMin = -yMax;
	auto xMin = yMin * aspect;
	auto xMax = yMax * aspect;

	return createPerspectiveCenter(xMin, xMax, yMin, yMax, zNear, zFar);
}

Matrix<4> MatrixBase<4>::createPerspectiveCenter(float left, float right, float bottom, float top, float zNear, float zFar) {
	auto x = (2.0f * zNear) / (right - left);
	auto y = (2.0f * zNear) / (top - bottom);
	auto a = (right + left) / (right - left);
	auto b = (top + bottom) / (top - bottom);
	auto c = -(zFar + zNear) / (zFar - zNear);
	auto d = -(2.0f * zFar * zNear) / (zFar - zNear);

	return Matrix<4>(
		x, 0,  a, 0,
		0, y,  b, 0,
		0, 0,  c, d,
		0, 0, -1, 0
	);
}

Matrix<4> MatrixBase<4>::createLookAt(Vector<3> eye, Vector<3> target, Vector<3> up) {
	auto z = (eye - target).normalized();
	auto x = up.cross(z).normalized();
	auto y = z.cross(x);

	return Matrix<4>(
		x.x, x.y, x.z, -(x * eye),
		y.x, y.y, y.z, -(y * eye),
		z.x, z.y, z.z, -(z * eye),
		0, 0, 0, 1
	);
}

Matrix<4> MatrixBase<4>::createTranslation(Vector<3> trans) {
	return Matrix<4>(
		1, 0, 0, trans.x,
		0, 1, 0, trans.y,
		0, 0, 1, trans.z,
		0, 0, 0, 1
	);
}

Matrix<4> MatrixBase<4>::createScalation(Vector<3> trans) {
	return Matrix<4>(
		trans.x, 0, 0, 0,
		0, trans.y, 0, 0,
		0, 0, trans.z, 0,
		0, 0, 0, 1
	);
}

Matrix<4> MatrixBase<4>::createRotationZ(float dir) {
	return createRotationZ(std::sin(dir), std::cos(dir));
}

Matrix<4> MatrixBase<4>::createRotationZ(float sin, float cos) {
	return Matrix<4>(
		cos, -sin, 0, 0,
		sin, cos, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

Matrix<4> MatrixBase<4>::createOrthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
	auto invRL = 1 / (right - left);
	auto invTB = 1 / (top - bottom);
	auto invFN = 1 / (zFar - zNear);

	return mat4(
		2 * invRL, 0, 0, -(right + left) * invRL,
		0, 2 * invTB, 0, -(top + bottom) * invTB,
		0, 0, -2 * invFN, -(zFar + zNear) * invFN,
		0, 0, 0, 1
	);
}
