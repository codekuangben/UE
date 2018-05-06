﻿#ifndef __TableObject_H
#define __TableObject_H

#include <string>
#include "HAL/Platform.h"
#include "TableItemBodyBase.h"

class TableObjectItemBody : TableItemBodyBase
{
public:
    std::string mName;
    int mMaxNum;
    int mType;
    int mColor;
	std::string mObjResName;

	TableObjectItemBody();
	virtual ~TableObjectItemBody();

	virtual void parseBodyByteBuffer(ByteBuffer* bytes, uint32 offset);
};

#endif