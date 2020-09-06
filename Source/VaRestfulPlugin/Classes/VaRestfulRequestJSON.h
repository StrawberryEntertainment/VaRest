// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "Http.h"

#include "VaRestfulTypes.h"
#include "VaRestfulJsonObject.h"
#include "UnktomiFulLatentAction.h"
#include "VaRestfulLibrary.h"
#include "VaRestfulRequestJSON.generated.h"


/** Generate a delegates for callback events */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestFulComplete, class UVaRestfulRequestJSON*, Request);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestFulFail, class UVaRestfulRequestJSON*, Request);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaticRequestFulComplete, class UVaRestfulRequestJSON*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaticRequestFulFail, class UVaRestfulRequestJSON*);


/**
 * General helper class http requests via blueprints
 */
UCLASS(BlueprintType, Blueprintable)
class VARESTFULPLUGIN_API UVaRestfulRequestJSON : public UObject
{
	GENERATED_UCLASS_BODY()

	template <class T>
	friend class FVaRestfulLatentAction;

public:
	//////////////////////////////////////////////////////////////////////////
	// Construction

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (GET)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
	static UVaRestfulRequestJSON* ConstructRequestGet(UObject* WorldContextObject, const FString& Url, UVaRestfulJsonObject* VaRestfulQuery)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(Url);
		Request->SetVerb(ERequestFulVerb::GET);
		Request->SetContentType(ERequestFulContentType::x_www_form_urlencoded_url);
		Request->SetRequestObject(VaRestfulQuery);

		return Request;
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (POST Form)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
	static UVaRestfulRequestJSON* ConstructRequestPostForm(UObject* WorldContextObject, const FString& Url, UVaRestfulJsonObject* VaRestfulBody, UVaRestfulJsonObject* VaRestfulQuery = nullptr)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(ConstructURL(Url, VaRestfulQuery));
		Request->SetVerb(ERequestFulVerb::POST);
		Request->SetContentType(ERequestFulContentType::x_www_form_urlencoded_body);
		Request->SetRequestObject(VaRestfulBody);

		return Request;
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (POST JSON)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
	static UVaRestfulRequestJSON* ConstructRequestPostJson(UObject* WorldContextObject, const FString& Url, UVaRestfulJsonObject* VaRestfulBody, UVaRestfulJsonObject* VaRestfulQuery = nullptr)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(ConstructURL(Url, VaRestfulQuery));
		Request->SetVerb(ERequestFulVerb::POST);
		Request->SetContentType(ERequestFulContentType::json);
		Request->SetRequestObject(VaRestfulBody);

		return Request;
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (PUT Form)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
	static UVaRestfulRequestJSON* ConstructRequestPutForm(UObject* WorldContextObject, const FString& Url, UVaRestfulJsonObject* VaRestfulBody, UVaRestfulJsonObject* VaRestfulQuery = nullptr)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(ConstructURL(Url, VaRestfulQuery));
		Request->SetVerb(ERequestFulVerb::PUT);
		Request->SetContentType(ERequestFulContentType::x_www_form_urlencoded_body);
		Request->SetRequestObject(VaRestfulBody);

		return Request;
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (PUT JSON)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
		static UVaRestfulRequestJSON* ConstructRequestPutJson(UObject* WorldContextObject, const FString& Url, UVaRestfulJsonObject* VaRestfulBody, UVaRestfulJsonObject* VaRestfulQuery = nullptr)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(ConstructURL(Url, VaRestfulQuery));
		Request->SetVerb(ERequestFulVerb::PUT);
		Request->SetContentType(ERequestFulContentType::json);
		Request->SetRequestObject(VaRestfulBody);

