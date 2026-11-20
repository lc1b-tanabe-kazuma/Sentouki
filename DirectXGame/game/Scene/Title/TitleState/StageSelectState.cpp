#include "StageSelectState.h"
#include "GameTitle.h"
#include "engine/sceneEngine/SceneManager.h"

void StageSelectState::Update(GameTitle* gameTitle) {

	// マウス座標
	KamataEngine::Vector2 mousePos =
		gameTitle->aim_->GetUIMousePosition();

	// ステージ画像のスライド処理
	if(gameTitle->isSliding_) {

		gameTitle->slideTimer_ += 1.0f / 30.0f;

		// スライド終了
		if(gameTitle->slideTimer_ >= gameTitle->kSlideDuration_) {

			gameTitle->slideTimer_ = gameTitle->kSlideDuration_;
			gameTitle->stageIndex_ = gameTitle->nextStage_;
			gameTitle->isSliding_ = false;
		}
	}

	// 各ボタンのマウス判定
	bool isRightHover = gameTitle->IsMouseOver(
		mousePos,
		gameTitle->rightButtonPos,
		gameTitle->rightButtonSize
	);

	bool isLeftHover = gameTitle->IsMouseOver(
		mousePos,
		gameTitle->leftButtonPos,
		gameTitle->leftButtonSize
	);

	bool isStartHover = gameTitle->IsMouseOver(
		mousePos,
		gameTitle->startButtonPos,
		gameTitle->startButtonSize
	);

	// 右ボタン
	if(isRightHover) {

		gameTitle->buttonTimer_ += 1.0f / 30.0f;

		float alpha =
			(sin(gameTitle->buttonTimer_ * 3.14159f * 2.0f) + 1.0f)
			/ 2.0f * 0.5f + 0.5f;

		gameTitle->rightButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, alpha }
		);

		// 左クリック
		if(gameTitle->GetInput()->IsTriggerMouse(0)
			&& !gameTitle->isSliding_) {

			gameTitle->nextStage_ = gameTitle->stageIndex_ + 1;

			// 最大ステージを超えたら最初に戻す
			if(gameTitle->nextStage_ >= GameTitle::kMaxStage) {
				gameTitle->nextStage_ = 0;
			}

			gameTitle->slideDirection_ = 1;
			gameTitle->isSliding_ = true;
			gameTitle->slideTimer_ = 0.0f;
		}

	} else {

		gameTitle->rightButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}

	// 左ボタン
	if(isLeftHover) {

		gameTitle->buttonTimer_ += 1.0f / 30.0f;

		float alpha =
			(sin(gameTitle->buttonTimer_ * 3.14159f * 2.0f) + 1.0f)
			/ 2.0f * 0.5f + 0.5f;

		gameTitle->leftButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, alpha }
		);

		// 左クリック
		if(gameTitle->GetInput()->IsTriggerMouse(0)
			&& !gameTitle->isSliding_) {

			gameTitle->nextStage_ = gameTitle->stageIndex_ - 1;

			// 0未満なら最後のステージに戻す
			if(gameTitle->nextStage_ < 0) {
				gameTitle->nextStage_ = GameTitle::kMaxStage - 1;
			}

			gameTitle->slideDirection_ = -1;
			gameTitle->isSliding_ = true;
			gameTitle->slideTimer_ = 0.0f;
		}

	} else {

		gameTitle->leftButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}

	// スタートボタン
	if(isStartHover) {

		gameTitle->buttonTimer_ += 1.0f / 30.0f;

		float alpha =
			(sin(gameTitle->buttonTimer_ * 3.14159f * 2.0f) + 1.0f)
			/ 2.0f * 0.5f + 0.5f;

		gameTitle->startButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, alpha }
		);

		// スライド中でなければゲーム開始
		if(gameTitle->GetInput()->IsTriggerMouse(0)
			&& !gameTitle->isSliding_) {

			SceneManager::GetInstance()->SetStage(
				gameTitle->stageIndex_
			);

			SceneManager::GetInstance()->ChangeScene("Game");
		}

	} else {

		gameTitle->startButtonSprite_->SetColor(
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}
}

void StageSelectState::Draw(GameTitle* gameTitle) {

	// 右ボタン
	gameTitle->rightButtonSprite_->Draw();

	// 左ボタン
	gameTitle->leftButtonSprite_->Draw();

	if(gameTitle->isSliding_) {

		float t =
			gameTitle->slideTimer_ / gameTitle->kSlideDuration_;

		// 現在の画像が画面外へ移動
		float currentX =
			640.0f - (t * 1280.0f * gameTitle->slideDirection_);

		// 次の画像が画面中央へ移動
		float nextX =
			640.0f + ((1.0f - t) * 1280.0f
				* gameTitle->slideDirection_);

		gameTitle->stageSprites_[gameTitle->stageIndex_]->SetPosition(
			{ currentX, 360.0f }
		);

		gameTitle->stageSprites_[gameTitle->stageIndex_]->Draw();

		gameTitle->stageSprites_[gameTitle->nextStage_]->SetPosition(
			{ nextX, 360.0f }
		);

		gameTitle->stageSprites_[gameTitle->nextStage_]->Draw();

	} else {

		gameTitle->stageSprites_[gameTitle->stageIndex_]->SetPosition(
			{ 640.0f, 360.0f }
		);

		gameTitle->stageSprites_[gameTitle->stageIndex_]->Draw();
	}

	// スタートボタン
	gameTitle->startButtonSprite_->Draw();
}