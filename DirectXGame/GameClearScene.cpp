#include "GameClearScene.h"
#include "math.h"
#include <cstdlib>
#include <ctime>

using namespace KamataEngine;


void GameClearScene::Initialize(Score* score) 
{ 
	//スコア
	score_ = score; 

	// フェードとフェード管理の初期化
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	phase_ = Phase::kFadeIn;

	timer = 0;
}

GameClearScene::~GameClearScene() {}


void GameClearScene::Update() {

	fade_->Update();
	switch (phase_) {
	case GameClearScene::Phase::kFadeIn:
		if (fade_->isFinished()) {
			phase_ = Phase::kMain; // フェードイン完了 -> メインフェーズへ
		}
		break;
	case GameClearScene::Phase::kMain:
		if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			isFinish = true;
		}
		break;
	case GameClearScene::Phase::kfadeOut:
		if (fade_->isFinished()) {
			// 2秒後にシーン転移
			timer++;
			if (timer > 120) {
				isFinish = true;
			}
		}

		break;
	default:
		break;
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
	// フェード
	fade_->Draw(dxCommon->GetCommandList());

	// 2d描画
	Sprite::PreDraw(dxCommon->GetCommandList());

	score_->Draw();
	score_->RankingDraw();

	Sprite::PostDraw();

}