		return Request;
	}
	
	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (DELETE)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
	static UVaRestfulRequestJSON* ConstructRequestDelete(UObject* WorldContextObject, const FString& Url, UVaRestfulJsonObject* VaRestfulQuery)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(Url);
		Request->SetVerb(ERequestFulVerb::DEL);
		Request->SetContentType(ERequestFulContentType::x_www_form_urlencoded_url);
		Request->SetRequestObject(VaRestfulQuery);

		return Request;
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (DELETE Form)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
		static UVaRestfulRequestJSON* ConstructRequestDeleteForm(UObject* WorldContextObject, const FString& Url, UVaRestfulJsonObject* VaRestfulBody, UVaRestfulJsonObject* VaRestfulQuery = nullptr)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(ConstructURL(Url, VaRestfulQuery));
		Request->SetVerb(ERequestFulVerb::DEL);
		Request->SetContentType(ERequestFulContentType::x_www_form_urlencoded_body);
		Request->SetRequestObject(VaRestfulBody);

		return Request;
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (DELETE JSON)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
		static UVaRestfulRequestJSON* ConstructRequestDeleteJson(UObject* WorldContextObject, const FString& Url, UVaRestfulJsonObject* VaRestfulBody, UVaRestfulJsonObject* VaRestfulQuery = nullptr)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(ConstructURL(Url, VaRestfulQuery));
		Request->SetVerb(ERequestFulVerb::DEL);
		Request->SetContentType(ERequestFulContentType::json);
		Request->SetRequestObject(VaRestfulBody);

		return Request;
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (Extended)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
	static UVaRestfulRequestJSON* ConstructRequestEx(UObject* WorldContextObject, const FString& Url, ERequestFulVerb Verb, ERequestFulContentType ContentType, UVaRestfulJsonObject* VaRestfulJson)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(Url);
		Request->SetVerb(Verb);
		Request->SetContentType(ContentType);
		Request->SetRequestObject(VaRestfulJson);

		return Request;
	}
	
	/** HerrSergio ADDITION */
	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Json Request (Ultra)", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "VaRestful|Template")
	static UVaRestfulRequestJSON* ConstructRequestUltra(UObject* WorldContextObject, const FString& Url, ERequestFulVerb Verb,
		ERequestFulContentType ContentType, UVaRestfulJsonObject* VaRestfulJson, const FString& StringContent,
		const FString& BinaryContentType, const TArray<uint8>& BinaryContent)
	{
		UVaRestfulRequestJSON* Request = NewObject<UVaRestfulRequestJSON>();

		Request->SetURL(Url);
		Request->SetVerb(Verb);
		Request->SetContentType(ContentType);
		Request->SetRequestObject(VaRestfulJson);
		Request->SetStringRequestContent(StringContent);
		if(BinaryContentType.Len())
			Request->SetBinaryContentType(BinaryContentType);
		Request->SetBinaryRequestContent(BinaryContent);

		return Request;
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct URL with Json Object"), Category = "VaRestful|URL")
	static FString ConstructURL(const FString& Url, UVaRestfulJsonObject* VaRestfulJson)
	{
		return ConstructURLWithQuery(Url, URLEncode(VaRestfulJson));
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct URL with Query String"), Category = "VaRestful|URL")
	static FString ConstructURLWithQuery(const FString& Url, const FString& QueryString)
	{
		if (QueryString.Len() == 0)
			return Url;
		else if (Url.Contains("?"))
			return Url + "&" + QueryString;
		else
			return Url + "?" + QueryString;
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Construct URL with Map"), Category = "VaRestful|URL")
		static FString ConstructURLByMap(const FString& Url, const TMap<FString, FString>& QueryMap)
	{
		return ConstructURLWithQuery(Url, URLEncodeMap(QueryMap));
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "URL Encode String"), Category = "VaRestful|URL")
	static FString URLEncodeString(const FString& String)
	{
		return UVaRestfulLibrary::PercentEncode(String);
	}

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "URL Encode Json Object"), Category = "VaRestful|URL")
	static FString URLEncode(UVaRestfulJsonObject* VaRestfulJson);

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "URL Encode Map"), Category = "VaRestful|URL")
	static FString URLEncodeMap(const TMap<FString, FString>& QueryMap);

	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Use URL with Json Request"), Category = "VaRestful|URL")
	UVaRestfulRequestJSON* UseURL(const FString& Url)
	{
		SetURL(Url);
		return this;
	}

	//-----------------------------

	/** Set verb to the request */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
	void SetVerb(ERequestFulVerb Verb);

	/** Set custom verb to the request */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
	void SetCustomVerb(FString Verb);

	/** Set content type to the request. If you're using the x-www-form-urlencoded, 
	 * params/constaints should be defined as key=ValueString pairs from Json data */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
	void SetContentType(ERequestFulContentType ContentType);

	/** Set content type of the request for binary post data */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
	void SetBinaryContentType(const FString &ContentType);

	/** Set content of the request for binary post data */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
	void SetBinaryRequestContent(const TArray<uint8> &Content);

	/** Set content of the request as a plain string */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
	void SetStringRequestContent(const FString &Content);

	/** Sets optional header info */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
	void SetHeader(const FString& HeaderName, const FString& HeaderValue);


	//////////////////////////////////////////////////////////////////////////
	// Destruction and reset

	/** Reset all internal saved data */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility")
	void ResetData();

