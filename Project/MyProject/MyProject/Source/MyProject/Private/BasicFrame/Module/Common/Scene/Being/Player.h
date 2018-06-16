#pragma once

#include "SceneEntityBase.h"
#include "MClassInfo.h"
#include "MClassMacros.h"
#include "PlatformDefine.h"

MY_BEGIN_NAMESPACE(MyNS)

/**
 * @brief 场景中的玩家
 */
class Player : BeingEntity
{
	M_DECLARE_CLASS(BeingEntity, SceneEntityBase)

public:
	Player();

protected:
	virtual void _onPreInit() override;
	virtual void _onPostInit() override;

public:
	virtual void onDestroy() override;
	virtual void onPutInPool() override;
	virtual void autoHandle() override;
	virtual void initRender() override;
};

MY_END_NAMESPACE