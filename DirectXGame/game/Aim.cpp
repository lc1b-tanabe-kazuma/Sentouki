#define NOMINMAX
#include "Aim.h"

#include <algorithm>
#include <cassert>
#include <numbers>

#include "MyMath.h"
#include "imgui.h"

using namespace KamataEngine;

void Aim::Initialize(Camera* camera) {

	spriteGH_ = TextureManager::Load("aim/aim.png");

	sprite_ = KamataEngine::Sprite::Create(
		spriteGH_, { 0.0f, 0.0f },  // 初期位置
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // 色
		{ 0.5f, 0.5f }              // 中央を基準にする ← 重要
	);

	sprite_->SetSize({ 64.0f, 64.0f }); // スプライトのサイズを設定

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	input_ = Input::GetInstance();
}

void Aim::Update() {

	mousePos_ = input_->GetMousePosition();

	sprite_->SetPosition({ mousePos_.x, mousePos_.y });

	// クールタイムを減らす
	coolTime_ -= 1.0f / 30.0f;

	// クールタイムが0以下なら打てる
	if(coolTime_ <= 0.0f) {

		// 左クリックで打つ
		if(input_->IsTriggerMouse(0)) {
			isAttac_ = true;
			coolTime_ = 0.3f;
		}
	} else {

		// 打てない時は攻撃してない状態に
		isAttac_ = false;
	}

#ifdef _DEBUG
	ImGui::Begin("Aim");

	// スプライトのサイズを変更するためのImGuiドラッグフロート
	static float cursorSize = 64.0f;
	if(ImGui::DragFloat("Cursor Size", &cursorSize, 1.0f, 16.0f, 128.0f)) {
		// スプライトのサイズを更新
		sprite_->SetSize({ cursorSize, cursorSize });
	}
	ImGui::End();
#endif // DEBUG
}

void Aim::Draw() { sprite_->Draw(); }

Vector3 Aim::GetForward() {
	Matrix4x4 matWorld = Inverse(camera_->matView);

	Vector3 forward = { matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2] };

	// 正規化
	float len = std::sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);

	forward.x /= len;
	forward.y /= len;
	forward.z /= len;

	return forward;
}

Ray Aim::GetRayFromMouse() {
	float ndcX = (2.0f * mousePos_.x / windowWidth) - 1.0f;
	float ndcY = 1.0f - (2.0f * mousePos_.y / windowHeight);

	Vector4 clip = { ndcX, ndcY, 1.0f, 1.0f }; // ★ z=1（遠平面）

	Matrix4x4 invProj = Inverse(camera_->matProjection);
	Matrix4x4 invView = Inverse(camera_->matView);

	// Clip → View
	Vector4 view = Multiply(clip, invProj);
	view.x /= view.w;
	view.y /= view.w;
	view.z /= view.w;
	view.w = 1.0f;

	// View → World
	Vector4 world = Multiply(view, invView);

	// カメラ位置
	Vector3 cameraPos = { invView.m[3][0], invView.m[3][1], invView.m[3][2] };

	// 方向 = ワールド点 - カメラ位置
	Vector3 dir = { world.x - cameraPos.x, world.y - cameraPos.y, world.z - cameraPos.z };
	Normalize(dir);

	ray_.origin = cameraPos;
	ray_.direction = dir;

	return ray_;
}

Aim::~Aim() {
	if(sprite_) {
		delete sprite_;
		sprite_ = nullptr;
	}
}
