#include "BulletFactory.h"
#include "../../IBullet.h"
#include "Application/GameObject/Boss/System/Bullet/BossBullet.h"

using namespace BossSystemContext;

IBullet* BulletFactory::CreateBullet(const std::string& name)
{
    IBullet* newBullet = nullptr;

    if (name == "IBullet") {
        newBullet = new IBullet();
    }
    else if (name == "Tracking") {
        newBullet = new TrackingBullet();
    }
    else if (name == "Normal") {
        newBullet = new NormalBullet();
    }

    return newBullet;
}
