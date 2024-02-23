#pragma once

#include <jo\Jo.hpp>
#include "..\Math\Vec3.hpp"

/** @brief Axis aligned box
 */
struct AABB
{
	/** @brief Minimum point of the box
	 */
	Vec3 Minimum;

	/** @brief Maximum point of the box
	 */
	Vec3 Maximum;

	/** @brief Initializes a new instance of an axis aligned box
	 */
	AABB()
	{
		this->Minimum = Vec3();
		this->Maximum = Vec3();
	}

	/** @brief Initializes a new instance of an axis aligned box
	 * @param center Points describing the box
	 * @param extent Extent from center of the box
	 */
	AABB(const Vec3& center, const Vec3& extent)
	{
		this->Minimum = center - extent;
		this->Maximum = center + extent;
	}

	/** @brief Initializes a new instance of an axis aligned box
	 * @param points Points describing the box
	 * @param count Number of points
	 */
	AABB(const Vec3 * points, unsigned int count)
	{
		if (count > 0)
		{
			this->Minimum = points[0];
			this->Maximum = points[0];

			for (unsigned int point = 1; point < count; point++)
			{
				this->Minimum.x = JO_MIN(this->Minimum.x, points[point].x);
				this->Minimum.y = JO_MIN(this->Minimum.y, points[point].y);
				this->Minimum.z = JO_MIN(this->Minimum.z, points[point].z);
				
				this->Maximum.x = JO_MIN(this->Maximum.x, points[point].x);
				this->Maximum.y = JO_MIN(this->Maximum.y, points[point].y);
				this->Maximum.z = JO_MIN(this->Maximum.z, points[point].z);
			}
		}
		else
		{
			this->Minimum = Vec3();
			this->Maximum = Vec3();
		}
	}

	/** @brief Get the Center point of the box
	 * @return Center point of the AABB
	 */
	constexpr Vec3 GetCenter()
	{
		return (this->Minimum + this->Maximum) >> 1;
	}

	/** @brief Checks whether boxes collide
	 * @param other Box to test
	 * @return true If boxes do collide
	 */
	bool Collide(const AABB * box)
	{
		return (
			box->Minimum.x <= this->Maximum.x &&
			box->Maximum.x >= this->Minimum.x &&
			box->Minimum.y <= this->Maximum.y &&
			box->Maximum.y >= this->Minimum.y &&
			box->Minimum.z <= this->Maximum.z &&
			box->Maximum.z >= this->Minimum.z);
	}
	
	/** @brief Checks whether boxes collide
	 * @param point Point to test
	 * @return true If boxes do collide
	 */
	bool Collide(const Vec3 * point)
	{
		return (
			point->x >= this->Minimum.x &&
			point->x <= this->Maximum.x &&
			point->y >= this->Minimum.y &&
			point->y <= this->Maximum.y &&
			point->z >= this->Minimum.z &&
			point->z <= this->Maximum.z);
	}
};
