#pragma once
#include "../LightData.h"
#include "ILight.h"

class DirectionalLight : public ILight<CBufferDataDirectionalLight>
{
public:
	/// <summary>
	/// 作成関数
	/// </summary>
	/// <returns></returns>
	static DirectionalLight* CreateLight();
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="data"></param>
	void Update(const CBufferDataDirectionalLight& data)override;
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="rootParamIndex"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList,uint32_t rootParamIndex)override;

};

