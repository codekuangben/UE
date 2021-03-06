﻿//#include "MyProjectEditor.h"
#include "EditorTestAsset.h"
#include "MyAssetRegistry.h"
#include "Ctx.h"
#include "UObject/NameTypes.h"	// FName
#include "Containers/Array.h"	// TArray
#include "Misc/AssetRegistryInterface.h"	// EAssetRegistryDependencyType
#include "AssetData.h"	// FAssetData
#include "Engine/Texture2D.h"		// UTexture2D 

#include "WidgetBlueprint.h"		// Engine\Source\Editor\UMGEditor\Public\WidgetBlueprint.h, UWidgetBlueprint
#include "PaperFlipbook.h"   // Engine\Plugins\2D\Paper2D\Source\Paper2D\Classes\PaperFlipbook.h, UPaperFlipbook

MY_BEGIN_NAMESPACE(MyNS)

void EditorTestAsset::run()
{
	this->_testDep();
}

void EditorTestAsset::_testDep()
{
	TArray<FName> OutDependencies;
	// 这个是两个模块变量，不是一个模块变量
	//GMyAssetRegistry->GetDependencies(TEXT("Blueprint'/Game/MyAsset/MyTom/PakTest.PakTest'"), OutDependencies);
}
//
//void EditorTestAsset::_testGetAssetData()
//{
//	// Engine\Plugins\2D\Paper2D\Source\Paper2DEditor\Private\PaperSpriteFactory.cpp
//	// UObject* UPaperSpriteFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
//	FString objectPath = TEXT("");
//
//	FAssetData AssetData = GMyAssetRegistry->GetAssetByObjectPath(*objectPath);
//
//	if (AssetData.IsValid())
//	{
//		if (UTexture2D* NormalMapTexture = Cast<UTexture2D>(AssetData.GetAsset()))
//		{
//
//		}
//	}
//}
//
//// http://www.90cg.com/wordpress/ue4_unreal-asset-loading/
void EditorTestAsset::_testLoadAndPackageName()
{
	{
		FStringAssetReference testAssetRef = " /Game/UMG/NewWidgetBlueprint ";
		UObject* pObject = testAssetRef.TryLoad();
		UWidgetBlueprint* pWidgetBlueprint = Cast<UWidgetBlueprint>(pObject);
		//error C2664 : 'WidgetT *CreateWidget<UUserWidget,MyNS::TestAsset>(OwnerT *,TSubclassOf<UUserWidget>,FName)' : cannot convert argument 2 from 'TSubclassOf<UObject>' to 'TSubclassOf<UUserWidget>'
		//auto widget = CreateWidget<UUserWidget>(this, pWidgetBlueprint->GeneratedClass);
		// error C2664: 'WidgetT *CreateWidget<UUserWidget,MyNS::TestAsset>(OwnerT *,TSubclassOf<UUserWidget>,FName)': cannot convert argument 2 from 'T *' to 'TSubclassOf<UUserWidget>'
		//auto widget = CreateWidget<UUserWidget>(this, Cast<UUserWidget>(pWidgetBlueprint->GeneratedClass));
		//auto widget = CreateWidget<UUserWidget>(this, Cast<TSubclassOf<UUserWidget>>(pWidgetBlueprint->GeneratedClass));
		// do something
		// widget->AddToViewport();
		FString pathName1 = FPackageName::LongPackageNameToFilename(pObject->GetPathName(), FPackageName::GetAssetPackageExtension());
		FString pathName2 = FPackageName::LongPackageNameToFilename(pObject->GetOutermost()->GetPathName(), FPackageName::GetAssetPackageExtension());
	}
	{
		FStringAssetReference testAssetRef = " /Game/UGameAssets/Assets/horizo​​n/Maps/MainMap ";
		UObject* pObject = testAssetRef.TryLoad();
		UWorld* pWorld = Cast<UWorld>(pObject);
		FString pathName1 = FPackageName::LongPackageNameToFilename(pObject->GetPathName(), FPackageName::GetMapPackageExtension());
		FString pathName2 = FPackageName::LongPackageNameToFilename(pObject->GetOutermost()->GetPathName(), FPackageName::GetMapPackageExtension());
	}
	{
		FStringAssetReference testAssetRef = " /Game/UGameAssets/Assets/horizo​​n/flipbook/hero_down ";
		UPaperFlipbook* pObject = Cast<UPaperFlipbook>(testAssetRef.TryLoad());
	}
	{
		FStringAssetReference testAssetRef = " /Game/UGameAssets/Assets/horizo​​n/spriter/MyTexture ";
		UTexture2D* pObject = Cast<UTexture2D>(testAssetRef.TryLoad());
	}
}

MY_END_NAMESPACE