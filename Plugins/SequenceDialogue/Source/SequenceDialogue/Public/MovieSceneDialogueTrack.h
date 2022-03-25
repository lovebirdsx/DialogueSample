#pragma once

#include "CoreMinimal.h"
#include "IMovieSceneTrackTemplateProducer.h"
#include "MovieSceneNameableTrack.h"
#include "Tracks/MovieSceneSpawnTrack.h"
#include "Tracks/MovieScenePropertyTrack.h"
#include "MovieSceneDialogueTrack.generated.h"

struct FMovieSceneEvaluationTrack;
class UMovieSceneDialogueSection;

UCLASS(MinimalAPI)
class UMovieSceneDialogueTrack : public UMovieSceneNameableTrack, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()

public:
	static uint16 GetEvaluationPriority() { return UMovieSceneSpawnTrack::GetEvaluationPriority() + 120; }

	/** IMovieSceneTrackTemplateProducer */
	virtual void
	PostCompile(FMovieSceneEvaluationTrack& Track, const FMovieSceneTrackCompilerArgs& Args) const override;
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;

	/** UMovieSceneTrack interface */
	virtual FName GetTrackName() const override;
	virtual bool IsEmpty() const override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
	virtual void AddSection(UMovieSceneSection& Section) override;
	virtual void RemoveSection(UMovieSceneSection& Section) override;
	virtual void RemoveAllAnimationData() override;
	virtual UMovieSceneSection* CreateNewSection() override;
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override;
	virtual bool HasSection(const UMovieSceneSection& Section) const override;
	virtual bool SupportsMultipleRows() const override { return true; }

#if WITH_EDITORONLY_DATA
	virtual FText GetDefaultDisplayName() const override;
	virtual FText GetDisplayName() const override;
#endif

	SEQUENCEDIALOGUE_API UMovieSceneDialogueSection* GetSectionByID(int ID);

private:
	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;
};
