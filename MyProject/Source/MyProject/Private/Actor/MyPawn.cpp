#include "MyProject.h"
#include "MyPawn.h"

AMyPawn::AMyPawn(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void AMyPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AMyProjectGameState* MyGameState = Cast<AMyProjectGameState>(GetWorld()->GetGameState());
	if (MyGameState)
	{

	}
}

void AMyPawn::SetupPlayerInputComponent(class UInputComponent* InputComponentParam)
{
	Super::SetupPlayerInputComponent(InputComponentParam);

	//InputComponentParam->BindAction("ZoomIn", IE_Pressed, this, &AMyActorBase::CameraZoomIn);
	//InputComponentParam->BindAction("ZoomOut", IE_Pressed, this, &AMyActorBase::CameraZoomOut);
}