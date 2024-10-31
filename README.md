# 就職活動作品

<img src="https://github.com/Hnaoyan/ReadMeAsset/blob/main/gifs/GameScreen.gif?raw=true" alt = Title Image width="720">

## 開発環境
- DirectX12
- DirectXTex
- Assimp
- ImGui
- nlohmannJson

## 作品概要
3Dアクションシューティングゲームで敵と自機の1対1のゲームを作成しています。  
特徴としては板野サーカスを再現している誘導弾の挙動を利用した攻撃を使った戦いです。

## 力を入れている部分
板野サーカスを意識した誘導の挙動に力を入れています。  
誘導の挙動は以下のように分離して詳細な設定で作成しています。
- 弾の属性
  - [秀才型](#秀才型)
  - [優等型](#優等型)
  - [劣等型](#劣等型)
- 追従の速度生成
  - 初速度を与え、属性に応じた追従先座標へのベクトルを使った加速度計算で速度を計算します

このような速度の生成とそれらを詳細に決める属性を決めて動かしています。  

## 今後の制作
現在はそれらの動きを利用した敵の挙動・AIの作成とこれら誘導挙動をよりよく見せるための演出・エフェクトを作成していっています。

## 板野サーカスの弾属性

### 秀才型
- 対象の移動先を予測した位置に向けて追従してくる属性
<img src="https://github.com/Hnaoyan/ReadMeAsset/blob/main/gifs/T_MissileGenius.gif?raw=true" alt = Title Image width="720">

### 優等型
- 対象の位置に向けて追従してくる属性
<img src="https://github.com/Hnaoyan/ReadMeAsset/blob/main/gifs/T_MissileSuperior.gif?raw=true" alt = Title Image width="720">

### 劣等型
- 対象の位置から離れた位置に向けて追従してくる属性
<img src="https://github.com/Hnaoyan/ReadMeAsset/blob/main/gifs/T_MissileInferior.gif?raw=true" alt = Title Image width="720">
