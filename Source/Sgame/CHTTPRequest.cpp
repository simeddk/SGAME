#include "CHTTPRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"

ACHTTPRequest::ACHTTPRequest()
{
    //URL = "https://sgaintranet.cafe24.com/intranet/req/reg_online_consult_playx4.php?user_name=kim";
    URL = "https://sgaintranet.cafe24.com/intranet/req/reg_online_consult_playx4.php?";
}

void ACHTTPRequest::BeginPlay()
{
	Super::BeginPlay();
}

void ACHTTPRequest::SendData(const FText& InUserName, const FText& InPhone1, const FText& InPhone2, const FText& InPhone3, const FText& InAge, const FText& InAddress)
{
    //Http모듈. 멤버 변수로 획득 금지
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ACHTTPRequest::OnResponseReceived);

    //JSON Elenemt Array(요소 전달)
    TSharedRef<FJsonObject> JsonFieldObject = MakeShared<FJsonObject>();

    const FText& nameText = InUserName;
    URL.Append("user_name=");
    URL.Append(FString::Printf(TEXT("%s"), *nameText.ToString()));

    const FText& phone1Text = InPhone1;
    URL.Append("&user_phone1=");
    URL.Append(FString::Printf(TEXT("%s"), *phone1Text.ToString()));

    const FText& phone2Text = InPhone2;
    URL.Append("&user_phone2=");
    URL.Append(FString::Printf(TEXT("%s"), *phone2Text.ToString()));

    const FText& phone3Text = InPhone3;
    URL.Append("&user_phone3=");
    URL.Append(FString::Printf(TEXT("%s"), *phone3Text.ToString()));

    //나이+주소 -> user_contents,
    const FText& ageText = InAge;
    const FText& addressText = InAddress;
    FString userContentsStr = FString::Printf(TEXT("%s/%s"), *ageText.ToString(), *addressText.ToString());

    URL.Append("&user_contents=");
    URL.Append(FString::Printf(TEXT("%s"), *userContentsStr));
    GLog->Log(*URL);

    //JsonFieldObject->SetStringField("user_contents", "");
    //JsonFieldObject->SetStringField("selected_branch", "14");
    //JsonFieldObject->SetStringField("red_url", "https://sgaedu.co.kr/");
    ////JsonFieldObject->SetStringField("page_name", "NAME_None");

    ////선택과목 배열화
    //TArray< TSharedPtr<FJsonValue> > ItemsObj;
    //ItemsObj.Add(MakeShared<FJsonValueString>("1"));
    ////for (const int32 ItemIndex : Selected_SubjectNumber)
    ////{
    ////    TSharedPtr<FJsonObject> ItemObj = MakeShareable(new FJsonObject);
    ////    ItemObj->SetNumberField("value", ItemIndex);
    ////
    ////    //ItemsObj.Add(MakeShareable(new FJsonValueObject(ItemObj)));
    ////}

    //Todo. SelectedSubjects, 나이+주소 -> user_contents, phone1,2,3
    ////TArray<TSharedPtr<FJsonValue>> SelectedSubjects;
    ////SelectedSubjects.Add(MakeShared<FJsonValueString>(TEXT("1")));
    ////JsonFieldObject->SetArrayField(TEXT("arr_selected_subject"), SelectedSubjects);

    //JsonFieldObject->SetArrayField("arr_selected_subject", ItemsObj);

    //직렬화를 평문으로
    //FString RequestBody;
    //TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    //FJsonSerializer::Serialize(JsonFieldObject, Writer);
    FString RequestBody = TEXT("<html><body><input type='hidden' name='user_name' value='kim'></body></html>");

    GLog->Log("<JSON RESULT>>>>>>>>");
    GLog->Log(*RequestBody);
    GLog->Log("<<<<<<<<<JSON RESULT>");

    Request->SetURL(URL);
    Request->SetVerb("GET");
    //Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("Content-Type", "text/html");
    Request->SetContentAsString(RequestBody);
    bool bSuceess = Request->ProcessRequest();

    UE_LOG(LogTemp, Error, TEXT("Request result is %d"), bSuceess);
    
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

    GLog->Log("<Response RESULT>>>>>>>>");
    FString content = Response->GetContentAsString();
    UE_LOG(LogTemp, Warning, TEXT("%s"), *content);
    GLog->Log("<<<<<<<<<Response RESULT>");

    content = Response->GetHeader("user_name");
    UE_LOG(LogTemp, Warning, TEXT("user_name : %s"), *content);
}

void ACHTTPRequest::OnRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
    UE_LOG(LogTemp, Warning, TEXT(">>>"));
    UE_LOG(LogTemp, Warning, TEXT("Sent : %d"), BytesSent);
    UE_LOG(LogTemp, Warning, TEXT("Received : %d"), BytesReceived);
    UE_LOG(LogTemp, Warning, TEXT("<<<"));
}
