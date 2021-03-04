#include "vector3_c.h"
#include "../../common.hpp"

#include <random>
static double __degreeToRadian(double n) {
	return n * 0.017453292519943295;
}

#pragma region Vector3
Vector3_c::Vector3_c(float X, float Y, float Z)
	: x(X), y(Y), z(Z)
{
}

Vector3_c::Vector3_c()
	: x(0), y(0), z(0)
{
}



Vector3_c::Vector3_c(Vector3 & xyz) : x(xyz.x), y(xyz.y), z(xyz.z) {};

void Vector3_c::clear()
{
	*this = Vector3_c();
}

Vector3_c Vector3_c::Zero()
{
	return Vector3_c(0.0f, 0.0f, 0.0f);
}

Vector3_c Vector3_c::One()
{
	return Vector3_c(1.0f, 1.0f, 1.0f);
}

Vector3_c Vector3_c::WorldUp()
{
	return Vector3_c(0.0f, 0.0f, 1.0f);
}
Vector3_c Vector3_c::WorldDown()
{
	return Vector3_c(0.0f, 0.0f, -1.0f);
}
Vector3_c Vector3_c::WorldNorth()
{
	return Vector3_c(0.0f, 1.0f, 0.0f);
}
Vector3_c Vector3_c::WorldSouth()
{
	return Vector3_c(0.0f, -1.0f, 0.0f);
}
Vector3_c Vector3_c::WorldEast()
{
	return Vector3_c(1.0f, 0.0f, 0.0f);
}
Vector3_c Vector3_c::WorldWest()
{
	return Vector3_c(-1.0f, 0.0f, 0.0f);
}

Vector3_c Vector3_c::RelativeRight()
{
	return Vector3_c(1.0f, 0.0f, 0.0f);
}
Vector3_c Vector3_c::RelativeLeft()
{
	return Vector3_c(-1.0f, 0.0f, 0.0f);
}
Vector3_c Vector3_c::RelativeFront()
{
	return Vector3_c(0.0f, 1.0f, 0.0f);
}
Vector3_c Vector3_c::RelativeBack()
{
	return Vector3_c(0.0f, -1.0f, 0.0f);
}
Vector3_c Vector3_c::RelativeTop()
{
	return Vector3_c(0.0f, 0.0f, 1.0f);
}
Vector3_c Vector3_c::RelativeBottom()
{
	return Vector3_c(0.0f, 0.0f, -1.0f);
}

float Vector3_c::Length() const
{
	return static_cast<float>(sqrt((x*x) + (y*y) + (z*z)));
}
float Vector3_c::LengthSquared() const
{
	return (x * x) + (y * y) + (z * z);
}

void Vector3_c::Normalize()
{
	float length = this->Length();
	if (length == 0.0f)
		return;
	float num = 1.0f / length;
	x *= num;
	y *= num;
	z *= num;
}

float Vector3_c::DistanceTo(const Vector3_c& position) const
{
	return (position - *this).Length();
}

Vector3_c Vector3_c::Around(float distance) const
{
	return *this + Vector3_c::RandomXY() * distance;
}

Vector3_c Vector3_c::PointOnCircle(float radius, float angleInDegrees) const
{
	Vector3_c point;
	point.x = radius * cos(__degreeToRadian(angleInDegrees)) + this->x;
	point.y = radius * sin(__degreeToRadian(angleInDegrees)) + this->y;
	point.z = this->z;

	return point;
}



void Vector3_c::PointsOnCircle(std::vector<Vector3_c>& results, float fullRadius, float angleDifference, float intervalDistance, bool includeCentre) const
{
	auto& origin = *this;
	Vector3 current;
	float u, d;

	if (includeCentre)
		results.push_back(origin);
	if (intervalDistance == 0.0f)
		return;

	for (u = 0.0f; u < 360.0f; u += angleDifference)
	{
		for (d = intervalDistance; d < fullRadius; d += intervalDistance)
		{
			current.x = d * cos(u * 0.0174532925199433F) + origin.x;
			current.y = d * sin(u * 0.0174532925199433F) + origin.y;
			//current.z = origin.z; // this will stay uniform as we're only considering the circle
			results.push_back(current);
		}
	}
}

