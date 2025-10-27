#include "TitleScene.h"
using namespace KamataEngine;

void TitleScene::Initialize() {
	
	//フェードとフェード管理の初期化
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	phase_ = Phase::kFadeIn;

	timer = 0;

	bgm_ = new BGM();
	bgm_->Initialize();

	titleBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/In the Sweet By and By.mp3");

	//titleSprite_ = TextureManager::Load("Resources/titleText/")

}

void TitleScene::Update() {
	

	fade_->Update();
	bgm_->BGMPlay(titleBgmHandle_);
	switch (phase_) {
	case TitleScene::Phase::kFadeIn:
		if (fade_->isFinished()) {
			phase_ = Phase::kMain; // フェードイン完了 -> メインフェーズへ
		}
		break;
	case TitleScene::Phase::kMain:
		if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			isFinish = true;
			bgm_->BGMStop();
		}

		break;
	case TitleScene::Phase::kfadeOut:
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
	ImGui::Begin("Title Scene");
	ImGui::Text("Press SPACE to Start");
	ImGui::End();
#endif
}

void TitleScene::Draw() {

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	//フェード
	fade_->Draw(dxCommon->GetCommandList());

	// スプライト描画後処理
	Sprite::PostDraw();
}

TitleScene::~TitleScene() 
{ delete bgm_; }
