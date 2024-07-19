#pragma once

class Quaternion;
class Vector3;
class Vector2;

class Matrix4x4
{
public:
	float m[4][4];

	Matrix4x4() {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				if (x == y) {
					m[y][x] = 1.0f;
				}
				else {
					m[y][x] = 0.0f;
				}
			}
		}
	}
	// コピーコンストラクタ（深いコピーを行う）
	Matrix4x4(const Matrix4x4& other) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = other.m[i][j];
			}
		}
	}
	Matrix4x4(const Matrix4x4* other) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = other->m[i][j];
			}
		}
	}

public: // 演算関数
	/// <summary>
	/// 逆行列
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	static Matrix4x4 MakeInverse(const Matrix4x4& m);
	/// <summary>
	/// 単位行列
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 MakeIdentity4x4();
	/// <summary>
	/// 乗算
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	/// <summary>
	/// ベクターと行列の計算
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="matrix"></param>
	/// <returns></returns>
	static Vector3 TransformVector3(const Vector3& vector, const Matrix4x4& matrix);

	/// <summary>
	/// 転置行列
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	static Matrix4x4 MakeTranspose(const Matrix4x4& m);

public:
	/// <summary>
	/// 平行移動
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	/// <summary>
	/// スケール
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	/// <summary>
	/// X軸行列計算
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateXMatrix(float radian);
	/// <summary>
	/// Y軸行列計算
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateYMatrix(float radian);
	/// <summary>
	/// Z軸行列計算
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateZMatrix(float radian);
	/// <summary>
	/// 回転行列
	/// </summary>
	/// <param name="matX"></param>
	/// <param name="matY"></param>
	/// <param name="matZ"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate);

	/// <summary>
	/// クォータニオンを回転行列に
	/// </summary>
	/// <param name="rotate"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateMatrix(const Quaternion& rotate);

	/// <summary>
	/// アフィン変換
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="translate"></param>
	/// <returns></returns>
	static Matrix4x4
		MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// アフィン変換（クォータニオン
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="translate"></param>
	/// <returns></returns>
	static Matrix4x4
		MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

	/// <summary>
	/// UVTransform用・Z軸回転のみ
	/// </summary>
	/// <param name="transform"></param>
	/// <returns></returns>
	static Matrix4x4 MakeUvTransformMatirx(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// 任意軸の回転行列を求める計算
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns></returns>
	static Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

public: // その他計算関数
	static Matrix4x4 MakeOrthographicMatrix(
		float left, float top, float right, float bottom, float nearClip, float farClip);

	static Matrix4x4 MakeViewportMatrix(
		float left, float top, float width, float height, float minDepth, float maxDepth);

	/// <summary>
	/// プロジェクション行列作成
	/// </summary>
	/// <param name="fovY"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	/// <summary>
	/// ビルボード行列作成
	/// </summary>
	/// <param name="cameraMatrix"></param>
	/// <returns></returns>
	static Matrix4x4 MakeBillBoardMatrix(const Matrix4x4& cameraMatrix);
};
