#pragma once
#include <variant>

class IPlayerState;
class IdleState;
class JumpingState;
class FallingState;
class MovingState;
class BoostState;
class QuickBoostState;

using PlayerState = std::variant<IPlayerState*, IdleState*, MovingState*, BoostState*, QuickBoostState*, JumpingState*, FallingState*>;
