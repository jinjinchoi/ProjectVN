// Copyright © 2025 Choi-JinWoo. All Rights Reserved.


#include "Core/GameInstance/MiniGameGameInstanceSubsystem.h"

#include "Manager/Pooling/VNObjectPoolManager.h"

void UMiniGameGameInstanceSubsystem::InitSubsystem()
{
	check(ObjectPoolManagerClass);

	if (!ObjectPoolManager)
	{
		ObjectPoolManager = NewObject<UVNObjectPoolManager>(this, ObjectPoolManagerClass);
	}
	
	ObjectPoolManager->Init();
}

void UMiniGameGameInstanceSubsystem::ClearPoolManager() const
{
	if (ObjectPoolManager)
	{
		ObjectPoolManager->ClearPool();
	}
}
