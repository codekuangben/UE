#pragma once

#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

UCLASS()
class AMyHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:
	/** Main HUD update loop. */
	virtual void DrawHUD() override;

		
protected:
	/** kills background icon. */
	UPROPERTY()
	FCanvasIcon KillsBg;
};
