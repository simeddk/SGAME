#include "CHTTPRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"

ACHTTPRequest::ACHTTPRequest()
{
    URL = "https://sgaintranet.cafe24.com/intranet/req/reg_online_consult.php";
}

void ACHTTPRequest::BeginPlay()
{
	Super::BeginPlay();
}

void ACHTTPRequest::SendData(const FText& InUserName, const FText& InPhone1, const FText& InPhone2, const FText& InPhone3)
{
    //Http모듈이 이미 언리얼에 있었다니
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ACHTTPRequest::OnResponseReceived);
    Request->OnRequestProgress().BindUObject(this, &ACHTTPRequest::OnRequestProgress);

    //JSON Elenemt Array(요소 전달)
    TSharedRef<FJsonObject> JsonFieldObject = MakeShared<FJsonObject>();

    const FText& nameText = InUserName;
    JsonFieldObject->SetStringField("user_name", /*"KKS"*/FString::Printf(TEXT("%s"), *nameText.ToString()));

    const FText& phone1Text = InPhone1;
    JsonFieldObject->SetStringField("user_phone1", /*"010"*/FString::Printf(TEXT("%s"), *phone1Text.ToString()));
    
    const FText& phone2Text = InPhone2;
    JsonFieldObject->SetStringField("user_phone2", /*"7587"*/FString::Printf(TEXT("%s"), *phone2Text.ToString()));
    
    const FText& phone3Text = InPhone3;
    JsonFieldObject->SetStringField("user_phone3", /*"6244"*/FString::Printf(TEXT("%s"), *phone3Text.ToString()));

    JsonFieldObject->SetStringField("user_contents ", "");
    JsonFieldObject->SetStringField("selected_branch", "14");
    JsonFieldObject->SetStringField("red_url", "https://sgaedu.co.kr/");
    JsonFieldObject->SetStringField("page_name", "https://sgaedu.co.kr/");

    //선택과목 배열화
    TArray< TSharedPtr<FJsonValue> > ItemsObj;
    ItemsObj.Add(MakeShared<FJsonValueString>(TEXT("1")));
    //for (const int32 ItemIndex : Selected_SubjectNumber)
    //{
    //    TSharedPtr<FJsonObject> ItemObj = MakeShareable(new FJsonObject);
    //    ItemObj->SetNumberField("value", ItemIndex);
    //
    //    //ItemsObj.Add(MakeShareable(new FJsonValueObject(ItemObj)));
    //}

    //TArray<TSharedPtr<FJsonValue>> SelectedSubjects;
    //SelectedSubjects.Add(MakeShared<FJsonValueString>(TEXT("1")));
    //JsonFieldObject->SetArrayField(TEXT("arr_selected_subject"), SelectedSubjects);

    JsonFieldObject->SetArrayField(TEXT("arr_selected_subject"), ItemsObj);

    //직렬화를 평문으로
    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonFieldObject, Writer);

    GLog->Log("<JSON RESULT>>>>>>>>");
    GLog->Log(*RequestBody);
    GLog->Log("<<<<<<<<<JSON RESULT>");

    Request->SetURL(URL);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetContentAsString(RequestBody);
    Request->ProcessRequest();
    
    //한글이 하도 깨져서 테스트용
    UKismetSystemLibrary::DrawDebugString(GetWorld(), FVector(0, 0, 88), *nameText.ToString(), TestActor, FLinearColor::White, 3.f);
}

void ACHTTPRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    FString errorMsg;

    switch (Request->GetStatus())
    {
        case EHttpRequestStatus::NotStarted:                errorMsg = "NotStarted";                break;
        case EHttpRequestStatus::Processing:                errorMsg = "Processing";                break;
        case EHttpRequestStatus::Failed:                    errorMsg = "Failed";                    break;
        case EHttpRequestStatus::Failed_ConnectionError:    errorMsg = "Failed_ConnectionError";    break;
        case EHttpRequestStatus::Succeeded:                 errorMsg = "Succeeded";                 break;
    }

    UE_LOG(LogTemp, Error, TEXT("Response Staus : %s"), *errorMsg);
    UE_LOG(LogTemp, Error, TEXT("Response Code : %d"), Response->GetResponseCode());
}

void ACHTTPRequest::OnRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
    UE_LOG(LogTemp, Warning, TEXT(">>>"));
    UE_LOG(LogTemp, Warning, TEXT("Sent : %d"), BytesSent);
    UE_LOG(LogTemp, Warning, TEXT("Received : %d"), BytesReceived);
    UE_LOG(LogTemp, Warning, TEXT("<<<"));
}
