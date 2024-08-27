#pragma once
#include "../Utility/Singleton.h"
#include "../Math/MathLib.h"
#include <cassert>
#include <variant>
#include <map>
#include <string>

class GlobalVariables : public Singleton<GlobalVariables>
{
private:
	// 項目の型
	using Item = std::variant<int32_t, bool, float, Vector2, Vector3, std::string>;
	// グループ
	using Group = std::map<std::string, Item>;
	// 全データ
	std::map<std::string, Group> datas_;
	// グローバル変数の保存先のファイルパス
	inline static const std::string kDirectoryPath = "Resources/GlobalVarialbes/";

public:
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);
	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);
	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();
	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFile(const std::string& groupName);

public:	// 設定系関数
#pragma region 設定
	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value) 
	{
		// グループの参照を取得
		Group& group = datas_[groupName];
		// 新しい項目のデータを設定
		Item newItem{};
		newItem = value;
		// 設定した項目をstd::mapに追加
		group[key] = newItem;
	}
#pragma endregion

#pragma region 取得
	template<typename T>
	T GetValue(const std::string& groupName, const std::string& key) {
		// 指定グループが存在するか
		assert(datas_.find(groupName) != datas_.end());
		//  グループの参照を取得
		Group& group = datas_[groupName];
		// 指定グループに指定キーが存在するか
		assert(group.find(key) != group.end());
		// 指定グループから指定のキーの値を取得
		return std::get<T>(group[key]);
	}
#pragma endregion

#pragma region 追加
	template<typename T>
	void AddValue(const std::string& groupName, const std::string& key, const T& value)
	{
		// 項目が未登録なら
		if (datas_[groupName].find(key) == datas_[groupName].end()) {
			SetValue(groupName, key, value);
		}
	}
#pragma endregion

};