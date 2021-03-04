#pragma once
#include "../../common.hpp"
#include <vector>
#include "../../gta/vector.hpp"
#include "../../gta/natives.hpp"
#ifndef VECTOR3_C
#define VECTOR3_C
class Vector3_c
{
public:
	/// <summary>
	/// Gets or sets the x component of the vector.
	/// </summary>
	/// <value>The x component of the vector.</value>
	float x;

	/// <summary>
	/// Gets or sets the y component of the vector.
	/// </summary>
	/// <value>The y component of the vector.</value>
	float y;

	/// <summary>
	/// Gets or sets the z component of the vector.
	/// </summary>
	/// <value>The z component of the vector.</value>
	float z;

	/// <summary>
	/// Initializes a new instance of the <see cref="Vector3_c"/> class.
	/// </summary>
	/// <param name="x">Initial value for the x component of the vector.</param>
	/// <param name="y">Initial value for the y component of the vector.</param>
	/// <param name="z">Initial value for the z component of the vector.</param>
	Vector3_c(float X, float Y, float Z);

	/// <summary>
	/// Initializes a new instance of the <see cref="Vector3_c"/> class.
	/// </summary>
	Vector3_c();

	/// <summary>
	/// Initializes a new instance of the <see cref="Vector3_c"/> class.
	/// </summary>
	/// <param name="xyz">Initial value of the x,y,z components.</param>
	Vector3_c(Vector3& xyz);

	/// <summary>
	/// Nulls the vector (0,0,0)
	/// </summary>
	void clear();

	/// <summary>
	/// Returns a null vector. (0,0,0)
	/// </summary>
	static Vector3_c Zero();

	/// <summary>
	/// Returns a vector with all components as '1'. (1,1,1)
	/// </summary>
	static Vector3_c One();

	/// <summary>
	/// Returns the world Up vector. (0,0,1)
	/// </summary>
	static Vector3_c WorldUp();
	/// <summary>
	/// Returns the world Down vector. (0,0,-1)
	/// </summary>
	static Vector3_c WorldDown();
	/// <summary>
	/// Returns the world North vector. (0,1,0)
	/// </summary>
	static Vector3_c WorldNorth();
	/// <summary>
	/// Returns the world South vector. (0,-1,0)
	/// </summary>
	static Vector3_c WorldSouth();
	/// <summary>
	/// Returns the world East vector. (1,0,0)
	/// </summary>
	static Vector3_c WorldEast();
	/// <summary>
	/// Returns the world West vector. (-1,0,0)
	/// </summary>
	static Vector3_c WorldWest();

	/// <summary>
	/// Returns the relative Right vector. (1,0,0)
	/// </summary>
	static Vector3_c RelativeRight();
	/// <summary>
	/// Returns the relative Left vector. (-1,0,0)
	/// </summary>
	static Vector3_c RelativeLeft();
	/// <summary>
	/// Returns the relative Front vector. (0,1,0)
	/// </summary>
	static Vector3_c RelativeFront();
	/// <summary>
	/// Returns the relative Back vector. (0,-1,0)
	/// </summary>
	static Vector3_c RelativeBack();
	/// <summary>
	/// Returns the relative Top vector. (0,0,1)
	/// </summary>
	static Vector3_c RelativeTop();
	/// <summary>
	/// Returns the relative Bottom vector as used. (0,0,-1)
	/// </summary>
	static Vector3_c RelativeBottom();


	/// <summary>
	/// Calculates the length of the vector.
	/// </summary>
	/// <returns>The length of the vector.</returns>
	float Length() const;

	/// <summary>
	/// Calculates the squared length of the vector.
	/// </summary>
	/// <returns>The squared length of the vector.</returns>
	float LengthSquared() const;

	/// <summary>
	/// Converts the vector into a unit vector.
	/// </summary>
	void Normalize();

	/// <summary>
	/// Calculates the distance between two vectors.
	/// </summary>
	/// <param name="position">The second vector to calculate the distance to.</param>
	/// <returns>The distance to the other vector.</returns>
	float DistanceTo(const Vector3_c& position) const;

	/// <summary>
	/// Creates a random vector inside the circle around this position.
	/// </summary>
	/// <param name="distance">The distance from the vector within the new vector will lie.</param>
	/// <returns>A random vector around the position</returns>
	Vector3_c Around(float distance) const;

