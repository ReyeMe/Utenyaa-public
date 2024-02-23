#pragma once

#include <jo\jo.hpp>
#include "..\Objects\Model.hpp"
#include "std\vector.h"

/** @brief Loaded model objects manager
 */
class ModelManager
{
private:

	/** @brief Collection of loaded model objects
	 */
	inline static std::vector<Objects::Model*> models;

public:

	/** @brief Load model object into manager
	 * @param model Name of the model file on CD
	 * @return Index of loaded model object
	 */
	static unsigned short LoadModel(const char * model)
	{
        ModelManager::models.push_back(new Objects::Model(model));
		return ModelManager::models.size() - 1;
	}

	/** @brief Get the Model object from manager
	 * @param index Index of model object
	 * @return Pointer to the model object
	 */
	static Objects::Model * GetModel(unsigned short index)
	{
		if (index < ModelManager::models.size())
		{
			return ModelManager::models[index];
		}

		return nullptr;
	}
};
