#include "GameTitle.h"
#include "engine/sceneEngine/SceneManager.h"
#include "imgui.h"
#include "TitleState/NoneState.h"
#include "TitleState/TutorialState.h"
#include "TitleState/StageSelectState.h"

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

	// ステージ画像
	stageTH_[0] = TextureManager::Load("uvChecker.png");
	stageTH_[1] = TextureManager::Load("uvChecker.png");
	stageTH_[2] = TextureManager::Load("uvChecker.png");
	stageTH_[3] = TextureManager::Load("uvChecker.png");
	stageTH_[4] = TextureManager::Load("uvChecker.png");

	// ステージ画像スプライトの作成
	for(int i = 0; i < kMaxStage; i++) {
		stageSprites_[i] = Sprite::Create(stageTH_[i], { 64.0f, 64.0f });
		stageSprites_[i]->SetSize({ 512.0f, 256.0f });
		stageSprites_[i]->SetAnchorPoint({ 0.5f, 0.5f });
	}

	// 進むボタンの初期化
	startButtonSpriteTH_ = TextureManager::Load("UI/Start.png");
	startButtonSprite_ = Sprite::Create(
		startButtonSpriteTH_, startButtonPos, // 初期位置
		Vector4(1.0f, 1.0f, 1.0f, 1.0f)       // 色
	);
	startButtonSprite_->SetSize(startButtonSize);
	startButtonSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	// チュートリアルボタンの初期化
	tutorialButtonSpriteTH_ = TextureManager::Load("UI/tutorial.png");
	tutorialButtonSprite_ = Sprite::Create(
		tutorialButtonSpriteTH_, tutorialButtonPos, // 初期位置
		Vector4(1.0f, 1.0f, 1.0f, 1.0f)             // 色
	);
	tutorialButtonSprite_->SetSize(tutorialButtonSize);
	tutorialButtonSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	// 右ボタンのスプライトを設定
	rightButtonSpriteTH_ = TextureManager::Load("UI/UI_arrow.png");
	rightButtonSprite_ = Sprite::Create(
		rightButtonSpriteTH_, rightButtonPos, // 初期位置
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),      // 色
		Vector2(0.5f, 0.5f)                   // 中央を基準にする
	);
	rightButtonSprite_->SetSize(rightButtonSize);
	rightButtonSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	// 左ボタンのスプライトを設定
	leftButtonSpriteTH_ = TextureManager::Load("UI/UI_leftArrow.png");
	leftButtonSprite_ = Sprite::Create(
		leftButtonSpriteTH_, leftButtonPos, // 初期位置
		Vector4(1.0f, 1.0f, 1.0f, 1.0f),    // 色
		Vector2(0.5f, 0.5f)                 // 中央を基準にする
	);
	leftButtonSprite_->SetSize(leftButtonSize);
	leftButtonSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	// チュートリアル画像の初期化
	tutorialImageSpriteTH_ = TextureManager::Load("UI/rule.png");
	tutorialImageSprite_ = Sprite::Create(
		tutorialImageSpriteTH_, { 0.0f, 0.0f }, // 初期位置
		Vector4(1.0f, 1.0f, 1.0f, 1.0f)       // 色
	);

	// チュートリアル画像のサイズをウィンドウサイズに合わせる
	tutorialImageSprite_->SetSize({ windowWidth, windowHeight });

	// BGMの読み込み
	soundDataHandle_ = Audio::GetInstance()->LoadWave("audio/BGM/title.wav");
	// BGM再生
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true, 0.025f);

	// 決定音の読み込み
	clickSoundHandle_ = Audio::GetInstance()->LoadWave("audio/SE/kettei.wav");

	// 最初のStateを通常タイトル画面にする
	currentState_ = std::make_unique<NoneState>();
}

void GameTitle::Update() {

	// 照準の更新
	aim_->Update();

	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

#ifdef _DEBUG
	ImGui::Begin("stageNumber");
	ImGui::Text("Current Stage: %d", stageIndex_);
	ImGui::End();
#endif

	// 現在のStateを更新
	if(currentState_) {
		currentState_->Update(this);
	}

	// Stateの切り替え
	if(pendingState_) {
		currentState_ = std::move(pendingState_);
	}
}

// マウスがボタンの上にあるか判定
bool GameTitle::IsMouseOver(Vector2 mouse, Vector2 pos, Vector2 size) {
	return (mouse.x >= pos.x - size.x / 2 && mouse.x <= pos.x + size.x / 2 && mouse.y >= pos.y - size.y / 2 && mouse.y <= pos.y + size.y / 2);
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

	// 現在のStateで描画
	if(currentState_) {
		currentState_->Draw(this);
	}

	// 照準の描画
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
	delete startButtonSprite_;
	delete tutorialButtonSprite_;
	delete tutorialImageSprite_;
	delete rightButtonSprite_;
	delete leftButtonSprite_;
	for(int i = 0; i < kMaxStage; i++) {
		delete stageSprites_[i];
	}

	// シーン遷移前にタイトルBGMを停止
	Audio::GetInstance()->StopWave(voiceHandle_);
}