	/// <summary>
	/// Gets a point on the circumference of a circle of the provided radius. Z axis ignored.
	/// </summary>
	/// <param name="radius">The radius of the circle.</param>
	/// <param name="angleInDegrees">The angle (in degrees) that the new point will have from 0.</param>
	/// <returns>A point on the circumference of a circle of the provided radius.</returns>
	Vector3_c PointOnCircle(float radius, float angleInDegrees) const;

	/// <summary>
	/// Creates a list of points around this position within the given radius. Z axis ignored.
	/// </summary>
	/// <param name="results">The output points. The collection is not cleared before adding in points.</param>
	/// <param name="fullRadius">The (boundry) radius of the circle to be made.</param>
	/// <param name="angleDifference">The difference in angles (in degrees) of each 2 points.</param>
	/// <param name="intervalDistance">The distance between 2 consecutive points.</param>
	/// <param name="includeCentre">Determines whether the origin should also be included in the list of points.</param>
	///// <returns>A list of points around this position within the given radius.</returns>
	void PointsOnCircle(std::vector<Vector3_c>& results, float fullRadius, float angleDifference, float intervalDistance, bool includeCentre = false) const;

	///<summary>
	///Gets a point on the surface of a sphere
	///</summary>
	///<param name="radius">The radius of the sphere</param>
	///<param name="longitude">The longitude/pitch, in degrees, ranges from 0 to 360</param>
	///<param name="latitude">The latitude/yaw, in degrees, ranges from -90 to +90</param>
	Vector3_c PointOnSphere(float radius, float longitude, float latitude) const;

	/// <summary>
	/// Gets a point on the circumference of a sphere of the provided radius.
	/// </summary>
	/// <param name="radius">The radius of the sphere.</param>
	/// <param name="angleInDegrees">The angle (in degrees) that the new point will have from 0.</param>
	/// <returns>A point on the circumference of a sphere of the provided radius.</returns>
	/*Vector3_c PointOnSphere(float radius, float angleInDegrees)
	{
		Vector3_c point;
		point.x = this->x + radius * sin(u) * cos(v);
		point.y = this->x + radius * cos(u) * cos(v);
		point.z = this->z + radius * sin(v);

		return point;
	}*/

	/// <summary>
	/// Creates a list of points around this position within the given radius.
	/// </summary>
	/// <param name="fullRadius">The (boundry) radius of the sphere to be made.</param>
	/// <param name="angleInDegrees">The difference in angles (in degrees) of each 2 points.</param>
	/// <param name="intervalDistance">The distance between 2 consecutive points.</param>
	/// <param name="includeCentre">Determines whether the origin should also be included in the list of points.</param>
	/// <returns>A list of points around this position within the given radius.</returns>
	/*std::vector<Vector3_c> PointsOnSphere(float fullRadius, float angleInDegrees, float intervalDistance, bool includeCentre = false)
	{
		std::vector<Vector3_c> points;

		auto& origin = *this;
		Vector3_c current;
		float u, d;

		if (includeCentre) points.push_back(origin);
		if (intervalDistance == 0.0f) return points;

		for (u = 0.0f; u < 360.0f; u += angleInDegrees)
		{
			for (d = intervalDistance; d < fullRadius; d += intervalDistance)
			{
				current.x = d * cos(__degreeToRadian(u)) + origin.x;
				current.y = d * sin(__degreeToRadian(u)) + origin.y;
				current.z = origin.z; // this will stay uniform as we're only considering the circle
				points.push_back(current);
			}
		}

		return points;
	}*/


	/// <summary>
	/// Returns the vector as a float,[4 bytes],float,[4 bytes],float,[4 bytes] struct.
	/// </summary>
	/// <returns>The vector as a float,[4 bytes],float,[4 bytes],float,[4 bytes] struct.</returns>
	Vector3 ToTypeStruct() const;

	/// <summary>
	/// Copies the vector coordinates into a float array.
	/// </summary>
	/// <param name="out">The output array (needs 12 allocated bytes).</param>
	void ToArray(float* out) const;

	/// <summary>
	/// Copies the vector coordinates into a float array.
	/// </summary>
	/// <returns>The vector as an std::array of floats.</returns>
	std::array<float, 3> ToArray() const;
	
	/// <summary>
	/// Returns true if the vector is 0,0,0
	/// </summary>
	bool IsZero() const;
	/// <summary>
	/// Returns vector with modify
	/// </summary>
	Vector3_c Modify(float x, float y, float z);
	/// <summary>
	/// Returns a new normalized vector with random X and Y components.
	/// </summary>
	static Vector3_c RandomXY();

