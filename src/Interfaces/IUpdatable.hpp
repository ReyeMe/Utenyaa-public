#pragma once

#include "..\Utils\TrackableObject.hpp"  // Include necessary header for TrackableObject

/**
 * @brief Interface for updatable objects.
 * 
 * This interface extends TrackableObject to allow tracking of objects that implement it.
 */
struct IUpdatable : public TrackableObject<IUpdatable>
{
    /**
     * @brief Virtual function for updating the object.
     * 
     * Derived classes should implement this function to define the updating behavior.
     */
    virtual void Update() {}
};
