// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 


#include "E3DSFeatures/Public/JSON/E3DSJsonValue.h"
#include "E3DSFeatures/Public//JSON/E3DSJsonObject.h"
#include "Dom/JsonValue.h"
#include "CoreMinimal.h"
#include "E3DSFeaturesDefines.h"


UE3DSJsonValue::UE3DSJsonValue(const class FObjectInitializer& PCIP)
    : Super(PCIP)
{

}

UE3DSJsonValue* UE3DSJsonValue::ConstructJsonValueNumber(UObject* WorldContextObject, float Number)
{
    TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueNumber(Number));

    UE3DSJsonValue* NewValue = NewObject<UE3DSJsonValue>();
    NewValue->SetRootValue(NewVal);

    return NewValue;
}

UE3DSJsonValue* UE3DSJsonValue::ConstructJsonValueString(UObject* WorldContextObject, const FString& StringValue)
{
    TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueString(StringValue));

    UE3DSJsonValue* NewValue = NewObject<UE3DSJsonValue>();
    NewValue->SetRootValue(NewVal);

    return NewValue;
}

UE3DSJsonValue* UE3DSJsonValue::ConstructJsonValueBool(UObject* WorldContextObject, bool InValue)
{
    TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueBoolean(InValue));

    UE3DSJsonValue* NewValue = NewObject<UE3DSJsonValue>();
    NewValue->SetRootValue(NewVal);

    return NewValue;
}

UE3DSJsonValue* UE3DSJsonValue::ConstructJsonValueArray(UObject* WorldContextObject, const TArray<UE3DSJsonValue*>& InArray)
{
    // Prepare data array to create new value
    TArray< TSharedPtr<FJsonValue> > ValueArray;
    for (auto InVal : InArray)
    {
        ValueArray.Add(InVal->GetRootValue());
    }

    TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueArray(ValueArray));

    UE3DSJsonValue* NewValue = NewObject<UE3DSJsonValue>();
    NewValue->SetRootValue(NewVal);

    return NewValue;
}

UE3DSJsonValue* UE3DSJsonValue::ConstructJsonValueObject(UObject* WorldContextObject, UE3DSJsonObject *JsonObject)
{
    TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueObject(JsonObject->GetRootObject()));

    UE3DSJsonValue* NewValue = NewObject<UE3DSJsonValue>();
    NewValue->SetRootValue(NewVal);

    return NewValue;
}

UE3DSJsonValue* ConstructJsonValue(UObject* WorldContextObject, const TSharedPtr<FJsonValue>& InValue)
{
    TSharedPtr<FJsonValue> NewVal = InValue;

    UE3DSJsonValue* NewValue = NewObject<UE3DSJsonValue>();
    NewValue->SetRootValue(NewVal);

    return NewValue;
}

TSharedPtr<FJsonValue>& UE3DSJsonValue::GetRootValue()
{
    return JsonVal;
}

void UE3DSJsonValue::SetRootValue(TSharedPtr<FJsonValue>& JsonValue)
{
    JsonVal = JsonValue;
}


//////////////////////////////////////////////////////////////////////////
// FJsonValue API

EPFJson::Type UE3DSJsonValue::GetType() const
{
    if (!JsonVal.IsValid())
    {
        return EPFJson::None;
    }

    switch (JsonVal->Type)
    {
    case EJson::None:
        return EPFJson::None;

    case EJson::Null:
        return EPFJson::Null;

    case EJson::String:
        return EPFJson::String;

    case EJson::Number:
        return EPFJson::Number;

    case EJson::Boolean:
        return EPFJson::Boolean;

    case EJson::Array:
        return EPFJson::Array;

    case EJson::Object:
        return EPFJson::Object;

    default:
        return EPFJson::None;
    }
}

FString UE3DSJsonValue::GetTypeString() const
{
    if (!JsonVal.IsValid())
    {
        return "None";
    }

    switch (JsonVal->Type)
    {
    case EJson::None:
        return TEXT("None");

    case EJson::Null:
        return TEXT("Null");

    case EJson::String:
        return TEXT("String");

    case EJson::Number:
        return TEXT("Number");

    case EJson::Boolean:
        return TEXT("Boolean");

    case EJson::Array:
        return TEXT("Array");

    case EJson::Object:
        return TEXT("Object");

    default:
        return TEXT("None");
    }
}

bool UE3DSJsonValue::IsNull() const
{
    if (!JsonVal.IsValid())
    {
        return true;
    }

    return JsonVal->IsNull();
}

float UE3DSJsonValue::AsNumber() const
{
    if (!JsonVal.IsValid())
    {
        ErrorMessage(TEXT("Number"));
        return 0.f;
    }

    return JsonVal->AsNumber();
}

FString UE3DSJsonValue::AsString() const
{
    if (!JsonVal.IsValid())
    {
        ErrorMessage(TEXT("String"));
        return FString();
    }

    return JsonVal->AsString();
}

bool UE3DSJsonValue::AsBool() const
{
    if (!JsonVal.IsValid())
    {
        ErrorMessage(TEXT("Boolean"));
        return false;
    }

    return JsonVal->AsBool();
}

TArray<UE3DSJsonValue*> UE3DSJsonValue::AsArray() const
{
    TArray<UE3DSJsonValue*> OutArray;

    if (!JsonVal.IsValid())
    {
        ErrorMessage(TEXT("Array"));
        return OutArray;
    }

    TArray< TSharedPtr<FJsonValue> > ValArray = JsonVal->AsArray();
    for (auto Value : ValArray)
    {
        UE3DSJsonValue* NewValue = NewObject<UE3DSJsonValue>();
        NewValue->SetRootValue(Value);

        OutArray.Add(NewValue);
    }

    return OutArray;
}

UE3DSJsonObject* UE3DSJsonValue::AsObject()
{
    if (!JsonVal.IsValid())
    {
        ErrorMessage(TEXT("Object"));
        return nullptr;
    }

    TSharedPtr<FJsonObject> NewObj = JsonVal->AsObject();

    UE3DSJsonObject* JsonObj = NewObject<UE3DSJsonObject>();
    JsonObj->SetRootObject(NewObj);

    return JsonObj;
}


//////////////////////////////////////////////////////////////////////////
// Helpers

void UE3DSJsonValue::ErrorMessage(const FString& InType) const
{
    UE_LOG(LogE3DSFeatures, Error, TEXT("Json Value of type '%s' used as a '%s'."), *GetTypeString(), *InType);
}
