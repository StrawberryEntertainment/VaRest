// Copyright 2016 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "VaRestfulTypes.h"
#include "VaRestfulLibrary.generated.h"

class UVaRestfulRequestJSON;
class UVaRestfulJsonObject;

DECLARE_DYNAMIC_DELEGATE_OneParam(FVaRestfulCallDelegate, UVaRestfulRequestJSON*, Request);

USTRUCT()
struct FVaRestfulCallResponse
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	UVaRestfulRequestJSON* Request;
	
	UPROPERTY()
	UObject* WorldContextObject;
	
	UPROPERTY()
	FVaRestfulCallDelegate Callback;
	
	FDelegateHandle CompleteDelegateHandle;
	FDelegateHandle FailDelegateHandle;
	
	FVaRestfulCallResponse()
		: Request(nullptr)
		, WorldContextObject(nullptr)
	{
	}

};

/**
 * Useful tools for REST communications
 */
UCLASS()
class VARESTFULPLUGIN_API UVaRestfulLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	//////////////////////////////////////////////////////////////////////////
	// Helpers

public:
	/** Applies percent-encoding to text */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility")
	static FString PercentEncode(const FString& Source);

	/**
	 * Encodes a FString into a Base64 string
	 *
	 * @param Source	The string data to convert
	 * @return			A string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
	 */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility", meta = (DisplayName = "Base64 Encode"))
	static FString Base64Encode(const FString& Source);

	/**
	 * Decodes a Base64 string into a FString
	 *
	 * @param Source	The stringified data to convert
	 * @param Dest		The out buffer that will be filled with the decoded data
	 * @return			True if the buffer was decoded, false if it failed to decode
	 */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility", meta = (DisplayName = "Base64 Decode"))
	static bool Base64Decode(const FString& Source, FString& Dest);

	/**
	 * Encodes a byte array into a Base64 string
	 *
	 * @param Dara		The data to convert
	 * @return			A string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
	 */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility", meta = (DisplayName = "Base64 Encode Data"))
	static bool Base64EncodeData(const TArray<uint8>& Data, FString& Dest);

	/**
	 * Decodes a Base64 string into a byte array
	 *
	 * @param Source	The stringified data to convert
	 * @param Dest		The out buffer that will be filled with the decoded data
	 * @return			True if the buffer was decoded, false if it failed to decode
	 */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility", meta = (DisplayName = "Base64 Decode Data"))
	static bool Base64DecodeData(const FString& Source, TArray<uint8>& Dest);


	//////////////////////////////////////////////////////////////////////////
	// File system integration

public:
	/** 
	 * Load JSON from formatted text file
	 * @param Path		File name relative to the Content folder
	 */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility", meta = (WorldContext = "WorldContextObject"))
	static class UVaRestfulJsonObject* LoadJsonFromFile(UObject* WorldContextObject, const FString& Path);

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility", meta = (WorldContext = "WorldContextObject"))
	static bool SaveJsonFromFile(UObject* WorldContextObject, const FString& Path, UVaRestfulJsonObject* VaRestfulJsonObject, bool bToSingleString = false);

	//////////////////////////////////////////////////////////////////////////
	// Easy URL processing

public:

	/** HerrSergio ADDITION */ // Needed to use callback in Functions when EventGraph is not enough
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	static void Call(UObject* WorldContextObject, UVaRestfulRequestJSON* Request, const FVaRestfulCallDelegate& Callback);


	/** HerrSergio ADDITION */ // Needed to use callback in Functions when EventGraph is not enough
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	static void CallURL(UObject* WorldContextObject, UVaRestfulRequestJSON* Request, const FString& Url, const FVaRestfulCallDelegate& Callback);

	/** Easy way to process http requests */
	//UFUNCTION(BlueprintCallable, Category = "VaRest|Utility", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	//static void CallURL(UObject* WorldContextObject, const FString& URL, ERequestVerb Verb, ERequestContentType ContentType, UVaRestJsonObject* VaRestJson, const FVaRestCallDelegate& Callback);

	/** HerrSergio ADDITION */
//	UFUNCTION(BlueprintCallable, Category = "VaRest|Utility", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	//static void CallURLForForm(UObject* WorldContextObject, const FString& URL, ERequestVerb Verb, UVaRestJsonObject* VaRestJson, const FString& FormContent, const FVaRestCallDelegate& Callback);

	/** HerrSergio ADDITION */
	//UFUNCTION(BlueprintCallable, Category = "VaRest|Utility", meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
	//static void CallURLForBinaryData(UObject* WorldContextObject, const FString& URL, ERequestVerb Verb, const FString& BinaryContentType, const TArray<uint8>& BinaryContent, const FVaRestCallDelegate& Callback);


	/** Called when URL is processed (one for both success/unsuccess events)*/
	static void OnCallComplete(UVaRestfulRequestJSON* Request);

private:
	static TMap<UVaRestfulRequestJSON*, FVaRestfulCallResponse> RequestMap;

};
