// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/ArenaPlayerStart.h"



float AArenaPlayerStart::GetStartOrientX() const
{
	switch (StartOrient)
	{
	case EArenaPlayerStartOrient::Left: return -1.f;
	case EArenaPlayerStartOrient::Right: return 1.f;
	}

	return 0.f;
}
