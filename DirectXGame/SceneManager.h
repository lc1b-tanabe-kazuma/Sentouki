#pragma once
#include "SceneBase.h"
#include "SceneTransition.h"
#include <map>
#include <memory>
#include <string>

class SceneManager {
public:
	// シングルトンインスタンス取得
	static SceneManager* GetInstance();

	// 初期化
	void Initialize();
	// 終了
	void Finalize();

	/// <summary>
	/// シーンの登録
	/// </summary>
	/// <param name="name"> シーンの名前 </param>
	/// <param name="scene"> シーンのクラス </param>
	void RegisterScene(const std::string& name, std::unique_ptr<SceneBase> scene);

	/// <summary>
	/// シーンの切り替え
	/// </summary>
	/// <param name="name"> 次のシーン </param>
	void ChangeScene(const std::string& name);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetStage(int stage) { stageNumber_ = stage; }

	int GetStage() const { return stageNumber_; }

	void SetScore(int score) { score_ = score; }

	int GetScore() const { return score_; }

private:
	// シーン保存用
	std::map<std::string, std::unique_ptr<SceneBase>> scenes_;
	// 現在のシーン
	SceneBase* currentScene_ = nullptr;
	// Input
	KamataEngine::Input* input_ = nullptr;

	// トランジション管理
	SceneTransition* transition_ = nullptr;

	// シングルトン用
	static SceneManager* instance;
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	int stageNumber_ = 0;
	int score_ = 0;
};