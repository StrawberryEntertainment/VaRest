// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaRestfulRequestJSON.h"

#include "VaRestfulJsonObject.h"
#include "VaRestfulLibrary.h"
#include "VaRestfulSettings.h"
#include "VaRestfulPluginPrivatePCH.h"

#include "Misc/CoreMisc.h"
#include "Runtime/Launch/Resources/Version.h"

//FString UVaRestfulRequestJSON::DeprecatedResponseString(TEXT("DEPRECATED: Please use GetResponseContentAsString() instead"));

/*template <class T>
void FVaRestfulLatentAction<T>::Cancel()
{
	UObject *Obj = Request.Get();
	if (Obj != nullptr)
	{
		((UVaRestRequestJSON*)Obj)->Cancel();
	}
}*/

UVaRestfulRequestJSON::UVaRestfulRequestJSON(const class FObjectInitializer& PCIP)
	: Super(PCIP),
	BinaryContentType(TEXT("application/octet-stream"))
{
	ContinueAction = nullptr;

	RequestVerb = ERequestFulVerb::GET;
	RequestContentType = ERequestFulContentType::x_www_form_urlencoded_url;

	ResetData();
}

/*UVaRestRequestJSON* UVaRestRequestJSON::ConstructRequest(UObject* WorldContextObject)
{
	return NewObject<UVaRestRequestJSON>();
}

UVaRestRequestJSON* UVaRestRequestJSON::ConstructRequestExt(
	UObject* WorldContextObject,
	ERequestVerb Verb,
	ERequestContentType ContentType)
{
	UVaRestRequestJSON* Request = ConstructRequest(WorldContextObject);

	Request->SetVerb(Verb);
	Request->SetContentType(ContentType);

	return Request;
}*/

FString UVaRestfulRequestJSON::URLEncode(UVaRestfulJsonObject* VaRestfulJson)
{
	FString UrlParams = "";
	uint16 ParamIdx = 0;

	if (VaRestfulJson != nullptr)
	{
		// Loop through all the values and prepare additional url part
		for (auto RequestIt = VaRestfulJson->GetRootObject()->Values.CreateIterator(); RequestIt; ++RequestIt)
		{
			FString Key = RequestIt.Key();
			FString Value = RequestIt.Value().Get()->AsString();

			if (!Key.IsEmpty() && !Value.IsEmpty())
			{
				UrlParams += ParamIdx == 0 ? "" : "&";
				UrlParams += UVaRestfulLibrary::PercentEncode(Key) + "=" + UVaRestfulLibrary::PercentEncode(Value);
			}

			ParamIdx++;
		}
	}

	return UrlParams;
}

FString UVaRestfulRequestJSON::URLEncodeMap(const TMap<FString, FString>& QueryMap)
{
	FString UrlParams = "";
	uint16 ParamIdx = 0;

	// Loop through all the values and prepare additional url part
	for (auto RequestIt = QueryMap.CreateConstIterator(); RequestIt; ++RequestIt)
	{
		FString Key = RequestIt.Key();
		FString Value = RequestIt.Value();

		if (!Key.IsEmpty() && !Value.IsEmpty())
		{
			UrlParams += ParamIdx == 0 ? "" : "&";
			UrlParams += UVaRestfulLibrary::PercentEncode(Key) + "=" + UVaRestfulLibrary::PercentEncode(Value);
		}

		ParamIdx++;
	}

	return UrlParams;
}

void UVaRestfulRequestJSON::SetVerb(ERequestFulVerb Verb)
{
	RequestVerb = Verb;
}

void UVaRestfulRequestJSON::SetCustomVerb(FString Verb)
{
	CustomVerb = Verb;
}

void UVaRestfulRequestJSON::SetContentType(ERequestFulContentType ContentType)
{
	RequestContentType = ContentType;
}

void UVaRestfulRequestJSON::SetBinaryContentType(const FString& ContentType)
{
	BinaryContentType = ContentType;
}

void UVaRestfulRequestJSON::SetBinaryRequestContent(const TArray<uint8>& Bytes)
{
	RequestBytes = Bytes;
}

