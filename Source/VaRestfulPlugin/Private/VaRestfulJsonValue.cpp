// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaRestfulJsonValue.h"
#include "VaRestfulJsonObject.h"
#include "VaRestfulPluginPrivatePCH.h"

UVaRestfulJsonValue::UVaRestfulJsonValue(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

UVaRestfulJsonValue* UVaRestfulJsonValue::ConstructJsonValueNumber(UObject* WorldContextObject, float Number)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueNumber(Number));

	UVaRestfulJsonValue* NewValue = NewObject<UVaRestfulJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UVaRestfulJsonValue* UVaRestfulJsonValue::ConstructJsonValueString(UObject* WorldContextObject, const FString& StringValue)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueString(StringValue));

	UVaRestfulJsonValue* NewValue = NewObject<UVaRestfulJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UVaRestfulJsonValue* UVaRestfulJsonValue::ConstructJsonValueBool(UObject* WorldContextObject, bool InValue)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueBoolean(InValue));

	UVaRestfulJsonValue* NewValue = NewObject<UVaRestfulJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UVaRestfulJsonValue* UVaRestfulJsonValue::ConstructJsonValueArray(UObject* WorldContextObject, const TArray<UVaRestfulJsonValue*>& InArray)
{
	// Prepare data array to create new value
	TArray< TSharedPtr<FJsonValue> > ValueArray;
	for (auto InVal : InArray)
	{
		ValueArray.Add(InVal->GetRootValue());
	}

	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueArray(ValueArray));

	UVaRestfulJsonValue* NewValue = NewObject<UVaRestfulJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UVaRestfulJsonValue* UVaRestfulJsonValue::ConstructJsonValueObject(UObject* WorldContextObject, UVaRestfulJsonObject *JsonObject)
{
	TSharedPtr<FJsonValue> NewVal = MakeShareable(new FJsonValueObject(JsonObject->GetRootObject()));

	UVaRestfulJsonValue* NewValue = NewObject<UVaRestfulJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

UVaRestfulJsonValue* UVaRestfulJsonValue::ConstructJsonValue(UObject* WorldContextObject, const TSharedPtr<FJsonValue>& InValue)
{
	TSharedPtr<FJsonValue> NewVal = InValue;

	UVaRestfulJsonValue* NewValue = NewObject<UVaRestfulJsonValue>();
	NewValue->SetRootValue(NewVal);

	return NewValue;
}

TSharedPtr<FJsonValue>& UVaRestfulJsonValue::GetRootValue()
{
	return JsonVal;
}

void UVaRestfulJsonValue::SetRootValue(TSharedPtr<FJsonValue>& JsonValue)
{
	JsonVal = JsonValue;
}


//////////////////////////////////////////////////////////////////////////
// FJsonValue API

EVaFulJson::Type UVaRestfulJsonValue::GetType() const
{
	if (!JsonVal.IsValid())
	{
		return EVaFulJson::None;
	}

	switch (JsonVal->Type)
	{
	case EJson::None:
		return EVaFulJson::None;

	case EJson::Null:
		return EVaFulJson::Null;

	case EJson::String:
		return EVaFulJson::String;

	case EJson::Number:
		return EVaFulJson::Number;

	case EJson::Boolean:
		return EVaFulJson::Boolean;

	case EJson::Array:
		return EVaFulJson::Array;

	case EJson::Object:
		return EVaFulJson::Object;

	default:
		return EVaFulJson::None;
	}
}

FString UVaRestfulJsonValue::GetTypeString() const
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

bool UVaRestfulJsonValue::IsNull() const 
{
	if (!JsonVal.IsValid())
	{
		return true;
	}

	return JsonVal->IsNull();
}

float UVaRestfulJsonValue::AsNumber() const
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Number"));
		return 0.f;
	}

	return JsonVal->AsNumber();
}

FString UVaRestfulJsonValue::AsString() const
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("String"));
		return FString();
	}

	return JsonVal->AsString();
}

bool UVaRestfulJsonValue::AsBool() const
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Boolean"));
		return false;
	}

	return JsonVal->AsBool();
}

TArray<UVaRestfulJsonValue*> UVaRestfulJsonValue::AsArray() const
{
	TArray<UVaRestfulJsonValue*> OutArray;

	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Array"));
		return OutArray;
	}

	TArray< TSharedPtr<FJsonValue> > ValArray = JsonVal->AsArray();
	for (auto Value : ValArray)
	{
		UVaRestfulJsonValue* NewValue = NewObject<UVaRestfulJsonValue>();
		NewValue->SetRootValue(Value);

		OutArray.Add(NewValue);
	}

	return OutArray;
}

UVaRestfulJsonObject* UVaRestfulJsonValue::AsObject()
{
	if (!JsonVal.IsValid())
	{
		ErrorMessage(TEXT("Object"));
		return nullptr;
	}

	TSharedPtr<FJsonObject> NewObj = JsonVal->AsObject();

	UVaRestfulJsonObject* JsonObj = NewObject<UVaRestfulJsonObject>();
	JsonObj->SetRootObject(NewObj);

	return JsonObj;
}


//////////////////////////////////////////////////////////////////////////
// Helpers

void UVaRestfulJsonValue::ErrorMessage(const FString& InType) const
{
	UE_LOG(LogVaRest, Error, TEXT("Json Value of type '%s' used as a '%s'."), *GetTypeString(), *InType);
}
