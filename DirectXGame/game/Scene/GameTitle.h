#pragma once
#include "KamataEngine.h"
#include "engine/sceneEngine/SceneBase.h"
#include "UI/Aim.h"
#include "Skydome/Skydome.h"
#include "Grand/Ground.h"

using namespace KamataEngine;

// ゲームシーン
class GameTitle : public SceneBase {
public:
	GameTitle(KamataEngine::Input* input) : SceneBase(input) { }

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// マウスがボタンの上にあるか判定
	bool IsMouseOver(KamataEngine::Vector2 mouse, KamataEngine::Vector2 pos, KamataEngine::Vector2 size);

	~GameTitle();

private:

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera camera_;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// ボタンの状態
	enum class ButtonState { None, Tutorial, StageSelect };
	ButtonState buttonState_ = ButtonState::None;

	// ステージ選択
	int stageIndex_ = 0;
	static const int kMaxStage = 5;

	// 次に表示するステージ
	int nextStage_ = 0;

	// スライド中か
	bool isSliding_ = false;

	// スライドタイマー
	float slideTimer_ = 0.0f;

	// スライド時間
	const float kSlideDuration_ = 3.0f;

	// スライド方向
	int slideDirection_ = 1; // 右:1 左:-1

	// タイトルのUI
	uint32_t UI_titleTH;
	KamataEngine::Sprite* UI_title;

	// 照準
	Aim* aim_ = nullptr;

	// 
	void UpdateUI();

	// 天球
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelSkydome_;

	// 地面
	Ground* ground_ = nullptr;
	KamataEngine::Model* modelGround_;

	// ウィンドウの幅と高さ
	float windowWidth = 1280.0f;
	float windowHeight = 720.0f;

	float buttonTimer_ = 0.0f;

	// ステージ画像
	uint32_t stageTH_[kMaxStage];

	// ステージ画像スプライト
	KamataEngine::Sprite* stageSprites_[kMaxStage];

	// 進むボタン
	KamataEngine::Sprite* startButtonSprite_ = nullptr;
	KamataEngine::Vector2 startButtonSize = { 256.0f, 128.0f };
	KamataEngine::Vector2 startButtonPos = { windowWidth / 2.0f, windowHeight / 1.25f };
	uint32_t startButtonSpriteTH_ = 0;

	// チュートリアルボタン
	KamataEngine::Sprite* tutorialButtonSprite_ = nullptr;
	KamataEngine::Vector2 tutorialButtonSize = { 256.0f, 128.0f };
	KamataEngine::Vector2 tutorialButtonPos = { windowWidth / 1.25f, windowHeight / 1.25f };
	uint32_t tutorialButtonSpriteTH_ = 0;

	// 右ボタンの位置とサイズ
	KamataEngine::Vector2 rightButtonPos = { windowWidth / 1.18f, windowHeight / 2.0f };
	KamataEngine::Vector2 rightButtonSize = { 128.0f, 128.0f };
	KamataEngine::Sprite* rightButtonSprite_ = nullptr;
	uint32_t rightButtonSpriteTH_ = 0;

	// 左ボタンの位置とサイズ
	KamataEngine::Vector2 leftButtonPos = { windowWidth / 6.5f, windowHeight / 2.0f };
	KamataEngine::Vector2 leftButtonSize = { 128.0f, 128.0f };
	KamataEngine::Sprite* leftButtonSprite_ = nullptr;
	uint32_t leftButtonSpriteTH_ = 0;

	// チュートリアル画像
	KamataEngine::Sprite* tutorialImageSprite_ = nullptr;
	uint32_t tutorialImageSpriteTH_ = 0;

	// BGM関連
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;

	// SE（効果音）用ハンドル
	uint32_t clickSoundHandle_ = 0;
};