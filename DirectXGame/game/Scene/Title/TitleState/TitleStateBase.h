#pragma once

class GameTitle;

// タイトル画面の状態の基底クラス
class TitleStateBase {
public:
	virtual ~TitleStateBase() = default;

	// 状態ごとの更新処理
	virtual void Update(GameTitle* gameTitle) = 0;

	// 状態ごとの描画処理
	virtual void Draw(GameTitle* gameTitle) = 0;
};