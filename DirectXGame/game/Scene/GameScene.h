#pragma once
#include "KamataEngine.h"
#include "engine/sceneEngine/SceneBase.h"
#include "Aim.h"
#include "player/Player.h"

using namespace KamataEngine;

// ゲームシーン
class GameScene : public SceneBase {
public:
	GameScene(KamataEngine::Input* input) : SceneBase(input) { }

	~GameScene() override;

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	KamataEngine::Vector3 GetMouseWorldPosition();

private:

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera camera_;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// UI
	Aim* aim_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelBullet_ = nullptr;
};