	/// <summary>
	/// Returns a new normalized vector with random X, Y and Z components.
	/// </summary>
	static Vector3_c RandomXYZ();

	/// <summary>
	/// Adds two vectors.
	/// </summary>
	/// <param name="left">The first vector to add.</param>
	/// <param name="right">The second vector to add.</param>
	/// <returns>The sum of the two vectors.</returns>
	static Vector3_c Add(Vector3_c left, Vector3_c right);

	/// <summary>
	/// Subtracts two vectors.
	/// </summary>
	/// <param name="left">The first vector to subtract.</param>
	/// <param name="right">The second vector to subtract.</param>
	/// <returns>The difference of the two vectors.</returns>
	static Vector3_c Subtract(Vector3_c left, Vector3_c right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	static Vector3_c Multiply(Vector3_c value, float scale);

	/// <summary>
	/// Modulates a vector by another.
	/// </summary>
	/// <param name="left">The first vector to modulate.</param>
	/// <param name="right">The second vector to modulate.</param>
	/// <returns>The modulated vector.</returns>
	static Vector3_c Modulate(Vector3_c left, Vector3_c right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	static Vector3_c Divide(Vector3_c value, float scale);

	/// <summary>
	/// Reverses the direction of a given vector.
	/// </summary>
	/// <param name="value">The vector to negate.</param>
	/// <returns>A vector facing in the opposite direction.</returns>
	static Vector3_c Negate(Vector3_c value);

	/// <summary>
	/// Restricts a value to be within a specified range.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="min">The minimum value.</param>
	/// <param name="max">The maximum value.</param>
	/// <returns>The clamped value.</returns>
	static Vector3_c Clamp(Vector3_c value, Vector3_c min, Vector3_c max);

	/// <summary>
	/// Performs a linear interpolation between two vectors.
	/// </summary>
	/// <param name="start">Start vector.</param>
	/// <param name="end">End vector.</param>
	/// <param name="amount">Value between 0 and 1 indicating the weight of <paramref name="end"/>.</param>
	/// <returns>The linear interpolation of the two vectors.</returns>
	/// <remarks>
	/// This method performs the linear interpolation based on the following formula.
	/// <code>start + (end - start) * amount</code>
	/// Passing <paramref name="amount"/> a value of 0 will cause <paramref name="start"/> to be returned; a value of 1 will cause <paramref name="end"/> to be returned. 
	/// </remarks>
	static Vector3_c Lerp(Vector3_c start, Vector3_c end, float amount);

	/// <summary>
	/// Converts the vector into a unit vector.
	/// </summary>
	/// <param name="vector">The vector to normalize.</param>
	/// <returns>The normalized vector.</returns>
	static Vector3_c Normalize(Vector3_c vector);

	/// <summary>
	/// Calculates the dot product of two vectors.
	/// </summary>
	/// <param name="left">First source vector.</param>
	/// <param name="right">Second source vector.</param>
	/// <returns>The dot product of the two vectors.</returns>
	static float Dot(Vector3_c left, Vector3_c right);

	/// <summary>
	/// Calculates the cross product of two vectors.
	/// </summary>
	/// <param name="left">First source vector.</param>
	/// <param name="right">Second source vector.</param>
	/// <returns>The cross product of the two vectors.</returns>
	static Vector3_c Cross(Vector3_c left, Vector3_c right);

	/// <summary>
	/// Returns the reflection of a vector off a surface that has the specified normal. 
	/// </summary>
	/// <param name="vector">The source vector.</param>
	/// <param name="normal">Normal of the surface.</param>
	/// <returns>The reflected vector.</returns>
	/// <remarks>Reflect only gives the direction of a reflection off a surface, it does not determine 
	/// whether the original vector was close enough to the surface to hit it.</remarks>
	static Vector3_c Reflect(Vector3_c vector, Vector3_c normal);

	/// <summary>
	/// Returns a vector containing the smallest components of the specified vectors.
	/// </summary>
	/// <param name="value1">The first source vector.</param>
	/// <param name="value2">The second source vector.</param>
	/// <returns>A vector containing the smallest components of the source vectors.</returns>
	static Vector3_c Minimize(Vector3_c value1, Vector3_c value2);

	/// <summary>
	/// Returns a vector containing the largest components of the specified vectors.
	/// </summary>
	/// <param name="value1">The first source vector.</param>
	/// <param name="value2">The second source vector.</param>
	/// <returns>A vector containing the largest components of the source vectors.</returns>
	static Vector3_c Maximize(Vector3_c value1, Vector3_c value2);

	/// <summary>
	/// Adds two vectors.
	/// </summary>
	/// <param name="left">The first vector to add.</param>
	/// <param name="right">The second vector to add.</param>
	/// <returns>The sum of the two vectors.</returns>
	friend Vector3_c operator + (Vector3_c const& left, Vector3_c const& right);

	/// <summary>
	/// Subtracts two vectors.
	/// </summary>
	/// <param name="left">The first vector to subtract.</param>
	/// <param name="right">The second vector to subtract.</param>
	/// <returns>The difference of the two vectors.</returns>
	friend Vector3_c operator - (Vector3_c const& left, Vector3_c const& right);

	/// <summary>
	/// Reverses the direction of a given vector.
	/// </summary>
	/// <param name="value">The vector to negate.</param>
	/// <returns>A vector facing in the opposite direction.</returns>
	friend Vector3_c operator - (Vector3_c const& value);

	/// <summary>
	/// Multiplies 2 vectors.
	/// </summary>
	/// <param name="left">Vector 1.</param>
	/// <param name="right">Vector 2.</param>
	/// <returns>The product of the 2 vectors.</returns>
	friend Vector3_c operator * (Vector3_c const& left, Vector3_c const& right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3_c operator * (Vector3_c const& value, float const& scale);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3_c operator * (float const& scale, Vector3_c const& value);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3_c operator *= (Vector3_c& value, float const& scale);

	/// <summary>
	/// Divides one vector by another.
	/// </summary>
	/// <param name="left">Vector 1.</param>
	/// <param name="right">Vector 2.</param>
	/// <returns>The quotient of the 2 vectors.</returns>
	friend Vector3_c operator / (Vector3_c const& left, Vector3_c const& right);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3_c operator / (Vector3_c const& value, float const& scale);

	/// <summary>
	/// Scales a vector by the given value.
	/// </summary>
	/// <param name="value">The vector to scale.</param>
	/// <param name="scale">The amount by which to scale the vector.</param>
	/// <returns>The scaled vector.</returns>
	friend Vector3_c operator /= (Vector3_c& value, float const& scale);

	/// <summary>
	/// Tests for equality between two objects.
	/// </summary>
	/// <param name="left">The first value to compare.</param>
	/// <param name="right">The second value to compare.</param>
	/// <returns><c>true</c> if <paramref name="left"/> has the same value as <paramref name="right"/>; otherwise, <c>false</c>.</returns>
	friend bool operator == (Vector3_c const& left, Vector3_c const& right);

	/// <summary>
	/// Tests for inequality between two objects.
	/// </summary>
	/// <param name="left">The first value to compare.</param>
	/// <param name="right">The second value to compare.</param>
	/// <returns><c>true</c> if <paramref name="left"/> has a different value than <paramref name="right"/>; otherwise, <c>false</c>.</returns>
	friend bool operator != (Vector3_c const& left, Vector3_c const& right);


	/// <summary>
	/// Converts the value of the object to its equivalent string representation.
	/// </summary>
	/// <returns>The string representation of the value of this instance.</returns>
	std::string ToString() const;


	/// <summary>
	/// Returns a value that indicates whether the current instance is equal to the specified object. 
	/// </summary>
	/// <param name="other">Object to make the comparison with.</param>
	/// <returns><c>true</c> if the current instance is equal to the specified object; <c>false</c> otherwise.</returns>
	virtual bool Equals(const Vector3& other) const;

	/// <summary>
	/// Returns a value that indicates whether the current instance is equal to the specified object. 
	/// </summary>
	/// <param name="other">Object to make the comparison with.</param>
	/// <returns><c>true</c> if the current instance is equal to the specified object; <c>false</c> otherwise.</returns>
	virtual bool Equals(const Vector3_c& other) const;

	/// <summary>
	/// Determines whether the specified object instances are considered equal. 
	/// </summary>
	/// <param name="value1">The first value to compare.</param>
	/// <param name="value2">The second value to compare.</param>
	/// <returns><c>true</c> if <paramref name="value1"/> contains the same values as <paramref name="value2"/>; 
	/// otherwise, <c>false</c>.</returns>
	static bool Equals(const Vector3_c& value1, const Vector3_c& value2);


	static float DistanceBetween(const Vector3_c& value1, const Vector3_c& value2);

	static Vector3_c RotationToDirection(const Vector3_c& rotation);
	static Vector3_c DirectionToRotation(Vector3_c direction);


};


#endif
