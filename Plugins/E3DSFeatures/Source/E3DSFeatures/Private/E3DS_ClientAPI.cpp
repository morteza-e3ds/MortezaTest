// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 


#include "E3DS_ClientAPI.h"

#include "E3DSFeaturesDefines.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Engine/Engine.h"
#include "JSON/E3DS_JsonDecoder.h"
#include "Serialization/JsonSerializer.h"




UE3DS_ClientAPI* UE3DS_ClientAPI::RequestNewServer(FRequestForServer Request,
	FDelegateOnSuccessRequestNewServer OnSuccess, FDelegateOnFailureE3DSError OnFailure)
{
	// Objects containing request data
	UE3DS_ClientAPI* Manager = NewObject<UE3DS_ClientAPI>();
	if (Manager->IsSafeForRootSet()) Manager->AddToRoot();
	UE3DSJsonObject* OutRestJsonObj = NewObject<UE3DSJsonObject>();
	
	Manager->RequestHeaders.Empty();
	
	// Set HTTPS Headers
	Manager->SetHeader("apiKey",Request.API_Key);
	Manager->SetHeader("domain",Request.Domain);
	Manager->SetHeader("appName",Request.ServerAppName);

	FString CMD_Parameters = "-map=" + Request.ServerMapName + " " + "-maxPlayerNumPerDS=" + FString::FromInt(Request.MaxPlayer);
	
	Manager->SetHeader("cmdLineParameters",CMD_Parameters);
	

	// Assign delegates
	Manager->OnSuccessRequestForNewServer = OnSuccess;
	Manager->OnFail = OnFailure;
	Manager->OnE3DSResponse.AddDynamic(Manager, &UE3DS_ClientAPI::HelperRequestNewServer);
	
	Manager->E3DSRequestURL = "https://agw.eaglepixelstreaming.com/api/v3/ss/startServerApp";

	// Add Request to manager
	Manager->SetRequestObject(OutRestJsonObj);

	return Manager;
	
}

void UE3DS_ClientAPI::HelperRequestNewServer(FE3DSResponse Response, bool successful)
{
	FE3DSErrorHandle error = Response.ResponseError;

	if (error.hasError && OnFail.IsBound())
	{
		OnFail.Execute(error, error.hasError);
	}
	else if (!error.hasError && OnSuccessRequestForNewServer.IsBound())
	{
		const FServerInfo ResultStruct = UE3DS_JsonDecoder::DecodeRequestedNewServerInfo(Response.ResponseObject);
		OnSuccessRequestForNewServer.Execute(ResultStruct, successful);
	}
	this->RemoveFromRoot();
	
}

UE3DS_ClientAPI* UE3DS_ClientAPI::GetAllServerList(FRequestForServerList Request,
	FDelegateOnSuccessGetAllSeverList OnSuccess, FDelegateOnFailureE3DSError OnFailure)
{
	// Objects containing request data
	UE3DS_ClientAPI* Manager = NewObject<UE3DS_ClientAPI>();
	if (Manager->IsSafeForRootSet()) Manager->AddToRoot();
	UE3DSJsonObject* OutRestJsonObj = NewObject<UE3DSJsonObject>();

	Manager->RequestHeaders.Empty();
	
	
	// Set HTTPS Headers
	Manager->SetHeader("apiKey",Request.API_Key);
	Manager->SetHeader("domain",Request.Domain);
	Manager->SetHeader("appName",Request.ServerAppName);

	// Assign delegates
	Manager->OnSuccessGetAllSeverList = OnSuccess;
	Manager->OnFail = OnFailure;
	Manager->OnE3DSResponse.AddDynamic(Manager, &UE3DS_ClientAPI::HelperGetAllServerList);
	
	Manager->E3DSRequestURL = "https://agw.eaglepixelstreaming.com/api/v3/ss/dslist/";

	// Add Request to manager
	Manager->SetRequestObject(OutRestJsonObj);

	return Manager;
	
}

UE3DS_ClientAPI* UE3DS_ClientAPI::GetAllLatestVerisonServerList(FRequestForServerList Request,
	FDelegateOnSuccessGetAllSeverList OnSuccess, FDelegateOnFailureE3DSError OnFailure)
{
	// Objects containing request data
	UE3DS_ClientAPI* Manager = NewObject<UE3DS_ClientAPI>();
	if (Manager->IsSafeForRootSet()) Manager->AddToRoot();
	UE3DSJsonObject* OutRestJsonObj = NewObject<UE3DSJsonObject>();

	Manager->RequestHeaders.Empty();
	
	
	// Set HTTPS Headers
	Manager->SetHeader("apiKey",Request.API_Key);
	Manager->SetHeader("domain",Request.Domain);
	Manager->SetHeader("appName",Request.ServerAppName);

	// Assign delegates
	Manager->OnSuccessGetAllSeverList = OnSuccess;
	Manager->OnFail = OnFailure;
	Manager->OnE3DSResponse.AddDynamic(Manager, &UE3DS_ClientAPI::HelperGetAllServerList);
	
	Manager->E3DSRequestURL = "https://agw.eaglepixelstreaming.com/api/v3/ss/dslistOflatestverison/";

	// Add Request to manager
	Manager->SetRequestObject(OutRestJsonObj);

	return Manager;
	
}

