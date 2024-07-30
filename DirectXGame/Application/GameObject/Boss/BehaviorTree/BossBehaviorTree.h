#pragma once
#include <stdint.h>
#include <vector>
#include <memory>

namespace BossBehaviorTree
{
	enum class Condition : uint32_t
	{
		kIdol,		// 待機
		kFinished,	// 終了
		kRunning,	// 更新
	};
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
		// デストラクタ
		virtual ~INode() = default;
		/// <summary>
		/// 状態の返し
		/// </summary>
		/// <returns></returns>
		virtual NodeStatus Execute() = 0;
	protected:
		// ノードの状態
		NodeStatus status_;
	};
	/// <summary>
	/// 複数ノードを管理するノード
	/// </summary>
	class CompositeNode : public INode {
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
	class DecoratorNode : public INode {
	protected:
		INode* child_ = nullptr;

	};
	/// <summary>
	/// アクションノードなど終端になるノード
	/// </summary>
	class LeafNode : public INode {
	public:
		/// <summary>
		/// 更新
		/// </summary>
		/// <returns></returns>
		virtual bool Update() = 0;

	};

	class RootNode : public CompositeNode
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

	class SelectorNode : public CompositeNode 
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

	class SequenceNode : public CompositeNode
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
