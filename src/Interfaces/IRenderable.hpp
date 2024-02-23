#pragma once

#include "..\Utils\TrackableObject.hpp"  // Include necessary header for TrackableObject

/**
 * @brief Interface for renderable objects.
 * 
 * This interface extends TrackableObject to allow tracking of objects that implement it.
 */
struct IRenderable : public TrackableObject<IRenderable>
{
    /**
     * @brief Virtual function for rendering the object.
     * 
     * Derived classes should implement this function to define the rendering behavior.
     */
    virtual void Draw() {}
};
