// Copyright 2018 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "VaRestfulSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class VARESTFULPLUGIN_API UVaRestfulSettings : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	/** You can disable request content logging to avoid security vulnerability */
	UPROPERTY(Config, EditAnywhere, Category = "VaRestful")
	bool bExtendedLog;

};
