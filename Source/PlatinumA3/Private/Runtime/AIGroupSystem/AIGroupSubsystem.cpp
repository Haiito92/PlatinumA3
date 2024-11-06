#include "Runtime/AIGroupSystem/AIGroupSubsystem.h"

#include "Runtime/AIGroupSystem/AIBehaviour.h"
#include "Runtime/AIGroupSystem/AIGroupSubsystemSettings.h"

void UAIGroupSubsystem::InitSubsystem()
{
	const UAIGroupSubsystemSettings* Settings = GetDefault<UAIGroupSubsystemSettings>();

	if(Settings == nullptr) return;

	for (TSubclassOf<UAIBehaviour> BehaviorClass : Settings->BehaviorsClasses)
	{
		UAIBehaviour* Behaviour = NewObject<UAIBehaviour>(this, BehaviorClass);

		if(Behaviour == nullptr) continue;
		Behaviours.Add(Behaviour);
	}
}
