#include "MovieSceneDialogueTemplate.h"
#include "Engine/World.h"
#include "MovieSceneEvaluation.h"
#include "IMovieScenePlayer.h"
#include "DialogueInterface.h"
#include "GameFramework/GameModeBase.h"

#if WITH_EDITOR
#include "LevelEditor.h"
#include "ILevelViewport.h"
#include "Editor.h"
#include "SceneViewport.h"
#include "EditorStyleSet.h"

class SDialogueInfoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueInfoWidget)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		SetVisibility(EVisibility::SelfHitTestInvisible);

		ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .FillWidth(1)
			  .HAlign(HAlign_Center)
			  .VAlign(VAlign_Bottom)
			  .Padding(4.f, 4.f, 4.f, 150.f)
			[
				SNew(SBorder)
				.Content()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					  .AutoWidth()
					  .HAlign(HAlign_Center)
					  .VAlign(VAlign_Bottom)
					[
						SNew(STextBlock)
							.Visibility(EVisibility::Visible)
						    .TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
							.Text(this, &SDialogueInfoWidget::GetSpeakerName)
					]

					+ SHorizontalBox::Slot()
					  .AutoWidth()
					  .HAlign(HAlign_Center)
					  .VAlign(VAlign_Bottom)
					[
						SNew(STextBlock)
							.Visibility(EVisibility::Visible)
						    .TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
							.Text(this, &SDialogueInfoWidget::GetDialogueInfo)
					]
				]
			]
		];
	}

	FText GetSpeakerName() const
	{
		const FString Ret = SpeakerName.ToString() + TEXT(": ");
		return FText::FromString(Ret);
	}

	FText GetDialogueInfo() const
	{
		return DialogueInfo;
	}

	virtual bool SupportsKeyboardFocus() const override
	{
		return false;
	}

	FText DialogueInfo;
	FText SpeakerName;
};
#endif

struct FDialogueSharedTrackData : IPersistentEvaluationData
{
	FDialogueSharedTrackData(): bNeedExecute(false), bShow(false)
	{
	}

#if WITH_EDITOR
	void AddOrUpdateEditorPreview()
	{
		if (!DialogueWidget.IsValid())
		{
			const TIndirectArray<FWorldContext>& WorldContexts = GEditor->GetWorldContexts();
			if (WorldContexts.Num() > 0)
			{
				UEditorEngine* EditorEngine = CastChecked<UEditorEngine>(GEngine);
				FSceneViewport* SceneViewport = static_cast<FSceneViewport*>(EditorEngine->GetActiveViewport());

				if (SceneViewport && SceneViewport->GetViewportWidget().IsValid())
				{
					DialogueWidget = SNew(SDialogueInfoWidget);
					ViewportWidget = SceneViewport->GetViewportWidget().Pin()->GetContent();
					SWidget* Widget = ViewportWidget.Pin().Get();
					SOverlay* Overlay = static_cast<SOverlay*>(Widget);
					Overlay->AddSlot()
					[
						DialogueWidget.ToSharedRef()
					];
				}
			}
		}

		DialogueWidget.Get()->SpeakerName = SpeakerName;
		DialogueWidget.Get()->DialogueInfo = DialogueInfo;
	}

	void RemoveEditorPreview()
	{
		SWidget* Widget = ViewportWidget.Pin().Get();
		SOverlay* Overlay = static_cast<SOverlay*>(Widget);

		if (Overlay && DialogueWidget.IsValid())
		{
			Overlay->RemoveSlot(DialogueWidget.ToSharedRef());
		}

		Widget = nullptr;
		DialogueWidget = nullptr;
	}
#endif

	virtual ~FDialogueSharedTrackData() override
	{
#if WITH_EDITOR
		RemoveEditorPreview();
#endif
	}

	bool HasAnythingToDo() const
	{
		return bNeedExecute;
	}

	void SetInfo(const bool bNeedShow, const FText& Info, const FText& Name)
	{
		bShow = bNeedShow;
		DialogueInfo = Info;
		SpeakerName = Name;

		bNeedExecute = true;
	}

	bool IsInEditorAndNotPlaying(const IMovieScenePlayer& Player)
	{
#if WITH_EDITOR
		const UWorld* CurWorld = Player.GetPlaybackContext()->GetWorld();
		return GIsEditor && !CurWorld->IsPlayInEditor();
#else
		return false;
#endif
	}

	void ApplyForEditor()
	{
#if WITH_EDITOR
		if (bShow)
			AddOrUpdateEditorPreview();
		else
			RemoveEditorPreview();
#endif
	}

