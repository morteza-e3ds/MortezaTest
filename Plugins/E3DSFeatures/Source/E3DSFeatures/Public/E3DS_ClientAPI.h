// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Library/E3DS_Features_Library.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "E3DS_ClientAPI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnE3DS_ClientRequestCompleted, FE3DSResponse, Response, bool, successful);


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class E3DSFEATURES_API UE3DS_ClientAPI : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()


	
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FDelegateOnFailureE3DSError, FE3DSErrorHandle, Error,bool,HasError);
	
	//UE3DS_ClientAPI(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(BlueprintAssignable)
	FOnE3DS_ClientRequestCompleted OnE3DSResponse;

	/** Sets optional header info */
	UFUNCTION(BlueprintCallable, Category = "E3DS|Request")
	void SetHeader(const FString& HeaderName, const FString& HeaderValue);

	/** Set the Request Json object */
	void SetRequestObject(UE3DSJsonObject* JsonObject);

	/** Get the Response Json object */
	UE3DSJsonObject* GetResponseObject();

	/** Reset saved response data */
	void ResetResponseData();
	
	/** UOnlineBlueprintCallProxyBase interface */
	virtual void Activate() override;
	
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FDelegateOnSuccessRequestNewServer, FServerInfo, NewServerInfo, bool, IsSuccessfull);

	//Request For Create A New Server
	UFUNCTION(BlueprintCallable, Category = "E3DS | OnlineMultiplayer | Server ", meta = (DisplayName = "Request For A New Server",BlueprintInternalUseOnly = "true"))
	static UE3DS_ClientAPI* RequestNewServer(FRequestForServer Request, FDelegateOnSuccessRequestNewServer OnSuccess,FDelegateOnFailureE3DSError OnFailure);


	// Implements FE3DSClientRequestCompleted
	UFUNCTION(BlueprintCallable, Category = "E3DS | OnlineMultiplayer | Server ", meta = (BlueprintInternalUseOnly = "true"))
	void HelperRequestNewServer(FE3DSResponse Response,bool successful);

	TArray<FServerInfo> AllServer;
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FDelegateOnSuccessGetAllSeverList,const  TArray<FServerInfo>&, ServerList, bool, IsSuccessfull);
	
	//Getting All Server List
	UFUNCTION(BlueprintCallable, Category = "E3DS | OnlineMultiplayer | Server ", meta = (DisplayName = "Get All Server List",BlueprintInternalUseOnly = "true"))
	static UE3DS_ClientAPI* GetAllServerList(FRequestForServerList Request, FDelegateOnSuccessGetAllSeverList OnSuccess,FDelegateOnFailureE3DSError OnFailure);
	
	//Getting All Server List
	UFUNCTION(BlueprintCallable, Category = "E3DS | OnlineMultiplayer | Server ", meta = (DisplayName = "Get All Latest Verison Server List",BlueprintInternalUseOnly = "true"))
	static UE3DS_ClientAPI* GetAllLatestVerisonServerList(FRequestForServerList Request, FDelegateOnSuccessGetAllSeverList OnSuccess,FDelegateOnFailureE3DSError OnFailure);




	// Implements FE3DSClientRequestCompleted
	UFUNCTION(BlueprintCallable, Category = "E3DS | OnlineMultiplayer | Server ", meta = (BlueprintInternalUseOnly = "true"))
	void HelperGetAllServerList(FE3DSResponse Response,bool successful);


	
	// Declare Delegates
	FDelegateOnFailureE3DSError OnFail;
	FDelegateOnSuccessRequestNewServer OnSuccessRequestForNewServer;
	FDelegateOnSuccessGetAllSeverList OnSuccessGetAllSeverList;

private:
	
	/** PlayFab Request Info */
	FString E3DSRequestURL;

	/** Is the response valid JSON? */
	bool bIsValidJsonResponse;
	FString ResponseContent;
	int32 ResponseCode;

	/** Internal bind function for the IHTTPRequest::OnProcessRequestCompleted() event */
	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	/** Internal request data stored as JSON */
	UPROPERTY()
	UE3DSJsonObject* RequestJsonObj;

	/** Response data stored as JSON */
	UPROPERTY()
	UE3DSJsonObject* ResponseJsonObj;

	/** Mapping of header section to values. Used to generate final header string for request */
	
	TMap<FString, FString> RequestHeaders;
};
