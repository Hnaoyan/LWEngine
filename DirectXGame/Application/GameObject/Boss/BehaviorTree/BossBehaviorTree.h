#pragma once
#include <stdint.h>
#include <vector>
#include <memory>

/// <summary>
/// BossのBehaviorTree
/// </summary>
namespace BossBT
{
	// 実行状態
	enum class NodeCondition : uint32_t
	{
		kIdol,		// 待機
		kFinished,	// 終了
		kRunning,	// 更新
	};

	// 実行結果
	enum class NodeStatus : uint32_t 
	{
		kSuccess,	// 成功
		kFailed,	// 失敗
		kRunning,	// 途中
	};

	/// <summary>
	/// ノードの基クラス
	/// </summary>
	class INode {
	public:
		// コンストラクタ
		INode() { condition_ = NodeCondition::kIdol; }
		// デストラクタ
		virtual ~INode() = default;
		/// <summary>
		/// 実行
		/// </summary>
		/// <returns></returns>
		virtual NodeStatus Execute() = 0;
	protected:
		// ノードの状態
		NodeCondition condition_;
	};

	/// <summary>
	/// 複数ノードを管理するノード
	/// </summary>
	class Composite : public INode {
	protected:
		// ノードツリー
		std::vector<std::unique_ptr<INode>> childrens_;
		// 前に実行していたノード
		INode* currentRunningNode_ = nullptr;

	public:
		void AddChild(std::unique_ptr<INode> child) {
			childrens_.push_back(std::move(child));
		}

	};

	/// <summary>
	/// 単一のノードを持つノード
	/// </summary>
	class Decorator : public INode {
	protected:
		INode* child_ = nullptr;

	};

	/// <summary>
	/// アクションノードなど終端になるノード
	/// </summary>
	class Leaf : public INode {
	public:
		/// <summary>
		/// 更新
		/// </summary>
		/// <returns></returns>
		virtual bool Update() = 0;

	};

	class RootNode : public Composite
	{
	private:
		std::unique_ptr<INode> root_;

	public:
		void SetRoot(std::unique_ptr<INode> node) {
			root_ = std::move(node);
		}

		NodeStatus Execute() override {
			return NodeStatus::kFailed;
		}
	};

	class SelectorNode : public Composite 
	{
	public:
		NodeStatus Execute() override {
			if (currentRunningNode_) {
				NodeStatus status = currentRunningNode_->Execute();
				if (status == NodeStatus::kSuccess) {
					// 削除
					currentRunningNode_ = nullptr;
					return NodeStatus::kSuccess;
				}
				// 継続
				else if (status == NodeStatus::kRunning) {
					return NodeStatus::kRunning;
				}
				// 削除
				currentRunningNode_ = nullptr;
				return NodeStatus::kFailed;
			}
			for (std::vector<std::unique_ptr<INode>>::iterator it = childrens_.begin(); it != childrens_.end(); ++it) {
				NodeStatus status = (*it)->Execute();
				if (status == NodeStatus::kSuccess) {
					// 削除
					currentRunningNode_ = nullptr;
					return NodeStatus::kSuccess;
				}
				// 継続
				else if (status == NodeStatus::kRunning) {
					currentRunningNode_ = (*it).get();
					return NodeStatus::kRunning;
				}
				// 削除
				currentRunningNode_ = nullptr;
			}
			return NodeStatus::kFailed;
		}

	};

	class SequenceNode : public Composite
	{

	};

	class ConditionNode : public INode
	{

	};

	class ActionNode : public INode
	{
	public:
		//ActionNode() : status_(NodeStatus::kRunning) {}


	};

};
