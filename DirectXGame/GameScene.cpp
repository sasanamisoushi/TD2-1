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

	// タイマー
	num0Model_ = Model::CreateFromOBJ("num/0/0.obj", true);
	num1Model_ = Model::CreateFromOBJ("num/1/1.obj", true);
	num2Model_ = Model::CreateFromOBJ("num/2/2.obj", true);
	num3Model_ = Model::CreateFromOBJ("num/3/3.obj", true);
	num4Model_ = Model::CreateFromOBJ("num/4/4.obj", true);
	num5Model_ = Model::CreateFromOBJ("num/5/5.obj", true);
	num6Model_ = Model::CreateFromOBJ("num/6/6.obj", true);
	num7Model_ = Model::CreateFromOBJ("num/7/7.obj", true);
	num8Model_ = Model::CreateFromOBJ("num/8/8.obj", true);
	num9Model_ = Model::CreateFromOBJ("num/9/9.obj", true);
	
	gameTimer_ = kGameTimer_;

	numderTransform_.Initialize();
	numderTransform_.translation_ = {0.0f, 5.0f, 0.0f};
	numderTransform_.scale_ = {2.0f, 2.0f, 2.0f};
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
		if (gameTimer_ > 0)
			gameTimer_--;
		if (gameTimer_ <= 0) {
			gameTimer_ = 0;
			isGame_ = false;
			isFinish = true;
		}
	}

	int seconds = gameTimer_ / 60;//
	currentNumber_ = seconds % 10;

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
	 //seconds = gameTimer_ / 60;
	ImGui::Text("TIME: %d", seconds);
	
	ImGui::End();
#endif
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	
	// 数字の描画
	// --- 数字モデルの描画 ---
	switch (currentNumber_) {
	case 0:
		if (num0Model_)
			num0Model_->Draw(numderTransform_, camera_);
		break;
	case 1:
		if (num1Model_)
			num1Model_->Draw(numderTransform_, camera_);
		break;
	case 2:
		if (num2Model_)
			num2Model_->Draw(numderTransform_, camera_);
		break;
	case 3:
		if (num3Model_)
			num3Model_->Draw(numderTransform_, camera_);
		break;
	case 4:
		if (num4Model_)
			num4Model_->Draw(numderTransform_, camera_);
		break;
	case 5:
		if (num5Model_)
			num5Model_->Draw(numderTransform_, camera_);
		break;
	case 6:
		if (num6Model_)
			num6Model_->Draw(numderTransform_, camera_);
		break;
	case 7:
		if (num7Model_)
			num7Model_->Draw(numderTransform_, camera_);
		break;
	case 8:
		if (num8Model_)
			num8Model_->Draw(numderTransform_, camera_);
		break;
	case 9:
		if (num9Model_)
			num9Model_->Draw(numderTransform_, camera_);
		break;
	}


	//魚の描画
	for (auto& fish : fishes_) {
		fish->Draw();
	}

	player_->Draw();
	Model::PostDraw();
}