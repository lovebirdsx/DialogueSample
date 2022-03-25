#pragma once

#include "CoreMinimal.h"

class FSequenceDialogueModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};