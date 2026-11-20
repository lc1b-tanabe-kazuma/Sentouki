#include "TutorialState.h"
#include "GameTitle.h"
#include "NoneState.h"

void TutorialState::Update(GameTitle* gameTitle) {

	// スタートボタンの位置を右下にする
	gameTitle->startButtonPos = {
		gameTitle->windowWidth / 1.15f,
		gameTitle->windowHeight / 1.25f
	};

	gameTitle->startButtonSprite_->SetPosition(
		gameTitle->startButtonPos
	);

	// マウスがスタートボタンの上にあるか
	KamataEngine::Vector2 mousePos =
		gameTitle->aim_->GetUIMousePosition();

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

			Audio::GetInstance()->PlayWave(
				gameTitle->clickSoundHandle_, false, 0.1f
			);

			// 通常のタイトル画面へ戻る
			gameTitle->ChangeState(
				std::make_unique<NoneState>()
			);

			// スタートボタンの位置を中央に戻す
			gameTitle->startButtonPos = {
				gameTitle->windowWidth / 2.0f,
				gameTitle->windowHeight / 1.25f
			};

			gameTitle->startButtonSprite_->SetPosition(
				gameTitle->startButtonPos
			);
		}

	} else {

		gameTitle->startButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}
}

void TutorialState::Draw(GameTitle* gameTitle) {

	// ルール説明画像
	gameTitle->tutorialImageSprite_->Draw();

	// スタートボタン
	gameTitle->startButtonSprite_->Draw();
}