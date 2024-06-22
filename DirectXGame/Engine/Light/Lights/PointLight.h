#pragma once
#include "../LightData.h"
#include "ILight.h"

class PointLight : public ILight<CBufferDataPointLight>
{
public:
	/// <summary>
	/// 作成関数
	/// </summary>
	/// <returns></returns>
	static PointLight* CreateLight();
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="data"></param>
	void Update(const CBufferDataPointLight& data)override;
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="rootParamIndex"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndex)override;

};