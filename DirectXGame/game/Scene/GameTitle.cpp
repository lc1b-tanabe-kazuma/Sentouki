#include "GameTitle.h"
#include "engine/sceneEngine/SceneManager.h"

void GameTitle::Initialize() {
	//
	camera_.Initialize();

	// エイムの初期化
	aim_ = new Aim();
	aim_->Initialize(&camera_);

	//
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome;
	skydome_->Initialize(modelSkydome_, &camera_);

	// 地面モデルの作成
	modelGround_ = Model::CreateFromOBJ("Ground", true);

	// 地面の生成
	ground_ = new Ground();

	// 地面の初期化
	ground_->Initialize(modelGround_, &camera_);
}

void GameTitle::Update() {
	if(Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

	// 照準の更新
	aim_->Update();

	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();
}

void GameTitle::Draw() {

	// コマンドリストの取得
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw();

	// 地面の描画
	ground_->Draw();

	// 天球の描画
	skydome_->Draw();

	Model::PostDraw();

	// UI描画前処理
	Sprite::PreDraw(commandList);
	aim_->Draw();

	// UI描画後処理
	Sprite::PostDraw();
}

GameTitle::~GameTitle() {
	delete aim_;
	delete skydome_;
	delete modelSkydome_;
	delete ground_;
	delete modelGround_;
}