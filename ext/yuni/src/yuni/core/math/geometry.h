
#ifndef __YUNI_CORE_MATH_GEOMETRY_H__
# define __YUNI_CORE_MATH_GEOMETRY_H__

# include "geometry/point3D.h"
# include "geometry/vector3D.h"

namespace Yuni
{
namespace Geometry
{

	/*!
	** \brief Intersection point of a line and a plane
	**
	** \param linePoint Any point on the line
	** \param lineDirection Direction vector of the line
	** \param planePoint Any point in the plane
	** \param planeNormal Normal vector of the plane
	*/
	template<typename T>
	static Point3D<T> LinePlaneIntersection(const Point3D<T>& linePoint,
		const Vector3D<T>& lineDirection, const Point3D<T>& planePoint,
		const Vector3D<T>& planeNormal);


	/*!
	** \brief Projection point of a point on a line
	**
	** \param point The point to project
	** \param linePoint Any point in the line
	** \param lineDirection Direction vector of the line
	*/
	template<typename T>
	static Point3D<T> PointToLineProjection(const Point3D<T>& point,
		const Point3D<T>& linePoint, const Vector3D<T>& lineDirection);


	/*!
	** \brief Projection point of a point on a plane
	**
	** \param point The point to project
	** \param planePoint Any point in the plane
	** \param planeNormal Normal vector of the plane
	*/
	template<typename T>
	static Point3D<T> PointToPlaneProjection(const Point3D<T>& point,
		const Point3D<T>& planePoint, const Vector3D<T>& planeNormal);


	/*!
	** \brief Angle between two vectors
	*/
	template<typename T>
	static T VectorAngle(const Vector3D<T>& start, const Vector3D<T>& destination);


} // namespace Geometry
} // namespace Yuni

# include "geometry.hxx"

#endif // __YUNI_CORE_MATH_GEOMETRY_H__
