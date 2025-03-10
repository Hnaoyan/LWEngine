#pragma once
#include "CPU/Emitter/CPUParticleEmitter.h"
#include <vector>
#include <memory>
#include <unordered_map>

class CPUParticleManager
{
public:
	CPUParticleManager();
	~CPUParticleManager() = default;
	void Update();
	void Draw(ModelDrawDesc desc);
	/// <summary>
	/// 全消し
	/// </summary>
	void ClearEmitter() { emitters_.clear(); }
	/// <summary>
	/// 作成
	/// </summary>
	/// <param name="tag"></param>
	void CreateEmitter(const std::string& tag);
	/// <summary>
	/// 削除
	/// </summary>
	/// <param name="tag"></param>
	void DeleteEmitter(const std::string& tag);
	/// <summary>
	/// 検索
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	InstancedGroup* FindEmitter(const std::string& tag);
	//void AddEmitter();

private:
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>> emitters_;
};
