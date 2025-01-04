#pragma once

enum class TrackingTransitionMode
{
	kPreset,
	kReactive,
	kDefault,
};

/// メモ書き
///	
/// 内部処理で使うもの↓
/// pair<state,Endframe>でこれを
/// std::map<string,pair>で管理かな？
/// 
/// 初期設定で決めるもの↓
/// TransitionMode：
/// Preset＝決まったテーブル形式、
/// Reactive＝弾の状態・対象とのコミュニケーションで判断
/// の形式差
/// 