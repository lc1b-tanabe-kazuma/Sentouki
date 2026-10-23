#include "GameTitle.h"
#include "engine/sceneEngine/SceneManager.h"
#include "imgui.h"

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
}

void GameTitle::Update() {

	// 照準の更新
	aim_->Update();

	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

	Vector2 mousePos_ = aim_->GetUIMousePosition();

	// ステージ画像のスライド処理
	if(isSliding_) {
		slideTimer_ += 1.0f / 30.0f;

		// スライドが終了したら
		if(slideTimer_ >= kSlideDuration_) {
			slideTimer_ = kSlideDuration_;
			stageIndex_ = nextStage_;
			isSliding_ = false;
		}
	}

	// ステージの番号
#ifdef _DEBUG
	ImGui::Begin("stageNumber");
	ImGui::Text("Current Stage: %d", stageIndex_);
	ImGui::End();
#endif // DEBUG

	switch(buttonState_) {
	case GameTitle::ButtonState::None:
	{	// チュートリアルボタンの上にマウスがあるか
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

				// ステージ選択に遷移する
				buttonState_ = ButtonState::StageSelect;
			}
		} else {
			// 色を元に戻す
			startButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
		break;
	}
	case GameTitle::ButtonState::Tutorial:
	{	// スタートボタンの上にマウスがあるか
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
	case GameTitle::ButtonState::StageSelect:
	{
		bool isRightHover = IsMouseOver(mousePos_, rightButtonPos, rightButtonSize);
		bool isLeftHover = IsMouseOver(mousePos_, leftButtonPos, leftButtonSize);
		bool isStartHover = IsMouseOver(mousePos_, startButtonPos, startButtonSize);

		// 右ボタンの上にある時
		if(isRightHover) {
			// ボタンの色を点滅させる
			buttonTimer_ += 1.0f / 30.0f; // タイマーを進める
			float alpha = (sin(buttonTimer_ * 3.14159f * 2.0f) + 1.0f) / 2.0f * 0.5f + 0.5f;
			rightButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

			// 左クリック
			if(input_->IsTriggerMouse(0) && !isSliding_) {
				// ステージ変更
				nextStage_ = stageIndex_ + 1;

				// ステージが最大値を超えた場合は0に戻す
				if(nextStage_ >= kMaxStage) {
					nextStage_ = 0;
				}

				// スライド方向を右に設定
				slideDirection_ = 1;

				// スライド開始
				isSliding_ = true;
				slideTimer_ = 0.0f;
			}
		} else {
			// 色を元に戻す
			rightButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		// 左ボタンの上にある時
		if(isLeftHover) {
			// ボタンの色を点滅させる
			buttonTimer_ += 1.0f / 30.0f; // タイマーを進める
			float alpha = (sin(buttonTimer_ * 3.14159f * 2.0f) + 1.0f) / 2.0f * 0.5f + 0.5f;
			leftButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

			// 左クリック
			if(input_->IsTriggerMouse(0) && !isSliding_) {

				// ステージ変更
				nextStage_ = stageIndex_ - 1;

				// ステージが0未満になった場合は最大値に戻す
				if(nextStage_ < 0) {
					nextStage_ = kMaxStage - 1;
				}
				// スライド方向を左に設定
				slideDirection_ = -1;

				// スライド開始
				isSliding_ = true;
				slideTimer_ = 0.0f;
			}
		} else {
			// 色を元に戻す
			leftButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		// スタートボタンの上にある時
		if(isStartHover) {
			// ボタンの色を点滅させる
			buttonTimer_ += 1.0f / 30.0f; // タイマーを進める
			float alpha = (sin(buttonTimer_ * 3.14159f * 2.0f) + 1.0f) / 2.0f * 0.5f + 0.5f;
			startButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

			// 左クリック
			if(input_->IsTriggerMouse(0) && !isSliding_) {

				// ステージをSceneManagerに設定
				SceneManager::GetInstance()->SetStage(stageIndex_);

				// シーンをゲームシーンに切り替える
				SceneManager::GetInstance()->ChangeScene("Game");
				break;
			}
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
	case GameTitle::ButtonState::StageSelect:

		// 右ボタンの描画
		rightButtonSprite_->Draw();

		// 左ボタンの描画
		leftButtonSprite_->Draw();

		if(isSliding_) {
			float t = slideTimer_ / kSlideDuration_;

			// 現在の画像が画面外へ移動
			float currentX = 640.0f - (t * 1280.0f * slideDirection_);

			// 次の画像が画面中央へ移動
			float nextX = 640.0f + ((1.0f - t) * 1280.0f * slideDirection_);

			stageSprites_[stageIndex_]->SetPosition({ currentX, 360.0f });
			stageSprites_[stageIndex_]->Draw();

			stageSprites_[nextStage_]->SetPosition({ nextX, 360.0f });
			stageSprites_[nextStage_]->Draw();
		} else {
			stageSprites_[stageIndex_]->SetPosition({ 640.0f, 360.0f });
			stageSprites_[stageIndex_]->Draw();
		}
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
	delete rightButtonSprite_;
	delete leftButtonSprite_;
	for(int i = 0; i < kMaxStage; i++) {
		delete stageSprites_[i];
	}

	// シーン遷移前にタイトルBGMを停止
	Audio::GetInstance()->StopWave(voiceHandle_);
}