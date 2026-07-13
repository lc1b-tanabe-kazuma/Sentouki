#include "GameTitle.h"
#include "SceneManager.h"

void GameTitle::Initialize() {

}

void GameTitle::Finalize() { }

void GameTitle::Update() {
	if(Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}
}

void GameTitle::Draw() { }