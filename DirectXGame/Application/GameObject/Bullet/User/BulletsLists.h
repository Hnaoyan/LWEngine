#pragma once
#include <variant>

class IBullet;
namespace BossSystemContext
{
	class TrackingBullet;
	class NormalBullet;
}

using namespace BossSystemContext;

using BulletVariant = std::variant<IBullet*, TrackingBullet*, NormalBullet*>;