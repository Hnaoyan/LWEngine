#pragma once

class Boss;

class BossNode {
protected:

	enum NodeStatus {
		kReady,
		kSuccess,
		kFailure,
		kRunning,
	};

public:
	void PreInitialize(Boss* boss);
	virtual void Initialize();
	virtual void Execute();

protected:
	Boss* boss_ = nullptr;
};