void UVaRestfulRequestJSON::SetStringRequestContent(const FString& Content)
{
	StringRequestContent = Content;
}

void UVaRestfulRequestJSON::SetHeader(const FString& HeaderName, const FString& HeaderValue)
{
	RequestHeaders.Add(HeaderName, HeaderValue);
}


//////////////////////////////////////////////////////////////////////////
// Destruction and reset

void UVaRestfulRequestJSON::ResetData()
{
	if (!bIsInProcess)
	{
		ResetRequestData();
		ResetResponseData();
		bIsCompleted = false;
	}
	else
	{
		UE_LOG(LogVaRest, Log, TEXT("%s: Request cannot be reset while executed : %s %s %s"), *VA_FUNC_LINE, *HttpRequest->GetVerb(), *HttpRequest->GetURL(), *StringRequestContent);
	}
}

void UVaRestfulRequestJSON::ResetRequestData()
{
	if (RequestJsonObj != nullptr)
	{
		RequestJsonObj->Reset();
	}
	else
	{
		RequestJsonObj = NewObject<UVaRestfulJsonObject>();
	}

	// See issue #90
	// HttpRequest = FHttpModule::Get().CreateRequest();

	RequestBytes.Empty();
	StringRequestContent.Empty();
}

void UVaRestfulRequestJSON::ResetResponseData()
{
	if (ResponseJsonObj != nullptr)
	{
		ResponseJsonObj->Reset();
	}
	else
	{
		ResponseJsonObj = NewObject<UVaRestfulJsonObject>();
	}

	ResponseHeaders.Empty();
	ResponseCode = -1;
	ResponseSize = 0;

	bIsValidJsonResponse = false;
	bIsValidResponse = false;

	// #127 Reset string to deprecated state
	ResponseContent = "";// DeprecatedResponseString;

	ResponseData.Reset();
}

void UVaRestfulRequestJSON::Cancel()
{
	//ContinueAction = nullptr;
	HttpRequest->CancelRequest();
	//OnProcessRequestComplete(HttpRequest, FHttpResponsePtr(), false);
		//ResetResponseData();
	//bIsInProcess = false;
}


//////////////////////////////////////////////////////////////////////////
// JSON data accessors

UVaRestfulJsonObject* UVaRestfulRequestJSON::GetRequestObject()
{
	return RequestJsonObj;
}

void UVaRestfulRequestJSON::SetRequestObject(UVaRestfulJsonObject* JsonObject)
{
	RequestJsonObj = JsonObject;
}

UVaRestfulJsonObject* UVaRestfulRequestJSON::GetResponseObject()
{
	return ResponseJsonObj;
}

void UVaRestfulRequestJSON::SetResponseObject(UVaRestfulJsonObject* JsonObject)
{
	ResponseJsonObj = JsonObject;
}


///////////////////////////////////////////////////////////////////////////
// Response data access

FString UVaRestfulRequestJSON::GetURL()
{
	return HttpRequest->GetURL();
}

ERequestFulStatus UVaRestfulRequestJSON::GetStatus()
{
	return ERequestFulStatus((uint8)HttpRequest->GetStatus());
}

int32 UVaRestfulRequestJSON::GetResponseCode()
{
	return ResponseCode;
}

FString UVaRestfulRequestJSON::GetResponseHeader(const FString HeaderName)
{
	FString Result;

	FString* Header = ResponseHeaders.Find(HeaderName);
	if (Header != nullptr)
	{
		Result = *Header;
	}

	return Result;
}

TArray<FString> UVaRestfulRequestJSON::GetAllResponseHeaders()
{
	TArray<FString> Result;
	for (TMap<FString, FString>::TConstIterator It(ResponseHeaders); It; ++It)
	{
		Result.Add(It.Key() + TEXT(": ") + It.Value());
}
	return Result;
}


//////////////////////////////////////////////////////////////////////////
// URL processing

