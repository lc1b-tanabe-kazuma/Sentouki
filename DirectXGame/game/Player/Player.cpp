#include "Player.h"
#include "MyMath.h"
using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera, Model* bulletModel) {
	model_ = model;
	camera_ = camera;
	bulletModel_ = bulletModel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransform_.scale_ = { 1.3f, 1.3f, 1.3f };
	worldTransform_.rotation_.y = 3.14f / 2.0f;

	input_ = Input::GetInstance();
}

Player::~Player() {
	// 弾の解放
	for(PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();
}

void Player::Update() {
	Move();

	// 弾の更新
	for(PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if(bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
}

void Player::Draw() {
	model_->Draw(worldTransform_, *camera_);

	// 弾の描画
	for(PlayerBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}
}

// 移動処理
void Player::Move() {
	if(input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += moveSpeed_;
	}

	if(input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= moveSpeed_;
	}

	if(input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= moveSpeed_;
	}

	if(input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += moveSpeed_;
	}

	// ワールド変換行列の更新
	WorldTransformUpdate(worldTransform_);
}

void Player::Attack(Vector2 aimPos) {

	// 弾を生成
	PlayerBullet* newBullet = new PlayerBullet();

	// 弾の発射位置(自機のワールド座標)
	Vector3 bulletStartPos = GetWorldPosition();

	// プレイヤー位置
	Vector3 playerPos = worldTransform_.translation_;

	// プレイヤー位置から照準の位置への方向ベクトルを計算
	Vector3 bulletDir = { aimPos.x - playerPos.x, aimPos.y - playerPos.y, 0.0f };

	// 弾速
	const float kBulletSpeed = 0.1f;
	Vector3 bulletVelocity = bulletDir * kBulletSpeed;

	// 弾の初期化
	newBullet->Initialize(bulletModel_, bulletStartPos, bulletVelocity);

	// 弾をセット
	bullets_.push_back(newBullet);
}

Vector3 Player::GetWorldPosition() const { return worldTransform_.translation_; }