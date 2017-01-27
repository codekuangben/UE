﻿#include "MyProject.h"
#include "SceneSys.h"
#include "AddOnceAndCallOnceEventDispatch.h"
#include "Scene.h"
#include "AuxLevelLoader.h"
#include "IDispatchObject.h"
#include "SafePointer.h"

SceneSys::SceneSys()
{
	this->mOnSceneLoadedDispatch = SAFE_NEW AddOnceAndCallOnceEventDispatch();
	this->mAuxLevelLoader = nullptr;
	this->mScene = nullptr;

	//this->init();
}

void SceneSys::init()
{
	//this->mOnSceneLoadedDispatch = SAFE_NEW AddOnceAndCallOnceEventDispatch();
	//this->mAuxLevelLoader = SAFE_NEW MyNS::AuxLevelLoader();
}

void SceneSys::dispose()
{
	SAFE_DELETE(this->mOnSceneLoadedDispatch);
	SAFE_DELETE(this->mAuxLevelLoader);
}

Scene* SceneSys::getCurScene()
{
	return this->mScene;
}

bool SceneSys::isSceneLoaded()
{
	if (nullptr != this->mScene)
	{
		return this->mScene->isSceneLoaded();
	}

	return false;
}

void SceneSys::loadScene(std::string filename, EventDispatchDelegate func)
{
	// 卸载之前的场景
	this->unloadScene();

	// 加载新的场景
	this->mScene = new Scene();
	if (func != nullptr)
	{
		this->mOnSceneLoadedDispatch->addEventHandle(func);
	}

	this->loadSceneRes(filename);
}

void SceneSys::unloadScene()
{
	if (nullptr != this->mScene)
	{
		this->mScene->dispose();
		this->mScene = nullptr;
	}
	if (nullptr != this->mAuxLevelLoader)
	{
		this->mAuxLevelLoader->unload();
		this->mAuxLevelLoader = nullptr;
	}
}

void SceneSys::loadSceneRes(std::string filename)
{
	//Ctx.mInstance.mNetCmdNotify.isStopNetHandle = true;        // 加载场景需要停止处理消息，因为很多资源都要等到场景加载完成才初始化

	if (nullptr == this->mAuxLevelLoader)
	{
		this->mAuxLevelLoader = new MyNS::AuxLevelLoader();
	}

	this->mAuxLevelLoader->asyncLoad(filename, EventDispatchDelegate(this, &SceneSys::onSceneResLoadded));
}

void SceneSys::onSceneResLoadded(IDispatchObject* dispObj)
{
	//ResItem res = dispObj as ResItem;
	this->mOnSceneLoadedDispatch->dispatchEvent(mScene);

	//Ctx.mInstance.mNetCmdNotify.isStopNetHandle = false;        // 加载场景完成需要处理处理消息

	this->mAuxLevelLoader->unload();

	this->mScene->init();
}

// 卸载多有的场景
void SceneSys::unloadAll()
{
	this->unloadScene();
}