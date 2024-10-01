#pragma once
#include "Engine/Math/Vector/Vector2.h"
#include <vector>
#include <algorithm>

namespace LwLib {
	class Curve {
	public:
		/// <summary>
		/// ベジェ曲線
		/// </summary>
		/// <param name="p0"></param>
		/// <param name="p1"></param>
		/// <param name="p2"></param>
		/// <param name="t"></param>
		/// <returns></returns>
		inline static Vector2 BezierCurve(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
			Vector2 p0p1 = Vector2::Lerp(p0, p1, t);
			Vector2 p1p2 = Vector2::Lerp(p1, p2, t);
			return Vector2(Vector2::Lerp(p0p1, p1p2, t));
		}

		/// <summary>
		/// キャットむるさん
		/// </summary>
		/// <param name="p0"></param>
		/// <param name="p1"></param>
		/// <param name="p2"></param>
		/// <param name="p3"></param>
		/// <param name="t"></param>
		/// <returns></returns>
		inline static Vector3 CatmullRomSpline(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
			const float s = 0.5f;
			float t2 = t * t;
			float t3 = t2 * t;

			Vector3 e3 = (p0 * -1.0f) + (p1 * 3.0f) - (p2 * 3.0f) + p3;
			Vector3 e2 = (p0 * 2.0f) - (p1 * 5.0f) + (p2 * 4.0f) - p3;
			Vector3 e1 = (p0 * -1.0f) + p2;
			Vector3 e0 = p1 * 2.0f;
			return (e3 * t3 + e2 * t2 + e1 * t + e0) * s;
		}

		inline static Vector3 CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t) {
			assert(controlPoints.size() >= 4 && "制御点は4点未満です");

			// 区画数を制御点の数-1
			size_t division = controlPoints.size() - 1;
			// 1区間の長さ(全体を1)
			float areaWidth = 1.0f / division;

			// 区間内の始点を0 終点を1の時の現在地
			float t_2 = std::fmod(t, areaWidth) * division;
			// 0~1を出ないようにする
			t_2 = std::clamp<float>(t_2, 0.0f, 1.0f);

			// 区間番号
			size_t index = static_cast<size_t>(t / areaWidth);
			// 区間番号が上限を超えないようにする
			index = std::clamp<size_t>(index, 0, controlPoints.size() - 1);

			// 4線分のインデックス
			std::array<size_t, 4> indeces = {
				indeces[0] = index - 1,
				indeces[1] = index,
				indeces[2] = index + 1,
				indeces[3] = index + 2
			};
			// 制御点のサイズを超えないようにする
			indeces[0] = std::clamp<size_t>(indeces[0], 0, controlPoints.size() - 1);
			indeces[1] = std::clamp<size_t>(indeces[1], 0, controlPoints.size() - 1);
			indeces[2] = std::clamp<size_t>(indeces[2], 0, controlPoints.size() - 1);
			indeces[3] = std::clamp<size_t>(indeces[3], 0, controlPoints.size() - 1);

			// 最初の区間のp0はp1を重複使用
			if (index == 0) {
				indeces[0] = indeces[1];
			}
			// 最後の区間のp3はp2を重複使用
			if (indeces[3] >= controlPoints.size()) {
				indeces[2] -= 1;
				indeces[3] = indeces[2];
			}
			// 4点の座標
			std::array<Vector3, 4> points = {
				points[0] = controlPoints[indeces[0]],
				points[1] = controlPoints[indeces[1]],
				points[2] = controlPoints[indeces[2]],
				points[3] = controlPoints[indeces[3]]
			};

			return CatmullRomSpline(points[0], points[1], points[2], points[3], t_2);
		}

	};
}
