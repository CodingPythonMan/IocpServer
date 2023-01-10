#pragma once
#include "Allocator.h"

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xAlloc(sizeof(Type)));
	new(memory)Type(std::forward<Args>(args)...);// placement new
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xRelease(obj);
}