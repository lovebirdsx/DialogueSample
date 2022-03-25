#include "MovieSceneDialogueTrack.h"
#include "MovieSceneDialogueSection.h"
#include "Compilation/IMovieSceneTemplateGenerator.h"
#include "Evaluation/MovieSceneEvaluationTrack.h"
#include "MovieSceneDialogueTemplate.h"

#define LOCTEXT_NAMESPACE "MovieSceneDialogueTrack"

void UMovieSceneDialogueTrack::PostCompile(FMovieSceneEvaluationTrack& OutTrack,
                                           const FMovieSceneTrackCompilerArgs& Args) const
{
	OutTrack.SetEvaluationPriority(GetEvaluationPriority());
}

FMovieSceneEvalTemplatePtr UMovieSceneDialogueTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FMovieSceneDialogueSectionTemplate(*CastChecked<UMovieSceneDialogueSection>(&InSection));
}

FName UMovieSceneDialogueTrack::GetTrackName() const
{
	static FName MyTrackName = TEXT("Dialog");
	return MyTrackName;
}

bool UMovieSceneDialogueTrack::IsEmpty() const
{
	return Sections.Num() == 0;
}

bool UMovieSceneDialogueTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return SectionClass == UMovieSceneDialogueSection::StaticClass();
}

void UMovieSceneDialogueTrack::AddSection(UMovieSceneSection& Section)
{
	Sections.Add(&Section);
}

void UMovieSceneDialogueTrack::RemoveSection(UMovieSceneSection& Section)
{
	Sections.Remove(&Section);
}

void UMovieSceneDialogueTrack::RemoveAllAnimationData()
{
	Sections.Empty();
}

UMovieSceneSection* UMovieSceneDialogueTrack::CreateNewSection()
{
	return NewObject<UMovieSceneDialogueSection>(this, UMovieSceneDialogueSection::StaticClass(), NAME_None,
	                                             RF_Transactional);
}

const TArray<UMovieSceneSection*>& UMovieSceneDialogueTrack::GetAllSections() const
{
	return Sections;
}

bool UMovieSceneDialogueTrack::HasSection(const UMovieSceneSection& Section) const
{
	return Sections.Contains(&Section);
}

UMovieSceneDialogueSection* UMovieSceneDialogueTrack::GetSectionByID(const int ID)
{
	for (UMovieSceneSection* CurSection : Sections)
	{
		UMovieSceneDialogueSection* DialogueSection = Cast<UMovieSceneDialogueSection>(CurSection);
		if (DialogueSection && DialogueSection->GetUniqueID() == ID)
		{
			return DialogueSection;
		}
	}

	return nullptr;
}

#if WITH_EDITORONLY_DATA
FText UMovieSceneDialogueTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("DisplayName", "Dialogue");
}

FText UMovieSceneDialogueTrack::GetDisplayName() const
{
	static FText MyDisplayName = NSLOCTEXT("DialogueTrack", "TrackName", "Dialog");
	return MyDisplayName;
}
#endif

#undef LOCTEXT_NAMESPACE
