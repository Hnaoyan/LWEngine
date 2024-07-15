#pragma once
#include <variant>

#include "IPlayerState.h"
#include "IdleState.h"
#include "Vertical/JumpingState.h"
#include "Vertical/FallingState.h"

#include "Horizontal/MovingState.h"
#include "Horizontal/BoostState.h"

using PlayerState = std::variant<IPlayerState*, IdleState*, MovingState*, BoostState*, JumpingState*, FallingState*>;
