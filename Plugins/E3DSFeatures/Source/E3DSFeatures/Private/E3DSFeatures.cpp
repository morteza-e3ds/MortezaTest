// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 

#include "..\Public\E3DSFeatures.h"
#include "E3DSFeaturesDefines.h"

#define LOCTEXT_NAMESPACE "FE3DSBlueprintsModule"

void FE3DSFeaturesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FE3DSFeaturesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FE3DSFeaturesModule, E3DSFeatures)

DEFINE_LOG_CATEGORY(LogE3DSFeatures);

#undef LOCTEXT_NAMESPACE