﻿#ifndef __UiLoadingItem_H
#define __UiLoadingItem_H

#include <string>
#include "UiFormId.h"

using namespace NSFormId;

/**
 * @brief 正在加载的 Item ，主要是防止重复加载
 */
class UiLoadingItem
{
public:
	UiFormId mId;               // Form formId
    std::string mResPath;            // 拖放的控件 path 

public:
	UiLoadingItem();
};

#endif