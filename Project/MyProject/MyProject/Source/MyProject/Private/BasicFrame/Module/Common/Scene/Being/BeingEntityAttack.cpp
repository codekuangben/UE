﻿#include "MyProject.h"
#include "BeingEntityAttack.h"
#include "MClassFactory.h"

MY_BEGIN_NAMESPACE(MyNS)

BeingEntityAttack::BeingEntityAttack(BeingEntity* entity)
{
	mEntity = entity;
}

void BeingEntityAttack::init()
{

}

void BeingEntityAttack::dispose()
{

}

void BeingEntityAttack::onPutInPool()
{

}

void BeingEntityAttack::onTick(float delta, TickMode tickMode)
{

}

void BeingEntityAttack::overlapToEnter(BeingEntity bBeingEntity, const FHitResult& SweepResult)
{
	
}

void BeingEntityAttack::overlapToStay(BeingEntity bBeingEntity, const FHitResult& SweepResult)
{
	
}

void BeingEntityAttack::overlapToExit(BeingEntity bBeingEntity, const FHitResult& SweepResult)
{
	
}

MY_END_NAMESPACE