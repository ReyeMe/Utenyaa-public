#pragma once

#include "..\utils\std\utils.h"

template<typename T>
concept HasLoadSizeFunction = requires {
	{ std::declval<T>().LoadSize() } -> std::same_as<size_t>;
};

template<typename T>
T* GetAndIterate(char*& iterator, size_t count = 1)
{
	T* ptr = reinterpret_cast<T*>(iterator);

	if constexpr (HasLoadSizeFunction<T>)
	{
		iterator += ptr->LoadSize() * count;
	}
	else
	{
		iterator += (sizeof(T) * count);
	}

	return ptr;
}
