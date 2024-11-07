// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 

#pragma once


#include "CoreMinimal.h"
#include "JSON/E3DSJsonObject.h"

#include "E3DS_Features_Library.generated.h"


///Request For New Server
///
USTRUCT(BlueprintType)
struct FRequestForServer
{
	GENERATED_BODY()
	//Please Generate Your Api key from control E3DS panel and paste it here.Login E3DS Control Panel and go to https://account.eagle3dstreaming.com/api-keys-management
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "E3DS | Request | Server ")
	FString API_Key;

	UPROPERTY(BlueprintReadOnly, Category = "E3DS | Request | Server ")
	FString Domain = "prod3.eagle3dstreaming.com";

	// Please Enter your E3DS Control Panel App Name. If the name doesn't match your request will fail
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "E3DS | Request | Server ")
	FString ServerAppName;

	// Please Enter your E3DS Game Map Name Which is going to open as Server. If it's empty your request will fail
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "E3DS | Request | Server ")
	FString ServerMapName;

	// Please Enter how much player you want in a single Server. Then Minimum Player is 1 by default if it's 0 your request will fail make sure it's more than 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "E3DS | Request | Server ")
	int32 MaxPlayer = 1;
};

///Request For New Server
///
USTRUCT(BlueprintType)
struct FRequestForServerList
{
	GENERATED_BODY()
	//Please Generate Your Api key from control E3DS panel and paste it here.Login E3DS Control Panel and go to https://account.eagle3dstreaming.com/api-keys-management
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "E3DS | Request | Server ")
	FString API_Key;

	UPROPERTY(BlueprintReadOnly, Category = "E3DS | Request | Server ")
	FString Domain = "prod3.eagle3dstreaming.com";

	// Please Enter your E3DS Control Panel App Name. If the name doesn't match your request will fail
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "E3DS | Request | Server ")
	FString ServerAppName;
};

///All Dedicated Server Information
///
USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()

	// E3DS Control Panel Server Name
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "E3DS |ServerInfo")
	FString ServerAppName = "";

	// Current Server Map Name
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "E3DS |ServerInfo")
	FString ServerMapName="No Valid Map Name";

	// Current Server IP Adress
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "E3DS |ServerInfo")
	FString IP_Address= "0.0.0.0";

	// Current Server Port
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "E3DS |ServerInfo")
	int32 Port= 0;

	// Current Player in Current Server
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "E3DS |ServerInfo")
	int32 CurrentPlayer=0;

	// Maximum number of player in current Server
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "E3DS |ServerInfo")
	int32 MaxPlayer=0;

};



USTRUCT(BlueprintType)
struct E3DSFEATURES_API FE3DSErrorHandle
{
	GENERATED_USTRUCT_BODY()

	/** Is there an error */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E3DS | Error ")
	bool hasError = false;

	/** Holds the error code recieved from E3DS. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E3DS | Error ")
	int32 ErrorCode = 0;
	
	/** Holds the error message recieved from E3DS. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E3DS | Error ")
	FString ErrorMessage;
	
	// Decode the error if there is one
	void DecodeError(UE3DSJsonObject* ResponseData)
	{
		if(ResponseData !=nullptr)
		{
			if (ResponseData->HasField(TEXT("error")))
			{
				hasError=true;
				ErrorCode= ResponseData->GetIntegerField(TEXT("status"));
				ErrorMessage = ResponseData->GetStringField(TEXT("error"));
			}
		}
	};
	
};


USTRUCT(BlueprintType)
struct E3DSFEATURES_API FE3DSResponse
{
	GENERATED_USTRUCT_BODY()

	/** E3DS Response Object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E3DS | Response ")
	UE3DSJsonObject* ResponseObject  = nullptr ;
	
	/** Is there an error */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E3DS | Error ")
	FE3DSErrorHandle ResponseError  = {} ;
};