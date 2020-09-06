// Copyright 2016 Vladimir Alyamkin. All Rights Reserved.

#include "VaRestfulLibrary.h"
#include "VaRestfulRequestJSON.h"
#include "VaRestfulJsonObject.h"
#include "VaRestfulPluginPrivatePCH.h"
#include "Misc/Base64.h"

//////////////////////////////////////////////////////////////////////////
// Helpers

FString UVaRestfulLibrary::PercentEncode(const FString& Source)
{
	return FGenericPlatformHttp::UrlEncode(Source);
}

FString UVaRestfulLibrary::Base64Encode(const FString& Source)
{
	return FBase64::Encode(Source);
}

bool UVaRestfulLibrary::Base64Decode(const FString& Source, FString& Dest)
{
	return FBase64::Decode(Source, Dest);
}

bool UVaRestfulLibrary::Base64EncodeData(const TArray<uint8>& Data, FString& Dest)
{
	if (Data.Num() > 0)
	{
		Dest = FBase64::Encode(Data);
		return true;
	}
	
	return false;
}

bool UVaRestfulLibrary::Base64DecodeData(const FString& Source, TArray<uint8>& Dest)
{
	return FBase64::Decode(Source, Dest);
}


//////////////////////////////////////////////////////////////////////////
// File system integration

class UVaRestfulJsonObject* UVaRestfulLibrary::LoadJsonFromFile(UObject* WorldContextObject, const FString& Path)
{
	UVaRestfulJsonObject* Json = UVaRestfulJsonObject::ConstructJsonObject(WorldContextObject);

	FString JSONString;
	if (FFileHelper::LoadFileToString(JSONString, *(FPaths::ProjectContentDir() + Path)))
	{
		if (Json->DecodeJson(JSONString))
		{
			return Json;
		}
		else
		{
			UE_LOG(LogVaRest, Error, TEXT("%s: Can't decode json from file %s"), *VA_FUNC_LINE, *Path);
		}
	}
	else
	{
		UE_LOG(LogVaRest, Error, TEXT("%s: Can't open file %s"), *VA_FUNC_LINE, *Path);
	}

	return nullptr;
}

