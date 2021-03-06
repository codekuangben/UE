﻿#include "MyProject.h"
#include "LoopDepth.h"
#include "EventDispatchFunctionObject.h"
#include "MyMemoryConstructorFlag.h"
#include "MyMemoryAllocatorConfig.h"
#include "SafePointer.h"

MY_BEGIN_NAMESPACE(MyNS)

LoopDepth::LoopDepth()
{
	this->mLoopDepth = 0;
	this->mIncHandle = nullptr;
	this->mDecHandle = nullptr;
	this->mZeroHandle = nullptr;
}

void LoopDepth::init()
{

}

void LoopDepth::dispose()
{
	MY_SAFE_DISPOSE(this->mIncHandle);
	MY_SAFE_DISPOSE(this->mDecHandle);
	MY_SAFE_DISPOSE(this->mZeroHandle);
}

void LoopDepth::setIncHandle(EventDispatchDelegate handle)
{
	if (nullptr == this->mIncHandle)
	{
		this->mIncHandle = MY_NEW EventDispatchFunctionObject();
	}

	this->mIncHandle->setFuncObject(handle);
}

void LoopDepth::setDecHandle(EventDispatchDelegate handle)
{
	if (nullptr == this->mDecHandle)
	{
		this->mDecHandle = MY_NEW EventDispatchFunctionObject();
	}

	this->mDecHandle->setFuncObject(handle);
}

void LoopDepth::setZeroHandle(EventDispatchDelegate handle)
{
	if (nullptr == this->mZeroHandle)
	{
		this->mZeroHandle = MY_NEW EventDispatchFunctionObject();
	}

	this->mZeroHandle->setFuncObject(handle);
}

void LoopDepth::_incDepth()
{
	++this->mLoopDepth;

	if (nullptr != this->mIncHandle)
	{
		this->mIncHandle->call(nullptr);
	}
}

void LoopDepth::_decDepth()
{
	--this->mLoopDepth;

	if (nullptr != this->mDecHandle)
	{
		this->mDecHandle->call(nullptr);
	}

	if (0 == this->mLoopDepth)
	{
		if (nullptr != this->mZeroHandle)
		{
			this->mZeroHandle->call(nullptr);
		}
	}

	if (this->mLoopDepth < 0)
	{
		this->mLoopDepth = 0;
		// 错误，不对称
		//LogError("LoopDepth::_decDepth, Error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}
}

bool LoopDepth::_isInDepth()
{
	return this->mLoopDepth > 0;
}

MY_END_NAMESPACE