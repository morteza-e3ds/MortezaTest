// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 

#pragma once

#include "E3DSJsonObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "E3DSFeatures/Public/Library/E3DS_Features_Library.h"


#include "E3DS_JsonDecoder.generated.h"


UCLASS()
class E3DSFEATURES_API UE3DS_JsonDecoder : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	///////////////////////////////////////////////////////
	// Server Management
	//////////////////////////////////////////////////////

	/** Decode the Requested Server Result response object*/
	UFUNCTION(BlueprintCallable, Category = "E3DS | OnlineMultiplayer | Server Management")
	static FServerInfo DecodeRequestedNewServerInfo(UE3DSJsonObject* response);

	/** Decode the Requested Server Result response object*/
	UFUNCTION(BlueprintCallable, Category = "E3DS | OnlineMultiplayer | Server Management")
	static TArray<FServerInfo> DecodeGetAllServerList(UE3DSJsonObject* response);
	
};
