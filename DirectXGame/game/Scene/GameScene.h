#pragma once
#include "KamataEngine.h"
#include "engine/sceneEngine/SceneBase.h"
#include "UI/Aim.h"
#include "player/Player.h"
#include "enemy/Enemy.h"
#include "Skydome/Skydome.h"
#include "Grand/Ground.h"
#include "UI/LockOnMark.h"

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

	// 敵のスクリプトファイル読み込み
	void LoadEnemyPopData();

	// 敵のスクリプト実行
	void UpdateEnemyPopcomand();

	// 敵の出現待機中
	bool IsWaiting() const { return isWaiting_; }

	// 敵が全員WAITコマンドの時間が0になっていてかつ全員デリートされたか
	bool IsAllEnemiesWaited() const { return enemies_.empty() && !isWaiting_ && enemyPopComands.eof(); }

	void OnCollision();

	std::list<Enemy*> FindLockOnEnemies(const Ray& ray);

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

	// 敵を複数化(リスト)
	std::list<Enemy*> enemies_;
	KamataEngine::Model* modelEnemy_ = nullptr;

	// ロックオンしている敵
	std::list<Enemy*> lockOnTargets_;

	// ロックマンマーク
	KamataEngine::Model* modelLockOn_ = nullptr;
	std::list<LockOnMark*> lockOnMarks_;

	// 敵の発生コマンド
	std::stringstream enemyPopComands;

	// 敵の出現の待機中フラグ
	bool isWaiting_ = false;

	// 敵の出現の待機タイマー
	int32_t waitTimer_ = 0;

	// 天球
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;

	// 地面
	Ground* ground_ = nullptr;
	KamataEngine::Model* modelGround_ = nullptr;
};