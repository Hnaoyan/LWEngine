#pragma once
#include "../3D/Instancing/InstancedGroup.h"
#include <unordered_map>
#include <memory>

class InstancedManager
{
public:
	virtual ~InstancedManager() = default;

public:
	//virtual void Initialize();
	virtual void Update();
	virtual void Draw(ModelDrawDesc desc);
public:
	/// <summary>
	/// クラスター作成
	/// </summary>
	/// <param name="instance"></param>
	/// <param name="tag"></param>
	virtual void CreateCluster(std::unique_ptr<InstancedGroup> instance, const std::string& tag);

public:	// 全体的に使える関数
	/// <summary>
	/// 検索
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	InstancedGroup* FindCluster(const std::string& tag);
	/// <summary>
	/// 指定削除
	/// </summary>
	/// <param name="tag"></param>
	void DeleteCluster(const std::string& tag);
	/// <summary>
	/// 全削除
	/// </summary>
	void AllClear() { clusters_.clear(); }

protected:
	std::unordered_map<std::string, std::unique_ptr<InstancedGroup>> clusters_;
	Model* model_ = nullptr;
};
