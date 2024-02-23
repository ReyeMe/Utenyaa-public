#pragma once

#include "std\vector.h"  // Include necessary header for std::vector
#include "Message.hpp"

/**
 * @brief Template structure for objects that can be tracked.
 * @tparam T The type of the trackable object.
 */
template <typename T>
struct TrackableObject : public IMessageHandler
{
    inline static std::vector<T*> objects; /**< Static vector to store pointers to trackable objects. */

    /**
     * @brief Constructor for TrackableObject.
     *
     * Registers the object in the objects vector upon creation.
     */
    TrackableObject()
    {
        objects.push_back(static_cast<T*>(this));
    }

    /**
     * @brief Virtual destructor for TrackableObject.
     *
     * Unregisters the object from the objects vector upon destruction.
     */
    virtual ~TrackableObject()
    {
        auto it = std::find(objects.begin(), objects.end(), this);
        if (it != objects.end())
        {
            objects.erase(it);
        }
    }

    /**
     * @brief Finds the first object in the objects vector.
	 * @return First item
     */
    static T* FirstOrDefault()
    {
        for (T* obj : objects)
        {
            if (obj != nullptr)
            {
                return obj;
            }
        }

		return nullptr;
    }

    /**
     * @brief Finds the first object in the objects vector that satisfies a condition.
     * @tparam ConditionLambda The type of the lambda function specifying the condition.
     * @param condition The lambda function specifying the condition.
	 * @return First found item
     */
    template <typename ConditionLambda>
    static T* FirstOrDefault(ConditionLambda condition)
    {
        for (T* obj : objects)
        {
            if (obj != nullptr && condition(obj))
            {
                return obj;
            }
        }

		return nullptr;
    }
};
