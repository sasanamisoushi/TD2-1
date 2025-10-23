#include "GameClearScene.h"
#include "math.h"
#include <cstdlib>
#include <ctime>

using namespace KamataEngine;


void GameClearScene::Initialize(Score* score) 
{ 
	score_ = score; 
}

GameClearScene::~GameClearScene() {}


void GameClearScene::Update() {
	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isFinish = true;
	}

#ifdef _DEBUG
	ImGui::Begin("Game Clear Scene");
	ImGui::End();
#endif
}

void GameClearScene::Draw()
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	Model::PostDraw();

	// 2d描画
	Sprite::PreDraw(dxCommon->GetCommandList());

	score_->Draw();
	score_->RankingDraw();

	Sprite::PostDraw();

}
