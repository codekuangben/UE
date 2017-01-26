﻿#include "MyProject.h"
#include "LevelLoadItem.h"
#include "ResLoadType.h"
#include "EngineApi.h"
#include "Common.h"
#include "EngineApi.h"
#include "MyDelegateDef.h"

LevelLoadItem::LevelLoadItem()
{
	this->mMyDelegateLevelLoad = nullptr;
	this->mOnLevelLoadDelegate = nullptr;
	this->mOnLevelLoadDelegateHandle = nullptr;
}

LevelLoadItem::~LevelLoadItem()
{

}

void LevelLoadItem::setLevelName(std::string levelName)
{
	this->mLevelName = levelName;
}

std::string LevelLoadItem::getLevelName()
{
	return this->mLevelName;
}

void LevelLoadItem::reset()
{
	if (nullptr != this->mOnLevelLoadDelegate && nullptr != this->mOnLevelLoadDelegateHandle)
	{
		this->mOnLevelLoadDelegate.Remove(this->mOnLevelLoadDelegateHandle);
	}
}

void LevelLoadItem::load()
{
	Super::load();

	if (nullptr == this->mMyDelegateLevelLoad)
	{
		this->mMyDelegateLevelLoad = EngineApi::NewObject<UMyDelegateLevelLoad>();
	}

	this->mOnLevelLoadDelegate = FMyDelegateDef::MyDelegateBaseHandle::FDelegate::CreateUObject(this, &LevelLoadItem::assetAssetBundlesLevelLoaded);
	this->mOnLevelLoadDelegateHandle = this->mOnLevelLoadDelegate.Add(this->mOnLevelLoadDelegate);

	if (eLoadResource == this->mResLoadType)
	{
		this->mNonRefCountResLoadResultNotify->getResLoadState()->setSuccessLoaded();
		this->mNonRefCountResLoadResultNotify->loadResEventDispatch()->dispatchEvent(this);
	}
	else if (eLoadStreamingAssets == mResLoadType ||
		eLoadLocalPersistentData == mResLoadType)
	{
		// 需要加载 AssetBundles 加载
		if (this->mIsLoadNeedCoroutine)
		{
			this->loadFromAssetBundleByCoroutine();
		}
		else
		{
			this->loadFromAssetBundle();
		}
	}
	else if (eLoadWeb == mResLoadType)
	{
		//Ctx.mInstance.mCoroutineMgr.StartCoroutine(downloadAsset());
	}
}

void LevelLoadItem::unload()
{
	Super::unload();
}

void LevelLoadItem::loadFromAssetBundleByCoroutine()
{
	FLatentActionInfo info;
	info.ExecutionFunction = "onLevelLoaded";
	info.CallbackTarget = this->mMyDelegateLevelLoad;
	info.UUID = 0;
	// Linkage 一定要有值，具体参考
	// Engine\Source\Runtime\Engine\Private\LevelStreaming.cpp
	// Response.FinishAndTriggerIf(bIsOperationFinished, LatentInfo.ExecutionFunction, LatentInfo.Linkage, LatentInfo.CallbackTarget);
	// Engine\Source\Runtime\Engine\Private\LatentActionManager.cpp
	// if (LinkInfo.LinkID != INDEX_NONE)
	info.Linkage = 0;

	EngineApi::LoadStreamLevel(GEngineData->getMainActor(), this->mLevelName, true, false, info);
	//EngineApi::SetClientTravel(EngineApi::GetWorld(), TEXT("/Game/MyAsset/Map/TestMap2"), TRAVEL_Absolute);
	//EngineApi::ServerTravel(TEXT("/Game/MyAsset/Map/TestMap2?game=TDM?listen"));
}

void LevelLoadItem::loadFromAssetBundle()
{
	FLatentActionInfo info;
	info.ExecutionFunction = "onLevelLoaded";
	info.CallbackTarget = this->mMyDelegateLevelLoad;
	info.UUID = 0;
	// Linkage 一定要有值，具体参考
	// Engine\Source\Runtime\Engine\Private\LevelStreaming.cpp
	// Response.FinishAndTriggerIf(bIsOperationFinished, LatentInfo.ExecutionFunction, LatentInfo.Linkage, LatentInfo.CallbackTarget);
	// Engine\Source\Runtime\Engine\Private\LatentActionManager.cpp
	// if (LinkInfo.LinkID != INDEX_NONE)
	info.Linkage = 0;

	EngineApi::LoadStreamLevel(GEngineData->getMainActor(), this->mLevelName, true, true, info);
	//EngineApi::SetClientTravel(EngineApi::GetWorld(), TEXT("/Game/MyAsset/Map/TestMap2"), TRAVEL_Absolute);
	//EngineApi::ServerTravel(TEXT("/Game/MyAsset/Map/TestMap2?game=TDM?listen"));
}

void LevelLoadItem::assetAssetBundlesLevelLoaded(IDispatchObject* dispObj)
{
	//if (this->mAssetBundle != nullptr)
	if(true)
	{
		this->mNonRefCountResLoadResultNotify->getResLoadState()->setSuccessLoaded();
	}
	else
	{
		this->mNonRefCountResLoadResultNotify->getResLoadState()->setFailed();
	}

	this->mNonRefCountResLoadResultNotify->loadResEventDispatch->dispatchEvent(this);
}