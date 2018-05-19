﻿#ifndef __UtilTable_H
#define __UtilTable_H

#include "MyProject.h"
#include "string"

class MByteBuffer;

class UtilTable
{
public:
    static uint32 mPrePos;        // 记录之前的位置
    static uint16 msCnt;
	static void readString(MByteBuffer* bytes, std::string& tmpStr);
};

#endif