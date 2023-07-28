#include <Novice.h>
#include "Sphere.h"
#include <imgui.h>
#include <cstdint>
#include "MathFunction.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <imgui.h>
#include "Collision.h"
#include "Plane.h"
#include "Triangle.h"
#include "AABB.h"
#include "OBB.h"
#include "Bezier.h"
#include "CatmullromSpline.h"
#include "Physics.h"

const char kWindowTitle[] = "LE1A_16_マキユキノリ_タイトル";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewporMatrix);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
void DrawArm(const Matrix4x4& Shoulder, const Matrix4x4& Elbow, const Matrix4x4& Hand, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

// 二項演算子
Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtract(v1, v2); }
Vector3 operator*(float s, const Vector3& v) { return Multiply(s, v); }
Vector3 operator*(const Vector3& v, float s) { return s * v; }
Vector3 operator/(const Vector3& v, float s) { return Multiply((1.0f / s), v); }
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return Add(m1, m2); }
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) { return Subtract(m1, m2); }
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return Multiply(m1, m2); }

// 単項演算子
Vector3 operator-(const Vector3& v) { return { -v.x, -v.y, -v.z }; }
Vector3 operator+(const Vector3& v) { return v; }

// 複合代入演算子



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;


	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewProjectionMatrix;
	Matrix4x4 viewportMatrix;

	// デバッグカメラ
	bool isDebugCameraActive = false;
	Matrix4x4 debugCamera = MakeIdentity4x4();
	Vector3 debugCameraTranslate{ 0.0f, 0.0f, 0.0f };
	Vector3 debugCameraRotate{ 0.0f, 0.0f, 0.0f };



	/*Sphere sphere[2];
	sphere[0] = {
		.center{0.0f, 0.0f, 0.0f},
		.radius{0.5f}
	};*/

	//Plane plane = { {0.0f, 1.0f, 0.0f}, 0.5f };

	/*Segment segment = {
		.origin{0.0f, -0.6f, 0.0f},
		.diff{0.0f, 0.5f, 0.0f}
	};*/

	/*Triangle triangle = {
		{
			{0.0f, 1.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			 {-1.0f, 0.0f, 0.0f}
		}
	};*/

	/*AABB aabb1{
		.min{-0.5f, -0.5f, -0.5f},
		.max{0.5f, 0.5f, 0.5f}
	};*/
	/*AABB aabb2{
		.min{0.2f, 0.2f, 0.2f},
		.max{1.0f, 1.0f, 1.0f}
	};*/

	//Vector3 rotate{ 0.0f, 0.0f, 0.0f };
	/*OBB obb{
		.center{-1.0f, 0.0f, 0.0f},
		.orientations =
		{
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
		},
		.size{0.5f, 0.5f, 0.5f}
	};*/

	/*Vector3 controlPoint[3] = {
		{-0.8f, 0.58f, 1.0f},
		{1.76f, 1.0f, -0.3f},
		{0.94f, -0.7f, 2.3f}
	};*/

	Vector3 controlPoints[4] = {
		{-0.8f, 0.58f, 1.0f},
		{1.76f, 1.0f, -0.3f},
		{0.94f, -0.7f, 2.3f},
		{-0.53f, -0.26f, -0.15f},
	};

	Vector3 translates[3] = {
		{0.2f, 1.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f},
	};
	Vector3 rotates[3] = {
		{0.0f, 0.0f, -6.8f},
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f, 0.0f},
	};
	Vector3 scales[3] = {
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f}
	};

	Spring spring{};
	spring.anchor = { 0.0f, 0.0f, 0.0f };
	spring.naturallength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.position = { 0.5f, 0.38f, 0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = WHITE;

	Circle circle{};
	circle.center = { 0.0f, 0.0f, 0.0f };
	circle.radius = 0.8f;

	Pendulum pendulum;
	pendulum.anchor = { 0.0f, 1.0f, 0.0f };
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;

	float deltaTime = 1.0f / 60.0f;

	/*float angle = 0.0f;
	float angularVelocity = 3.14f;*/

	// カメラの位置と角度
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);

	bool start = false;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		

		if (keys[DIK_Q] != 0 && preKeys[DIK_Q] == 0) {
			if (isDebugCameraActive == false) {
				isDebugCameraActive = true;
				debugCamera = cameraMatrix;
				debugCameraRotate = cameraRotate;
				debugCameraTranslate = cameraTranslate;
			}
			else {
				isDebugCameraActive = false;
			}
		}
		if (isDebugCameraActive == true) {
			debugCamera = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, debugCameraRotate, debugCameraTranslate);
			viewMatrix = Inverse(debugCamera);
			projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
			viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
			viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
			ImGui::Begin("DebugCamera");
			ImGui::DragFloat3("Rotate", &debugCameraRotate.x, 0.01f);
			ImGui::DragFloat3("Translate", &debugCameraTranslate.x, 0.01f);
			ImGui::End();
		}
		else {
			viewMatrix = Inverse(cameraMatrix);
			projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
			viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
			viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		}

		if (start) {
			// バネ
			/*Vector3 diff = ball.position - spring.anchor;
			float length = Length(diff);
			if (length != 0.0f) {
				Vector3 direction = Normalize(diff);
				Vector3 restPosition = spring.anchor + direction * spring.naturallength;
				Vector3 displacement = length * (ball.position - restPosition);
				Vector3 restoringForce = -spring.stiffness * displacement;
				Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
				Vector3 force = restoringForce + dampingForce;
				ball.acceleration = force / ball.mass;
			}

			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;*/

			// 等速円運動
			/*angle += angularVelocity * deltaTime;
			ball.position.x = circle.center.x + std::cos(angle) * circle.radius;
			ball.position.y = circle.center.y + std::sin(angle) * circle.radius;
			ball.position.z = circle.center.z;*/

			// 振り子運動
			pendulum.angularAcceleration = -(9.8f / pendulum.length) * std::sin(pendulum.angle);
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angle += pendulum.angularVelocity * deltaTime;
			ball.position.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
			ball.position.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
			ball.position.z = pendulum.anchor.z;
		}
		

		/*Vector3 a{ 0.2f, 1.0f, 0.0f };
		Vector3 b{ 2.4f, 3.1f, 1.2f };
		Vector3 c = a + b;
		Vector3 d = a - b;
		Vector3 e = a * 2.4f;
		Vector3 f = a / 2.0f;
		Vector3 g = -a;
		Vector3 rotate{ 0.4f, 1.43f, -0.8f };
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
		Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;
		Matrix4x4 addMatrix = rotateXMatrix + rotateYMatrix;
		Matrix4x4 subMatrix = rotateXMatrix - rotateYMatrix;
		Vector3 h = {1.0f, 1.0f, 1.0f};
		h += a;
		Vector3 j = { 1.0f, 1.0f, 1.0f };
		j -= a;
		Vector3 k = { 1.0f, 1.0f, 1.0f };
		k *= 2.0f;
		Vector3 l = { 1.0f, 1.0f, 1.0f };
		l /= 5.0f;*/
 
		ImGui::Begin("Window");
		/*ImGui::DragFloat3("SpherCenter1", &sphere[0].center.x, 0.01f);
		ImGui::DragFloat("SpherRadius1", &sphere[0].radius, 0.01f);*/
		//ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		//plane.normal = Normalize(plane.normal);
		//ImGui::DragFloat("Plane.distance", &plane.distance, 0.01f);
		/*ImGui::DragFloat3("Line.origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Line.diff", &segment.diff.x, 0.01f);*/
		/*ImGui::DragFloat3("Triangle.vertices[0]", &triangle.vertices[0].x, 0.01f);
		ImGui::DragFloat3("Triangle.vertices[1]", &triangle.vertices[1].x, 0.01f);
		ImGui::DragFloat3("Triangle.vertices[2]", &triangle.vertices[2].x, 0.01f);*/
		/*ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);*/
		/*ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.01f);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.01f);
		aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);*/
		/*ImGui::DragFloat3("OBB.center", &obb.center.x, 0.01f);
		ImGui::DragFloat3("OBB.size", &obb.size.x, 0.01f);
		ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
		Matrix4x4 rotateMatrix = MakeRotate(rotate);
		obb.orientations[0].x = rotateMatrix.m[0][0];
		obb.orientations[0].y = rotateMatrix.m[0][1];
		obb.orientations[0].z = rotateMatrix.m[0][2];

		obb.orientations[1].x = rotateMatrix.m[1][0];
		obb.orientations[1].y = rotateMatrix.m[1][1];
		obb.orientations[1].z = rotateMatrix.m[1][2];

		obb.orientations[2].x = rotateMatrix.m[2][0];
		obb.orientations[2].y = rotateMatrix.m[2][1];
		obb.orientations[2].z = rotateMatrix.m[2][2];

		ImGui::DragFloat3("ControlPoint0", &controlPoint[0].x, 0.01f);
		ImGui::DragFloat3("ControlPoint1", &controlPoint[1].x, 0.01f);
		ImGui::DragFloat3("ControlPoint2", &controlPoint[2].x, 0.01f);

		ImGui::DragFloat3("ControlPoint0", &controlPoints[0].x, 0.01f);
		ImGui::DragFloat3("ControlPoint1", &controlPoints[1].x, 0.01f);
		ImGui::DragFloat3("ControlPoint2", &controlPoints[2].x, 0.01f);
		ImGui::DragFloat3("ControlPoint3", &controlPoints[3].x, 0.01f);*/

		/*ImGui::DragFloat3("Shoulder : translate", &translates[0].x, 0.01f);
		ImGui::DragFloat3("Shoulder : rotate", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("Shoulder : scale", &scales[0].x, 0.01f);
		ImGui::DragFloat3("Elbow : translate", &translates[1].x, 0.01f);
		ImGui::DragFloat3("Elbow : rotate", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("Elbow : scale", &scales[1].x, 0.01f);
		ImGui::DragFloat3("Head : translate", &translates[2].x, 0.01f);
		ImGui::DragFloat3("Head : rotate", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("Head : scale", &scales[2].x, 0.01f);*/

		/*ImGui::Text("c:%f, %f, %f", c.x, c.y, c.z);
		ImGui::Text("d:%f, %f, %f", d.x, d.y, d.z);
		ImGui::Text("e:%f, %f, %f", e.x, e.y, e.z);
		ImGui::Text("f:%f, %f, %f", f.x, f.y, f.z);
		ImGui::Text("g:%f, %f, %f", g.x, g.y, g.z);
		ImGui::Text("h:%f, %f, %f", h.x, h.y, h.z);
		ImGui::Text("j:%f, %f, %f", j.x, j.y, j.z);
		ImGui::Text("k:%f, %f, %f", k.x, k.y, k.z);
		ImGui::Text("l:%f, %f, %f", l.x, l.y, l.z);
		ImGui::Text("matrix:\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n",
			rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2],
			rotateMatrix.m[0][3], rotateMatrix.m[1][0], rotateMatrix.m[1][1],
			rotateMatrix.m[1][2], rotateMatrix.m[1][3], rotateMatrix.m[2][0],
			rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3],
			rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2],
			rotateMatrix.m[3][3]);
		ImGui::Text("matrix:\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n",
			addMatrix.m[0][0], addMatrix.m[0][1], addMatrix.m[0][2],
			addMatrix.m[0][3], addMatrix.m[1][0], addMatrix.m[1][1],
			addMatrix.m[1][2], addMatrix.m[1][3], addMatrix.m[2][0],
			addMatrix.m[2][1], addMatrix.m[2][2], addMatrix.m[2][3],
			addMatrix.m[3][0], addMatrix.m[3][1], addMatrix.m[3][2],
			addMatrix.m[3][3]);
		ImGui::Text("matrix:\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n",
			subMatrix.m[0][0], subMatrix.m[0][1], subMatrix.m[0][2],
			subMatrix.m[0][3], subMatrix.m[1][0], subMatrix.m[1][1],
			subMatrix.m[1][2], subMatrix.m[1][3], subMatrix.m[2][0],
			subMatrix.m[2][1], subMatrix.m[2][2], subMatrix.m[2][3],
			subMatrix.m[3][0], subMatrix.m[3][1], subMatrix.m[3][2],
			subMatrix.m[3][3]);*/
		if (ImGui::Button("Start")) {
			start = true;
		}
		

		ImGui::End();

		Matrix4x4 worldMatrix[3];
		for (int i = 0; i < 3; i++) {
			worldMatrix[i] = MakeIdentity4x4();
		}
		worldMatrix[0] = MakeAffineMatrix(scales[0], rotates[0], translates[0]);
		worldMatrix[1] = MakeAffineMatrix(scales[1], rotates[1], translates[1]);
		worldMatrix[1] = Multiply(worldMatrix[1], worldMatrix[0]);
		worldMatrix[2] = MakeAffineMatrix(scales[2], rotates[2], translates[2]);
		worldMatrix[2] = Multiply(worldMatrix[2], worldMatrix[1]);
		/*worldMatrix[1].m[0][0] = Transform(scales[1], worldMatrix[0]).x;
		worldMatrix[1].m[0][1] = Transform(scales[1], worldMatrix[0]).y;
		worldMatrix[1].m[0][2] = Transform(scales[1], worldMatrix[0]).z;
		worldMatrix[1].m[1][0] = Transform(rotates[1], worldMatrix[0]).x;
		worldMatrix[1].m[1][1] = Transform(rotates[1], worldMatrix[0]).y;
		worldMatrix[1].m[1][2] = Transform(rotates[1], worldMatrix[0]).z;
		worldMatrix[1].m[2][0] = Transform(translates[1], worldMatrix[0]).x;
		worldMatrix[1].m[2][1] = Transform(translates[1], worldMatrix[0]).y;
		worldMatrix[1].m[2][2] = Transform(translates[1], worldMatrix[0]).z;
		worldMatrix[2].m[0][0] = Transform(scales[2], worldMatrix[1]).x;
		worldMatrix[2].m[0][1] = Transform(scales[2], worldMatrix[1]).y;
		worldMatrix[2].m[0][2] = Transform(scales[2], worldMatrix[1]).z;
		worldMatrix[2].m[1][0] = Transform(rotates[2], worldMatrix[1]).x;
		worldMatrix[2].m[1][1] = Transform(rotates[2], worldMatrix[1]).y;
		worldMatrix[2].m[1][2] = Transform(rotates[2], worldMatrix[1]).z;
		worldMatrix[2].m[2][0] = Transform(translates[2], worldMatrix[1]).x;
		worldMatrix[2].m[2][1] = Transform(translates[2], worldMatrix[1]).y;
		worldMatrix[2].m[2][2] = Transform(translates[2], worldMatrix[1]).z;*/

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		/*if (IsCollision(sphere[0], plane)) {
			DrawSphere(sphere[0], viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawSphere(sphere[0], viewProjectionMatrix, viewportMatrix, WHITE);
		}*/
		/*if (IsCollision(segment, plane)) {

			Novice::DrawLine(int(screenPoint[0].x), int(screenPoint[0].y), int(screenPoint[1].x), int(screenPoint[1].y), RED);
		}
		else {
			Novice::DrawLine(int(screenPoint[0].x), int(screenPoint[0].y), int(screenPoint[1].x), int(screenPoint[1].y), WHITE);
		}
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);*/
		/*if (IsCollision(triangle, segment)) {
			DrawSegment(segment, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawSegment(segment, viewProjectionMatrix, viewportMatrix, WHITE);
		}

		DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, WHITE);*/

		/*if (IsCollision(aabb1, aabb2)) {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, WHITE);
		}
		DrawAABB(aabb2, viewProjectionMatrix, viewportMatrix, WHITE);*/
		/*DrawSegment(segment, viewProjectionMatrix, viewportMatrix, WHITE);
		if (IsCollision(aabb1, segment)) {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, WHITE);
		}*/
		/*if (IsCollision(sphere[0], aabb1)) {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, WHITE);
		}*/

		/*DrawSphere(sphere[0], viewProjectionMatrix, viewportMatrix, WHITE);
		if (IsCollision(obb, sphere[0])) {
			DrawOBB(obb, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawOBB(obb, viewProjectionMatrix, viewportMatrix, WHITE);
		}*/

		/*DrawSegment(segment, viewProjectionMatrix, viewportMatrix, WHITE);
		if (IsCollision(obb, segment)) {
			DrawOBB(obb, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawOBB(obb, viewProjectionMatrix, viewportMatrix, WHITE);
		}*/

		//DrawBezier(controlPoint[0], controlPoint[1], controlPoint[2], viewProjectionMatrix, viewportMatrix, 0x0000FFFF);

		//DrawCatmullRom(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], viewProjectionMatrix, viewportMatrix, 0x0000FFFF);

		//DrawArm(worldMatrix[0], worldMatrix[1], worldMatrix[2], viewProjectionMatrix, viewportMatrix);

		/*Segment segment_sp = {
			.origin{spring.anchor},
			.diff{ball.position},
		};
		DrawSegment(segment_sp, viewProjectionMatrix, viewportMatrix, WHITE);*/
		Segment segment_pe = {
			.origin{pendulum.anchor},
			.diff{ball.position - pendulum.anchor},
		};
		DrawSegment(segment_pe, viewProjectionMatrix, viewportMatrix, WHITE);
		DrawBall(ball, viewProjectionMatrix, viewportMatrix, ball.color);

		

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x, y, "%s", name);
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight + 20, "%6.02f", matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfwidth = 2.0f; // Gridの半分の幅
	const uint32_t kSubdivision = 10; // 分割数
	const float kGridEvery = (kGridHalfwidth * 2.0f) / float(kSubdivision); // 1つ分の長さ
	// 奥から手前への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// ワールド座標系上の始点と終点を求める
		Vector3 start = { -(kGridEvery * float(kSubdivision / uint32_t(kGridHalfwidth))), 0.0f, kGridEvery * float(float(kSubdivision) / kGridHalfwidth - float(zIndex)) };
		Vector3 end = { (kGridEvery * float(kSubdivision / uint32_t(kGridHalfwidth))), 0.0f, kGridEvery * float(float(kSubdivision) / kGridHalfwidth - float(zIndex)) };
		// スクリーン座標系まで変換をかける
		Vector3 ndcVertex = Transform(start, viewProjectionMatrix);
		start = Transform(ndcVertex, viewportMatrix);
		ndcVertex = Transform(end, viewProjectionMatrix);
		end = Transform(ndcVertex, viewportMatrix);
		// 変換した座標を使って表示、色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、なんでも良い
		if (zIndex == (kSubdivision / int(kGridHalfwidth))) {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0x000000FF);
		}
		else {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xAAAAAAFF);
		}

	}
	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// ワールド座標系上の始点と終点を求める
		Vector3 start = { kGridEvery * float(float(zIndex) - float(kSubdivision) / kGridHalfwidth), 0.0f, -(kGridEvery * float(kSubdivision / uint32_t(kGridHalfwidth))) };
		Vector3 end = { kGridEvery * float(float(zIndex) - float(kSubdivision) / kGridHalfwidth), 0.0f, (kGridEvery * float(kSubdivision / uint32_t(kGridHalfwidth))) };
		// スクリーン座標系まで変換をかける
		Vector3 ndcVertex = Transform(start, viewProjectionMatrix);
		start = Transform(ndcVertex, viewportMatrix);
		ndcVertex = Transform(end, viewProjectionMatrix);
		end = Transform(ndcVertex, viewportMatrix);
		// 変換した座標を使って表示、色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、なんでも良い
		if (zIndex == (kSubdivision / int(kGridHalfwidth))) {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0x000000FF);
		}
		else {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xAAAAAAFF);
		}
	}
}




