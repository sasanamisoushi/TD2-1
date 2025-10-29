#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include "GameClearScene.h"
#include "Score.h"
#include <Windows.h>
#include "TutorialScene.h"
#include "BGM.h"

using namespace KamataEngine;

// シーンの状態を管理するenum
enum class Scene {
	kUnknown = 0,
	kTitle,
	kTutorial,
	kGame,
	kGameClear,
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"2142_気まぐれフィッシング");

	// 描画のためのDirectXCommmonインスタンスの取得
	DirectXCommon* dxCommmon = DirectXCommon::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	

	// シーンオブジェクトへのポインタ
	GameScene* gameScene = nullptr;
	TitleScene* titleScene = nullptr;
	GameClearScene* gameClearScene = nullptr;
	TutorialScene* tutorialScene = nullptr;
	BGM* bgm_ = nullptr;

	// 現在と次のシーンの状態変数
	Scene currentSceneEnum = Scene::kTitle;
	Scene nextSceneEnum = currentSceneEnum;

	// 初期シーンの設定
	titleScene = new TitleScene();
	titleScene->Initialize();

	Score* score;
	score = new Score();
	score->Initialize();

	bgm_ = new BGM();
	bgm_->Initialize();

	int isBoss = false;

	uint32_t gameClearBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/Sunny Day.mp3");
	uint32_t gamePlayBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/All the Fixings.mp3");
	uint32_t titleBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/In the Sweet By and By.mp3");
	uint32_t bossBgm_ = Audio::GetInstance()->LoadWave("./BGM/Run.mp3");

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// ImGuiのフレーム開始
		imguiManager->Begin();

		// シーンの切り替えチェック
		if (currentSceneEnum != nextSceneEnum) {
			// 古いシーンのクリーンアップ
			switch (currentSceneEnum) {
			case Scene::kTitle:
				delete titleScene;
				titleScene = nullptr;
				break;
			case Scene::kTutorial:
				delete tutorialScene;
				tutorialScene = nullptr;
				break;
			case Scene::kGame:
				delete gameScene;
				gameScene = nullptr;
				break;
			case Scene::kGameClear:
				delete gameClearScene;
				gameClearScene = nullptr;
				break;
			default:
				break;
			}

			// 新しいシーンに更新
			currentSceneEnum = nextSceneEnum;

			// 新しいシーンの初期化
			switch (currentSceneEnum) {
			case Scene::kTitle:
				titleScene = new TitleScene();
				titleScene->Initialize();
				isBoss = false;
				break;
			case Scene::kTutorial:
				tutorialScene = new TutorialScene();
				tutorialScene->Initialize();
				break;
			case Scene::kGame:
				gameScene = new GameScene();
				gameScene->Initialize(score);
				break;
			case Scene::kGameClear:
				gameClearScene = new GameClearScene();
				gameClearScene->Initialize(score);
				break;
			}
		}

			// 現在のシーンを更新
			switch (currentSceneEnum) {
			case Scene::kTitle:
				if (titleScene != nullptr) {
					titleScene->Update();
				    bgm_->BGMPlay(titleBgmHandle_);
				    score->ResetScore();
				    score->isScoreBossClear = false;
					if (titleScene->IsFinished()) {
						nextSceneEnum = Scene::kTutorial;
					}
				}
				break;
		    case Scene::kTutorial:
			    if (tutorialScene != nullptr)
				{

				    tutorialScene->Update();
				    if (tutorialScene->IsFinished()) {
					    nextSceneEnum = Scene::kGame;
					    bgm_->BGMStop();
				    }
			    }
			    break;
			case Scene::kGame:
				if (gameScene != nullptr)
				{
				    if (!score->isScoreBossClear)
					{
					    bgm_->BGMPlay(gamePlayBgmHandle_);
				    }
					else
					{
					    if (!isBoss) 
						{
						    bgm_->BGMStop();
						    isBoss = true;
					    }
					    bgm_->BGMPlay(bossBgm_);
				    }
					gameScene->Update();
					if (gameScene->IsFinished()) {
						nextSceneEnum = Scene::kGameClear;
					    bgm_->BGMStop();
					}
				}
				break;
			case Scene::kGameClear:
				if (gameClearScene != nullptr) 
				{
				    bgm_->BGMPlay(gameClearBgmHandle_);
					gameClearScene->Update();
					if (gameClearScene->IsFinished()) {
						nextSceneEnum = Scene::kTitle;
					    bgm_->BGMStop();
					}
				}
				break;
			}

			imguiManager->End();

			// 描画開始
			dxCommmon->PreDraw();
			// 現在のシーンを描画
			switch (currentSceneEnum) {
			case Scene::kTitle:
				if (titleScene != nullptr) {
					titleScene->Draw();
				}
				break;
		    case Scene::kTutorial:
			    if (tutorialScene != nullptr) {
				    tutorialScene->Draw();
			    }
			    break;
			case Scene::kGame:
				if (gameScene != nullptr) {
					gameScene->Draw();
				}
				break;
			case Scene::kGameClear:
				if (gameClearScene != nullptr) {
					gameClearScene->Draw();
				}
				break;
			}
			imguiManager->Draw();
			// 描画終了
			dxCommmon->PostDraw();
		
	}

	// 最終的なクリーンアップ
	switch (currentSceneEnum) {
	case Scene::kTitle:
		delete titleScene;
		break;
	case Scene::kGame:
		delete gameScene;
		break;
	case Scene::kGameClear:
		delete gameClearScene;
		break;
	}

	delete score;
	score = nullptr;

	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}