void UE3DS_ClientAPI::HelperGetAllServerList(FE3DSResponse Response, bool successful)
{
	FE3DSErrorHandle error = Response.ResponseError;

	if (error.hasError && OnFail.IsBound())
	{
		OnFail.Execute(error, error.hasError);
	}
	else if (!error.hasError && OnSuccessGetAllSeverList.IsBound())
	{
		const TArray<FServerInfo> AllServerList = UE3DS_JsonDecoder::DecodeGetAllServerList(Response.ResponseObject);
		OnSuccessGetAllSeverList.Execute(AllServerList, successful);
	}
	this->RemoveFromRoot();
}

void UE3DS_ClientAPI::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Be sure that we have no data from previous response
	ResetResponseData();
	
	if (!IsValidLowLevel())
	{
		UE_LOG(LogE3DSFeatures, Log, TEXT("The request object is invalid during OnProcessRequestComplete."));
		return;
	}

	if (!OnE3DSResponse.IsBound())
	{
		UE_LOG(LogE3DSFeatures, Log, TEXT("OnE3DSResponse has come un-bound during OnProcessRequestComplete."));
		return;
	}

	FE3DSResponse E3DSResponse;

	if (!bWasSuccessful)
	{
		UE_LOG(LogE3DSFeatures, Log, TEXT("Request failed: %s"), *Request->GetURL());

		// Broadcast the result event
		E3DSResponse.ResponseError.hasError = true;
		E3DSResponse.ResponseError.ErrorCode = 503;
		E3DSResponse.ResponseError.ErrorMessage = "Unable to contact server";
		OnE3DSResponse.Broadcast(E3DSResponse, false);

		return;
	}

	

	// Save response data as a string
	ResponseContent = Response->GetContentAsString();

	// Save response code as int32
	ResponseCode = Response->GetResponseCode();

	// Try to deserialize data to JSON
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);
	FJsonSerializer::Deserialize(JsonReader, ResponseJsonObj->GetRootObject());

	// Decide whether the request was successful
	bIsValidJsonResponse = bWasSuccessful && ResponseJsonObj->GetRootObject().IsValid();

	// Log errors
	if (!bIsValidJsonResponse)
		UE_LOG(LogE3DSFeatures, Warning, TEXT("JSON could not be decoded!"));

	// Log response state
	UE_LOG(LogE3DSFeatures, Log, TEXT("Response : %s"), *ResponseContent);

	E3DSResponse.ResponseObject = ResponseJsonObj;
	E3DSResponse.ResponseError.DecodeError(ResponseJsonObj);

	// Broadcast the result event
	OnE3DSResponse.Broadcast(E3DSResponse, !E3DSResponse.ResponseError.hasError);
	
}


// Called On Any Function Has Been Called
void UE3DS_ClientAPI::Activate()
{
	AddToRoot();
	
	FString RequestUrl = E3DSRequestURL;
	RequestUrl.TrimStartInline();
	RequestUrl.TrimEndInline();


	FHttpModule& HttpModule = FHttpModule::Get();

	// Create an http request
	// The request will execute asynchronously, and call us back on the Lambda below
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = HttpModule.CreateRequest();
	
	HttpRequest->SetURL(RequestUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	
	// Serialize data to json string
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RequestJsonObj->GetRootObject().ToSharedRef(), Writer);

	// Set Json content
	HttpRequest->SetContentAsString(OutputString);

	UE_LOG(LogE3DSFeatures, Log, TEXT("Request Json: %s %s %sJSON(%s%s%s)JSON"), *HttpRequest->GetVerb(), *HttpRequest->GetURL(), LINE_TERMINATOR, LINE_TERMINATOR, *OutputString, LINE_TERMINATOR);


	// Getting All Response Headers And Set Header In HTTPS
	for (TMap<FString, FString>::TConstIterator It(RequestHeaders); It; ++It)
		HttpRequest->SetHeader(It.Key(), It.Value());
	
	// Bind event
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UE3DS_ClientAPI::OnProcessRequestComplete);

	HttpRequest->ProcessRequest();

	
	
}


void UE3DS_ClientAPI::SetHeader(const FString& HeaderName, const FString& HeaderValue)
{

	RequestHeaders.Add(HeaderName, HeaderValue);
}

void UE3DS_ClientAPI::SetRequestObject(UE3DSJsonObject* JsonObject)
{
	RequestJsonObj = JsonObject;
}

UE3DSJsonObject* UE3DS_ClientAPI::GetResponseObject()
{
	return  ResponseJsonObj;
}

void UE3DS_ClientAPI::ResetResponseData()
{
	if (ResponseJsonObj != nullptr)
		ResponseJsonObj->Reset();
	else
		ResponseJsonObj = NewObject<UE3DSJsonObject>();
	bIsValidJsonResponse = false;
}