void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}

void DrawArm(const Matrix4x4& Shoulder, const Matrix4x4& Elbow, const Matrix4x4& Hand, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	
	Matrix4x4 S_E = Subtract(Elbow, Shoulder);
	Matrix4x4 E_H = Subtract(Hand, Elbow);
	Segment segment[2] = {
		{
			.origin{Shoulder.m[3][0], Shoulder.m[3][1], Shoulder.m[3][2]},
			.diff{S_E.m[3][0], S_E.m[3][1], S_E.m[3][2]}

		},
		{
			.origin{Elbow.m[3][0], Elbow.m[3][1], Elbow.m[3][2]},
			.diff{E_H.m[3][0], E_H.m[3][1], E_H.m[3][2]}
		},
	};
	DrawSegment(segment[0], viewProjectionMatrix, viewportMatrix, WHITE);
	DrawSegment(segment[1], viewProjectionMatrix, viewportMatrix, WHITE);


	Sphere sphere[3] = {
		{
			.center{Shoulder.m[3][0], Shoulder.m[3][1], Shoulder.m[3][2]},
			.radius{0.05f}
		},
		{
			.center{Elbow.m[3][0], Elbow.m[3][1], Elbow.m[3][2]},
			.radius{0.05f}
		},
		{
			.center{Hand.m[3][0], Hand.m[3][1], Hand.m[3][2]},
			.radius{0.05f}
		},

	};
	
	DrawSphere(sphere[0], viewProjectionMatrix, viewportMatrix, RED);
	DrawSphere(sphere[1], viewProjectionMatrix, viewportMatrix, GREEN);
	DrawSphere(sphere[2], viewProjectionMatrix, viewportMatrix, BLUE);
}
