#pragma once

class IGameObject
{
public:

	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void ImGui() {};

};
