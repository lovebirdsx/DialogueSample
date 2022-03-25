#include "SequenceDialogueEditor.h"
#include "Modules/ModuleManager.h"
#include "ISequencerModule.h"
#include "DialogueTrackEditor.h"

#define LOCTEXT_NAMESPACE "FSequenceDialogueEditorModule"

void FSequenceDialogueEditorModule::StartupModule()
{
	ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>("Sequencer");
	DialogueTrackEditorHandle = SequencerModule.RegisterTrackEditor(
		FOnCreateTrackEditor::CreateStatic(&FDialogueTrackEditor::CreateTrackEditor));
}

void FSequenceDialogueEditorModule::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("Sequencer"))
	{
		return;
	}

	ISequencerModule& SequencerModule = FModuleManager::Get().GetModuleChecked<ISequencerModule>("Sequencer");
	SequencerModule.UnRegisterTrackEditor(DialogueTrackEditorHandle);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSequenceDialogueEditorModule, SequenceDialogueEditor)