	void ApplyForRuntime(const IMovieScenePlayer& Player)
	{
		const UWorld* CurWorld = Player.GetPlaybackContext()->GetWorld();
		AGameModeBase* CurGameMode = CurWorld->GetAuthGameMode();
		const IDialogueInterface* RawInterface = Cast<IDialogueInterface>(static_cast<UObject*>(CurGameMode));
		const bool bHasInterface = RawInterface || (CurGameMode && CurGameMode->GetClass()->ImplementsInterface(
			UDialogueInterface::StaticClass()));

		if (!bHasInterface)
			return;

		if (RawInterface)
			RawInterface->ShowDialogueInfo(bShow, SpeakerName, DialogueInfo);
		else
			IDialogueInterface::Execute_ShowDialogueInfo(CurGameMode, bShow, SpeakerName, DialogueInfo);
	}

	void Apply(const IMovieScenePlayer& Player)
	{
		bNeedExecute = false;

		if (!Player.GetPlaybackContext())
			return;

		if (IsInEditorAndNotPlaying(Player))
			ApplyForEditor();
		else
			ApplyForRuntime(Player);
	}

private:
	bool bNeedExecute;
	bool bShow;

	FText DialogueInfo;
	FText SpeakerName;

#if WITH_EDITOR
	TWeakPtr<SWidget> ViewportWidget;
	TSharedPtr<SDialogueInfoWidget> DialogueWidget;
#endif
};


struct FDialogueExecutionToken : IMovieSceneSharedExecutionToken
{
	virtual void Execute(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	{
		FDialogueSharedTrackData* TrackData = PersistentData.Find<FDialogueSharedTrackData>(
			FMovieSceneDialogueSharedTrack::GetSharedDataKey());
		if (TrackData)
			TrackData->Apply(Player);
	}
};

FMovieSceneDialogueSectionTemplate::FMovieSceneDialogueSectionTemplate(const UMovieSceneDialogueSection& Section)
	: DialogueInfo(Section.GetDialogueInfo()), SpeakerName(Section.GetDialogueSpeakerName())
{
}

void FMovieSceneDialogueSectionTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand,
                                                  const FMovieSceneContext& Context,
                                                  const FPersistentEvaluationData& PersistentData,
                                                  FMovieSceneExecutionTokens& ExecutionTokens) const
{
	const FMovieSceneSharedDataId SharedDataId = FMovieSceneDialogueSharedTrack::GetSharedDataKey().UniqueId;
	const FDialogueSharedTrackData* TrackData = PersistentData.Find<FDialogueSharedTrackData>(
		FMovieSceneDialogueSharedTrack::GetSharedDataKey());
	if (TrackData && TrackData->HasAnythingToDo() && !ExecutionTokens.FindShared(SharedDataId))
	{
		ExecutionTokens.AddShared(SharedDataId, FDialogueExecutionToken());
	}
}

void FMovieSceneDialogueSectionTemplate::Setup(FPersistentEvaluationData& PersistentData,
                                               IMovieScenePlayer& Player) const
{
	FDialogueSharedTrackData& TrackData = PersistentData.GetOrAdd<FDialogueSharedTrackData>(
		FMovieSceneDialogueSharedTrack::GetSharedDataKey());
	TrackData.SetInfo(true, DialogueInfo, SpeakerName);
}

void FMovieSceneDialogueSectionTemplate::TearDown(FPersistentEvaluationData& PersistentData,
                                                  IMovieScenePlayer& Player) const
{
	FDialogueSharedTrackData& TrackData = PersistentData.GetOrAdd<FDialogueSharedTrackData>(
		FMovieSceneDialogueSharedTrack::GetSharedDataKey());
	TrackData.SetInfo(false, DialogueInfo, SpeakerName);
	TrackData.Apply(Player);
}

FSharedPersistentDataKey FMovieSceneDialogueSharedTrack::GetSharedDataKey()
{
	static FMovieSceneSharedDataId DataId(FMovieSceneSharedDataId::Allocate());
	return FSharedPersistentDataKey(DataId, FMovieSceneEvaluationOperand());
}

void FMovieSceneDialogueSharedTrack::TearDown(FPersistentEvaluationData& PersistentData,
                                              IMovieScenePlayer& Player) const
{
}

void FMovieSceneDialogueSharedTrack::Evaluate(const FMovieSceneEvaluationOperand& Operand,
                                              const FMovieSceneContext& Context,
                                              const FPersistentEvaluationData& PersistentData,
                                              FMovieSceneExecutionTokens& ExecutionTokens) const
{
}
