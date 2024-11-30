#include "CPlayerController.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/WidgetComponent.h"

ACPlayerController::ACPlayerController()
{
    WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
    WidgetInteraction->InteractionSource = EWidgetInteractionSource::Custom;
    WidgetInteraction->bShowDebug = true;

    CursorSpeed = 3.f;
}

void ACPlayerController::BeginPlay()
{
    Super::BeginPlay();
    CursorPosition = FVector2D(960.0f, 540.0f); // ȭ�� �߾ӿ� �ʱ�ȭ (1920x1080 ����)
    
}

void ACPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAxis("MoveCursorX", this, &ACPlayerController::MoveCursorX);
    InputComponent->BindAxis("MoveCursorY", this, &ACPlayerController::MoveCursorY);
    InputComponent->BindAction("GamePadConfirm", IE_Pressed, this, &ACPlayerController::Click);
}

void ACPlayerController::MoveCursorX(float Value)
{
    if (FMath::Abs(Value) > 0.1f) // �Է� ���� ����� ũ��
    {
        CursorPosition.X += Value * CursorSpeed; // X ��ǥ ����
        
        FViewport* Viewport = CastChecked<ULocalPlayer>(Player)->ViewportClient->Viewport;
        float ViewportSizeX = (float)Viewport->GetSizeXY().X;

        CursorPosition.X = FMath::Clamp(CursorPosition.X, 0.0f, ViewportSizeX); // ����Ʈ ����
        UpdateMousePosition();
    }
}

void ACPlayerController::MoveCursorY(float Value)
{
    if (FMath::Abs(Value) > 0.1f) // �Է� ���� ����� ũ��
    {
        CursorPosition.Y += Value * CursorSpeed; // Y ��ǥ ����

       FViewport* Viewport = CastChecked<ULocalPlayer>(Player)->ViewportClient->Viewport;
       float ViewportSizeY = (float)Viewport->GetSizeXY().Y;

       CursorPosition.Y = FMath::Clamp(CursorPosition.Y, 0.0f, ViewportSizeY); // ����Ʈ ����
       UpdateMousePosition();
    }
}

void ACPlayerController::UpdateMousePosition()
{
    FViewport* Viewport = CastChecked<ULocalPlayer>(Player)->ViewportClient->Viewport;
    Viewport->SetMouse(CursorPosition.X, CursorPosition.Y); // Ŀ�� ��ġ ����
}

void ACPlayerController::Click()
{
   /* FPointerEvent PointerEvent(
        0, CursorPosition, CursorPosition, FVector2D::ZeroVector,
        false, FSlateApplication::Get().GetPressedMouseButtons(),
        0, FModifierKeysState()
    );

    FSlateApplication::Get().ProcessMouseButtonDownEvent(PointerEvent);
    FSlateApplication::Get().ProcessMouseButtonUpEvent(PointerEvent);*/
    
}