#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "ISequencer.h"
#include "MovieSceneTrack.h"
#include "ISequencerSection.h"
#include "ISequencerTrackEditor.h"
#include "MovieSceneTrackEditor.h"

class FMenuBuilder;

class FDialogueTrackEditor
	: public FMovieSceneTrackEditor
{
public:
	explicit FDialogueTrackEditor(TSharedRef<ISequencer> InSequencer);

	virtual ~FDialogueTrackEditor() override
	{
	}

	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);

public:
	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject,
	                                                           UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	virtual const FSlateBrush* GetIconBrush() const override;
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;
	virtual TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track,
	                                                    const FBuildEditWidgetParams& Params) override;

	virtual void BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track) override;

private:
	void AddNewSection(const UMovieScene* MovieScene, UMovieSceneTrack* DialogueTrack);
	void OnAddTrack();

	TSharedRef<SWidget> BuildAddVisibilityTriggerMenu(UMovieSceneTrack* DialogueTrack);

	void OnAddNewSection(UMovieSceneTrack* LevelVisibilityTrack);
};
