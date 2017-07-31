#include "MyProject.h"
#include "MClassInfo.h"
#include "GObject.h"
#include "TypeDef.h"
#include "MClassFactory.h"
#include <stdlib.h>

void MClassInfo::Construct(const char* className, Creator creatorFunc, const MClassInfo* parentClass, SizeT instSize)
{
	my_assert(0 != className);
	my_assert(parentClass != this);

	this->mParent = parentClass;
	this->mCreator = creatorFunc;
	this->mInstanceSize = instSize;

	this->mName = className;
	if (nullptr != className)
	{
		if (!MClassFactory::Instance()->ClassExists(this->mName))
		{
			MClassFactory::Instance()->Register(this, this->mName);
		}
#if MY_DEBUG
		else
		{
			const MClassInfo* checkClassInfo = MClassFactory::Instance()->GetClassInfo(this->mName);
			my_assert(0 != checkClassInfo);
			if (checkClassInfo != this)
			{
				
			}
		}
#endif
	}
}

MClassInfo::MClassInfo(const char* className, Creator creatorFunc, const MClassInfo* parentClass, SizeT instSize)
{
	this->Construct(className, creatorFunc, parentClass, instSize);
}

GObject* MClassInfo::Create() const
{
	if (0 == this->mCreator)
	{
		return 0;
	}
	else
	{
		return this->mCreator();
	}
}

bool MClassInfo::IsDerivedFrom(const MClassInfo& other) const
{
	const MClassInfo* cur;
	for (cur = this; cur != 0; cur = cur->GetParent())
	{
		if (cur == &other)
		{
			return true;
		}
	}
	return false;
}

bool MClassInfo::IsDerivedFrom(const std::string& otherClassName) const
{
	const MClassInfo* cur;
	for (cur = this; cur != 0; cur = cur->GetParent())
	{
		if (cur->mName == otherClassName)
		{
			return true;
		}
	}
	return false;
}

void* MClassInfo::AllocInstanceMemory()
{
#if MY_OBJECTS_USE_MEMORYPOOL
	//void* ptr = Memory::ObjectPoolAllocator->Alloc(this->instanceSize);
#else
	//void* ptr = Memory::Alloc(Memory::ObjectHeap, this->instanceSize);
#endif
	void* ptr = malloc(mInstanceSize);
	return ptr;
}

void MClassInfo::FreeInstanceMemory(void* ptr)
{
#if MY_OBJECTS_USE_MEMORYPOOL
	//Memory::ObjectPoolAllocator->Free(ptr, this->instanceSize);
#else
	//Memory::Free(Memory::ObjectHeap, ptr);
#endif
	free(ptr);
}