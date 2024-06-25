#include "GameSystem.h"
#include "../../Engine/LwLib/LwEngineLib.h"

float GameSystem::sSpeedFactor = 1.0f;

float GameSystem::GameSpeedFactor()
{
    if (sSpeedFactor <= 1.0f) {
        sSpeedFactor = 1.0f;
    }
    return (1.0f / sSpeedFactor) * kDeltaTime;
}
