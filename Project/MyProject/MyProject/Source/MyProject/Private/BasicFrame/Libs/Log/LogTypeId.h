#pragma once

#include "PlatformDefine.h"

MY_BEGIN_NAMESPACE(MyNS)

enum class LogTypeId
{
	eLogCommon,         // 通用日志
	eLogDataStruct,		// 数据结构日志
	eLogPriorityListCheck,
	eLogNoPriorityListCheck, 
};

MY_END_NAMESPACE