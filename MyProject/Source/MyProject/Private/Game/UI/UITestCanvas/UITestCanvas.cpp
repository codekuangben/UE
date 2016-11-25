#include "MyProject.h"
#include "UITestCanvas.h"
#include "Common.h"
#include "BinaryLoadItem.h"
#include "Ctx.h"
#include "TableSys.h"
#include "DataAssetLoadItem.h"

UUITestCanvas::UUITestCanvas(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UUITestCanvas::onReady()
{
	loadCanvas();
	//loadNoCanvas();
	//loadNamedSlot();
	//loadOverlay();
	//loadCanvasWithNoCanvas();
	//EngineApi::addEventHandle((UButton*)m_umgWidget->GetWidgetFromName("Button_32"), (UObject*)this, "OnConfirmGeneric");
	//unloadCanvas();
}

void UUITestCanvas::OnConfirmGeneric()
{
	//g_pTableSys->getItem(TableID::TABLE_OBJECT, 100);

	//UPackage* tblPkg = CreatePackage(NULL, TEXT("/Game/Table/ObjectBase_client"));
	//UDataAssetLoadItem* ObjectPtr = LoadObject<UDataAssetLoadItem>(tblPkg, TEXT("/Game/Table/ObjectBase_client"));
}

void UUITestCanvas::loadCanvas()
{
	loadUWidget(TEXT("/Game/UMG/UITestCanvas_Canvas.UITestCanvas_Canvas_C"));
	//loadUWidget(TEXT("/CacheData/UMG/UITestCanvas_Canvas.UITestCanvas_Canvas_C"));
	m_GUIWin->m_uiRoot->AddToViewport();
	UButton* pButton = (UButton*)(m_GUIWin->m_uiRoot->GetWidgetFromName("Button_32"));
	UButtonSlot* pBtnSlot = Cast<UButtonSlot>(pButton->GetContentSlot());

	// �����ƶ�λ��
	//testMove(pButton);

	// �������ؿؼ�
	//testHide(pButton);
}

void UUITestCanvas::loadNoCanvas()
{
	loadUWidget(TEXT("/Game/UMG/UITestCanvas_NoCanvas.UITestCanvas_NoCanvas_C"));
	m_GUIWin->m_uiRoot->AddToViewport();
}

void UUITestCanvas::loadCanvasWithNoCanvas()
{
	UClass* widgetClass = nullptr;
	widgetClass = StaticLoadClass(UUserWidget::StaticClass(), NULL, TEXT("/Game/UMG/UITestCanvas_Canvas.UITestCanvas_Canvas_C"));
	UUserWidget* parentWidget = nullptr;
	if (nullptr != widgetClass)
	{
		parentWidget = NewObject<UUserWidget>(EngineApi::getGameInstance(), widgetClass);
		parentWidget->AddToViewport();
	}

	UUserWidget* childWidget = nullptr;

	widgetClass = StaticLoadClass(UUserWidget::StaticClass(), NULL, TEXT("/Game/UMG/UITestCanvas_NoCanvas.UITestCanvas_NoCanvas_C"));
	if (nullptr != widgetClass)
	{
		childWidget = NewObject<UUserWidget>(EngineApi::getGameInstance(), widgetClass);
	}

	if (nullptr != parentWidget && nullptr != childWidget)
	{
		parentWidget->SetContentForSlot("NamedSlot_24", childWidget);
	}
}

void UUITestCanvas::loadNamedSlot()
{
	loadUWidget(TEXT("/Game/UMG/UITestNamedSlot.UITestNamedSlot_C"));
	//loadUWidget(TEXT("/Game/Blueprints/NewWidgetBlueprint.NewWidgetBlueprint_C"));
	m_GUIWin->m_uiRoot->AddToViewport();	// һ��Ҫ�� AddToViewport ��Ȼ���ٻ�ȡ�ؼ������� GetWidgetFromName ��ʱ��ᱨ��
	// Ŀǰ�Ѿ��е� NamedSlot �� "NamedSlot_28"
	m_GUIWin->m_uiRoot->GetSlotNames(m_namedSlotArr);
	m_namedSlot = (UNamedSlot*)(m_GUIWin->m_uiRoot->GetWidgetFromName("NamedSlot_28"));
	//UWidget* widget = m_umgWidget->GetWidgetFromName("Button_16");
}

void UUITestCanvas::loadOverlay()
{
	loadUWidget(TEXT("/Game/UMG/UITestOveray.UITestOveray_C"));
	m_GUIWin->m_uiRoot->AddToViewport();
	// Overlay_17 �Ѿ��е� Overlay 
	m_overlay = (UOverlay*)(m_GUIWin->m_uiRoot->GetWidgetFromName("Overlay_17"));
	UWidget* widget = m_GUIWin->m_uiRoot->GetWidgetFromName("Button_32");
}

void UUITestCanvas::unloadCanvas()
{
	m_GUIWin->m_uiRoot->RemoveFromViewport();
	m_GUIWin->m_uiRoot = nullptr;
}

void UUITestCanvas::testMove(UWidget* ptr)
{
	UCanvasPanelSlot* pPanelSlot = Cast<UCanvasPanelSlot>(ptr->Slot);	// �����ҵ� UButton �� UCanvasPanelSlot Ԫ����
	//UCanvasPanelSlot* pPanelSlot = Cast<UCanvasPanelSlot>(pButton->GetContentSlot());
	pPanelSlot->SetPosition(FVector2D(700, 600));

	// ��ȡ CanvasPanel 
	//UCanvasPanel* pCanvasPanel = (UCanvasPanel*)(m_umgWidget->GetWidgetFromName("UCanvasPanel"));
	// UCanvasPanel ��û�� UPanelSlot �ģ���Ϊ UCanvasPanel �̳� UPanelWidget���� UButton �̳� UContentWidget ���� GetContentSlot ���� UContentWidget �е�
	//UCanvasPanelSlot* pPanelSlot = Cast<UCanvasPanelSlot>(pCanvasPanel->GetContentSlot());
}

void UUITestCanvas::testHide(UWidget* ptr)
{
	//ptr->Visibility = ESlateVisibility::Hidden;
	ptr->SetVisibility(ESlateVisibility::Hidden);
}