#include "Scene/GameScene.h"
#include "engine/sceneEngine/SceneManager.h"

void GameScene::Initialize() {
	// エイムの初期化
	aim_ = new Aim();
	aim_->Initialize(&camera_);
}

void GameScene::Finalize() {
	delete aim_;
}

void GameScene::Update() {
	aim_->Update();

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
	aim_->Draw();

	// UI描画後処理
	Sprite::PostDraw();
}