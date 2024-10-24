// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/ArenaPlayerRespawn.h"


float AArenaPlayerRespawn::GetStartOrientX() const
{
	switch (StartOrient)
	{
	case EArenaPlayerStartOrient::Left: return -1.f;
	case EArenaPlayerStartOrient::Right: return 1.f;
	}

	return 0.f;
}