void UVaRestfulRequestJSON::SetURL(const FString& Url)
{
	// Be sure to trim URL because it can break links on iOS
	FString TrimmedUrl = Url;

#if ENGINE_MINOR_VERSION >= 18
	TrimmedUrl.TrimStartInline();
	TrimmedUrl.TrimEndInline();
#else
	TrimmedUrl.Trim();
	TrimmedUrl.TrimTrailing();
#endif

	HttpRequest->SetURL(TrimmedUrl);
}

void UVaRestfulRequestJSON::ProcessURL(const FString& Url)
{
	SetURL(Url);
	ProcessRequest();
}

void UVaRestfulRequestJSON::Apply(UVaRestfulRequestJSON *&Result, UObject* WorldContextObject, struct FLatentActionInfo LatentInfo)
{
	ContinueAction = FUnktomiFulLatentAction<UVaRestfulRequestJSON*>::RegisterLatentAction<FVaRestfulLatentAction<UVaRestfulRequestJSON*>>(this, Result, WorldContextObject, LatentInfo);
	ProcessRequest();
}

void UVaRestfulRequestJSON::ApplyURL(const FString& Url, UVaRestfulRequestJSON *&Result, UObject* WorldContextObject, FLatentActionInfo LatentInfo)
{
	/** HerrSergio REFACTOR */
	SetURL(Url);
	Apply(Result, WorldContextObject, LatentInfo);
}

/*
void UVaRestRequestJSON::ApplyURLForObject(const FString& URL, ERequestVerb Verb, ERequestContentType ContentType, UVaRestJsonObject* VaRestJson, UVaRestRequestJSON *&Result, UObject* WorldContextObject, struct FLatentActionInfo LatentInfo)
{
	if (ContentType == ERequestContentType::binary)
	{
		UE_LOG(LogVaRest, Warning, TEXT("Binary content with Obj and without bytes"))
	}

	SetVerb(Verb);
	SetContentType(ContentType);
	SetRequestObject(VaRestJson);
	SetBinaryContentType("");
	SetBinaryRequestContent(TArray<uint8>());
	SetStringRequestContent("");

	ApplyURL(URL, Result, WorldContextObject, LatentInfo);
}

void UVaRestRequestJSON::ApplyURLForForm(const FString& URL, ERequestVerb Verb, UVaRestJsonObject* VaRestJson, const FString& FormContent, UVaRestRequestJSON *&Result, UObject* WorldContextObject, struct FLatentActionInfo LatentInfo)
{
	SetVerb(Verb);
	SetContentType(ERequestContentType::x_www_form_urlencoded_url);
	SetRequestObject(VaRestJson);
	SetStringRequestContent(FormContent);
	SetBinaryContentType("");
	SetBinaryRequestContent(TArray<uint8>());

	ApplyURL(URL, Result, WorldContextObject, LatentInfo);

}

void UVaRestRequestJSON::ApplyURLForBinaryData(const FString& URL, ERequestVerb Verb, const FString& BinaryContentType, const TArray<uint8>& BinaryContent, UVaRestRequestJSON *&Result, UObject* WorldContextObject, struct FLatentActionInfo LatentInfo)
{
	SetVerb(Verb);
	SetContentType(ERequestContentType::binary);
	SetBinaryContentType(BinaryContentType);
	SetBinaryRequestContent(BinaryContent);
	SetRequestObject(nullptr);
	SetStringRequestContent("");

	ApplyURL(URL, Result, WorldContextObject, LatentInfo);
}
*/

void UVaRestfulRequestJSON::ExecuteProcessRequest()
{
	if (HttpRequest->GetURL().Len() == 0)
	{
		UE_LOG(LogVaRest, Error, TEXT("Request execution attempt with empty URL"));
		return;
	}

	ProcessRequest();
}

