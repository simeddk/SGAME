#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

class UWidgetInteractionComponent;

UCLASS()
class SGAME_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACPlayerController();

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	void MoveCursorX(float Value);
	void MoveCursorY(float Value);
	void Click();

	void UpdateMousePosition();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UWidgetInteractionComponent* WidgetInteraction;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Cursor")
	FVector2D CursorPosition;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Cursor")
	float CursorSpeed;
	
};
