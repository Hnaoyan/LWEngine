#include "Vector3.h"
#include "../Quaternion.h"

Vector3 Vector3::RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Vector3 result{};
	Quaternion r = { vector.x,vector.y,vector.z,0 };
	Quaternion normalize = Quaternion::Normalize(quaternion);
	Quaternion rotateQuat = Quaternion::Multiply(Quaternion::Multiply(normalize, r), Quaternion::Conjugate(normalize));
	result = { rotateQuat.x,rotateQuat.y,rotateQuat.z };

	return result;
}