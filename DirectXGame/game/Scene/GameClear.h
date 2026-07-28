#pragma once
#include "KamataEngine.h"
#include "engine/sceneEngine/SceneBase.h"
#include "Skydome/Skydome.h"
#include "Grand/Ground.h"

class GameClear : public SceneBase {
public:

	GameClear(KamataEngine::Input* input) : SceneBase(input) { };

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	~GameClear() override;

private:

	// カメラ
	KamataEngine::Camera camera_;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelSkydome_;

	// 地面
	Ground* ground_ = nullptr;
	KamataEngine::Model* modelGround_;
}; 