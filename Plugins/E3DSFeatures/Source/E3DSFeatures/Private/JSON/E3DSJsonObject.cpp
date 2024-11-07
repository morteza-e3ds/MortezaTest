// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 

#include "E3DSFeatures/Public/JSON/E3DSJsonObject.h"

#include "E3DSFeaturesDefines.h"
#include "JSON/E3DSJsonValue.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"


typedef TJsonWriterFactory< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriterFactory;
typedef TJsonWriter< TCHAR, TCondensedJsonPrintPolicy<TCHAR> > FCondensedJsonStringWriter;


UE3DSJsonObject::UE3DSJsonObject(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	Reset();
}

UE3DSJsonObject* UE3DSJsonObject::ConstructJsonObject(UObject* WorldContextObject)
{
    return NewObject<UE3DSJsonObject>();
}

void UE3DSJsonObject::Reset()
{
    if (JsonObj.IsValid())
    {
        JsonObj.Reset();
    }

    JsonObj = MakeShareable(new FJsonObject());
}

TSharedPtr<FJsonObject>& UE3DSJsonObject::GetRootObject()
{
    return JsonObj;
}

void UE3DSJsonObject::SetRootObject(TSharedPtr<FJsonObject>& JsonObject)
{
    JsonObj = JsonObject;
}


//////////////////////////////////////////////////////////////////////////
// Serialization

FString UE3DSJsonObject::EncodeJson() const
{
    if (!JsonObj.IsValid())
    {
        return TEXT("");
    }

    FString OutputString;
    TSharedRef< FCondensedJsonStringWriter > Writer = FCondensedJsonStringWriterFactory::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);

    return OutputString;
}

bool UE3DSJsonObject::DecodeJson(const FString& JsonString)
{
    TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(*JsonString);
    if (FJsonSerializer::Deserialize(Reader, JsonObj) && JsonObj.IsValid())
    {
        return true;
    }

    // If we've failed to deserialize the string, we should clear our internal data
    Reset();

    UE_LOG(LogE3DSFeatures, Error, TEXT("Json decoding failed for: %s"), *JsonString);

    return false;
}


//////////////////////////////////////////////////////////////////////////
// FJsonObject API

TArray<FString> UE3DSJsonObject::GetFieldNames()
{
    TArray<FString> Result;

    if (!JsonObj.IsValid())
    {
        return Result;
    }

    JsonObj->Values.GetKeys(Result);

    return Result;
}

bool UE3DSJsonObject::HasField(const FString& FieldName) const
{
    if (!JsonObj.IsValid())
    {
        return false;
    }

    return JsonObj->HasField(FieldName);
}

void UE3DSJsonObject::RemoveField(const FString& FieldName)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    JsonObj->RemoveField(FieldName);
}

UE3DSJsonValue* UE3DSJsonObject::GetField(const FString& FieldName) const
{
    if (!JsonObj.IsValid())
    {
        return nullptr;
    }

    TSharedPtr<FJsonValue> NewVal = JsonObj->TryGetField(FieldName);

    UE3DSJsonValue* NewValue = NewObject<UE3DSJsonValue>();
    NewValue->SetRootValue(NewVal);

    return NewValue;
}

void UE3DSJsonObject::SetField(const FString& FieldName, UE3DSJsonValue* JsonValue)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    JsonObj->SetField(FieldName, JsonValue->GetRootValue());
}

void UE3DSJsonObject::SetFieldNull(const FString& FieldName)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    TSharedPtr<FJsonValue> myNull = MakeShareable(new FJsonValueNull());

    JsonObj->SetField(FieldName, myNull);
}

//////////////////////////////////////////////////////////////////////////
// FJsonObject API Helpers (easy to use with simple Json objects)

float UE3DSJsonObject::GetNumberField(const FString& FieldName) const
{
    if (!JsonObj.IsValid())
    {
        return 0.0f;
    }

    return JsonObj->GetNumberField(FieldName);
}

void UE3DSJsonObject::SetNumberField(const FString& FieldName, float Number)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    JsonObj->SetNumberField(FieldName, Number);
}

int32 UE3DSJsonObject::GetIntegerField(const FString& FieldName)
{
    if (!JsonObj->HasTypedField<EJson::Number>(FieldName))
    {
        UE_LOG(LogE3DSFeatures, Warning, TEXT("No field with name %s of type Number"), *FieldName);
        return 0;
    }

    return JsonObj->GetIntegerField(FieldName);
}

FString UE3DSJsonObject::GetStringField(const FString& FieldName) const
{
    if (!JsonObj.IsValid())
    {
        return TEXT("");
    }

    return JsonObj->GetStringField(FieldName);
}

void UE3DSJsonObject::SetStringField(const FString& FieldName, const FString& StringValue)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    JsonObj->SetStringField(FieldName, StringValue);
}

bool UE3DSJsonObject::GetBoolField(const FString& FieldName) const
{
    if (!JsonObj.IsValid())
    {
        return false;
    }

    return JsonObj->GetBoolField(FieldName);
}

void UE3DSJsonObject::SetBoolField(const FString& FieldName, bool InValue)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    JsonObj->SetBoolField(FieldName, InValue);
}

TArray<UE3DSJsonValue*> UE3DSJsonObject::GetArrayField(const FString& FieldName)
{
    TArray<UE3DSJsonValue*> OutArray;
    if (!JsonObj.IsValid())
    {
        return OutArray;
    }

    TArray< TSharedPtr<FJsonValue> > ValArray = JsonObj->GetArrayField(FieldName);
    for (auto Value : ValArray)
    {
        UE3DSJsonValue* NewValue = NewObject<UE3DSJsonValue>();
        NewValue->SetRootValue(Value);

        OutArray.Add(NewValue);
    }

    return OutArray;
}

