// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"

#include "E3DSJsonValue.generated.h"

class UE3DSJsonObject;

/**
 * Represents all the types a Json Value can be.
 */
UENUM(BlueprintType)
namespace EPFJson
{
    enum Type
    {
        None,
        Null,
        String,
        Number,
        Boolean,
        Array,
        Object,
    };
}

/**
 * Blueprintable FJsonValue wrapper
 */
UCLASS(BlueprintType, Blueprintable)
class E3DSFEATURES_API UE3DSJsonValue : public UObject
{
    GENERATED_UCLASS_BODY()

    /** Create new Json Number value
    * float used instead of double to make the function Blueprintable! */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json Number Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "E3DS | Json")
         static UE3DSJsonValue* ConstructJsonValueNumber(UObject* WorldContextObject, float Number);

    /** Create new Json String value */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json String Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "E3DS | Json")
        static UE3DSJsonValue* ConstructJsonValueString(UObject* WorldContextObject, const FString& StringValue);

    /** Create new Json Bool value */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json Bool Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "E3DS | Json")
        static UE3DSJsonValue* ConstructJsonValueBool(UObject* WorldContextObject, bool InValue);

    /** Create new Json Array value */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json Array Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "E3DS | Json")
        static UE3DSJsonValue* ConstructJsonValueArray(UObject* WorldContextObject, const TArray<UE3DSJsonValue*>& InArray);

    /** Create new Json Object value */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json Object Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "E3DS | Json")
    static UE3DSJsonValue* ConstructJsonValueObject(UObject* WorldContextObject, UE3DSJsonObject *JsonObject);

    /** Create new Json value from FJsonValue (to be used from PlayFabJsonObject) */
    static UE3DSJsonValue* ConstructJsonValue(UObject* WorldContextObject, const TSharedPtr<FJsonValue>& InValue);

    /** Get the root Json value */
    TSharedPtr<FJsonValue>& GetRootValue();

    /** Set the root Json value */
    void SetRootValue(TSharedPtr<FJsonValue>& JsonValue);


    //////////////////////////////////////////////////////////////////////////
    // FJsonValue API

    /** Get type of Json value (Enum) */
    UFUNCTION(BlueprintCallable, Category = "PlayFab | Json")
        EPFJson::Type GetType() const;

    /** Get type of Json value (String) */
    UFUNCTION(BlueprintCallable, Category = "PlayFab | Json")
        FString GetTypeString() const;

    /** Returns true if this value is a 'null' */
    UFUNCTION(BlueprintCallable, Category = "PlayFab | Json")
        bool IsNull() const;

    /** Returns this value as a double, throwing an error if this is not an Json Number
     * Attn.!! float used instead of double to make the function blueprintable! */
    UFUNCTION(BlueprintCallable, Category = "PlayFab | Json")
        float AsNumber() const;

    /** Returns this value as a number, throwing an error if this is not an Json String */
    UFUNCTION(BlueprintCallable, Category = "PlayFab | Json")
        FString AsString() const;

    /** Returns this value as a boolean, throwing an error if this is not an Json Bool */
    UFUNCTION(BlueprintCallable, Category = "PlayFab | Json")
        bool AsBool() const;

    /** Returns this value as an array, throwing an error if this is not an Json Array */
    UFUNCTION(BlueprintCallable, Category = "PlayFab | Json")
        TArray<UE3DSJsonValue*> AsArray() const;

    /** Returns this value as an object, throwing an error if this is not an Json Object */
    UFUNCTION(BlueprintCallable, Category = "PlayFab | Json")
        UE3DSJsonObject* AsObject();


    //////////////////////////////////////////////////////////////////////////
    // Data

private:
    /** Internal JSON data */
    TSharedPtr<FJsonValue> JsonVal;


    //////////////////////////////////////////////////////////////////////////
    // Helpers

protected:
    /** Simple error logger */
    void ErrorMessage(const FString& InType) const;

};
