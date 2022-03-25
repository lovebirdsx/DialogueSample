#pragma once

#include "CoreMinimal.h"

class FSequenceDialogueEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	FDelegateHandle DialogueTrackEditorHandle;
};
