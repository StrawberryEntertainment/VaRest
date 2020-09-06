// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "VaRestfulPlugin.h"
#include "VaRestfulSettings.h"
#include "VaRestfulJsonObject.h"
#include "VaRestfulJsonValue.h"
#include "VaRestfulRequestJSON.h"
#include "VaRestfulPluginPrivatePCH.h"

//#include "UObject/Package.h"
//#include "Misc/ConfigCacheIni.h"

#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "VaRestful"

class FVaRestfulPlugin : public IVaRestfulPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		// @HACK Force classes to be compiled on shipping build
		UVaRestfulJsonObject::StaticClass();
		UVaRestfulJsonValue::StaticClass();
		UVaRestfulRequestJSON::StaticClass();

		// Register settings
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			/*SettingsModule->RegisterSettings("Project", "Plugins", "VaRestful",
				LOCTEXT("RuntimeSettingsName", "VaRestful Kit"),
				LOCTEXT("RuntimeSettingsDescription", "Configure API keys for VaRestful"),
				GetMutableDefault<UVaRestfulSettings>()
			);*/
		}
	}

	virtual void ShutdownModule() override
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			//SettingsModule->UnregisterSettings("Project", "Plugins", "VaRestful");
		}
	}
};

IMPLEMENT_MODULE( FVaRestfulPlugin, VaRestfulPlugin )

DEFINE_LOG_CATEGORY(LogVaRest);

#undef LOCTEXT_NAMESPACE
