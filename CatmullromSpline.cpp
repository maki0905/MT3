#include "CatmullromSpline.h"
#include "Sphere.h"
#include <vector>
#include "Novice.h"
Vector3 CatmullRom(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float t) {
	Vector3 result;
	float t2 = t * t;
	float t3 = t2 * t;

	float P0 = -t3 + 2.0f * t2 - t;
	float P1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
	float P2 = -3.0f * t3 + 4.0f * t2 + t;
	float P3 = t3 - t2;



	result.x = (P0 * p0.x + P1 * p1.x + P2 * p2.x + P3 * p3.x) * 0.5f;
	result.y = (P0 * p0.y + P1 * p1.y + P2 * p2.y + P3 * p3.y) * 0.5f;
	result.z = (P0 * p0.z + P1 * p1.z + P2 * p2.z + P3 * p3.z) * 0.5f;

	return result;
}

void DrawCatmullRom(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Vector3& controlPoint3, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Sphere sphere[4] = {
		{
			.center{controlPoint0.x, controlPoint0.y, controlPoint0.z},
			.radius{0.01f}
		},
		{
			.center{controlPoint1.x, controlPoint1.y, controlPoint1.z},
			.radius{0.01f}
		},
		{
			.center{controlPoint2.x, controlPoint2.y, controlPoint2.z},
			.radius{0.01f}
		},
		{
			.center{controlPoint3.x, controlPoint3.y, controlPoint3.z},
			.radius{0.01f}
		},
	};

	for (int i = 0; i < 4; i++) {
		DrawSphere(sphere[i], viewProjectionMatrix, viewportMatrix, 0x000000FF);
	}
	const uint32_t Division = 32;
	std::vector<Vector3> point;
	for (uint32_t index = 0; index < Division + 1; index++) {
		float t = float(index) / float(Division);
		Vector3 pos = CatmullRom(controlPoint3, controlPoint0, controlPoint1, controlPoint2, t);
		point.push_back(pos);

	}
	for (uint32_t index = 0; index < Division + 1; index++) {
		float t = float(index) / float(Division);
		Vector3 pos = CatmullRom(controlPoint0, controlPoint1, controlPoint2, controlPoint3, t);
		point.push_back(pos);

	}
	for (uint32_t index = 0; index < Division + 1; index++) {
		float t = float(index) / float(Division);
		Vector3 pos = CatmullRom(controlPoint1, controlPoint2, controlPoint3, controlPoint0, t);
		point.push_back(pos);

	}
	
	for (uint32_t index = 0; index < point.size() - 1; index++) {
		Vector3 screenPoint[2];
		Vector3 ndcVertex = Transform(point[index], viewProjectionMatrix);
		screenPoint[0] = Transform(ndcVertex, viewportMatrix);
		ndcVertex = Transform(point[index + 1], viewProjectionMatrix);
		screenPoint[1] = Transform(ndcVertex, viewportMatrix);
		Novice::DrawLine(int(screenPoint[0].x), int(screenPoint[0].y), int(screenPoint[1].x), int(screenPoint[1].y), color);
	}
	

}

