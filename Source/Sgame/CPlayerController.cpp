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
    CursorPosition = FVector2D(960.0f, 540.0f); // 화면 중앙에 초기화 (1920x1080 기준)
    
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
    if (FMath::Abs(Value) > 0.1f) // 입력 값이 충분히 크면
    {
        CursorPosition.X += Value * CursorSpeed; // X 좌표 갱신
        
        FViewport* Viewport = CastChecked<ULocalPlayer>(Player)->ViewportClient->Viewport;
        float ViewportSizeX = (float)Viewport->GetSizeXY().X;

        CursorPosition.X = FMath::Clamp(CursorPosition.X, 0.0f, ViewportSizeX); // 뷰포트 제한
        UpdateMousePosition();
    }
}

void ACPlayerController::MoveCursorY(float Value)
{
    if (FMath::Abs(Value) > 0.1f) // 입력 값이 충분히 크면
    {
        CursorPosition.Y += Value * CursorSpeed; // Y 좌표 갱신

       FViewport* Viewport = CastChecked<ULocalPlayer>(Player)->ViewportClient->Viewport;
       float ViewportSizeY = (float)Viewport->GetSizeXY().Y;

       CursorPosition.Y = FMath::Clamp(CursorPosition.Y, 0.0f, ViewportSizeY); // 뷰포트 제한
       UpdateMousePosition();
    }
}

void ACPlayerController::UpdateMousePosition()
{
    FViewport* Viewport = CastChecked<ULocalPlayer>(Player)->ViewportClient->Viewport;
    Viewport->SetMouse(CursorPosition.X, CursorPosition.Y); // 커서 위치 갱신
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