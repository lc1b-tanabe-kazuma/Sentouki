#define NOMINMAX
#include "Scene/GameScene.h"
#include "engine/sceneEngine/SceneManager.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <numbers>
#include "MyMath.h"

using namespace std;
using namespace KamataEngine;

GameScene::~GameScene() {
	delete player_;
	delete modelPlayer_;
	delete modelBullet_;
	delete aim_;
	for(Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete modelEnemy_;
	delete modelSkydome_;
	delete skydome_;
	delete modelGround_;
	delete ground_;

	// ロックオンマークの解放
	for(LockOnMark* mark : lockOnMarks_) {
		delete mark;
	}

	// ロックオンマークモデルの解放
	delete modelLockOn_;

	// 敵のロックマンリストの解放
	lockOnTargets_.clear();

	// 敵の出現コマンドの解放
	enemyPopComands.str("");
}

void GameScene::Initialize() {

	// カメラの初期化
	camera_.Initialize();

	//
	worldTransform_.Initialize();

	// エイムの初期化
	aim_ = new Aim();
	aim_->Initialize(&camera_);

	// プレイヤーの初期化
	player_ = new Player();
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelBullet_ = Model::CreateFromOBJ("playerBullet", true);
	player_->Initialize(modelPlayer_, &camera_, modelBullet_, aim_);

	// 敵モデル
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	// 敵のロックマンマークモデル
	modelLockOn_ = Model::CreateFromOBJ("mark", true);

	// 天球
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome;
	skydome_->Initialize(modelSkydome_, &camera_);

	// 地面モデルの作成
	modelGround_ = Model::CreateFromOBJ("Ground", true);

	// 地面の生成
	ground_ = new Ground();

	// 地面の初期化
	ground_->Initialize(modelGround_, &camera_);
}

void GameScene::Update() {

	enemies_.remove_if([this](Enemy* enemy) {

		enemy->OutFlag();

		if(enemy->IsDead() || enemy->IsOut()) {

			// ロックオン対象から削除
			lockOnTargets_.remove(enemy);

			// この敵に対応するマークを削除
			lockOnMarks_.remove_if([enemy](LockOnMark* mark) {

				if(mark->GetTarget() == enemy) {
					delete mark;
					return true;
				}

				return false;
				});

			delete enemy;
			return true;
		}

		return false;
		});

	// 敵が全て消えたら最初から出現し直す
	if(enemies_.empty()) {
		// 既存のコマンドストリームをクリアしてファイルから再読み込みする
		LoadEnemyPopData(); // Resources/enemyPopData.csv を再読み込み

		// 待機状態を解除して即実行できるようにする
		waitTimer_ = 0;
	}

	// 照準の更新
	aim_->Update();

	// 現在照準が合っている敵を取得
	Ray ray = aim_->GetRayFromMouse();

	std::list<Enemy*> targets =
		FindLockOnEnemies(ray);

	// ロックオン対象に追加
	for(Enemy* target : targets) {

		// まだロックオンしていなければ追加
		if(std::find(lockOnTargets_.begin(),
			lockOnTargets_.end(),
			target) == lockOnTargets_.end()) {

			// ロックオン対象に追加
			lockOnTargets_.push_back(target);

			// マークを作成
			LockOnMark* mark = new LockOnMark();

			// このマークが追いかける敵を指定
			mark->Initialize(modelLockOn_, &camera_, target);

			// マークをリストに追加
			lockOnMarks_.push_back(mark);
		}
	}

	// 攻撃する時
	if(aim_->IsAttac()) {

		// ロックオンした敵すべてに弾を発射
		for(Enemy* target : lockOnTargets_) {
			player_->Attack(target);
		}

		// 発射したらロックオン解除
		lockOnTargets_.clear();
	}

#ifdef DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("rote", &camera_.rotation_.x, 0.1f);
	ImGui::DragFloat3("transe", &camera_.translation_.x, 0.1f);
	ImGui::End();
	camera_.UpdateMatrix();
#endif // DEBUG

	// 敵のスクリプト実行
	UpdateEnemyPopcomand();

	// 敵キャラの更新
	for(Enemy* enemy : enemies_) {
		enemy->Update();
	}

	// プレイヤーの更新
	player_->Update();

	// 当たり判定
	OnCollision();

	// ロックオンマークの更新
	for(LockOnMark* mark : lockOnMarks_) {
		mark->Update();
	}

	ground_->Update();
	skydome_->Update();

	if(input_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GameClear");
	}
}

Vector3 GameScene::GetMouseWorldPosition() {
	Ray ray = aim_->GetRayFromMouse();

	// z = 0 の平面との交点
	float t = -ray.origin.z / ray.direction.z;

	return { ray.origin.x + ray.direction.x * t, ray.origin.y + ray.direction.y * t, 0.0f };
}

// 敵のスクリプトファイル読み込み
void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	ifstream file;
	file.open("Resources/enemy/enemyPopData.csv");

#ifdef DEBUG
	assert(file.is_open());
#endif // DEBUG

	// ファイルの内容を文字列ストリームにコピー
	enemyPopComands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

// 敵のスクリプト実行
void GameScene::UpdateEnemyPopcomand() {

	// 待機処理
	if(isWaiting_) {
		// 待機タイマーをデクリメント
		waitTimer_--;
		// タイマーが0になったら待機終了
		if(waitTimer_ <= 0) {
			isWaiting_ = false;
		}
		// 待機中は他の処理をしない
		return;
	}

	// 1行分の文字列を入れる変数
	string line;

	// コマンド実行ループ
	while(getline(enemyPopComands, line)) {
		// 1行分の文字列を文字列ストリームに変換
		stringstream line_stream(line);

		string word;
		// カンマ区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始める行はコメント
		if(word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if(word.find("POP") == 0) {
			// 敵の生成
			// x座標
			getline(line_stream, word, ',');
			float x = (float)atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)atof(word.c_str());

			// 移動ベクトルの取得
			// 移動ベクトルX
			getline(line_stream, word, ',');
			float vx = (float)atof(word.c_str());

			// 移動ベクトルY
			getline(line_stream, word, ',');
			float vy = (float)atof(word.c_str());

			// 移動ベクトルZ
			getline(line_stream, word, ',');
			float vz = (float)atof(word.c_str());

			// 敵を発生させる
			Enemy* newEnemies = new Enemy();
			newEnemies->Initialize(modelEnemy_, &camera_);
			newEnemies->SetPosition(Vector3(x, y, z));
			newEnemies->SetMoveVector(Vector3(vx, vy, vz));

			enemies_.push_back(newEnemies);

			// デバッグ出力
			OutputDebugStringA(("Enemy Spawned at: " + to_string(x) + "," + to_string(y) + "," + to_string(z) + "\n").c_str());
		}

		// WAITコマンド
		else if(word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			// 待ち時間
			int32_t waitFrame = atoi(word.c_str());

			// 待機開始
			isWaiting_ = true;

			// 待機タイマーをセット
			waitTimer_ = waitFrame;

			// ループを抜ける
			break;
		}
	}
}

std::list<Enemy*> GameScene::FindLockOnEnemies(const Ray& ray) {

	std::list<Enemy*> targets;

	// レイから敵までの許容距離
	const float lockOnRange = 3.0f;

	for(Enemy* enemy : enemies_) {

		// 死んでいる敵は対象外
		if(enemy->IsDead()) {
			continue;
		}

		Vector3 enemyPos = enemy->GetWorldPosition();

		// レイの始点から敵へのベクトル
		Vector3 toEnemy =
			enemyPos - ray.origin;

		// レイ方向に対して敵がどれくらい先にいるか
		float distance =
			Dot(toEnemy, ray.direction);

		// カメラより後ろにいる敵は無視
		if(distance < 0.0f) {
			continue;
		}

		// レイ上の最近点
		Vector3 closestPoint =
			ray.origin + ray.direction * distance;

		// レイから敵までの距離
		Vector3 difference =
			enemyPos - closestPoint;

		float distanceFromRay =
			Length(difference);

		// レイから一定距離以内ならロックオン候補
		if(distanceFromRay <= lockOnRange) {
			targets.push_back(enemy);
		}
	}

	return targets;
}

void GameScene::OnCollision() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自機の弾リスト取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

#pragma region プレイヤーの弾と敵の当たり判定
	// 敵の座標を取得
	for(Enemy* enemy : enemies_) {
		posA = enemy->GetWorldPosition();

		// プレイヤーの弾の座標を取得
		for(PlayerBullet* bullet : playerBullets) {
			posB = bullet->GetPosition();
			if(IsCollision(posA, enemy->GetRadius(), posB, bullet->GetRadius())) {

				// ---- 通常弾 ----
				bullet->OnCollision();
				enemy->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::Draw() {

	// コマンドリストの取得
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw();

	ground_->Draw();
	skydome_->Draw();

	player_->Draw();

	for(Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	for(LockOnMark* mark : lockOnMarks_) {
		mark->Draw();
	}

	// 3Dオブジェクト後処理
	Model::PostDraw();

	// UI描画前処理
	Sprite::PreDraw(commandList);

	// エイムの描画
	aim_->Draw();

	// UI描画後処理
	Sprite::PostDraw();
}