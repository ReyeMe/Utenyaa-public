#pragma once

extern "C" {
#include <jo/jo.h>
#include "stddef.h"
}

static inline bool initMem = []()
{
    extern unsigned int _bstart;
    extern unsigned int _bend;

    jo_memset(&_bstart, 0, &_bend - &_bstart);
    jo_memset((void*)JO_WORK_RAM_SYSTEM_WORK, 0, JO_WORK_RAM_SYSTEM_WORK_SIZE);
    return true;
};

template <class Class, typename Type>
struct JoType
{
    Type* JoPtr()
    {
        static_assert(sizeof(Class) == sizeof(Type), "Casts are impossible, size does not match");
        return (Type*)this;
    }

    const Type* JoConstPtr() const
    {
        static_assert(sizeof(Class) == sizeof(Type), "Casts are impossible, size does not match");
        return (const Type*)this;
    }
};

inline void* operator new(size_t size)
{
    return jo_malloc_with_behaviour(size, JO_MALLOC_TRY_REUSE_SAME_BLOCK_SIZE);
}

inline void operator delete(void* ptr)
{
    jo_free(ptr);
}

inline void operator delete(void* ptr, unsigned int)
{
    jo_free(ptr);
}

inline void* operator new(size_t, void* ptr)
{
    return ptr;
}

// Add overload for array form of new operator
inline void* operator new[](size_t size)
{
    return jo_malloc_with_behaviour(size, JO_MALLOC_TRY_REUSE_SAME_BLOCK_SIZE);
}

// Add overload for array form of delete operator
inline void operator delete[](void* ptr)
{
    jo_free(ptr);
}

// Add overload for array form of delete operator with size parameter
inline void operator delete[](void* ptr, size_t size)
{
    jo_free(ptr);
}

// Add overload for array form of placement new
inline void* operator new[](size_t, void* ptr)
{
    return ptr;
}
