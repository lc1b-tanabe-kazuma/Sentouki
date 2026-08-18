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

	~GameTitle();

private:

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera camera_;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	uint32_t UI_titleTH;
	KamataEngine::Sprite* UI_title;

	Aim* aim_ = nullptr;

	void UpdateUI();

	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelSkydome_;

	// 地面
	Ground* ground_ = nullptr;
	KamataEngine::Model* modelGround_;
};