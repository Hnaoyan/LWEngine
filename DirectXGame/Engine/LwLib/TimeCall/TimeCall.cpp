#include "TimeCall.h"

LwLib::TimeCall::TimeCall(std::function<void()> function, uint32_t time) : function_(function), time_(time) {}

void LwLib::TimeCall::Update()
{
	if (isFinished) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished = true;
		//コールバック関数の呼び出し
		function_();
	}
}