Vector3_c Vector3_c::PointOnSphere(float radius, float longitude, float latitude) const
{
	float u = __degreeToRadian(longitude);
	float v = __degreeToRadian(latitude);
	Vector3_c point;
	point.x = radius * sin(u) * cos(v) + this->x;
	point.y = radius * cos(u) * cos(v) + this->y;
	point.z = radius * sin(v) + this->z;

	return point;
}

Vector3 Vector3_c::ToTypeStruct() const
{
	Vector3 Return;
	Return.x = this->x;
	Return.y = this->y;
	Return.z = this->z;
	return Return;
}

void Vector3_c::ToArray(float* out) const
{
	out[0] = this->x;
	out[1] = this->y;
	out[2] = this->z;
}


bool Vector3_c::IsZero() const
{
	return (this->x == 0 && this->y == 0 && this->z == 0);
}
Vector3_c Vector3_c::Modify(float x, float y, float z)
{
	this->x += x;
	this->y += y;
	this->z += z;
	return *this;
}
static float get_random_float_in_range(float min, float max)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	//boost::uniform_real<float> u(min, max);
	std::uniform_real_distribution<float> u(min, max); // inclusive
	//boost::variate_generator< boost::mt19937&, boost::uniform_real<float> > gen(rng, u);
	return u(rng);
	//return gen();
}
Vector3_c Vector3_c::RandomXY()
{
	Vector3_c v;
	v.x = get_random_float_in_range(0.0f, 1.0f) - 0.5f;
	v.y = get_random_float_in_range(0.0f, 1.0f) - 0.5f;
	v.z = 0.0f;
	v.Normalize();
	return v;
}




