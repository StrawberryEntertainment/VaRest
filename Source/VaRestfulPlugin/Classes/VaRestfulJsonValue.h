// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaRestfulJsonValue.generated.h"

class UVaRestfulJsonObject;
class FJsonValue;

/**
 * Represents all the types a Json Value can be.
 */
UENUM(BlueprintType)
namespace EVaFulJson
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
class VARESTFULPLUGIN_API UVaRestfulJsonValue : public UObject
{
	GENERATED_UCLASS_BODY()

	/** Create new Json Number value
	 * Attn.!! float used instead of double to make the function blueprintable! */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Number Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Json")
	static UVaRestfulJsonValue* ConstructJsonValueNumber(UObject* WorldContextObject, float Number);

	/** Create new Json String value */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json String Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Json")
	static UVaRestfulJsonValue* ConstructJsonValueString(UObject* WorldContextObject, const FString& StringValue);

	/** Create new Json Bool value */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Bool Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Json")
	static UVaRestfulJsonValue* ConstructJsonValueBool(UObject* WorldContextObject, bool InValue);

	/** Create new Json Array value */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Array Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Json")
	static UVaRestfulJsonValue* ConstructJsonValueArray(UObject* WorldContextObject, const TArray<UVaRestfulJsonValue*>& InArray);

	/** Create new Json Object value */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Object Value", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Json")
	static UVaRestfulJsonValue* ConstructJsonValueObject(UObject* WorldContextObject, UVaRestfulJsonObject *JsonObject);

	/** Create new Json value from FJsonValue (to be used from VaRestfulJsonObject) */
	static UVaRestfulJsonValue* ConstructJsonValue(UObject* WorldContextObject, const TSharedPtr<FJsonValue>& InValue);

	/** Get the root Json value */
	TSharedPtr<FJsonValue>& GetRootValue();

	/** Set the root Json value */
	void SetRootValue(TSharedPtr<FJsonValue>& JsonValue);


	//////////////////////////////////////////////////////////////////////////
	// FJsonValue API

	/** Get type of Json value (Enum) */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	EVaFulJson::Type GetType() const;

	/** Get type of Json value (String) */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	FString GetTypeString() const;

	/** Returns true if this value is a 'null' */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	bool IsNull() const;

	/** Returns this value as a double, throwing an error if this is not an Json Number
	 * Attn.!! float used instead of double to make the function blueprintable! */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	float AsNumber() const;

	/** Returns this value as a number, throwing an error if this is not an Json String */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	FString AsString() const;

	/** Returns this value as a boolean, throwing an error if this is not an Json Bool */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	bool AsBool() const;

	/** Returns this value as an array, throwing an error if this is not an Json Array */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	TArray<UVaRestfulJsonValue*> AsArray() const;

	/** Returns this value as an object, throwing an error if this is not an Json Object */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	UVaRestfulJsonObject* AsObject();

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	void SwapRef(UVaRestfulJsonValue * AnotherValue)
	{
		if (this != AnotherValue && AnotherValue)
		{
			auto Temp = JsonVal;
			JsonVal = AnotherValue->JsonVal;
			AnotherValue->JsonVal = Temp;
		}
		else if (AnotherValue == nullptr)
		{
			JsonVal.Reset();
		}
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Json")
	void CopyRefFrom(UVaRestfulJsonValue * AnotherValue)
	{
		if (this != AnotherValue && AnotherValue)
		{
			JsonVal = AnotherValue->JsonVal;
		}
		else if (AnotherValue == nullptr)
		{
			JsonVal.Reset();
		}
	}

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
