#pragma once
#include "AbstractBulletFactory.h"
#include "../BulletEnums.h"
#include <memory>

class BulletBuilder;
class IBullet;

/// <summary>
/// 弾の生成クラス（ファクトリークラス
/// </summary>
class BulletFactory : public AbstractBulletFactory
{
public:
	InstancedUnit* CreateUnit(const std::string& name) override;

	std::unique_ptr<IBullet> CreateBullet(const BulletBuilder& builder, BulletType type);

};
