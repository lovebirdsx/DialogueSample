#pragma once

#include "CoreMinimal.h"
#include "Evaluation/PersistentEvaluationData.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "MovieSceneDialogueSection.h"
#include "MovieSceneDialogueTemplate.generated.h"

USTRUCT()
struct FMovieSceneDialogueSectionTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()

	FMovieSceneDialogueSectionTemplate()
	{
	}

	explicit FMovieSceneDialogueSectionTemplate(const UMovieSceneDialogueSection& Section);

private:
	virtual UScriptStruct& GetScriptStructImpl() const override
	{
		return *StaticStruct();
	}

	virtual void SetupOverrides() override
	{
		EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag);
	}

	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context,
	                      const FPersistentEvaluationData& PersistentData,
	                      FMovieSceneExecutionTokens& ExecutionTokens) const override;
	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;

	UPROPERTY()
	FText DialogueInfo;

	UPROPERTY()
	FText SpeakerName;
};

USTRUCT()
struct FMovieSceneDialogueSharedTrack : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()

	static FSharedPersistentDataKey GetSharedDataKey();

private:
	virtual UScriptStruct& GetScriptStructImpl() const override
	{
		return *StaticStruct();
	}

	virtual void SetupOverrides() override
	{
		EnableOverrides(RequiresTearDownFlag);
	}

	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;

	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context,
	                      const FPersistentEvaluationData& PersistentData,
	                      FMovieSceneExecutionTokens& ExecutionTokens) const override;
};
