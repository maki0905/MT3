#include "Bezier.h"
#include "Sphere.h"
#include <vector>
#include "Lerp.h"
#include "Novice.h"
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Sphere sphere[3] = {
		{
			.center{controlPoint0.x, controlPoint0.y, controlPoint0.z},
			.radius{0.05f}
		},
		{
			.center{controlPoint1.x, controlPoint1.y, controlPoint1.z},
			.radius{0.05f}
		},
		{
			.center{controlPoint2.x, controlPoint2.y, controlPoint2.z},
			.radius{0.05f}
		}
	};

	for (int i = 0; i < 3; i++) {
		DrawSphere(sphere[i], viewProjectionMatrix, viewportMatrix, 0x000000FF);
	}
	const uint32_t Division = 32;
	std::vector<Vector3> startPoint;
	startPoint.push_back(controlPoint0);
	for (uint32_t index = 0; index < Division + 1; index++) {
		float t0 = float(index) / float(Division);
		//float t1 = float(index + 1) / float(Division);

		// 制御点p0,p1を線形補間
		Vector3 p0p1 = Lerp(controlPoint0, controlPoint1, t0);
		// 制御点p1,p2を線形補間
		Vector3 p1p2 = Lerp(controlPoint1, controlPoint2, t0);
		// 制御点p0p1,p1p2を線形補間
		Vector3 p = Lerp(p0p1, p1p2, t0);

		Vector3 screenPoint[2];
		Vector3 ndcVertex = Transform(startPoint[index], viewProjectionMatrix);
		screenPoint[0] = Transform(ndcVertex, viewportMatrix);
		ndcVertex = Transform(p, viewProjectionMatrix);
		screenPoint[1] = Transform(ndcVertex, viewportMatrix);
		Novice::DrawLine(int(screenPoint[0].x), int(screenPoint[0].y), int(screenPoint[1].x), int(screenPoint[1].y), color);

		startPoint.push_back(p);

	}


}
