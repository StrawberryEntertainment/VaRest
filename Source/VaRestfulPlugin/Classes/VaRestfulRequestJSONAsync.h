// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "VaRestfulRequestJSON.h"
#include "VaRestfulRequestJSONAsync.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class VARESTFULPLUGIN_API UVaRestfulRequestJSONAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY()
	UVaRestfulRequestJSON *VaRestfulRequestJSON;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestEnded, UVaRestfulRequestJSON*, Request, bool, bSuccess);

	UPROPERTY(BlueprintAssignable)
	FOnRequestEnded OnRequestEnded;

	FDelegateHandle OnStaticRequestComplete_Handle, OnStaticRequestFail_Handle;

public:

	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request", meta = (BlueprintInternalUseOnly = "true"))
	static UVaRestfulRequestJSONAsync * Call(UVaRestfulRequestJSON *RestRequestJSON)
	{
		UVaRestfulRequestJSONAsync * AsyncVaRestfulRequestJSON = NewObject<UVaRestfulRequestJSONAsync>(RestRequestJSON);
		AsyncVaRestfulRequestJSON->VaRestfulRequestJSON = RestRequestJSON;

		if (RestRequestJSON)
		{
			AsyncVaRestfulRequestJSON->OnStaticRequestComplete_Handle = RestRequestJSON->OnStaticRequestComplete.AddUObject(AsyncVaRestfulRequestJSON, &UVaRestfulRequestJSONAsync::OnRequestCompleted);
			AsyncVaRestfulRequestJSON->OnStaticRequestFail_Handle = RestRequestJSON->OnStaticRequestFail.AddUObject(AsyncVaRestfulRequestJSON, &UVaRestfulRequestJSONAsync::OnRequestFailed);
		}

		return AsyncVaRestfulRequestJSON;
	}  

	UFUNCTION(BlueprintCallable, Category = "VaRestful|Request", meta = (BlueprintInternalUseOnly = "true"))
	static UVaRestfulRequestJSONAsync * CallURL(UVaRestfulRequestJSON *RestRequestJSON, const FString& Url)
	{
		UVaRestfulRequestJSONAsync * AsyncVaRestfulRequestJSON = Call(RestRequestJSON);
		if (RestRequestJSON)
		{
			RestRequestJSON->SetURL(Url);
		}

		return AsyncVaRestfulRequestJSON;
	}

protected:

	UFUNCTION()
	void OnRequestCompleted(UVaRestfulRequestJSON *RestRequestJSON)
	{
		OnRequestCompletedOrFailed(RestRequestJSON, true);
	}
	
	UFUNCTION()
	void OnRequestFailed(UVaRestfulRequestJSON *RestRequestJSON)
	{
		OnRequestCompletedOrFailed(RestRequestJSON, false);
	}

	UFUNCTION()
	void OnRequestCompletedOrFailed(UVaRestfulRequestJSON *RestRequestJSON, bool bSuccess)
	{
		if (RestRequestJSON && this->VaRestfulRequestJSON == RestRequestJSON)
		{
			RestRequestJSON->OnStaticRequestComplete.Remove(OnStaticRequestComplete_Handle);
			RestRequestJSON->OnStaticRequestComplete.Remove(OnStaticRequestFail_Handle);

			OnRequestEnded.Broadcast(RestRequestJSON, bSuccess);

			SetReadyToDestroy();
		}
	}

	virtual void Activate() override
	{
		if (VaRestfulRequestJSON)
		{
			Super::Activate();
			VaRestfulRequestJSON->ExecuteProcessRequest();
		}
		else
		{
			SetReadyToDestroy();
		}
	}
};
