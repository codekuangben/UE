﻿#include "MyProject.h"
#include "MyGameInstanceBase.h"

#include "AppFrame.h"
//#include "Ctx.h"
#include "UtilStr.h"
#include "MyOnlineSessionBase.h"
#include "LogCategoryDef.h"
#include "MyActorBase.h"
#include "MyMemoryConstructorFlag.h"
#include "MyMemoryAllocatorConfig.h"
#include "MyMemoryDefaultAlloc.h"

MY_USING_NAMESPACE(MyNS)

UMyGameInstanceBase::UMyGameInstanceBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 逻辑指针不要放在构造函数中，因为脚本会调用一次构造函数，这样就会申请多个指针
	//mAppFrame = new AppFrame();

	// 这个设置等价于在 Project Setting 中的设置
	//std::string aaa = "aaa";
	//GetDefault<UGeneralProjectSettings>()->ProjectDisplayedTitle = UtilStr::ConvStdStr2FText(aaa);
	//GetDefault<UInputSettings>()->DefaultViewportMouseCaptureMode = EMouseCaptureMode::CaptureDuringMouseDown;
}

void UMyGameInstanceBase::Init()
{
	Super::Init();

	// UGameEngine 创建后，就创建 UGameInstance，这个是除 UGameEngine 外最早初始化的地方
	AppFrame::initApp();
	//AppFrame::runApp();
}

void UMyGameInstanceBase::Shutdown()
{
	Super::Shutdown();

	AppFrame::quitApp();
}

void UMyGameInstanceBase::FinishDestroy()
{
	Super::FinishDestroy();
}

//void UMyGameInstanceBase::Tick(float DeltaSeconds)
//{
//
//}

TSubclassOf<UOnlineSession> UMyGameInstanceBase::GetOnlineSessionClass()
{
	return UMyOnlineSessionBase::StaticClass();
}

bool UMyGameInstanceBase::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool ret = Super::ProcessConsoleExec(Cmd, Ar, Executor);

	if (!ret)
	{
		//for (AMyActorBase* actor : mActorVec)
		//{
		//	ret = actor->ProcessConsoleExec(Cmd, Ar, Executor);
		//}
	}

	UE_LOG(ComWarning, Warning, TEXT("--- Cmd:%s exec:%d"), Cmd, (int32)ret);

	return ret;
}

void UMyGameInstanceBase::LoadMapAsync(FName LevelName)
{
	mCachedLevelName = LevelName;

	// Check whether requested map exists, this could be very slow if LevelName is a short package name
	FString LongPackageName;
	bool bOutSuccess = FPackageName::SearchForPackageOnDisk(LevelName.ToString(), &LongPackageName);
	if (!bOutSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("Map not found"));
		return;
	}

	int32 value = LoadPackageAsync(LongPackageName, FLoadPackageAsyncDelegate::CreateUObject(this, &UMyGameInstanceBase::FinishedStreamingLevel));
	UE_LOG(LogTemp, Error, TEXT("Loading Map %s async"), *LongPackageName);
}

void UMyGameInstanceBase::FinishedStreamingLevel(const FName& PackageName, UPackage* InLevelPackage, EAsyncLoadingResult::Type Result)
{
	if (InLevelPackage)
	{
		UE_LOG(LogTemp, Error, TEXT("Finished Loading"));

		mLevelPackage = InLevelPackage;
		mLevelPackage->AddToRoot();

		//OnFinishedLevelLoad.Broadcast();
	}
}