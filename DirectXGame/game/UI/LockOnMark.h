#pragma once
#include "KamataEngine.h"

// 前方宣言
class Enemy;

class LockOnMark {
public:

	// 初期化
    void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, Enemy* target);
   
	// 更新
    void Update();
    
	// 描画
    void Draw();

    Enemy* GetTarget() const {
        return target_;
    }

private:
	// モデル
    KamataEngine::Model* model_ = nullptr;

	// カメラ
    KamataEngine::Camera* camera_ = nullptr;

	// ワールド変換データ
    KamataEngine::WorldTransform worldTransform_;
    
    // マークのオフセット位置
	float offset_ = 5.0f;

    //
    Enemy* target_ = nullptr;
};