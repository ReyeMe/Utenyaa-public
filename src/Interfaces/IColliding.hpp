#pragma once

#include "..\utils\Math\Vec3.hpp"
#include "..\utils\Geometry\AABB.hpp"
#include "..\Utils\TrackableObject.hpp"  // Include necessary header for TrackableObject

/**
 * @brief Interface for colliding objects.
 * 
 * This interface extends TrackableObject to allow tracking of objects that implement it.
 */
struct IColliding : public TrackableObject<IColliding>
{
	/** @brief Indicates whether collider for this entity is enabled
	 * @return True if enabled
	 */
	virtual bool IsColliderEnabled() { return false; }

	/**
	 * @brief Get object bounds
	 * 
	 * @param result Axis aligned bounding box
	 */
	virtual void GetBounds(AABB * result) {}

    /**
     * @brief Virtual function for coliding with other object.
     * Derived classes should implement this function to define the collision behavior.
	 * 
	 * @param other Object to check collision against
     */
    virtual bool Collide(Vec3 * other)
	{
		if (this->IsColliderEnabled())
		{
			AABB box;
			this->GetBounds(&box);
			return box.Collide(other);
		}

		return false;
	}

    /**
     * @brief Virtual function for coliding with other object.
     * Derived classes should implement this function to define the collision behavior.
	 * 
	 * @param other Object to check collision against
     */
    virtual bool Collide(AABB * other)
	{
		if (this->IsColliderEnabled())
		{
			AABB box;
			this->GetBounds(&box);
			return box.Collide(other);
		}

		return false;
	}

    /**
     * @brief Virtual function for coliding with other object.
     * Derived classes should implement this function to define the collision behavior.
	 * 
	 * @param other Object to check collision against
     */
    virtual bool Collide(IColliding * other)
	{
		if (this->IsColliderEnabled())
		{
			AABB box;
			other->GetBounds(&box);
			return this->Collide(&box);
		}

		return false;
	}
};
