#pragma once
#include <variant>

#include "IPlayerState.h"
#include "IdleState.h"
#include "JumpingState.h"
#include "MovingState.h"
#include "FallingState.h"
#include "BoostState.h"

using PlayerState = std::variant<IPlayerState*, IdleState*, MovingState*, BoostState*, JumpingState*, FallingState*>;
