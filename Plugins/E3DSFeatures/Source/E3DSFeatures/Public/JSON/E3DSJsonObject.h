// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 

#pragma once
#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

#include "E3DSJsonObject.generated.h"



class UE3DSJsonValue;

UCLASS()
class E3DSFEATURES_API UE3DSJsonObject : public UObject
{
    GENERATED_UCLASS_BODY()

/** Create new Json object */
UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct Json Object", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "PlayFab | Json")
    static UE3DSJsonObject* ConstructJsonObject(UObject* WorldContextObject);

    /** Reset all internal data */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void Reset();

    /** Get the root Json object */
    TSharedPtr<FJsonObject>& GetRootObject();

    /** Set the root Json object */
    void SetRootObject(TSharedPtr<FJsonObject>& JsonObject);


    //////////////////////////////////////////////////////////////////////////
    // Serialization

    /** Serialize Json to string */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        FString EncodeJson() const;

    /** Construct Json object from string */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        bool DecodeJson(const FString& JsonString);


    //////////////////////////////////////////////////////////////////////////
    // FJsonObject API

    /** Returns a list of field names that exist in the object */
    UFUNCTION(BlueprintPure, Category = "E3DS | Json")
        TArray<FString> GetFieldNames();

    /** Checks to see if the FieldName exists in the object */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        bool HasField(const FString& FieldName) const;

    /** Remove field named FieldName */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void RemoveField(const FString& FieldName);

    /** Get the field named FieldName as a JsonValue */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
       UE3DSJsonValue* GetField(const FString& FieldName) const;

    /** Add a field named FieldName with a Value */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetField(const FString& FieldName, UE3DSJsonValue* JsonValue);

    /** Add a field named FieldName with a null value */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetFieldNull(const FString& FieldName);

    /** Get the field named FieldName as a Json Array */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
       TArray<UE3DSJsonValue*> GetArrayField(const FString& FieldName);

    /** Set an ObjectField named FieldName and value of Json Array */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetArrayField(const FString& FieldName, const TArray<UE3DSJsonValue*>& InArray);

    /** Adds all of the fields from one json object to this one */
   UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void MergeJsonObject(UE3DSJsonObject* InJsonObject, bool Overwrite);


    //////////////////////////////////////////////////////////////////////////
    // FJsonObject API Helpers (easy to use with simple Json objects)

    /** Get the field named FieldName as a number. Ensures that the field is present and is of type Json number.
     * Attn.!! float used instead of double to make the function blueprintable! */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        float GetNumberField(const FString& FieldName) const;

    /** Add a field named FieldName with Number as value
     * Attn.!! float used instead of double to make the function blueprintable! */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetNumberField(const FString& FieldName, float Number);

    /** Get the field named FieldName as an Integer. Ensures that the field is present and is of type Json number. */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        int32 GetIntegerField(const FString& FieldName) ;

    /** Get the field named FieldName as a string. */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        FString GetStringField(const FString& FieldName) const;

    /** Add a field named FieldName with value of StringValue */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetStringField(const FString& FieldName, const FString& StringValue);

    /** Get the field named FieldName as a boolean. */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        bool GetBoolField(const FString& FieldName) const;

    /** Set a boolean field named FieldName and value of InValue */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetBoolField(const FString& FieldName, bool InValue);

    /** Get the field named FieldName as a Json object. */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        UE3DSJsonObject* GetObjectField(const FString& FieldName) const;

    /** Set an ObjectField named FieldName and value of JsonObject */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetObjectField(const FString& FieldName, UE3DSJsonObject* JsonObject);


    //////////////////////////////////////////////////////////////////////////
    // Array fields helpers (uniform arrays)

    /** Get the field named FieldName as a Number Array. Use it only if you're sure that array is uniform!
     * Attn.!! float used instead of double to make the function blueprintable! */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        TArray<float> GetNumberArrayField(const FString& FieldName);

    /** Set an ObjectField named FieldName and value of Number Array
     * Attn.!! float used instead of double to make the function blueprintable! */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetNumberArrayField(const FString& FieldName, const TArray<float>& NumberArray);

    /** Get the field named FieldName as a String Array. Use it only if you're sure that array is uniform! */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        TArray<FString> GetStringArrayField(const FString& FieldName);

    /** Set an ObjectField named FieldName and value of String Array */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetStringArrayField(const FString& FieldName, const TArray<FString>& StringArray);

    /** Get the field named FieldName as a Bool Array. Use it only if you're sure that array is uniform! */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        TArray<bool> GetBoolArrayField(const FString& FieldName);

    /** Set an ObjectField named FieldName and value of Bool Array */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetBoolArrayField(const FString& FieldName, const TArray<bool>& BoolArray);

    /** Get the field named FieldName as an Object Array. Use it only if you're sure that array is uniform! */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        TArray<UE3DSJsonObject*> GetObjectArrayField(const FString& FieldName);

    /** Set an ObjectField named FieldName and value of Ob Array */
    UFUNCTION(BlueprintCallable, Category = "E3DS | Json")
        void SetObjectArrayField(const FString& FieldName, const TArray<UE3DSJsonObject*>& ObjectArray);


    //////////////////////////////////////////////////////////////////////////
    // Data

    private:
    /** Internal JSON data */
    TSharedPtr<FJsonObject> JsonObj;
    
};