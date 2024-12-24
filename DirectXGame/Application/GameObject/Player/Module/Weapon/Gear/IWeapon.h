#pragma once
#include "Engine/3D/WorldTransform.h"

class IGameObject;

class IWeapon
{
public:
	IWeapon() {};
	virtual ~IWeapon() = default;

	virtual void Initialize(IGameObject* object);
	virtual void Update();

public:
	WorldTransform worldTransform_;

};
