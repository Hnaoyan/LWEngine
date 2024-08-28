#pragma once
#include <variant>

class IPlayerState;
class IdleState;
class MovingState;
class BoostState;
class JumpingState;
class FallingState;
class AssendingState;

using PlayerState = std::variant<IPlayerState*, IdleState*, MovingState*, BoostState*, JumpingState*, FallingState*, AssendingState*>;
