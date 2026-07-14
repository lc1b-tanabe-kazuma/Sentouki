#include "Scene/GameScene.h"
#include "engine/sceneEngine/SceneManager.h"

GameScene::~GameScene() {
	delete player_;
	delete modelPlayer_;
	delete modelBullet_;
	delete aim_;
}

void GameScene::Initialize() {

	// カメラの初期化
	camera_.Initialize();

	//
	worldTransform_.Initialize();

	// プレイヤーの初期化
	player_ = new Player();
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelBullet_ = Model::CreateFromOBJ("playerBullet", true);
	player_->Initialize(modelPlayer_, &camera_, modelBullet_);

	// エイムの初期化
	aim_ = new Aim();
	aim_->Initialize(&camera_);
}

void GameScene::Update() {
	aim_->Update();

	if(aim_->IsAttac()) {
		Vector3 worldPos = GetMouseWorldPosition();

		player_->Attack({ worldPos.x, worldPos.y });
	}

	// プレイヤーの更新
	player_->Update();

	if(input_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}
}

void GameScene::Draw() {
	// コマンドリストの取得
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// UI描画前処理
	Sprite::PreDraw(commandList);

	// エイムの描画
	aim_->Draw();

	// UI描画後処理
	Sprite::PostDraw();

	// 3Dオブジェクト描画前処理
	Model::PreDraw();

	// プレイヤーの描画
	player_->Draw();

	// 3Dオブジェクト後処理
	Model::PostDraw();
}

Vector3 GameScene::GetMouseWorldPosition() {
	Ray ray = aim_->GetRayFromMouse();

	// z = 0 の平面との交点
	float t = -ray.origin.z / ray.direction.z;

	return { ray.origin.x + ray.direction.x * t, ray.origin.y + ray.direction.y * t, 0.0f };
}
