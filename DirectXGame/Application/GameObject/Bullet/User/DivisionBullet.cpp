#include "DivisionBullet.h"
#include "Application/Collision/ColliderFilter.h"

void DivisionBullet::Initialize()
{
	// 基底の初期化・コライダーのマスク設定
	IBullet::Initialize();
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

}

void DivisionBullet::Update()
{
	// 破裂するタイミング


}

void DivisionBullet::OnCollision(ColliderObject object)
{
	object;
}
