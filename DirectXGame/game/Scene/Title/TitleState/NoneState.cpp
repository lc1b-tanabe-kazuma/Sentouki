#include "NoneState.h"
#include "GameTitle.h"
#include "TutorialState.h"
#include "StageSelectState.h"

void NoneState::Update(GameTitle* gameTitle) {

	// マウス座標を取得
	KamataEngine::Vector2 mousePos =
		gameTitle->aim_->GetUIMousePosition();

	// チュートリアルボタンの判定
	bool isTutorialHover = gameTitle->IsMouseOver(
		mousePos,
		gameTitle->tutorialButtonPos,
		gameTitle->tutorialButtonSize
	);

	if(isTutorialHover) {

		// ボタンの点滅
		gameTitle->buttonTimer_ += 1.0f / 30.0f;

		float alpha =
			(sin(gameTitle->buttonTimer_ * 3.14159f * 2.0f) + 1.0f)
			/ 2.0f * 0.5f + 0.5f;

		gameTitle->tutorialButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, alpha }
		);

		// 左クリック
		if(gameTitle->GetInput()->IsTriggerMouse(0)) {

			KamataEngine::Audio::GetInstance()->PlayWave(
				gameTitle->clickSoundHandle_, false, 0.1f
			);

			// チュートリアル画面へ
			gameTitle->ChangeState(
				std::make_unique<TutorialState>()
			);
		}

	} else {

		gameTitle->tutorialButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}

	// スタートボタンの判定
	bool isStartHover = gameTitle->IsMouseOver(
		mousePos,
		gameTitle->startButtonPos,
		gameTitle->startButtonSize
	);

	if(isStartHover) {

		// ボタンの点滅
		gameTitle->buttonTimer_ += 1.0f / 30.0f;

		float alpha =
			(sin(gameTitle->buttonTimer_ * 3.14159f * 2.0f) + 1.0f)
			/ 2.0f * 0.5f + 0.5f;

		gameTitle->startButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, alpha }
		);

		// 左クリック
		if(gameTitle->GetInput()->IsTriggerMouse(0)) {

			KamataEngine::Audio::GetInstance()->PlayWave(
				gameTitle->clickSoundHandle_, false, 0.1f
			);

			// タイトルBGMを停止
			KamataEngine::Audio::GetInstance()->StopWave(
				gameTitle->voiceHandle_
			);

			// ステージ選択画面へ
			gameTitle->ChangeState(
				std::make_unique<StageSelectState>()
			);
		}

	} else {

		gameTitle->startButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}
}

void NoneState::Draw(GameTitle* gameTitle) {

	// チュートリアルボタン
	gameTitle->tutorialButtonSprite_->Draw();

	// スタートボタン
	gameTitle->startButtonSprite_->Draw();
}