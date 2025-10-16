#include "GameScene.h"
#include "math.h"
#include <cstdlib>
#include <ctime>

using namespace KamataEngine;

void GameScene::Initialize() {
	// 実行ごとに乱数変更
	srand((unsigned int)time(nullptr));

	player_ = new Player();

	model_ = Model::CreateFromOBJ("player", true);
	playerModel_ = Model::CreateFromOBJ("enemy", true);

	getTimer_ = 30;

	Vector3 playerPosition = {5, 10, 0};
	Vector3 lurePosition = {0, 8, 0};
	player_->Initialize(model_, playerModel_, &camera_, lurePosition, playerPosition);

	// カメラの初期化
	camera_.Initialize();

	camera_.translation_ = {0.0f, 5.0f, -20.0f};
	camera_.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_.UpdateMatrix();

	// 魚のモデル
	fishModel_ = Model::CreateFromOBJ("fish");
	bigFishModel_ = Model::CreateFromOBJ("fish");

	// 制限数
	const int totalFishMax = 10; // 全体の最大数
	const int bigFishMax = 4;    // 大きい魚の最大数
	bigCount = 0;
	smallCount = 0;

	int attempts = 0;
	while (attempts < totalFishMax) {
		

		// 魚を生成
		Fish* fish = new Fish();
		BigFish* bigFish = new BigFish();

		bool moveRight = (rand() % 2 == 0);
		bool isBigFish = (rand() % 100 < 40);

		// 魚の初期化
		//fish->Initialize(fishModel_, &camera_, fishPos, moveRight, getTimer_);

		Vector3 fishPos;
		bool setPos = false;

		// 複数の魚を出す

		for (int confirmation = 0; confirmation < 50 && !setPos; confirmation++) {

			// ランダムな位置に生成
			fishPos = {
				0.0f, 
				static_cast<float>(rand() % 60) / 10.0f - 2.0f, 
				static_cast<float>((rand() % 40 - 20) / 10.0f)
			};

			setPos = true;

			// すでにある魚たちと距離チェック
			for (auto& fishS : fishes_) {
				float distanceY = fabs(fishPos.y - fishS->GetWorldPosition().y);
				if (distanceY < 1.5f) {
					setPos = false;
					break;
				}
			}

			// 大きい魚の距離チェック
			for (auto& BigFish : BigFishes_) {
				float distanceY = fabs(fishPos.y - BigFish->GetWorldPosition().y);
				if (distanceY < 2.5f) {
					setPos = false;
					break;
				}
			}
		}

		if (!setPos) {
			fishPos = {
			    0.0f,
			    static_cast<float>((rand() % 40) / 10.0f + 1.0f), // 1〜5くらい
			    0.0f};
		}

		if (isBigFish && bigCount < bigFishMax) {
			// 大きい魚の初期化
			bigFish->Initialize(bigFishModel_, &camera_, fishPos, moveRight);
			// 配列に登録
			BigFishes_.push_back(bigFish);
			bigCount++;
		} else {
			// 魚の初期化
			fish->Initialize(fishModel_, &camera_, fishPos, moveRight, getTimer_);
			// 配列に登録
			fishes_.push_back(fish);
			smallCount++;
		}
		attempts++;
	}

	// タイマー
	// 数字の描画
	numTexHandles_[0] = TextureManager::Load("num/0.png");
	numTexHandles_[1] = TextureManager::Load("num/1.png");
	numTexHandles_[2] = TextureManager::Load("num/2.png");
	numTexHandles_[3] = TextureManager::Load("num/3.png");
	numTexHandles_[4] = TextureManager::Load("num/4.png");
	numTexHandles_[5] = TextureManager::Load("num/5.png");
	numTexHandles_[6] = TextureManager::Load("num/6.png");
	numTexHandles_[7] = TextureManager::Load("num/7.png");
	numTexHandles_[8] = TextureManager::Load("num/8.png");
	numTexHandles_[9] = TextureManager::Load("num/9.png");

	// 数字の初期化
	numSprite_[0] = Sprite::Create(numTexHandles_[0], {30, 30});
	numSprite_[1] = Sprite::Create(numTexHandles_[0], {50, 30});
	numSprite_[2] = Sprite::Create(numTexHandles_[0], {70, 30});
}

GameScene::~GameScene() {
	delete model_;
	delete playerModel_;
	delete player_;
	for (auto& fish : fishes_) {
		delete fish;
	}
	fishes_.clear();

	delete fishModel_;

	for (auto& Bigfish : BigFishes_) {
		delete Bigfish;
	}
	BigFishes_.clear();

	delete bigFishModel_;
}

