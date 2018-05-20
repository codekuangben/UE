﻿#include "MyProject.h"
#include "MaterialInsRes.h"
#include "ResItem.h"
#include "Materials/MaterialInstanceDynamic.h"	// UMaterialInstanceDynamic
#include "Materials/MaterialInterface.h"	// UMaterialInterface
#include "Materials/MaterialInstance.h"	// UMaterialInstance, class UMaterialInstance : public UMaterialInterface
#include "Materials/Material.h"	// UMaterial, class UMaterial : public UMaterialInterface

MaterialInsRes::MaterialInsRes()
{

}

UMaterialInterface* MaterialInsRes::getMaterial()
{
	return this->mMaterial;
}

void MaterialInsRes::initImpl(ResItem* res)
{
	// 获取资源单独保存
	this->mMaterial = Cast<UMaterialInterface>(res->getObject(res->getPrefabName()));
	this->mMaterialDyn = UMaterialInstanceDynamic::Create(this->mMaterial, nullptr);

	Super::initImpl(res);
}

void MaterialInsRes::unload()
{
	if (this->mMaterial != nullptr)
	{
		// 这个接口不知道行不行
		//UtilSysLibWrap::UnloadAsset(this->mMaterial);
		this->mMaterial = nullptr;

		// 这个接口肯定可以
		//UtilSysLibWrap.UnloadUnusedAssets();
	}

	Super::unload();
}