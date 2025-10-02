#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {}

void GameScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isFinish = true;
	}

#ifdef _DEBUG
	ImGui::Begin("Game Scene");
	ImGui::End();
#endif
}

void GameScene::Draw() {}
