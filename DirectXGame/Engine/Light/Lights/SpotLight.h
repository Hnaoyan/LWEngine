#pragma once
#include "ILight.h"

/// <summary>
/// 方向光源
/// </summary>
class SpotLight : public ILight<CBufferDataSpotLight>
{
public:
	/// <summary>
	/// 作成関数
	/// </summary>
	/// <returns></returns>
	static SpotLight* CreateLight();
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="data"></param>
	void Update(const CBufferDataSpotLight& data)override;
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="rootParamIndex"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)override;

};