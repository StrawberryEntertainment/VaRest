#pragma once

#include "Engine/LatentActionManager.h"
#include "LatentActions.h"
#include "VaRestfulRequestJSON.h"
#include "UObject/WeakObjectPtr.h"
/**
 * @author Original latent action class by https://github.com/unktomi
 */
template <class T> 
class FUnktomiFulLatentAction : public FPendingLatentAction
{
public:
	virtual void Call(const T &Value)
	{
		Result = Value;
		Called = true;
	}

	void operator()(const T &Value)
	{
		Call(Value);
	}

	virtual void Cancel()
	{

	}

	FUnktomiFulLatentAction(FWeakObjectPtr RequestObj, T& ResultParam, const FLatentActionInfo& LatentInfo) :
		Called(false),
		Request(RequestObj),
		ExecutionFunction(LatentInfo.ExecutionFunction),
		OutputLink(LatentInfo.Linkage),
		CallbackTarget(LatentInfo.CallbackTarget),
		Result(ResultParam)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Response.FinishAndTriggerIf(Called, ExecutionFunction, OutputLink, CallbackTarget);
	}

	virtual void NotifyObjectDestroyed()
	{
		Cancel();
	}

	virtual void NotifyActionAborted()
	{
		Cancel();
	}

protected:
	bool Called;
	FWeakObjectPtr Request;

public:
	const FName ExecutionFunction;
	const int32 OutputLink;
	const FWeakObjectPtr CallbackTarget;
	T& Result;

	template<class TUnktomiLatentAction = FUnktomiFulLatentAction<T>>
	static TUnktomiLatentAction * RegisterLatentAction(FWeakObjectPtr RequestObj, T& Result, UObject* WorldContextObject, const FLatentActionInfo& LatentInfo);
};

template <class T> 
class FVaRestfulLatentAction : public FUnktomiFulLatentAction<T>
{

public:

	using FUnktomiFulLatentAction<T>::FUnktomiFulLatentAction;

	virtual void Cancel() override;
};