void UVaRestfulRequestJSON::ProcessRequest()
{
	if (bIsInProcess || bIsCompleted)
	{
		UE_LOG(LogVaRest, Log, TEXT("%s: Request cannot be reused : %s %s %s"), *VA_FUNC_LINE, *HttpRequest->GetVerb(), *HttpRequest->GetURL(), *StringRequestContent);
		return;
	}

	// Cache default settings for extended logs
	const UVaRestfulSettings* DefaultSettings = GetDefault<UVaRestfulSettings>();

	// Set verb
	switch (RequestVerb)
	{
	case ERequestFulVerb::GET:
		HttpRequest->SetVerb(TEXT("GET"));
		break;

	case ERequestFulVerb::POST:
		HttpRequest->SetVerb(TEXT("POST"));
		break;

	case ERequestFulVerb::PUT:
		HttpRequest->SetVerb(TEXT("PUT"));
		break;

	//case ERequestFulVerb::PATCH:/** Added by HerrSergio */
	//	HttpRequest->SetVerb(TEXT("PATCH"));
	//	break;

	case ERequestFulVerb::DEL:
		HttpRequest->SetVerb(TEXT("DELETE"));
		break;

	case ERequestFulVerb::CUSTOM:
		HttpRequest->SetVerb(CustomVerb);
		break;

	default:
		break;
	}

	// Set content-type
	switch (RequestContentType)
	{
	case ERequestFulContentType::x_www_form_urlencoded_url:
	{
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

		/*FString UrlParams = "";
		uint16 ParamIdx = 0;
		if (RequestJsonObj != nullptr)
		{
			// Loop through all the values and prepare additional url part
			for (auto RequestIt = RequestJsonObj->GetRootObject()->Values.CreateIterator(); RequestIt; ++RequestIt)
			{
				FString Key = RequestIt.Key();
				FString Value = RequestIt.Value().Get()->AsString();

				if (!Key.IsEmpty() && !Value.IsEmpty())
				{
					UrlParams += ParamIdx == 0 ? "?" : "&";
					UrlParams += UVaRestLibrary::PercentEncode(Key) + "=" + UVaRestLibrary::PercentEncode(Value);
				}

				ParamIdx++;
			}
		}*/

		// Apply params
		//HttpRequest->SetURL(HttpRequest->GetURL() + UrlParams);
		/** HerrSergio CORRECTION ? */
		FString UrlParams = URLEncode(RequestJsonObj);
		HttpRequest->SetURL(ConstructURLWithQuery(HttpRequest->GetURL(), UrlParams));

		// Add optional string content
		if (!StringRequestContent.IsEmpty())
		{
			HttpRequest->SetContentAsString(StringRequestContent);
		}

		// Check extended log to avoid security vulnerability (#133)
		if (DefaultSettings->bExtendedLog)
		{
			UE_LOG(LogVaRest, Log, TEXT("%s: Request (urlencoded): %s %s %s %s"), *VA_FUNC_LINE, *HttpRequest->GetVerb(), *HttpRequest->GetURL(), *UrlParams, *StringRequestContent);
		}
		else
		{
			UE_LOG(LogVaRest, Log, TEXT("%s: Request (urlencoded): %s %s (check bExtendedLog for additional data)"), *VA_FUNC_LINE, *HttpRequest->GetVerb(), *HttpRequest->GetURL());
		}

		break;
	}
	case ERequestFulContentType::x_www_form_urlencoded_body:
	{
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

		/*FString UrlParams = "";
		uint16 ParamIdx = 0;
		if (RequestJsonObj != nullptr)
		{
			// Loop through all the values and prepare additional url part
			for (auto RequestIt = RequestJsonObj->GetRootObject()->Values.CreateIterator(); RequestIt; ++RequestIt)
			{
				FString Key = RequestIt.Key();
				FString Value = RequestIt.Value().Get()->AsString();

				if (!Key.IsEmpty() && !Value.IsEmpty())
				{
					UrlParams += ParamIdx == 0 ? "" : "&";
					UrlParams += UVaRestLibrary::PercentEncode(Key) + "=" + UVaRestLibrary::PercentEncode(Value);
				}

				ParamIdx++;
			}
		}
		*/

		// Apply params
		//HttpRequest->SetContentAsString(UrlParams);
		/** HerrSergio CORRECTION ? */
		FString UrlParams = URLEncode(RequestJsonObj);
		HttpRequest->SetContentAsString(UrlParams);

		// Check extended log to avoid security vulnerability (#133)
		if (DefaultSettings->bExtendedLog)
		{
			UE_LOG(LogVaRest, Log, TEXT("%s: Request (url body): %s %s %s"), *VA_FUNC_LINE, *HttpRequest->GetVerb(), *HttpRequest->GetURL(), *UrlParams);
		}
		else
		{
			UE_LOG(LogVaRest, Log, TEXT("%s: Request (url body): %s %s (check bExtendedLog for additional data)"), *VA_FUNC_LINE, *HttpRequest->GetVerb(), *HttpRequest->GetURL());
		}

		break;
	}
	case ERequestFulContentType::binary:
	{
		HttpRequest->SetHeader(TEXT("Content-Type"), BinaryContentType);
		HttpRequest->SetContent(RequestBytes);

		UE_LOG(LogVaRest, Log, TEXT("Request (binary): %s %s"), *HttpRequest->GetVerb(), *HttpRequest->GetURL());

		break;
	}
	case ERequestFulContentType::json:
	{
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

		// Serialize data to json string
		FString OutputString;
		TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);

		/** HerrSergio CORRECTION ? */
		if (RequestJsonObj == nullptr)
		{
			UE_LOG(LogVaRest, Error, TEXT("Request Json Object missing"));
		}
		else if (!FJsonSerializer::Serialize(RequestJsonObj->GetRootObject().ToSharedRef(), Writer))
		{
			UE_LOG(LogVaRest, Error, TEXT("Request Json Object not serializable"));
		}

		// Set Json content
		HttpRequest->SetContentAsString(OutputString);

		UE_LOG(LogVaRest, Log, TEXT("Request (json): %s %s %sJSON(%s%s%s)JSON"), *HttpRequest->GetVerb(), *HttpRequest->GetURL(), LINE_TERMINATOR, LINE_TERMINATOR, *OutputString, LINE_TERMINATOR);

		break;
	}

	default:
		break;
	}

	// Apply additional headers
	for (TMap<FString, FString>::TConstIterator It(RequestHeaders); It; ++It)
	{
		HttpRequest->SetHeader(It.Key(), It.Value());
	}

	// Bind event
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UVaRestfulRequestJSON::OnProcessRequestComplete);

	bIsInProcess = true;

	/** HerrSergio CORRECTION */
	// Execute the request
	if (!HttpRequest->ProcessRequest())
	{
		OnProcessRequestComplete(HttpRequest, FHttpResponsePtr(), false);
	}
}


