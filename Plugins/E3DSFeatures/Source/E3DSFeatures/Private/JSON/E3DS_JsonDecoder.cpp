// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 

#include "E3DSFeatures/Public/JSON/E3DS_JsonDecoder.h"
#include "Commandlets/Commandlet.h"


// Decode Request New Server Info
FServerInfo UE3DS_JsonDecoder::DecodeRequestedNewServerInfo(UE3DSJsonObject* response)
{
	FServerInfo TempResultStruct;
	
	
	UE3DSJsonObject* DataObj = !(response->HasField("data")) ? nullptr : response->GetObjectField("data");

	if(DataObj !=nullptr)
	{
		
		TArray<FString> OutTokens;
		TArray<FString> OutSwitches;
		TMap<FString,FString> OutParams;

		const FString CMD_Parameter= !(DataObj->HasField("CmdLineParameters4DS")) ? "No Valid Parameters" : DataObj->GetStringField("CmdLineParameters4DS");

		UCommandlet::ParseCommandLine(*CMD_Parameter,OutTokens,OutSwitches,OutParams);
		
		TempResultStruct.ServerAppName = !(DataObj->HasField("appName")) ? "" : DataObj->GetStringField("appName");
		TempResultStruct.ServerMapName =  *OutParams.Find(TEXT("map"));
		TempResultStruct.IP_Address = !(DataObj->HasField("serverPublicIp")) ? "0.0.0.0" : DataObj->GetStringField("serverPublicIp");
		TempResultStruct.Port = !(DataObj->HasField("dsPort")) ? 0 : DataObj->GetIntegerField("dsPort");
		TempResultStruct.CurrentPlayer = !(DataObj->HasField("playerNum")) ? 0 : DataObj->GetIntegerField("playerNum");
		TempResultStruct.MaxPlayer= FCString::Atoi(**OutParams.Find(TEXT("maxPlayerNumPerDS")));
	}
	return TempResultStruct;
}


// Decode The Response JSON object to Unreal Struct 
TArray<FServerInfo> UE3DS_JsonDecoder::DecodeGetAllServerList(UE3DSJsonObject* response)
{
	TArray<FServerInfo> TempStructArray;
	UE3DSJsonObject* DataObj = !(response->HasField("data")) ? nullptr : response->GetObjectField("data");

	if(DataObj != nullptr)
	{	
		for (const auto & Loc_ServerListObj : DataObj->GetObjectArrayField("dsServerList"))
		{
			UE3DSJsonObject* AppInfoObj =!(Loc_ServerListObj->HasField("appInfo")) ? nullptr : Loc_ServerListObj->GetObjectField("appInfo");

			if(AppInfoObj !=nullptr)
			{
				FServerInfo Loc_ServerInfo;

				TArray<FString> OutTokens;
				TArray<FString> OutSwitches;
				TMap<FString,FString> OutParams;
				
				FString CMD_Parameter= !(AppInfoObj->HasField("CmdLineParameters4DS")) ? "No Valid Parameters" : AppInfoObj->GetStringField("CmdLineParameters4DS");
				
				UCommandlet::ParseCommandLine(*CMD_Parameter,OutTokens,OutSwitches,OutParams);
				
				Loc_ServerInfo.ServerAppName = !(AppInfoObj->HasField("appName")) ? "No Valid App Name" : AppInfoObj->GetStringField("appName");


				FString* MapValue = OutParams.Find(TEXT("map"));
				FString tt = MapValue ? *MapValue : "";
				
					//FString tt=*OutParams.Find(TEXT("map"));
				
					Loc_ServerInfo.ServerMapName = tt;
				Loc_ServerInfo.IP_Address= !(Loc_ServerListObj->HasField("serverPublicIp")) ? "No IP Address" : Loc_ServerListObj->GetStringField("serverPublicIp");
				Loc_ServerInfo.Port=!(AppInfoObj->HasField("dsPort")) ? 0 : AppInfoObj->GetIntegerField("dsPort");
				Loc_ServerInfo.CurrentPlayer=!(AppInfoObj->HasField("playerNum")) ? 0 : AppInfoObj->GetIntegerField("playerNum");
				
				//Loc_ServerInfo.MaxPlayer = FCString::Atoi(**OutParams.Find(TEXT("maxPlayerNumPerDS")));
				FString* MaxPlayerStr = OutParams.Find(TEXT("maxPlayerNumPerDS"));
				Loc_ServerInfo.MaxPlayer = MaxPlayerStr ? FCString::Atoi(**MaxPlayerStr) : 10;

				
				TempStructArray.Add(Loc_ServerInfo);
			}
		}
	}
	
	return TempStructArray;
}
