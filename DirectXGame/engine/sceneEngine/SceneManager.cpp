#include "engine/sceneEngine/SceneManager.h"
#include "Scene/GameScene.h"
#include "Scene/GameTitle.h"

using namespace std;
using namespace KamataEngine;

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance() {
	if(instance == nullptr) {
		instance = new SceneManager;
	}
	return instance;
}

void SceneManager::Initialize() {
	// Inputのインスタンスを取得
	input_ = Input::GetInstance();

	// ここにシーンを羅列
	RegisterScene("Title", std::make_unique<GameTitle>(input_));
	RegisterScene("Game", std::make_unique<GameScene>(input_));

	transition_ = SceneTransition::GetInstance();
	transition_->Initialize();

	// 最初のシーンにタイトルを設定(ChengeSceneで設定しないことでフェードをスキップ)
	auto it = scenes_.find("Title");
	if(it != scenes_.end()) {
		currentScene_ = it->second.get();
		currentScene_->Initialize();
	}
}

void SceneManager::Finalize() {
	delete instance;
	instance = nullptr;
}

void SceneManager::RegisterScene(const std::string& name, std::unique_ptr<SceneBase> scene) { scenes_[name] = std::move(scene); }

void SceneManager::ChangeScene(const std::string& name) {

	// すでにトランジション中ならシーン切り替えを受け付けない
	// 多重遷移防止（バグ防止）
	if(transition_ && transition_->IsTransitioning()) {
		return;
	}

	// フェードトランジション開始
	transition_->StartTransition("Fade", [this, name]() {
		// ===== 現在のシーンの終了処理 =====
		if(currentScene_) {
			// リソース解放や状態リセット
			currentScene_->Finalize();
		}

		// ===== シーンを毎回新しく生成する =====
		// シーンを使い回すと前回の状態（座標・フラグ・コンテナなど）が残る
		// → 2周目以降で「一瞬表示がおかしくなる」原因になる
		// 毎回newすることで完全にクリーンな状態から開始できる
		if(name == "Game") {
			scenes_[name] = std::make_unique<GameScene>(input_);
		} else if(name == "Title") {
			scenes_[name] = std::make_unique<GameTitle>(input_);
		}

		// ===== 新しいシーンに切り替え =====
		currentScene_ = scenes_[name].get();

		// 初期化（オブジェクト生成・リソース読み込みなど）
		currentScene_->Initialize();

		// そのまま描画すると(0,0,0)などが一瞬見える
		// ここで1回Updateを通して状態を確定させる
		currentScene_->Update();
		});
}

void SceneManager::Update() {
	bool isTransitioning = transition_ && transition_->IsTransitioning();

	if(currentScene_) {
		currentScene_->SetActive(!isTransitioning); // ← 追加
		currentScene_->Update();
	}

	// トランジション進行中はトランジションのUpdateのみ
	if(transition_ && transition_->IsTransitioning()) {
		transition_->Update();
	}
}

void SceneManager::Draw() {
	if(currentScene_) {
		currentScene_->Draw();
	}

	if(transition_ && transition_->IsTransitioning()) {
		transition_->Draw();
	}
}