protected:

	/** Reset saved request data */
	//UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
	void ResetRequestData();

	/** Reset saved response data */
	//UFUNCTION(BlueprintCallable, Category = "VaRestful|Response")
	void ResetResponseData();


	/** Set the Response Json object */
	//UFUNCTION(BlueprintCallable, Category = "VaRestful|Response")
	void SetResponseObject(UVaRestfulJsonObject* JsonObject);

public:

	/** Set the Request Json object */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
	void SetRequestObject(UVaRestfulJsonObject* JsonObject);

	/** Cancel latent response waiting  */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Response")
	void Cancel();



	//////////////////////////////////////////////////////////////////////////
	// JSON data accessors

	/** Get the Request Json object */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VaRestful|Request")
	UVaRestfulJsonObject* GetRequestObject();

	/** Get the Response Json object */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VaRestful|Response")
	UVaRestfulJsonObject* GetResponseObject();


	///////////////////////////////////////////////////////////////////////////
	// Request/response data access

	/** Get url of http request */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VaRestful|Request")
	FString GetURL();

	/** Get status of http request */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VaRestful|Request")
	ERequestFulStatus GetStatus();

	/** Get the response code of the last query */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VaRestful|Response")
	int32 GetResponseCode();

	/** Get value of desired response header */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VaRestful|Response")
	FString GetResponseHeader(const FString HeaderName);
	
	/** Get list of all response headers */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VaRestful|Response")
	TArray<FString> GetAllResponseHeaders();


	//////////////////////////////////////////////////////////////////////////
	// URL processing

public:
	/** Setting request URL */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request")
		void SetURL(const FString& Url);// = TEXT("http://alyamkin.com"));

	/** Open URL with current setup */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request", meta = (DisplayName = "Process URL"))
		virtual void ProcessURL(const FString& Url);// = TEXT("http://alyamkin.com"));
	   


	/** HerrSergio ADDITION */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request", meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	virtual void Apply(UVaRestfulRequestJSON *&Result, UObject* WorldContextObject, struct FLatentActionInfo LatentInfo);
	
	/** Open URL in latent mode */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request", meta = (Latent, LatentInfo = "LatentInfo", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	virtual void ApplyURL(const FString& Url, UVaRestfulRequestJSON *&Result, UObject* WorldContextObject, struct FLatentActionInfo LatentInfo);



	/** Check URL and execute process request */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request", meta = (DisplayName = "Process Request"))
	virtual void ExecuteProcessRequest(); 

protected:
	/** Apply current internal setup to request and process it */
	void ProcessRequest();

	//////////////////////////////////////////////////////////////////////////
	// Request callbacks

private:
	/** Internal bind function for the IHTTPRequest::OnProcessRequestCompleted() event */
	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	/** Event occured when the request has been completed */
	UPROPERTY(BlueprintAssignable, Category = "VaRestful|Event")
	FOnRequestFulComplete OnRequestComplete;

	/** Event occured when the request wasn't successfull */
	UPROPERTY(BlueprintAssignable, Category = "VaRestful|Event")
	FOnRequestFulFail OnRequestFail;
	
	/** Event occured when the request has been completed */
	FOnStaticRequestFulComplete OnStaticRequestComplete;
	
	/** Event occured when the request wasn't successfull */
	FOnStaticRequestFulFail OnStaticRequestFail;
	

	//////////////////////////////////////////////////////////////////////////
	// Tags

public:
	/** Add tag to this request */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility")
	void AddTag(FName Tag);

	/** 
	 * Remove tag from this request 
	 *
	 * @return Number of removed elements 
	 */
	UFUNCTION(BlueprintCallable, Category = "VaRestful|Utility")
	int32 RemoveTag(FName Tag);

	/** See if this request contains the supplied tag */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VaRestful|Utility")
	bool HasTag(FName Tag) const;

protected:
	/** Array of tags that can be used for grouping and categorizing */
	TArray<FName> Tags;


	//////////////////////////////////////////////////////////////////////////
	// Data

public:
	/**
	 * Get request response stored as a string 
	 * @param bCacheResponseContent - Set true if you plan to use it few times to prevent deserialization each time
	 */
	UFUNCTION(BlueprintPure, Category = "VaRestful|Response")
		FString GetResponseContentAsString() const;// bool bCacheResponseContent = true);

	UFUNCTION(BlueprintPure, Category = "VaRestful|Response")
		const TArray<uint8>& GetResponseContent() const;

