#include "GameTitle.h"
#include "engine/sceneEngine/SceneManager.h"

void GameTitle::Initialize() {
	// エイムの初期化
	aim_ = new Aim();
	aim_->Initialize(&camera_);
}

void GameTitle::Finalize() { 
	delete aim_;
}

void GameTitle::Update() {
	if(Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

	
	aim_->Update();
}

void GameTitle::Draw() {

	// コマンドリストの取得
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	
	// UI描画前処理
	Sprite::PreDraw(commandList);
	aim_->Draw();

	// UI描画後処理
	Sprite::PostDraw();
}