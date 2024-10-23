// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PDA/PDA_StateDatas.h"

float UPDA_StateDatas::GetFloatVariable(FString variable) const
{
	if(FloatVariables.Find(variable) != NULL)
	{
		return FloatVariables.FindRef(variable);
	}else
	{
		return 0.0f;
	}
}

bool UPDA_StateDatas::GetBooleanVariable(FString variable) const
{
	if(BooleanVariables.Find(variable) != NULL)
	{
		return BooleanVariables.FindRef(variable);
	}else
	{
		return false;
	}
}