//////////////////////////////////////////////////////////////////////////
// Request callbacks

void UVaRestfulRequestJSON::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (HttpRequest != Request || !bIsInProcess || bIsCompleted)
	{
		UE_LOG(LogVaRest, Log, TEXT("%s: Request cannot be completed that way : %s %s %s"), *VA_FUNC_LINE, *HttpRequest->GetVerb(), *HttpRequest->GetURL(), *StringRequestContent);
		return;
	}

	if (HttpRequest->OnProcessRequestComplete().IsBoundToObject(this))
	{
		HttpRequest->OnProcessRequestComplete().Unbind();
	}


	// Be sure that we have no data from previous response
	ResetResponseData();

	bIsInProcess = false;
	bIsCompleted = true;

	// Check we have a response and save response code as int32
	if (Response.IsValid())
	{
		ResponseCode = Response->GetResponseCode();
	}

	// Check we have result to process futher
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogVaRest, Error, TEXT("Request failed (%d): %s"), ResponseCode, *Request->GetURL());;

		// Broadcast the result event
		OnRequestFail.Broadcast(this);
		OnStaticRequestFail.Broadcast(this);

		/** HerrSergio CORRECTION */
		// Finish the latent action
		if (ContinueAction)
		{
			FVaRestfulLatentAction<UVaRestfulRequestJSON*> *K = ContinueAction;
			ContinueAction = nullptr;

			K->Call(this);
		}

		return;
	}

#if !(PLATFORM_IOS || PLATFORM_ANDROID)
	// Log response state
	UE_LOG(LogVaRest, Log, TEXT("Response (%d): %sJSON(%s%s%s)JSON"), ResponseCode, LINE_TERMINATOR, LINE_TERMINATOR, *Response->GetContentAsString(), LINE_TERMINATOR);
