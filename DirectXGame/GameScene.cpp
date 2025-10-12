#include "GameScene.h"
#include "math.h"
#include "Time.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	player_ = new Player();

	model_ = Model::CreateFromOBJ("player", true);
	playerModel_ = Model::CreateFromOBJ("enemy", true);

	Vector3 playerPosition = {5, 5, 0};
	Vector3 lurePosition = {1, 5, 0};
	player_->Initialize(model_, playerModel_, &camera_, lurePosition, playerPosition);

	// カメラの初期化
	camera_.Initialize();

	camera_.translation_ = {0.0f, 5.0f, -20.0f};
	camera_.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_.UpdateMatrix();

	// 魚のモデル
	fishModel_ = Model::CreateFromOBJ("fish");

	// 複数の魚を出す

	for (int i = 0; i < 5; i++) {

		// 魚を生成
		Fish* fish = new Fish();

		// x座標をランダムに配置
		Vector3 fishPos{0.0f, static_cast<float>(rand() % 10 + -3), 0.0f};

		// 50%の確率で右か左向きになる
		bool moveRight = (rand() % 2 == 0);

		// 魚の初期化
		fish->Initialize(fishModel_, &camera_, fishPos, moveRight);

		// 配列に登録
		fishes_.push_back(fish);
	}

	//タイマー
	gameTimer_ = kGameTimer;
	isGame_ = true;
}

GameScene::~GameScene() {
	delete model_;
	delete playerModel_;
	delete player_;
	for (auto& fish : fishes_) {
		delete fish;
	}
	fishes_.clear();
}

void GameScene::Update()
{
	// タイマー処理
	if (isGame_) {
		gameTimer_--;
		if (gameTimer_ <= 0) {
			gameTimer_ = 0;
			isGame_ = false;

			// ここでゲーム終了フラグを立てる！
			isFinish = true;
		}
	}

	//魚の挙動
	for (auto& fish : fishes_) {
		fish->Update();
	}

	
	player_->Update();
	if (Input::GetInstance()->TriggerKey(DIK_S)) {
		isFinish = true;
	}

#ifdef _DEBUG
	ImGui::Begin("Game Scene");
	

	int index = 0;
	for (auto& fish : fishes_) {
		const Vector3& pos = fish->GetWorldPosition();
	
		ImGui::Text("Fish %d", index);
		ImGui::SameLine();
		ImGui::Text("Pos: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
	index++;
	}


	ImGui::Text("playerPos %f,%f,%f", player_->GetPlayerPos().x, player_->GetPlayerPos().y, player_->GetPlayerPos().z);
	ImGui::Text("lurePos %f,%f,%f", player_->GetLurePos().x, player_->GetLurePos().y, player_->GetLurePos().z);

	//タイム
	int seconds = gameTimer_ / 60;
	ImGui::Text("TIME: %d", seconds);
	
	ImGui::End();
#endif
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	//魚の描画
	for (auto& fish : fishes_) {
		fish->Draw();
	}

	player_->Draw();
	Model::PostDraw();
}