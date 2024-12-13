# 就職活動作品
[![DebugBuild](https://github.com/Hnaoyan/LWEngine/actions/workflows/DebugBuild.yml/badge.svg)](https://github.com/Hnaoyan/LWEngine/actions/workflows/DebugBuild.yml)
[![ReleaseBuild](https://github.com/Hnaoyan/LWEngine/actions/workflows/ReleaseBuild.yml/badge.svg)](https://github.com/Hnaoyan/LWEngine/actions/workflows/ReleaseBuild.yml)
## 作品概要
3Dアクションシューティングゲームでボスとの1対1のゲームを作成しています。  
板野サーカスを再現している誘導弾の挙動を利用した攻撃を使った戦いです。
<img src="https://github.com/Hnaoyan/ReadMeAsset/blob/main/gifs/GameScreen.gif?raw=true" alt = Title Image width="720">

## 開発環境
- 環境
  - DirectX12
  - WindowsSDKバージョン:10.0 (最新のインストールされているバージョン)
- 外部ライブラリ
  - DirectXTex
  - Assimp
  - ImGui
  - nlohmannJson

## 力を入れている部分
板野サーカスを意識した誘導の挙動に力を入れています。  
弾の誘導先座標を分けたり誘導先座標へのベクトル作成を詳細に分けることで実装しています。
- 弾の属性
  - [秀才型](#秀才型)
  - [優等型](#優等型)
  - [劣等型](#劣等型) 

## 今後の制作
これらの動きを利用した敵の挙動・AIの作成とこれら誘導挙動をよりよく見せるための演出・エフェクトを進めています。

## 板野サーカスの弾属性

### 秀才型
対象の移動先を予測した位置に向けて追従してくる属性  
<img src="https://github.com/Hnaoyan/ReadMeAsset/blob/main/gifs/T_MissileGenius.gif?raw=true" alt = Title Image width="720">

### 優等型
対象の位置に向けて追従してくる属性  
<img src="https://github.com/Hnaoyan/ReadMeAsset/blob/main/gifs/T_MissileSuperior.gif?raw=true" alt = Title Image width="720">

### 劣等型
対象の位置から離れた位置に向けて追従してくる属性  
<img src="https://github.com/Hnaoyan/ReadMeAsset/blob/main/gifs/T_MissileInferior.gif?raw=true" alt = Title Image width="720">

