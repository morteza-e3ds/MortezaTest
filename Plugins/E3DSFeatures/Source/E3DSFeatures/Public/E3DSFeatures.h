// Plugin:			EagleFeatures
// Company:			Eagle3DStreaming
// Copyright:       Copyright (C) 2024 Eagle3DStreaming
// Website :		https://www.eagle3dstreaming.com/
// Developer:		Masud Raihan Sagor 

#pragma once

#include "Modules/ModuleManager.h"

class FE3DSFeaturesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
