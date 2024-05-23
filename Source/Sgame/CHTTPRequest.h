#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "CHTTPRequest.generated.h"

UCLASS()
class SGAME_API ACHTTPRequest : public AActor
{
	GENERATED_BODY()
	
public:	
	ACHTTPRequest();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Exec, Category = "HTTP")
		void SendData(const FText& InUserName, const FText& InPhone1, const FText& InPhone2, const FText& InPhone3, const FText& InAge, const FText& InAddress);


private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);

private:
	UPROPERTY(EditAnywhere)
		FString URL;

	//UPROPERTY(EditInstanceOnly)
	//	TArray<FText> Selected_SubjectText;

	UPROPERTY(EditInstanceOnly)
		TArray<int32> Selected_SubjectNumber;

	UPROPERTY(EditInstanceOnly)
		AActor* TestActor;

};
