﻿#include "MyProject.h"
#include "AuxMObjectLoaderBase.h"
#include "UtilMath.h"
#include "UtilApi.h"
#include "ObjectAssetInsRes.h"
#include "Common.h"
#include "ResInsEventDispatch.h"
#include "ResLoadState.h"

namespace MyNS
{
	AuxMObjectLoaderBase::AuxMObjectLoaderBase(std::string path, bool isNeedInsPrefab, bool isInsNeedCoroutine)
		: Super(path)
	{
		this->mIsInsNeedCoroutine = isInsNeedCoroutine;
		this->mIsDestroySelf = true;
		this->mIsNeedInsPrefab = isNeedInsPrefab;

		this->mIsSetInitOrientPos = false;
		this->mIsSetFakePos = false;
	}

	void AuxMObjectLoaderBase::setIsInitOrientPos(bool isSet)
	{
		this->mIsSetInitOrientPos = isSet;
	}

	void AuxMObjectLoaderBase::setIsFakePos(bool isSet)
	{
		this->mIsSetFakePos = isSet;
	}

	void AuxMObjectLoaderBase::dispose()
	{
		if (this->mIsDestroySelf)
		{
			if (this->mSelfGo != nullptr)
			{
				UtilApi::DestroyImmediate(Cast<AActor>(this->mSelfGo));
			}
		}

		Super::dispose();
	}

	UObject* AuxMObjectLoaderBase::getSelfGo()
	{
		return this->mSelfGo;
	}

	void AuxMObjectLoaderBase::setSelfGo(UObject* value)
	{
		this->mSelfGo = value;
	}

	bool AuxMObjectLoaderBase::isDestroySelf()
	{
		return this->mIsDestroySelf;
	}

	void AuxMObjectLoaderBase::setDestroySelf(bool value)
	{
		this->mIsDestroySelf = value;
	}

	std::string AuxMObjectLoaderBase::getLogicPath()
	{
		if (this->mPrefabRes != nullptr)
		{
			return this->mPrefabRes->getLogicPath();
		}

		return this->mPath;
	}

	void AuxMObjectLoaderBase::syncLoad(std::string path, EventDispatchDelegate evtHandle)
	{
		Super::syncLoad(path, evtHandle);

		if (this->isInvalid())
		{
			this->mPrefabRes = GObjectAssetInsMgr->getAndSyncLoadRes(path, evtHandle);
			this->onPrefabLoaded(mPrefabRes);
		}
		else if (this->hasLoadEnd())
		{
			this->onPrefabLoaded(mPrefabRes);
		}
	}

	// 异步加载对象
	void AuxMObjectLoaderBase::asyncLoad(std::string path, EventDispatchDelegate evtHandle)
	{
		Super::asyncLoad(path, evtHandle);

		if (this->isInvalid())
		{
			this->mPrefabRes = GObjectAssetInsMgr->getAndAsyncLoadRes(path, EventDispatchDelegate(this, &AuxMObjectLoaderBase::onPrefabLoaded));
		}
		else if (this->hasLoadEnd())
		{
			this->onPrefabLoaded(this->mPrefabRes);
		}
	}

	void AuxMObjectLoaderBase::onPrefabLoaded(IDispatchObject* dispObj)
	{
		if (nullptr != dispObj)
		{
			// 一定要从这里再次取值，因为如果这个资源已经加载，可能在返回之前就先调用这个函数，因此这个时候 mPrefabRes 还是空值
			this->mPrefabRes = (ObjectAssetInsRes*)dispObj;

			if (this->mPrefabRes->hasSuccessLoaded())
			{
				this->mResLoadState->setSuccessLoaded();

				if (this->mIsNeedInsPrefab)
				{
					if (this->mIsInsNeedCoroutine)
					{
						this->mResInsEventDispatch = new ResInsEventDispatch();
						this->mResInsEventDispatch->addEventHandle(EventDispatchDelegate(this, &AuxMObjectLoaderBase::onPrefabIns));

						if (this->mIsSetFakePos)
						{
							this->mPrefabRes->InstantiateObject(this->mPrefabRes->getPrefabName(), this->mIsSetInitOrientPos, UtilApi::FAKE_POS, UtilMath::UnitQuat, this->mResInsEventDispatch);
						}
						else
						{
							this->mPrefabRes->InstantiateObject(this->mPrefabRes->getPrefabName(), this->mIsSetInitOrientPos, UtilMath::ZeroVec3, UtilMath::UnitQuat, this->mResInsEventDispatch);
						}
					}
					else
					{
						if (this->mIsSetFakePos)
						{
							this->setSelfGo(this->mPrefabRes->InstantiateObject(this->mPrefabRes->getPrefabName(), this->mIsSetInitOrientPos, UtilApi::FAKE_POS, UtilMath::UnitQuat));
						}
						else
						{
							this->setSelfGo(this->mPrefabRes->InstantiateObject(this->mPrefabRes->getPrefabName(), this->mIsSetInitOrientPos, UtilMath::ZeroVec3, UtilMath::UnitQuat));
						}

						this->onAllFinish();
					}
				}
				else
				{
					this->onAllFinish();
				}
			}
			else if (this->mPrefabRes->hasFailed())
			{
				this->mResLoadState->setFailed();

				GObjectAssetInsMgr->unload(this->mPrefabRes->getResUniqueId(), EventDispatchDelegate(this, &AuxMObjectLoaderBase::onPrefabLoaded));
				this->mPrefabRes = nullptr;

				if (this->mEvtHandle != nullptr)
				{
					this->mEvtHandle->dispatchEvent(this);
				}
			}
		}
		else
		{
			if (this->mEvtHandle != nullptr)
			{
				this->mEvtHandle->dispatchEvent(this);
			}
		}
	}

