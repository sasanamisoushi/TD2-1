#include "GameScene.h"
#include "math.h"

using namespace KamataEngine;

void GameScene::Initialize() 
{
	player_ = new Player();

	model_ = Model::CreateFromOBJ("player", true);
	playerModel_ = Model::CreateFromOBJ("enemy", true);

	Vector3 playerPosition = {5, 5, 0};
	Vector3 lurePosition = {1, 5, 0};

	camera_.Initialize();
	player_->Initialize(model_, playerModel_, &camera_, lurePosition, playerPosition);
}

GameScene::~GameScene() 
{
	delete model_;
	delete playerModel_;
	delete player_; 
}

void GameScene::Update() 
{
	player_->Update();
	if (Input::GetInstance()->TriggerKey(DIK_S)) {
		isFinish = true;
	}

#ifdef _DEBUG
	ImGui::Begin("Game Scene");

	ImGui::Text("playerPos %f,%f,%f", player_->GetPlayerPos().x, player_->GetPlayerPos().y, player_->GetPlayerPos().z);
	ImGui::Text("lurePos %f,%f,%f", player_->GetLurePos().x, player_->GetLurePos().y, player_->GetLurePos().z);

	ImGui::End();
#endif
}

void GameScene::Draw() 
{ 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Model::PreDraw(dxCommon->GetCommandList());

	player_->Draw();

	Model::PostDraw();
}
