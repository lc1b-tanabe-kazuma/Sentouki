#include "Boss.h"
#include "MyMath.h"
#include "Player/Player.h"
#include <numbers>

using namespace KamataEngine;

void Boss::Initialize(Model* model, KamataEngine::Camera* camera) {

#ifdef DEBUG
	// NULLポインタのチェック
	assert(model);
#endif // DEBUG

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTranseform_.Initialize();
	worldTranseform_.translation_ = { 0.0f, -10.0f, 50.0f };
	worldTranseform_.rotation_ = { 0.0f, -3.14f, 0.0f };
	worldTranseform_.scale_ = { 1.5f, 1.5f, 1.5f };

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	input_ = Input::GetInstance();
}

void Boss::Update() {

	// 行列を定数バッファに転送
	WorldTransformUpdate(worldTranseform_);
#ifdef _DEBUG
	// ボスの座標を画面表示する
	ImGui::Begin("Boss Position");
	ImGui::Text("x: %.2f", worldTranseform_.translation_.x);
	ImGui::Text("y: %.2f", worldTranseform_.translation_.y);
	ImGui::Text("z: %.2f", worldTranseform_.translation_.z);

	// ボスの体力を画面表示する
	ImGui::Text("HP: %d", bossHp_);
	ImGui::End();
#endif // DEBUG
}

void Boss::Draw() {

	// 描画
	model_->Draw(worldTranseform_, *camera_);
}

Vector3 Boss::GetPosition() { return worldTranseform_.translation_; }

void Boss::OnCollosion(int damage) {

	// ボスの体力を減らす
	bossHp_ -= damage;

	// 体力が0以下ならデスフラグを立てる
	if(bossHp_ <= 0) {
		isDead_ = true;
	}
}

Boss::~Boss() {

}

Vector3 Boss::GetWorldPosition() const {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTranseform_.matWorld_.m[3][0];
	worldPos.y = worldTranseform_.matWorld_.m[3][1];
	worldPos.z = worldTranseform_.matWorld_.m[3][2];
	return worldPos;
}