protected:
	/** Response size */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VaRestful|Response")
	int32 ResponseSize;
	
	/** DEPRECATED: Please use GetResponseContentAsString() instead */
	UPROPERTY()//VisibleAnywhere, BlueprintReadOnly, Category = "VaRestful|Response")
	FString ResponseContent;

	UPROPERTY()//VisibleAnywhere, BlueprintReadOnly, Category = "VaRestful|Response")
	TArray<uint8> ResponseData;

	/** Is the response valid JSON? */
	UPROPERTY()//VisibleAnywhere, BlueprintReadOnly, Category = "VaRestful|Response")
	uint8 bIsValidJsonResponse : 1;
	
	UPROPERTY()//VisibleAnywhere, BlueprintReadOnly, Category = "VaRestful|Response")
	uint8 bIsValidResponse : 1;

	UPROPERTY()//VisibleAnywhere, BlueprintReadOnly, Category = "VaRestful|Response")
	uint8 bIsInProcess : 1;

	UPROPERTY()//VisibleAnywhere, BlueprintReadOnly, Category = "VaRestful|Response")
	uint8 bIsCompleted : 1;

	UFUNCTION(BlueprintPure, Category = "VaRestful|Response")
	bool IsValidJson() const
	{
		return bIsValidJsonResponse;
	}

	UFUNCTION(BlueprintPure, Category = "VaRestful|Response")
	bool IsValid() const
	{
		return bIsValidResponse;
	}	
	
	UFUNCTION(BlueprintPure, Category = "VaRestful|Response")
	bool IsInProcess() const
	{
		return bIsInProcess;
	}

	UFUNCTION(BlueprintPure, Category = "VaRestful|Response")
	bool IsCompleted() const
	{
		return bIsCompleted;
	}

protected:
	/** Default value for deprecated ResponseContent variable */
	//static FString DeprecatedResponseString;

protected:

	/** Latent action helper */
	FVaRestfulLatentAction<UVaRestfulRequestJSON*>* ContinueAction;

	/** Internal request data stored as JSON */
	UPROPERTY()
	UVaRestfulJsonObject* RequestJsonObj;

	TArray<uint8> RequestBytes;
	FString BinaryContentType;

	/** Used for special cases when used wants to have plain string data in request. 
	 * Attn.! Content-type x-www-form-urlencoded only. */
	FString StringRequestContent;

	/** Response data stored as JSON */
	UPROPERTY()
	UVaRestfulJsonObject* ResponseJsonObj;

	/** Verb for making request (GET,POST,etc) */
	ERequestFulVerb RequestVerb;

	/** Content type to be applied for request */
	ERequestFulContentType RequestContentType;

	/** Mapping of header section to values. Used to generate final header string for request */
	TMap<FString, FString> RequestHeaders;

	/** Cached key/value header pairs. Parsed once request completes */
	TMap<FString, FString> ResponseHeaders;

	/** Http Response code */
	int32 ResponseCode;

	/** Custom verb that will be used with RequestContentType == CUSTOM */
	FString CustomVerb;

	/** Request we're currently processing */
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	
public:
	/** Returns reference to internal request object */
	TSharedRef<IHttpRequest> GetHttpRequest() const { return HttpRequest; };
	
};

template <class T> 
void FVaRestfulLatentAction<T>::Cancel() 
	{
		UObject *Obj = this->Request.Get();
		if (Obj != nullptr)
		{
			((UVaRestfulRequestJSON*)Obj)->ContinueAction = nullptr;
			((UVaRestfulRequestJSON*)Obj)->Cancel();
		}
	}

template <class T> 
	template<class TUnktomiLatentAction>
	 TUnktomiLatentAction * FUnktomiFulLatentAction<T>::RegisterLatentAction(FWeakObjectPtr RequestObj, T& Result, UObject* WorldContextObject, const FLatentActionInfo& LatentInfo)
	{
		// Prepare latent action
		if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
		{
			FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
			TUnktomiLatentAction *Kont = LatentActionManager.FindExistingAction<TUnktomiLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

			if (Kont != nullptr)
			{
				Kont->Cancel();
				LatentActionManager.RemoveActionsForObject(LatentInfo.CallbackTarget);
			}

			Kont = new FVaRestfulLatentAction<T>(RequestObj, Result, LatentInfo);
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, Kont);
			return Kont;
		}

		return nullptr;
	}
