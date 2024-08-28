#pragma once
#include <variant>

class IPlayerState;
class IdleState;
class IdleHorizontal;
class IdleVertical;
class MovingState;
class BoostState;
class JumpingState;
class FallingState;
class AssendingState;

using PlayerState = std::variant<IPlayerState*, IdleState*, MovingState*, BoostState*,
	JumpingState*, FallingState*, AssendingState*, IdleHorizontal*, IdleHorizontal*>;
