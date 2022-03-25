#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSection.h"
#include "MovieSceneDialogueSection.generated.h"


UCLASS(MinimalAPI)
class UMovieSceneDialogueSection : public UMovieSceneSection
{
	GENERATED_UCLASS_BODY()
public:
	FText GetDialogueInfo() const { return DialogueInfo; }
	void SetDialogueInfo(const FText& Info) { DialogueInfo = Info; }

	FText GetDialogueSpeakerName() const { return SpeakerName; }
	void SetDialogueSpeakerName(const FText& Name) { SpeakerName = Name; }

private:
	UPROPERTY(EditAnywhere, Category = Dialogue)
	FText DialogueInfo;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	FText SpeakerName;
};
