#pragma once

class Boss;

class ActionController
{
public:
	void Initialize(Boss* boss);

private:
	Boss* boss_ = nullptr;
};