Vector3_c Vector3_c::Add(Vector3_c left, Vector3_c right)
{
	return Vector3_c(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3_c Vector3_c::Subtract(Vector3_c left, Vector3_c right)
{
	return Vector3_c(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector3_c Vector3_c::Multiply(Vector3_c value, float scale)
{
	return Vector3_c(value.x * scale, value.y * scale, value.z * scale);
}

Vector3_c Vector3_c::Modulate(Vector3_c left, Vector3_c right)
{
	return Vector3_c(left.x * right.x, left.y * right.y, left.z * right.z);
}

Vector3_c Vector3_c::Divide(Vector3_c value, float scale)
{
	return Vector3_c(value.x / scale, value.y / scale, value.z / scale);
}

Vector3_c Vector3_c::Negate(Vector3_c value)
{
	return Vector3_c(-value.x, -value.y, -value.z);
}

Vector3_c Vector3_c::Clamp(Vector3_c value, Vector3_c min, Vector3_c max)
{
	float x = value.x;
	x = (x > max.x) ? max.x : x;
	x = (x < min.x) ? min.x : x;

	float y = value.y;
	y = (y > max.y) ? max.y : y;
	y = (y < min.y) ? min.y : y;

	float z = value.z;
	z = (z > max.z) ? max.z : z;
	z = (z < min.z) ? min.z : z;

	return Vector3_c(x, y, z);
}

Vector3_c Vector3_c::Lerp(Vector3_c start, Vector3_c end, float amount)
{
	Vector3_c vector;

	vector.x = start.x + ((end.x - start.x) * amount);
	vector.y = start.y + ((end.y - start.y) * amount);
	vector.z = start.z + ((end.z - start.z) * amount);

	return vector;
}

Vector3_c Vector3_c::Normalize(Vector3_c vector)
{
	vector.Normalize();
	return vector;
}

float Vector3_c::Dot(Vector3_c left, Vector3_c right)
{
	return (left.x * right.x + left.y * right.y + left.z * right.z);
}

Vector3_c Vector3_c::Cross(Vector3_c left, Vector3_c right)
{
	Vector3_c result;
	result.x = left.y * right.z - left.z * right.y;
	result.y = left.z * right.x - left.x * right.z;
	result.z = left.x * right.y - left.y * right.x;
	return result;
}

Vector3_c Vector3_c::Reflect(Vector3_c vector, Vector3_c normal)
{
	Vector3_c result;
	float dot = ((vector.x * normal.x) + (vector.y * normal.y)) + (vector.z * normal.z);

	result.x = vector.x - ((2.0f * dot) * normal.x);
	result.y = vector.y - ((2.0f * dot) * normal.y);
	result.z = vector.z - ((2.0f * dot) * normal.z);

	return result;
}

Vector3_c Vector3_c::Minimize(Vector3_c value1, Vector3_c value2)
{
	Vector3_c vector;
	vector.x = (value1.x < value2.x) ? value1.x : value2.x;
	vector.y = (value1.y < value2.y) ? value1.y : value2.y;
	vector.z = (value1.z < value2.z) ? value1.z : value2.z;
	return vector;
}

Vector3_c Vector3_c::Maximize(Vector3_c value1, Vector3_c value2)
{
	Vector3_c vector;
	vector.x = (value1.x > value2.x) ? value1.x : value2.x;
	vector.y = (value1.y > value2.y) ? value1.y : value2.y;
	vector.z = (value1.z > value2.z) ? value1.z : value2.z;
	return vector;
}

Vector3_c operator + (Vector3_c const& left, Vector3_c const& right)
{
	return Vector3_c(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3_c operator - (Vector3_c const& left, Vector3_c const& right)
{
	return Vector3_c(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector3_c operator - (Vector3_c const& value)
{
	return Vector3_c(-value.x, -value.y, -value.z);
}

Vector3_c operator * (Vector3_c const& left, Vector3_c const& right)
{
	return Vector3_c(left.x * right.x, left.y * right.y, left.z * right.z);
}

Vector3_c operator * (Vector3_c const& value, float const& scale)
{
	return Vector3_c(value.x * scale, value.y * scale, value.z * scale);
}

Vector3_c operator * (float const& scale, Vector3_c const& value)
{
	return Vector3_c(value.x * scale, value.y * scale, value.z * scale);
}

Vector3_c operator *= (Vector3_c& value, float const& scale)
{
	value.x *= scale;
	value.y *= scale;
	value.z *= scale;
	return value;
}

Vector3_c operator / (Vector3_c const& left, Vector3_c const& right)
{
	return Vector3_c(left.x / right.x, left.y / right.y, left.z / right.z);
}

Vector3_c operator / (Vector3_c const& value, float const& scale)
{
	return Vector3_c(value.x / scale, value.y / scale, value.z / scale);
}

Vector3_c operator /= (Vector3_c& value, float const& scale)
{
	value.x /= scale;
	value.y /= scale;
	value.z /= scale;
	return value;
}

bool operator == (Vector3_c const& left, Vector3_c const& right)
{
	return Vector3_c::Equals(left, right);
}

bool operator != (Vector3_c const& left, Vector3_c const& right)
{
	return !Vector3_c::Equals(left, right);
}


std::string Vector3_c::ToString() const
{
	std::stringstream ss;
	ss << "X:" << std::setprecision(4) << std::fixed << this->x;
	ss << " Y:" << std::setprecision(4) << std::fixed << this->y;
	ss << " Z:" << std::setprecision(4) << std::fixed << this->z;
	return ss.str();
}

bool Vector3_c::Equals(const Vector3& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

bool Vector3_c::Equals(const Vector3_c& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

bool Vector3_c::Equals(const Vector3_c& value1, const Vector3_c& value2)
{
	return (value1.x == value2.x && value1.y == value2.y && value1.z == value2.z);
}


float Vector3_c::DistanceBetween(const Vector3_c& value1, const Vector3_c& value2)
{
	return (value1 - value2).Length();
}

Vector3_c Vector3_c::RotationToDirection(const Vector3_c& rotation)
{
	float retz = rotation.z * 0.0174532924F; // Degree to radian
	float retx = rotation.x * 0.0174532924F;
	float absx = abs(cos(retx));
	return Vector3_c(-sin(retz) * absx, cos(retz) * absx, sin(retx));
}
Vector3_c Vector3_c::DirectionToRotation(Vector3_c direction)
{
	direction.Normalize();
	float xx = atan2(direction.z, direction.y) / 0.0174532924F; // Radian to degree
	float yy = 0;
	float zz = -atan2(direction.x, direction.y) / 0.0174532924F;
	return Vector3_c(xx, yy, zz);
}

#pragma endregion