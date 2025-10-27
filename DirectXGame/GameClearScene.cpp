#include "GameClearScene.h"
#include "math.h"
#include <cstdlib>
#include <ctime>

using namespace KamataEngine;


void GameClearScene::Initialize(Score* score) 
{ 
	score_ = score; 
	bgm_ = new BGM();
	bgm_->Initialize();
}

GameClearScene::~GameClearScene()
{
	delete bgm_; 
}


void GameClearScene::Update() 
{
	bgm_->gameClearBGMPlay();
	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) 
	{
		isFinish = true;
		bgm_->gameClearBGMStop();
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
