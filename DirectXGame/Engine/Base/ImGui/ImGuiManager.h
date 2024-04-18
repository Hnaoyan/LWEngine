#pragma once
#include "../../Utility/Singleton.h"

class DirectXCommon;
class WindowAPI;

class ImGuiManager : public Singleton<ImGuiManager>
{
public:
	void Initialize(DirectXCommon* dxCommon,WindowAPI* winApp);

	void Begin();
	void End();
	void Draw();
	void Finalize();

private:
	DirectXCommon* dxCommon_ = nullptr;

};