void GameScene::Update() {

	// 魚の挙動
	for (auto& fish : fishes_) {
		fish->Update();
	}

	for (auto& bigFish : BigFishes_) {
		bigFish->Update();
	}

	
	
	// 魚が取れた時
	int caughtFishCount = 0;
	fishes_.remove_if([&caughtFishCount](Fish* fish) {
		if (fish->IsLureCheck()) {
			delete fish;
			caughtFishCount++;
			return true;
		}
		return false;
	});

	// 大きい魚が取れた時
	BigFishes_.remove_if([&caughtFishCount](BigFish* bigfish) {
		if (bigfish->IsLureCheck()) {
			delete bigfish;
			caughtFishCount++;
			return true;
		}
		return false;
	});

	//捕まえた数だけ再生成
	for (int i = 0; i < caughtFishCount; i++) {
		SpawnFish(false); // 小さい魚を追加
	}

	player_->Update();
	if (Input::GetInstance()->TriggerKey(DIK_S)) {
		isFinish = true;
	}

	CheckAllCollisions();

	// タイマー処理
	if (isGame_) {
		if (gameTimer_ > 0) {
			gameTimer_--;
		}
		if (gameTimer_ <= 0) {
			gameTimer_ = 0;
			isGame_ = false;
			isFinish = true;
		}
	}


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
		ImGui::Text("GetTimer %d", BigFish->fishGetTimer_);
		index++;
	}

	ImGui::Text("Spawned %d small fish and %d big fish\n", smallCount, bigCount);

	ImGui::Text("playerPos %f,%f,%f", player_->GetPlayerPos().x, player_->GetPlayerPos().y, player_->GetPlayerPos().z);
	ImGui::Text("lurePos %f,%f,%f", player_->GetLurePos().x, player_->GetLurePos().y, player_->GetLurePos().z);

	ImGui::End();
#endif
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	// 魚の描画
	for (auto& fish : fishes_) {
		fish->Draw();
	}

	for (auto& bigFish : BigFishes_) {
		bigFish->Draw();
	}

	player_->Draw();
	Model::PostDraw();

	//2d描画
	Sprite::PreDraw(dxCommon->GetCommandList());

	// 数字の描画
	// 残り秒数
	int seconds = gameTimer_ / 60; // 60FPS → 秒換算
	int hundreds = (seconds / 100) % 10;
	int tens = (seconds / 10) % 10;
	int ones = seconds % 10;

	// スプライトに対応する数字をセット
	numSprite_[0]->SetTextureHandle(numTexHandles_[hundreds]); // 三桁
	numSprite_[1]->SetTextureHandle(numTexHandles_[tens]);     // 二桁
	numSprite_[2]->SetTextureHandle(numTexHandles_[ones]);     // 一桁
	// 描画
	numSprite_[0]->Draw();
	numSprite_[1]->Draw();
	numSprite_[2]->Draw();

	Sprite::PostDraw();
}

void GameScene::CheckAllCollisions() {
	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと魚全ての当たり判定
	for (Fish* fish : fishes_) {
		aabb2 = fish->GetAABB();

		// ルアーと魚が当たっているとき
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(fish);

			fish->OnCollision(player_);
		}
		// ルアーと魚が当たってないとき
		else {
			fish->OutCollision();
		}
	}

	// 自キャラと大きい魚全ての当たり判定
	for (BigFish* Bigfish : BigFishes_) {
		aabb2 = Bigfish->GetAABB();

		// ルアーと魚が当たっているとき
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(Bigfish);

			Bigfish->OnCollision(player_);
		}
		// ルアーと魚が当たってないとき
		else {
			Bigfish->OutCollision();
		}
	}
}

void GameScene::SpawnFish(bool isBigFish) {
	bool moveRight = (rand() % 2 == 0);
	Vector3 fishPos;
	bool setPos = false;

	for (int confirmation = 0; confirmation < 50 && !setPos; confirmation++) {
		fishPos = {0.0f, static_cast<float>(rand() % 60) / 10.0f - 2.0f, static_cast<float>((rand() % 40 - 20) / 10.0f)};
		setPos = true;

		// 小さい魚と距離確認
		for (auto& fishS : fishes_) {
			float distanceY = fabs(fishPos.y - fishS->GetWorldPosition().y);
			if (distanceY < 1.5f) {
				setPos = false;
				break;
			}
		}

		// 大きい魚と距離確認
		for (auto& BigFish : BigFishes_) {
			float distanceY = fabs(fishPos.y - BigFish->GetWorldPosition().y);
			if (distanceY < 2.5f) {
				setPos = false;
				break;
			}
		}
	}

	if (!setPos) {
		fishPos = {0.0f, static_cast<float>((rand() % 40) / 10.0f + 1.0f), 0.0f};
	}

	if (isBigFish) {
		BigFish* bigFish = new BigFish();
		bigFish->Initialize(bigFishModel_, &camera_, fishPos, moveRight);
		BigFishes_.push_back(bigFish);
		bigCount++;
	} else {
		Fish* fish = new Fish();
		fish->Initialize(fishModel_, &camera_, fishPos, moveRight,getTimer_);
		fishes_.push_back(fish);
		smallCount++;
	}
}
