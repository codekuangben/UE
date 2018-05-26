#pragma once

#include "MClassInfo.h"
#include "MClassMacros.h"

class GObjectBase
{
	M_DECLARE_ROOT_CLASS(GObjectBase)

public:
	GObjectBase();

protected:
	virtual ~GObjectBase();
};