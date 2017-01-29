#pragma once

#include "ITickedObject.h"
#include "IDelayTask.h"
#include <list>

class DelayTaskMgr : public ITickedObject
{
protected:
	int mFrameInterval;	// 帧间隔
	int mTaskNumPerFrameInterval;	// 每一个帧间隔执行任务数量
	int mCurTaskNum;				// 当前执行的任务数量
	int mPreFrame;	// 之前帧
	int mCurFrame;	// 当前帧

	std::list<IDelayTask*> mDelayTaskList;

public:
	DelayTaskMgr();
	~DelayTaskMgr();

public:
	void init();
	void dispose();

	virtual void onTick(float delta) override;
	void addTask(IDelayTask* task);
	void execTask();
};