	void AuxMObjectLoaderBase::onPrefabIns(IDispatchObject* dispObj)
	{
		this->mResInsEventDispatch = (ResInsEventDispatch*)dispObj;
		this->setSelfGo(this->mResInsEventDispatch->getInsGO());
		this->onAllFinish();
	}

	// 所有的资源都加载完成
	void AuxMObjectLoaderBase::onAllFinish()
	{
		if (this->mIsNeedInsPrefab)
		{
			if (this->mSelfGo != nullptr)
			{
				this->mResLoadState->setSuccessLoaded();
			}
			else
			{
				this->mResLoadState->setFailed();
			}
		}
		else
		{
			if (nullptr != this->mPrefabRes && this->mPrefabRes->hasSuccessLoaded())
			{
				this->mResLoadState->setSuccessLoaded();
			}
			else
			{
				this->mResLoadState->setFailed();
			}
		}

		if (this->mEvtHandle != nullptr)
		{
			this->mEvtHandle->dispatchEvent(this);
		}
	}

	void AuxMObjectLoaderBase::unload()
	{
		if (this->mPrefabRes != nullptr)
		{
			GObjectAssetInsMgr->unload(this->mPrefabRes->getResUniqueId(), EventDispatchDelegate(this, &AuxMObjectLoaderBase::onPrefabLoaded));
			this->mPrefabRes = nullptr;
		}

		if (this->mResInsEventDispatch != nullptr)
		{
			this->mResInsEventDispatch->setIsValid(false);
			this->mResInsEventDispatch = nullptr;
		}

		if (this->mEvtHandle != nullptr)
		{
			this->mEvtHandle->clearEventHandle();
			this->mEvtHandle = nullptr;
		}
	}

	UObject* AuxMObjectLoaderBase::getGameObject()
	{
		return this->mSelfGo;
	}

	// 获取预制模板
	UObject* AuxMObjectLoaderBase::getPrefabTmpl()
	{
		UObject* ret = nullptr;
		if (nullptr != this->mPrefabRes)
		{
			ret = Cast<UObject>(this->mPrefabRes->getObject());
		}
		return ret;
	}

	void AuxMObjectLoaderBase::setClientDispose(bool isDispose)
	{

	}

	bool AuxMObjectLoaderBase::isClientDispose()
	{
		return false;
	}

	UObject* AuxMObjectLoaderBase::InstantiateObject(EventDispatchDelegate insHandle)
	{
		if (nullptr == this->mInsEventDispatch && nullptr != insHandle)
		{
			this->mInsEventDispatch = new ResInsEventDispatch();
		}
		if (nullptr != insHandle)
		{
			this->mInsEventDispatch->addEventHandle(insHandle);
		}

		if (this->mIsInsNeedCoroutine)
		{
			if (nullptr == this->mResInsEventDispatch)
			{
				this->mResInsEventDispatch = new ResInsEventDispatch();
			}
			this->mResInsEventDispatch->addEventHandle(EventDispatchDelegate(this,&AuxMObjectLoaderBase::onInstantiateObjectFinish));

			if (this->mIsSetFakePos)
			{
				this->mPrefabRes->InstantiateObject(this->mPrefabRes->getPrefabName(), this->mIsSetInitOrientPos, UtilApi::FAKE_POS, UtilMath::UnitQuat, this->mResInsEventDispatch);
			}
			else
			{
				this->mPrefabRes->InstantiateObject(this->mPrefabRes->getPrefabName(), this->mIsSetInitOrientPos, UtilMath::ZeroVec3, UtilMath::UnitQuat, this->mResInsEventDispatch);
			}
		}
		else
		{
			if (this->mIsSetFakePos)
			{
				this->setSelfGo(this->mPrefabRes->InstantiateObject(this->mPrefabRes->getPrefabName(), this->mIsSetInitOrientPos, UtilApi::FAKE_POS, UtilMath::UnitQuat));
			}
			else
			{
				this->setSelfGo(this->mPrefabRes->InstantiateObject(this->mPrefabRes->getPrefabName(), this->mIsSetInitOrientPos, UtilMath::ZeroVec3, UtilMath::UnitQuat));
			}

			this->onInstantiateObjectFinish();
		}

		return this->mSelfGo;
	}

	void AuxMObjectLoaderBase::onInstantiateObjectFinish(IDispatchObject* dispObj)
	{
		if (nullptr != dispObj)
		{
			this->setSelfGo(this->mResInsEventDispatch->getInsGO());
		}

		if (nullptr != this->mInsEventDispatch)
		{
			this->mInsEventDispatch->dispatchEvent(this);
		}
	}
}