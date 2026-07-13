#include "GameScene.h"
#include "SceneManager.h"

void GameScene::Initialize() {

}

void GameScene::Finalize() { }

void GameScene::Update() {
	if(input_->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}
}

void GameScene::Draw() { }