#endif

	// Process response headers
	TArray<FString> Headers = Response->GetAllHeaders();
	for (FString Header : Headers)
	{
		FString Key;
		FString Value;
		if (Header.Split(TEXT(": "), &Key, &Value))
		{
			ResponseHeaders.Add(Key, Value);
		}
	}

	// Try to deserialize data to JSON
	ResponseData = Response->GetContent();
	ResponseContent = Response->GetContentAsString();

	ResponseSize = ResponseJsonObj->DeserializeFromUTF8Bytes((const ANSICHAR*)ResponseData.GetData(), ResponseData.Num());

	// Decide whether the request was successful
	bIsValidResponse = true;
	bIsValidJsonResponse = ResponseSize > 0;

	ResponseSize = ResponseData.Num();

	/** HerrSergio REFACTOR */
	//if (!bIsValidJsonResponse) WHY WAS THAT THRERE!?
	//{
		// Save response data as a string
		//ResponseSize = ResponseContent.GetAllocatedSize(); 
	//}

	// Log errors
	if (!bIsValidJsonResponse)
	{
		if (!ResponseJsonObj->GetRootObject().IsValid())
		{
			// As we assume it's recommended way to use current class, but not the only one,
			// it will be the warning instead of error
			UE_LOG(LogVaRest, Warning, TEXT("JSON could not be decoded!"));
		}
	}

	// Broadcast the result event
	OnRequestComplete.Broadcast(this);
	OnStaticRequestComplete.Broadcast(this);

	// Finish the latent action
	if (ContinueAction)
	{
		/** HerrSergio CORRECTION */
		FVaRestfulLatentAction<UVaRestfulRequestJSON*> *K = ContinueAction;
		ContinueAction = nullptr;

		/** HerrSergio CORRECTION */
		K->Call(this);
	}
}


//////////////////////////////////////////////////////////////////////////
// Tags

void UVaRestfulRequestJSON::AddTag(FName Tag)
{
	if (Tag != NAME_None)
	{
		Tags.AddUnique(Tag);
	}
}

int32 UVaRestfulRequestJSON::RemoveTag(FName Tag)
{
	return Tags.Remove(Tag);
}

bool UVaRestfulRequestJSON::HasTag(FName Tag) const
{
	return (Tag != NAME_None) && Tags.Contains(Tag);
}


//////////////////////////////////////////////////////////////////////////
// Data

const TArray<uint8>& UVaRestfulRequestJSON::GetResponseContent() const
{
	return ResponseData;
}

FString UVaRestfulRequestJSON::GetResponseContentAsString() const //bool bCacheResponseContent)
{
	
	//bool bRegenerateContent = false; /** HerrSergio REFACTOR */

	// Check we have valid json response
	//if (!bIsValidJsonResponse) WHY WAS THAT THERE!?
	//if(!bIsValidJsonResponse || !bRegenerateContent)
	{
		// We've cached response content in OnProcessRequestComplete()
		return ResponseContent;
	}

	// Check we have valid response object
	/*(if (!ResponseJsonObj || !ResponseJsonObj->IsValidLowLevel())
	{
		// Discard previous cached string if we had one
		ResponseContent = DeprecatedResponseString;

		return TEXT("Invalid response");
	}*/

	// Check if we should re-genetate it in runtime
	/*if (!bCacheResponseContent)
	{
		UE_LOG(LogVaRest, Warning, TEXT("%s: Use of uncashed getter could be slow"), *VA_FUNC_LINE);
		return ResponseJsonObj->EncodeJson();
	}*/

	// Check that we haven't cached content yet
	/*if (ResponseContent == DeprecatedResponseString)
	{
		UE_LOG(LogVaRest, Warning, TEXT("%s: Response content string is cached"), *VA_FUNC_LINE);
		ResponseContent = ResponseJsonObj->EncodeJson();
	}*/

	// Return previously cached content now
	//return ResponseContent;
}
