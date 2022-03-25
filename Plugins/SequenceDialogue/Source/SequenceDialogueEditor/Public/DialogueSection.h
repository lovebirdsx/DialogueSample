#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "ISequencerSection.h"

class FSequencerSectionPainter;
class ISectionLayoutBuilder;
class UMovieSceneDialogueSection;

class FDialogueSection
	: public ISequencerSection
	  , public TSharedFromThis<FDialogueSection>
{
public:
	explicit FDialogueSection(UMovieSceneDialogueSection& InSectionObject);

	// ISequencerSectionInterface
	virtual UMovieSceneSection* GetSectionObject() override;
	virtual TSharedRef<SWidget> GenerateSectionWidget() override;
	virtual int32 OnPaintSection(FSequencerSectionPainter& InPainter) const override;
	virtual FText GetSectionTitle() const override;
	// virtual void GenerateSectionLayout( ISectionLayoutBuilder& LayoutBuilder ) const override { }

private:
	FSlateColor GetBackgroundColor() const;
	FText GetVisibilityText() const;
	FText GetVisibilityToolTip() const;

	bool OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation);
	FReply OnDrop(TSharedPtr<FDragDropOperation> DragDropOperation);

	UMovieSceneDialogueSection& SectionObject;
	FText DisplayName;
};
