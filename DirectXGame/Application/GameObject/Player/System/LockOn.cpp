#include "LockOn.h"
#include <cassert>

void LockOn::Initialize(Player* player)
{
	assert(player);
	player_ = player;
}
