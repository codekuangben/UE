#pragma once

//~~~~~~~~~~~~ UMG ~~~~~~~~~~~~~~~~
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Custom Slate Element
#include "MySlateImage.h"

#include "MyUMGImage.generated.h"

/**
 * @author JoySoftEdgeImage by Rama
 * @brief UMG, Custom Widget Components And Render Code, Usable In UMG Designer
 * @url https://wiki.unrealengine.com/UMG,_Custom_Widget_Components_And_Render_Code,_Usable_In_UMG_Designer
 */

UCLASS()
class UMyUMGImage : public UWidget
{
	GENERATED_UCLASS_BODY()//Custom Slate Element 
protected:
	TSharedPtr<SMySlateImage> MyImage;

	//~~~~~~~~~~
	//Thickness
	// 	Rama's custom UMG Widget property!
	//~~~~~~~~~~
public:
	/** Thickness */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joy Soft Edge Image")
	float Thickness;

	//Delegate verison if you want it!
	//		-Rama
	//UPROPERTY()
	//FGetFloat ThicknessDelegate;

	/** Update thickness of Soft Edge Image! Yay! */
	UFUNCTION(BlueprintCallable, Category = "Joy Soft Edge Image")
	void SetThickness(float InThickness);

	//~~~ Rest is copied from UMG Image.h ~~~
public:

	/** Image to draw */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Appearance)
	FSlateBrush Brush;

	/** A bindable delegate for the Image. */
	UPROPERTY()
	FGetSlateBrush BrushDelegate;

	/** Color and opacity */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Appearance)
	FLinearColor ColorAndOpacity;

	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
	FGetLinearColor ColorAndOpacityDelegate;

public:

	UPROPERTY(EditDefaultsOnly, Category = Events)
	FOnPointerEvent OnMouseButtonDownEvent;

public:

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOpacity(float InOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushFromAsset(USlateBrushAsset* Asset);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrushFromTexture(UTexture2D* Texture);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetBrushFromMaterial(UMaterialInterface* Material);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	UMaterialInstanceDynamic* GetDynamicMaterial();

	// UWidget interface
	virtual void SynchronizeProperties() override;
	// End of UWidget interface

	// UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface

#if WITH_EDITOR
	// warning C4996: 'UWidget::GetEditorIcon': GetEditorIcon is deprecated. Please define widget icons in your style set in the form ClassIcon.MyWidget, and register your style through FClassIconFinder::(Un)RegisterIconSource Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
	// UWidget interface
	//virtual const FSlateBrush* GetEditorIcon() override;
	virtual const FText GetPaletteCategory() override;
	// 最新的使用这个接口
	/** Find the small icon to use for the supplied class */
	//DEPRECATED(4.13, "Please use FSlateIconFinder::FindIconForClass directly (followed by FSlateIcon::GetIcon().")
	//static const FSlateBrush* FindIconForClass(const UClass* InClass, const FName& InDefaultName = FName())
	// End UWidget interface
#endif

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

	/** Translates the bound brush data and assigns it to the cached brush used by this widget. */
	const FSlateBrush* ConvertImage(TAttribute<FSlateBrush> InImageAsset) const;

	FReply HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, ColorAndOpacity);
};