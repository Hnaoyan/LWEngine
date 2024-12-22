#pragma once
#include "../Utility/Singleton.h"
#include "LightLists.h"
#include <unordered_map>
#include <string>
#include <memory>

class DirectionalLight;

/// <summary>
/// ライト管理クラス
/// </summary>
class LightingManager
{
public:
	~LightingManager() = default;
	LightingManager();
public:
	void Initialize();
	void Update();
	void ImGuiDraw();
	//void StaticInitialize(ID3D12Device* device);
public:	// アクセッサ
	DirectionalLight* GetDirectional() { return drLight_.get(); }
	SpotLight* GetSpot() { return spLight_.get(); }
	PointLight* GetPoint() { return ptLight_.get(); }
private:
	// ライト
	//std::unordered_map<std::string, std::unique_ptr<ILight>> lights_;

	std::unique_ptr<DirectionalLight> drLight_;
	std::unique_ptr<SpotLight> spLight_;
	std::unique_ptr<PointLight> ptLight_;

	CBufferDataDirectionalLight drData_{};
	CBufferDataSpotLight spData_{};
	CBufferDataPointLight ptData_{};


};