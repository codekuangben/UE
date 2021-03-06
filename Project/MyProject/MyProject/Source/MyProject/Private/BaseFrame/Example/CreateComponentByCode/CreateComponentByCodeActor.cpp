#include "MyProject.h"
#include "CreateComponentByCodeActor.h"
#include "Components/SkeletalMeshComponent.h"	// USkeletalMeshComponent
#include "Animation/AnimSingleNodeInstance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"		// UBehaviorTreeComponent
#include  "BehaviorTree/BehaviorTree.h"		// UBehaviorTree
#include "Engine/EngineTypes.h"	// FAttachmentTransformRules

//ACreateComponentByCodeActor::ACreateComponentByCodeActor(const class FObjectInitializer& PCIP)
//	: Super(PCIP)
//{
//
//}

void ACreateComponentByCodeActor::createSkeletalMeshComponent()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> npcSkeletalMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> npcAmin(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));
	UAnimBlueprintGeneratedClass* AminClass = LoadObject<UAnimBlueprintGeneratedClass>(UAnimBlueprintGeneratedClass::StaticClass(), TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));
	this->mMeshCompPtr = this->CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NpcMesh"));
	this->mMeshCompPtr->SetSkeletalMesh(npcSkeletalMesh.Object);
	this->mMeshCompPtr->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	UClass* ap = (UClass*)npcAmin.Object->GeneratedClass;
	this->mMeshCompPtr->SetAnimInstanceClass(ap);
	this->mMeshCompPtr->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	//	this->RootComponent = this->mMeshCompPtr;
	// warning C4996: 'USceneComponent::AttachTo': This function is deprecated, please use AttachToComponent instead. Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
	//this->mMeshCompPtr->AttachTo(this->RootComponent);
	this->mMeshCompPtr->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACreateComponentByCodeActor::createBehaviorTreeComponent()
{
	this->mBehaviorTreeCompPtr = this->CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("NpcBehaviorTree"));
	UBehaviorTree* BehaviorTree = LoadObject<UBehaviorTree>(UBehaviorTree::StaticClass(), TEXT("BehaviorTree'/Game/UBehaviorTree/ThirdPerson_BehaviorTreeBP.ThirdPerson_BehaviorTreeBP'"));
	this->mBehaviorTreeCompPtr->StartTree(*BehaviorTree);
}