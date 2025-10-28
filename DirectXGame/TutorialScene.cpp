#include "TutorialScene.h"
#include "KamataEngine.h"
#include <stdio.h>

using namespace KamataEngine;

TutorialScene::~TutorialScene() 
{
	for (int i = 0; i < 2; i++)
	{
		delete tutorialSprites[i];
		tutorialSprites[i] = nullptr;
	}
}

void TutorialScene::Initialize() {
	// フェードとフェード管理の初期化
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	phase_ = Phase::kFadeIn;

	timer = 0;

	tutorialScoreGround_[0] = TextureManager::Load("./Resources/scoreBackground/scoreBackground.png");
	tutorialScoreGround_[1] = TextureManager::Load("./Resources/scoreBackground/scoreBackground.png");

	for (int i = 0; i < 2; i++)
	{
		tutorialSprites[i] = Sprite::Create(tutorialScoreGround_[i], {0.0f, 0.0f});
	}
	page = 0;
}

void TutorialScene::Update() 
{
	fade_->Update();
	switch (phase_) {
	case TutorialScene::Phase::kFadeIn:
		if (fade_->isFinished()) {
			phase_ = Phase::kMain; // フェードイン完了 -> メインフェーズへ
		}
		break;
	case TutorialScene::Phase::kMain:
		if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
			if (page == 0)
			{
				page = 1;
			}
			else if (page == 1)
			{
				isFinish = true;
			}
		}
		break;
	case TutorialScene::Phase::kfadeOut:
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
}

void TutorialScene::Draw() 
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 2d描画
	Sprite::PreDraw(dxCommon->GetCommandList());

	if (page == 0) 
	{
		tutorialSprites[0]->Draw();
	}
	if (page == 1) 
	{
		tutorialSprites[1]->Draw();
	}

	Sprite::PostDraw();
}
