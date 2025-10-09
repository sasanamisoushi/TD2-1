#include "GameScene.h"
#include "math.h"

using namespace KamataEngine;

void GameScene::Initialize() 
{
	player_ = new Player();

	model_ = Model::CreateFromOBJ("player", true);
	playerModel_ = Model::CreateFromOBJ("enemy", true);

	Vector3 playerPosition = {5, 10, 0};
	Vector3 lurePosition = {0, 8, 0};
	player_->Initialize(model_, playerModel_, &camera_, lurePosition, playerPosition);

	// カメラの初期化
	camera_.Initialize();

	camera_.translation_ = {0.0f, 5.0f, -20.0f};
	camera_.rotation_ = {0.0f,0.0f, 0.0f};
	camera_.UpdateMatrix();  

	
	//魚のモデル
	fishModel_ = Model::CreateFromOBJ("fish"); 
	bigFishModel_ = Model::CreateFromOBJ("fish");

	//制限数
	const int totalFishMax = 10;//全体の最大数
	const int bigFishMax = 4;//大きい魚の最大数

	//複数の魚を出す
	for (int i = 0; i < totalFishMax; i++) {

		// 魚を生成
		Fish* fish = new Fish();
		BigFish* bigFish = new BigFish();

		// x座標をランダムに配置
		Vector3 fishPos{0.0f, static_cast<float>(rand()%10+-3), 0.0f};

		// 50%の確率で右か左向きになる
		bool moveRight = (rand() % 2 == 0);

		//大きい魚を一定の確率でだす
		bool isBigFish = (rand() % 100 < 40);

		//座標の重なりをなくす
		bool setPos = false;

		for (int confirmation = 0; confirmation < 20 && !setPos; confirmation++) {
		
			//ランダムな位置に生成
			fishPos = {
				0.0f, 
				static_cast<float>(rand() % 10 - 3), 
				0.0f
			};

			setPos = true;

			//すでにある魚たちと距離チェック
			for (auto& fishS : fishes_) {
			
				Vector3 pos = fishS->GetWorldPosition();
				Vector3 distance = {fishPos.x - pos.x, fishPos.y - pos.y, 0.0f};
				float distanceSP = distance.x * distance.x + distance.y * distance.y;
				if (distanceSP < 2.0f * 2.0f) {
					setPos = false;
					break;
				}
			}

			//大きい魚の距離チェック
			for (auto& BigFish : BigFishes_) {

				Vector3 pos = BigFish->GetWorldPosition();
				Vector3 distance = {fishPos.x - pos.x, fishPos.y - pos.y, 0.0f};
				float distanceSP = distance.x * distance.x + distance.y * distance.y;
				if (distanceSP < 2.0f * 2.0f) {
					setPos = false;
					break;
				}
			}


		}


		if (isBigFish && bigCount < bigFishMax) {
			//大きい魚の初期化
			bigFish->Initialize(bigFishModel_, &camera_, fishPos, moveRight);
			//配列に登録
			BigFishes_.push_back(bigFish);
			bigCount++;
		} else {
			// 魚の初期化
			fish->Initialize(fishModel_, &camera_, fishPos, moveRight);
			// 配列に登録
			fishes_.push_back(fish);
			smallCount++;
		}
	}
}

GameScene::~GameScene() {
	delete model_;
	delete playerModel_;
	delete player_;
	for (auto& fish : fishes_) {
		delete fish;
	}
	fishes_.clear();

	for (auto& Bigfish : BigFishes_) {
		delete Bigfish;
	}
	BigFishes_.clear();
}

void GameScene::Update()
{

	//魚の挙動
	for (auto& fish : fishes_) {
		fish->Update();
	}


	for (auto& bigFish : BigFishes_) {
		bigFish->Update();
	}

	// 魚が取れた時
	fishes_.remove_if([](Fish* fish) {
		if (fish->IsLureCheck()) {
			delete fish;
			return true;
		}
		return false;
	});
	
	player_->Update();
	if (Input::GetInstance()->TriggerKey(DIK_S)) {
		isFinish = true;
	}

	CheckAllCollisions();

#ifdef _DEBUG
	ImGui::Begin("Game Scene");
	

	int index = 0;
	for (auto& fish : fishes_) {
		const Vector3& pos = fish->GetWorldPosition();
	
		ImGui::Text("Fish %d", index);
		ImGui::SameLine();
		ImGui::Text("Pos: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
		ImGui::Text("GetTimer %d", fish->fishGetTimer_);
	index++;
	}

	for (auto& BigFish : BigFishes_) {
		const Vector3& pos = BigFish->GetWorldPosition();

		ImGui::Text("BigFish %d", index);
		ImGui::SameLine();
		ImGui::Text("Pos: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
		index++;
	}

	printf("Spawned %d small fish and %d big fish\n", smallCount, bigCount);


	ImGui::Text("playerPos %f,%f,%f", player_->GetPlayerPos().x, player_->GetPlayerPos().y, player_->GetPlayerPos().z);
	ImGui::Text("lurePos %f,%f,%f", player_->GetLurePos().x, player_->GetLurePos().y, player_->GetLurePos().z);

	

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

	for (auto& bigFish : BigFishes_) {
		bigFish->Draw();
	}
	

	player_->Draw();
	Model::PostDraw();
}

void GameScene::CheckAllCollisions() 
{
	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと魚全ての当たり判定
	for (Fish* fish : fishes_)
	{
		aabb2 = fish->GetAABB();

		// ルアーと魚が当たっているとき
		if (IsCollision(aabb1,aabb2)) 
		{
 			player_->OnCollision(fish);

			fish->OnCollision(player_);
		}
		// ルアーと魚が当たってないとき
		else
		{
			fish->OutCollision();
		}
	}
}
