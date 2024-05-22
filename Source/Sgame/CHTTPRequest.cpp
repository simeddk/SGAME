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

    HttpModule = &FHttpModule::Get();
    if (!HttpModule)
    {
        UE_LOG(LogTemp, Error, TEXT("Couldn't get http module"));
        return;
    }

    UE_LOG(LogTemp, Display, TEXT("http module is valid"));
}

void ACHTTPRequest::SendData(const FText& InUserName, const FText& InPhone1, const FText& InPhone2, const FText& InPhone3)
{
    //Http모듈이 이미 언리얼에 있었다니
    TSharedRef<IHttpRequest> Request = HttpModule->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ACHTTPRequest::OnResponseReceived);
    Request->SetURL(URL);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");

    //JSON Binary Array(전체 직렬화)
    TArray<TSharedPtr<FJsonValue>> JsonDocumentArray;

    //JSON Elenemt Array(요소 전달)
    TSharedPtr<FJsonObject> JsonFieldObject = MakeShareable(new FJsonObject());

    const FText& nameText = InUserName;
    JsonFieldObject->SetStringField("user_name", FString::Printf(TEXT("%s"), *nameText.ToString()));

    const FText& phone1Text = InPhone1;
    JsonFieldObject->SetStringField("user_phone1", FString::Printf(TEXT("%s"), *phone1Text.ToString()));
    
    const FText& phone2Text = InPhone2;
    JsonFieldObject->SetStringField("user_phone2", FString::Printf(TEXT("%s"), *phone2Text.ToString()));
    
    const FText& phone3Text = InPhone3;
    JsonFieldObject->SetStringField("user_phone3", FString::Printf(TEXT("%s"), *phone3Text.ToString()));

    JsonFieldObject->SetStringField("red_url", "https://sgaedu.co.kr/");
    UE_LOG(LogTemp, Warning, TEXT("Redirection URL is https://sgaedu.co.kr/"));

    JsonFieldObject->SetNumberField("selected_branch", 14);
    //JsonFieldObject->SetStringField("selected_branch", "14"); //이것도 string일 필요가 있을까?

    //한글이 하도 깨져서 테스트용
    UKismetSystemLibrary::DrawDebugString(GetWorld(), FVector(0, 0, 88), *nameText.ToString(), TestActor, FLinearColor::White, 3.f);


    //선택과목 배열화.. 인데 배열을 통째로 넘겨야 하는걸까, 특정 엘리먼트만 넘겨야 하는걸까
    TArray< TSharedPtr<FJsonValue> > ItemsObj;
    
    //선택과목 배열 초기화
   /* for (const FText& ItemData : Selected_SubjectText)
    {
        TSharedPtr<FJsonObject> ItemObj = MakeShareable(new FJsonObject);
        ItemObj->SetStringField(TEXT("value"), ItemData.ToString());

        ItemsObj.Add(MakeShareable(new FJsonValueObject(ItemObj)));
    }*/

    for (const int32 ItemIndex : Selected_SubjectNumber)
    {
        TSharedPtr<FJsonObject> ItemObj = MakeShareable(new FJsonObject);
        ItemObj->SetNumberField("value", ItemIndex);

        ItemsObj.Add(MakeShareable(new FJsonValueObject(ItemObj)));
    }

    //Case1 : 배열을 통짜로 넘기는 경우
#ifndef AddressOfArray 
    JsonFieldObject->SetArrayField(TEXT("arr_selected_subject"), ItemsObj);
#endif

    //Case2 : 특정 엘리먼트를 넘기는 경우 -> DEREPECATED(오버라이딩 없음)
#ifdef AddressOfElementOnly
    FString getString;
    if (ItemsObj[0].Get()->TryGetString(getString))
        JsonFieldObject->SetStringField("arr_selected_subject", getString);
    else
        UE_LOG(LogTemp, Error, TEXT("Could not get selected subject string"));
#endif

    //Case3 : 하드코딩
#ifdef FORCEINIT
    JsonFieldObject->SetStringField(TEXT("arr_selected_subject"), FString::Printf(TEXT("%s"), *Selected_SubjectText[0].ToString()));
#endif

    //최종 쌈
    JsonDocumentArray.Add(MakeShareable(new FJsonValueObject(JsonFieldObject)));

    //직렬화를 평문으로
    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonDocumentArray, Writer);

    GLog->Log("<JSON RESULT>>>>>>>>");
    GLog->Log(*RequestBody);
    GLog->Log("<<<<<<<<<JSON RESULT>");

    Request->SetContentAsString(RequestBody);
    Request->ProcessRequest();
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