bool UVaRestfulLibrary::SaveJsonFromFile(UObject* WorldContextObject, const FString& Path, UVaRestfulJsonObject* VaRestfulJsonObject, bool bToSingleString)
{
	if (VaRestfulJsonObject)
	{
		FString JSONString = bToSingleString ? VaRestfulJsonObject->EncodeJsonToSingleString() : VaRestfulJsonObject->EncodeJson();
		if (FFileHelper::SaveStringToFile(JSONString, *(FPaths::ProjectContentDir() + Path)))
		{
			return true;
		}
		else
		{
			UE_LOG(LogVaRest, Error, TEXT("%s: Can't save file %s"), *VA_FUNC_LINE, *Path);
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
// Easy URL processing

TMap<UVaRestfulRequestJSON*, FVaRestfulCallResponse> UVaRestfulLibrary::RequestMap;

void UVaRestfulLibrary::Call(UObject* WorldContextObject, UVaRestfulRequestJSON* Request, const FVaRestfulCallDelegate& Callback)
{
	if (Request)
	{
		FVaRestfulCallResponse Response;
		Response.Request = Request;
		Response.WorldContextObject = WorldContextObject;
		Response.Callback = Callback;

		Response.CompleteDelegateHandle = Request->OnStaticRequestComplete.AddStatic(&UVaRestfulLibrary::OnCallComplete);
		Response.FailDelegateHandle = Request->OnStaticRequestFail.AddStatic(&UVaRestfulLibrary::OnCallComplete);

		RequestMap.Add(Request, Response);

		//Request->ResetResponseData();
		Request->ProcessURL(Request->GetURL());
	}
}

void UVaRestfulLibrary::CallURL(UObject* WorldContextObject, UVaRestfulRequestJSON* Request, const FString& Url, const FVaRestfulCallDelegate& Callback)
{
	if (Request)
	{
		Request->SetURL(Url);
		Call(WorldContextObject, Request, Callback);
	}
}

/*
void UVaRestLibrary::CallURL(UObject* WorldContextObject, const FString& URL, ERequestVerb Verb, ERequestContentType ContentType, UVaRestJsonObject* VaRestJson, const FVaRestCallDelegate& Callback)
{
	if (ContentType == ERequestContentType::binary)
	{
		UE_LOG(LogVaRest, Warning, TEXT("Binary content with Obj and without bytes"))
	}

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (World == nullptr)
	{
		UE_LOG(LogVaRest, Error, TEXT("UVaRestLibrary: Wrong world context"))
		return;
	}
	
	// Check we have valid data json
	if (VaRestJson == nullptr)
	{
		VaRestJson = UVaRestJsonObject::ConstructJsonObject(WorldContextObject);
	}
	
	UVaRestRequestJSON* Request = NewObject<UVaRestRequestJSON>();
	
	Request->SetVerb(Verb);
	Request->SetContentType(ContentType);
	Request->SetRequestObject(VaRestJson);
	
	FVaRestCallResponse Response;
	Response.Request = Request;
	Response.WorldContextObject = WorldContextObject;
	Response.Callback = Callback;

	Response.CompleteDelegateHandle = Request->OnStaticRequestComplete.AddStatic(&UVaRestLibrary::OnCallComplete);
	Response.FailDelegateHandle = Request->OnStaticRequestFail.AddStatic(&UVaRestLibrary::OnCallComplete);

	RequestMap.Add(Request, Response);

	Request->ResetResponseData();
	Request->ProcessURL(URL);
}


void UVaRestLibrary::CallURLForForm(UObject* WorldContextObject, const FString& URL, ERequestVerb Verb, UVaRestJsonObject* VaRestJson, const FString& FormContent, const FVaRestCallDelegate& Callback)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (World == nullptr)
	{
		UE_LOG(LogVaRest, Error, TEXT("UVaRestLibrary: Wrong world context"))
			return;
	}

	// Check we have valid data json
	if (VaRestJson == nullptr)
	{
		VaRestJson = UVaRestJsonObject::ConstructJsonObject(WorldContextObject);
	}

	UVaRestRequestJSON* Request = NewObject<UVaRestRequestJSON>();

	Request->SetVerb(Verb);
	Request->SetContentType(ERequestContentType::x_www_form_urlencoded_url);
	Request->SetRequestObject(VaRestJson);
	Request->SetStringRequestContent(FormContent);

	FVaRestCallResponse Response;
	Response.Request = Request;
	Response.WorldContextObject = WorldContextObject;
	Response.Callback = Callback;

	Response.CompleteDelegateHandle = Request->OnStaticRequestComplete.AddStatic(&UVaRestLibrary::OnCallComplete);
	Response.FailDelegateHandle = Request->OnStaticRequestFail.AddStatic(&UVaRestLibrary::OnCallComplete);

	RequestMap.Add(Request, Response);

	Request->ResetResponseData();
	Request->ProcessURL(URL);
}



void UVaRestLibrary::CallURLForBinaryData(UObject* WorldContextObject, const FString& URL, ERequestVerb Verb, const FString& BinaryContentType, const TArray<uint8>& BinaryContent, const FVaRestCallDelegate& Callback)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (World == nullptr)
	{
		UE_LOG(LogVaRest, Error, TEXT("UVaRestLibrary: Wrong world context"))
			return;
	}

	UVaRestRequestJSON* Request = NewObject<UVaRestRequestJSON>();

	Request->SetVerb(Verb);
	Request->SetContentType(ERequestContentType::binary);
	Request->SetBinaryContentType(BinaryContentType);
	Request->SetBinaryRequestContent(BinaryContent);

	FVaRestCallResponse Response;
	Response.Request = Request;
	Response.WorldContextObject = WorldContextObject;
	Response.Callback = Callback;

	Response.CompleteDelegateHandle = Request->OnStaticRequestComplete.AddStatic(&UVaRestLibrary::OnCallComplete);
	Response.FailDelegateHandle = Request->OnStaticRequestFail.AddStatic(&UVaRestLibrary::OnCallComplete);

	RequestMap.Add(Request, Response);

	Request->ResetResponseData();
	Request->ProcessURL(URL);
}
*/

void UVaRestfulLibrary::OnCallComplete(UVaRestfulRequestJSON* Request)
{
	if (!RequestMap.Contains(Request))
	{
		return;
	}
	
	FVaRestfulCallResponse* Response = RequestMap.Find(Request);
	
	Request->OnStaticRequestComplete.Remove(Response->CompleteDelegateHandle);
	Request->OnStaticRequestFail.Remove(Response->FailDelegateHandle);
	
	Response->Callback.ExecuteIfBound(Request);
	
	Response->WorldContextObject = nullptr;
	Response->Request = nullptr;
	RequestMap.Remove(Request);
}
