#pragma once

/// <summary>
/// Pipelineの配列番号など
/// </summary>
namespace Pipeline
{
	enum class Order : int {
		kSkybox,
		kSpirte,
		kLine,
		kTriangle,
		kModel,
		kSkinningModel,
		kParticle,
		kPostEffect,
		kInstancedModel,
		kTrail,
		kCountOfParameter,
	};
	// 通常モデルのレジスタ番号
	enum class ModelRegister : int {
		kMaterial,
		kTexture,
		kMapTexture,
		kDissolveTexture,
		kWorldTransform,
		kViewProjection,
		kDirectionalLight,
		kPointLight,
		kSpotLight,
		// サイズを取得する為の値
		kCountOfParameter,
	};

	enum class SkyBoxRegister : int {
		kMaterial,
		kTexture,
		kViewProjection,
		kWorldTransform,
		kCountOfParameter,
	};

	enum class LineRegister :int {
		kViewProjection,
		kLineData,
		kCountOfParameter,
	};

	enum class InstancedUnitRegister : int {
		kMaterial,
		kTexture,
		kMapTexture,
		kWorldTransform,
		kViewProjection,
		kDirectionalLight,
		kPointLight,
		kSpotLight,
		// サイズを取得する為の値
		kCountOfParameter,
	};

	// スキニングモデル用のレジスタ番号
	enum class SkinningModelRegister : int {
		kMaterial,
		kTexture,
		kMapTexture,
		kWorldTransform,
		kViewProjection,
		kDirectionalLight,
		kPointLight,
		kSpotLight,

		kMatrixPalette,
		kCountOfParameter,
	};

	// パーティクルのレジスタ用
	enum class GPUParticleDrawRegister : int {
		kMaterial,
		kTexture,
		kMatrixs,
		kCamera,

		kCountOfParameter,
	};
	// ブレンドモード
	enum class BlendMode : int {
		kNone,
		kAlpha,
		kAdd,
		kSubtract,
		kMultiply,
		kScreen,
		// サイズカウント
		kCountOfBlendMode,
	};
	// 描画種類
	enum class DrawType : int {
		kModel,
		kParticle,

		// サイズカウント
		kCountOfDrawer,
	};
}

///
/// PostEffect
/// 
namespace Pipeline 
{
	// ポストエフェクト
	enum class PostEffectRegister : int {
		kTexture,
		kDissolveTexture,
		kVignette,
		kBlur,
		kDissolve,
		kNoise,
		kHSV,
		kBloom,
		kGrayscale,
		kCountOfParameter,
	};
	// ポストエフェクトの種類
	enum class PostEffectType : int {
		kAlpha,			// 通常
		kGrayScale,			// グレースケール
		kVignette,			// ビネット
		kGrayscaleVignette,	// グレースケールとビネット
		kSmoothing,			// 平滑
		kGaussian,			// ぼかし
		kRadialBlur,		// ブラー
		kDissolve,			// Dissolve
		kNoise,				// ランダムノイズ
		kLuminanceOutline,	// 簡易アウトライン
		kBloom,	// ブルーム
		// USER
		kVignetteBlur,	// ビネットとブラーの合わせ（ブルームも掛ける）
		kGrayscaleBloom,	// グレースケールとブルームの合わせ
		kCountOfType,
	};
}

///
/// GPU
/// 
namespace Pipeline {
	enum class GPUParticleRegister : int {
		kUAVParticle,	// 書き込み
		kUAVFreeListIndex,	// リストの番号
		kUAVFreeList,		// リスト
		kEmitter,		// エミッター
		kPerTime,		// 時間関係のデータ
		kCountOfParameter,	// 数
	};
	enum class GPUParticleCSRegister : int {
		kUAVParticle,	// 書き込み
		kUAVFreeListIndex,	// リストの番号
		kUAVFreeList,		// リスト
		kEmitter,		// エミッター
		kPerTime,		// 時間関係のデータ
		kCountOfParameter,	// 数
	};
	enum class GPUParticlePipeline : int {
		kInitialize,
		kEmit,
		kUpdate,
		kCountOfProcess,
	};
}