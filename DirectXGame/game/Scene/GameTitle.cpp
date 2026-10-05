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
}

void GameTitle::Update() {

	// 照準の更新
	aim_->Update();

	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

	Vector2 mousePos_ = aim_->GetWorldPosition();

	switch(buttonState_) {
	case GameTitle::ButtonState::None:
	{

		// チュートリアルボタンの上にマウスがあるか
		bool isTutorialHover = IsMouseOver(mousePos_, tutorialButtonPos, tutorialButtonSize);
		if(isTutorialHover) {
			// ボタンの色を点滅させる
			buttonTimer_ += 1.0f / 30.0f; // タイマーを進める
			float alpha = (sin(buttonTimer_ * 3.14159f * 2.0f) + 1.0f) / 2.0f * 0.5f + 0.5f;
			tutorialButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

			// 左クリック
			if(input_->IsTriggerMouse(0)) {
				// 決定音を単発再生
				Audio::GetInstance()->PlayWave(clickSoundHandle_, false, 0.1f);

				// チュートリアルボタンが押された場合、チュートリアル画像を表示する
				buttonState_ = ButtonState::Tutorial;
			}
		} else {
			// 色を元に戻す
			tutorialButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		// スタートボタンの上にマウスがあるか
		bool isStartHover = IsMouseOver(mousePos_, startButtonPos, startButtonSize);

		if(isStartHover) {

			// ボタンの色を点滅させる
			buttonTimer_ += 1.0f / 30.0f; // タイマーを進める
			float alpha = (sin(buttonTimer_ * 3.14159f * 2.0f) + 1.0f) / 2.0f * 0.5f + 0.5f;
			startButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

			// 左クリック
			if(input_->IsTriggerMouse(0)) {
				// 決定音を単発再生
				Audio::GetInstance()->PlayWave(clickSoundHandle_, false, 0.1f);

				// シーン遷移前にタイトルBGMを停止
				Audio::GetInstance()->StopWave(voiceHandle_);

				// ゲームシーンに遷移
				SceneManager::GetInstance()->ChangeScene("Game");
			}
		} else {
			// 色を元に戻す
			startButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
		break;
	}
	case GameTitle::ButtonState::Tutorial:
	{

		// スタートボタンの上にマウスがあるか
		bool isStartHover = IsMouseOver(mousePos_, startButtonPos, startButtonSize);

		// スタートボタンの場所を右下にする
		startButtonPos = { windowWidth / 1.15f, windowHeight / 1.25f };
		startButtonSprite_->SetPosition(startButtonPos);

		if(isStartHover) {

			// ボタンの色を点滅させる
			buttonTimer_ += 1.0f / 30.0f; // タイマーを進める
			float alpha = (sin(buttonTimer_ * 3.14159f * 2.0f) + 1.0f) / 2.0f * 0.5f + 0.5f;
			startButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

			// 左クリック
			if(input_->IsTriggerMouse(0)) {

				// 決定音を単発再生
				Audio::GetInstance()->PlayWave(clickSoundHandle_, false, 0.1f);

				// チュートリアル画像を閉じる
				buttonState_ = ButtonState::None;

				// スタートボタンの場所を中央に戻す
				startButtonPos = { windowWidth / 2.0f, windowHeight / 1.25f };
				startButtonSprite_->SetPosition(startButtonPos);
			}
		} else {
			// 色を元に戻す
			startButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
		break;
	}
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

	switch(buttonState_) {
	case GameTitle::ButtonState::None:

		// チュートリアルボタンの描画
		tutorialButtonSprite_->Draw();
		break;
	case GameTitle::ButtonState::Tutorial:

		// チュートリアル画像の描画
		tutorialImageSprite_->Draw();
		break;
	}

	// スタートボタンの描画
	startButtonSprite_->Draw();

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

	// シーン遷移前にタイトルBGMを停止
	Audio::GetInstance()->StopWave(voiceHandle_);

}