void UE3DSJsonObject::SetArrayField(const FString& FieldName, const TArray<UE3DSJsonValue*>& InArray)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > ValArray;

    // Process input array and COPY original values
    for (auto InVal : InArray)
    {
        TSharedPtr<FJsonValue> JsonVal = InVal->GetRootValue();

        switch (InVal->GetType())
        {
        case EPFJson::None:
            break;

        case EPFJson::Null:
            ValArray.Add(MakeShareable(new FJsonValueNull()));
            break;

        case EPFJson::String:
            ValArray.Add(MakeShareable(new FJsonValueString(JsonVal->AsString())));
            break;

        case EPFJson::Number:
            ValArray.Add(MakeShareable(new FJsonValueNumber(JsonVal->AsNumber())));
            break;

        case EPFJson::Boolean:
            ValArray.Add(MakeShareable(new FJsonValueBoolean(JsonVal->AsBool())));
            break;

        case EPFJson::Array:
            ValArray.Add(MakeShareable(new FJsonValueArray(JsonVal->AsArray())));
            break;

        case EPFJson::Object:
            ValArray.Add(MakeShareable(new FJsonValueObject(JsonVal->AsObject())));
            break;

        default:
            break;
        }
    }

    JsonObj->SetArrayField(FieldName, ValArray);
}

void UE3DSJsonObject::MergeJsonObject(UE3DSJsonObject* InJsonObject, bool Overwrite)
{
    TArray<FString> Keys = InJsonObject->GetFieldNames();

    for (auto Key : Keys)
    {
        if (Overwrite == false && HasField(Key))
        {
            continue;
        }

        SetField(Key, InJsonObject->GetField(Key));
    }
}

UE3DSJsonObject* UE3DSJsonObject::GetObjectField(const FString& FieldName) const
{
    if (!JsonObj.IsValid())
    {
        return nullptr;
    }

    TSharedPtr<FJsonObject> JsonObjField = JsonObj->GetObjectField(FieldName);

    UE3DSJsonObject* OutRestJsonObj = NewObject<UE3DSJsonObject>();
    OutRestJsonObj->SetRootObject(JsonObjField);

    return OutRestJsonObj;
}

void UE3DSJsonObject::SetObjectField(const FString& FieldName, UE3DSJsonObject* JsonObject)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    JsonObj->SetObjectField(FieldName, JsonObject->GetRootObject());
}


//////////////////////////////////////////////////////////////////////////
// Array fields helpers (uniform arrays)

TArray<float> UE3DSJsonObject::GetNumberArrayField(const FString& FieldName)
{
    TArray<float> NumberArray;

    if (!JsonObj.IsValid())
    {
        return NumberArray;
    }

    TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
    for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
    {
        NumberArray.Add((*It)->AsNumber());
    }

    return NumberArray;
}

void UE3DSJsonObject::SetNumberArrayField(const FString& FieldName, const TArray<float>& NumberArray)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > EntriesArray;

    for (auto Number : NumberArray)
    {
        EntriesArray.Add(MakeShareable(new FJsonValueNumber(Number)));
    }

    JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<FString> UE3DSJsonObject::GetStringArrayField(const FString& FieldName)
{
    TArray<FString> StringArray;

    if (!JsonObj.IsValid())
    {
        return StringArray;
    }

    TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
    for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
    {
        StringArray.Add((*It)->AsString());
    }

    return StringArray;
}

void UE3DSJsonObject::SetStringArrayField(const FString& FieldName, const TArray<FString>& StringArray)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > EntriesArray;

    for (auto String : StringArray)
    {
        EntriesArray.Add(MakeShareable(new FJsonValueString(String)));
    }

    JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<bool> UE3DSJsonObject::GetBoolArrayField(const FString& FieldName)
{
    TArray<bool> BoolArray;

    if (!JsonObj.IsValid())
    {
        return BoolArray;
    }

    TArray<TSharedPtr<FJsonValue> > JsonArrayValues = JsonObj->GetArrayField(FieldName);
    for (TArray<TSharedPtr<FJsonValue> >::TConstIterator It(JsonArrayValues); It; ++It)
    {
        BoolArray.Add((*It)->AsBool());
    }

    return BoolArray;
}

void UE3DSJsonObject::SetBoolArrayField(const FString& FieldName, const TArray<bool>& BoolArray)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > EntriesArray;

    for (auto Boolean : BoolArray)
    {
        EntriesArray.Add(MakeShareable(new FJsonValueBoolean(Boolean)));
    }

    JsonObj->SetArrayField(FieldName, EntriesArray);
}

TArray<UE3DSJsonObject*> UE3DSJsonObject::GetObjectArrayField(const FString& FieldName)
{
    TArray<UE3DSJsonObject*> OutArray;

    if (!JsonObj.IsValid())
    {
        return OutArray;
    }

    TArray< TSharedPtr<FJsonValue> > ValArray = JsonObj->GetArrayField(FieldName);
    for (auto Value : ValArray)
    {
        TSharedPtr<FJsonObject> NewObj = Value->AsObject();

        UE3DSJsonObject* NewJson = NewObject<UE3DSJsonObject>();
        NewJson->SetRootObject(NewObj);

        OutArray.Add(NewJson);
    }

    return OutArray;
}

void UE3DSJsonObject::SetObjectArrayField(const FString& FieldName, const TArray<UE3DSJsonObject*>& ObjectArray)
{
    if (!JsonObj.IsValid())
    {
        return;
    }

    TArray< TSharedPtr<FJsonValue> > EntriesArray;

    for (auto Value : ObjectArray)
    {
        EntriesArray.Add(MakeShareable(new FJsonValueObject(Value->GetRootObject())));
    }

    JsonObj->SetArrayField(FieldName, EntriesArray);
}