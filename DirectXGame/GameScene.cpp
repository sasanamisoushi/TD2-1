#include "GameScene.h"
#include "math.h"
#include <cstdlib>
#include <ctime>
#include <numbers>

using namespace KamataEngine;

void GameScene::Initialize(Score* score) {
	// 実行ごとに乱数変更
	srand((unsigned int)time(nullptr));

	// カメラの初期化
	camera_.Initialize();

	camera_.translation_ = {0.0f, 5.0f, -20.0f};
	camera_.rotation_ = {0.0f, 0.0f, 0.0f};
	camera_.UpdateMatrix();

	// プレイヤーの初期化
	player_ = new Player();

	model_ = Model::CreateFromOBJ("network", true);
	playerModel_ = Model::CreateFromOBJ("player", true);

	getTimer_ = 90;

	Vector3 playerPosition = {-11, 10, 0};
	Vector3 lurePosition = {0, 7, 0};
	player_->Initialize(model_, playerModel_, &camera_, lurePosition, playerPosition);

	// フェードの初期化
	fade_ = new Fade();
	fade_->Initialize();

	// FadeクラスのStartメソッドがフェードの方向と時間を引数にとると仮定
	fade_->Start(Fade::Status::FadeIn, 1.0f); // フェードイン開始
	timer = 0;

	phase_ = Phase::kFadeIn; // フェーズをフェードインに設定

	// 魚のモデル
	fishModel_ = Model::CreateFromOBJ("fish");
	bigFishModel_ = Model::CreateFromOBJ("fish");
	rubbishModel_ = Model::CreateFromOBJ("trash", true);
	swimmyModel_ = Model::CreateFromOBJ("suimii", true);
	bearModel_ = Model::CreateFromOBJ("bear", true);
	bearLureModel_ = Model::CreateFromOBJ("bearLure", true);
	weatherModel_ = Model::CreateFromOBJ("weather", true);
	backgroundModel_ = Model::CreateFromOBJ("gameBackground");

	// 背景モデルの生成
	backgroundModel_ = Model::CreateFromOBJ("gameBackground", true);

	score_ = score;

	// イベントの初期化
	swimmyEvent_ = new SwimmyEvent();
	swimmyEvent_->Initialize(fishModel_, swimmyModel_, &camera_);

	
	//クマイベントの初期化
	Vector3 bearLurePosition = {0, 7, 0}; // ルアーの描画位置
	Vector3 bearPosition = {-12, 10, 0};  // 熊の描画位置

	bearEvent_ = new bearEvent();
	bearEvent_->Initialize(bearLureModel_, bearModel_, &camera_, bearLurePosition, bearPosition);

	// 天気イベントの初期化

	weatherEvent_ = new weatherEvent();
	weatherEvent_->Initialize();
	

	// 背景オブジェクトのワールド座標設定
	backgroundTransform_.Initialize();
	backgroundTransform_.translation_ = {0.0f,5.0f, 10.0f}; // ← zを大きくしてt奥に
	backgroundTransform_.scale_ = {5.0f, 3.0f, 10.0f};        // サイズ調整

	WorldTransformUpdate(backgroundTransform_);

	// ★ イベント終了時に魚を再生成
	swimmyEvent_->SetOnEventEnd([this]() {
		for (int i = 0; i < 10; i++) { // 10匹くらい再生成
			SpawnFish();
		}
	});

	// 制限数
	const int totalFishMax = 15; // 全体の最大数
	const int bigFishMax = 5;    // 大きい魚の最大数
	const int EventFisMax = 1;   // イベントの魚の最大数
	bigCount = 0;
	smallCount = 0;
	rubbishCount = 0;
	eventCount = 0;

	int attempts = 0;
	while (attempts < totalFishMax) {

		bool moveRight = (rand() % 2 == 0);
		bool isBigFish = (rand() % 100 < 40);
		bool isRubbish = (rand() % 100 < 20);
		bool isEvent = (rand() % 100 < 10);

		Vector3 fishPos;
		bool setPos = false;

		// 複数の魚を出す

		for (int confirmation = 0; confirmation < 50 && !setPos; confirmation++) {

			// ランダムな位置に生成
			fishPos = {0.0f, static_cast<float>(rand() % 60) / 10.0f - 2.0f, static_cast<float>((rand() % 40 - 20) / 10.0f)};

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

			// ゴミの距離チェック
			for (auto& Rubbishs : rubbishes_) {
				float distanceY = fabs(fishPos.y - Rubbishs->GetWorldPosition().y);
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

		if (isRubbish) {
			Rubbish* rubbish = new Rubbish();
			rubbish->Initialize(rubbishModel_, &camera_, score_, fishPos, moveRight);
			rubbishes_.push_back(rubbish);
			rubbishCount++;
		} else if (isEvent && eventCount < EventFisMax) {
			EventFish* eventFish = new EventFish();
			Model* eventModel = nullptr;
			EventFish::FishEventType eventType{};

			int eventTypeRand = rand() % 3;
			switch (eventTypeRand) {
			case 0:
				eventModel = swimmyModel_;
				eventType = EventFish::FishEventType::SwimmyGroup;
				break;
			case 1:
				eventModel = bearModel_;
				eventType = EventFish::FishEventType::BearHelp;
				break;
			case 2:
				eventModel = weatherModel_;
				eventType = EventFish::FishEventType::WeatherChange;
				break;
			}

			eventFish->Initialize(eventModel, &camera_, score_, fishPos, moveRight);
			eventFish->SetEventType(eventType);
			events_.push_back(eventFish);
			eventCount++;

		} else if (isBigFish && bigCount < bigFishMax) {
			BigFish* bigFish = new BigFish();
			// 大きい魚の初期化（FishのInitializeと一致する形）
			bigFish->Initialize(bigFishModel_, &camera_, score_, fishPos, moveRight);
			BigFishes_.push_back(bigFish);
			bigCount++;
		} else {
			Fish* fish = new Fish();
			// 魚の初期化
			fish->Initialize(fishModel_, &camera_, score_, fishPos, moveRight, getTimer_);
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

	for (auto& rubbishs : rubbishes_) {

		delete rubbishs;
	}
	rubbishes_.clear();

	delete rubbishModel_;

	delete swimmyEvent_;
	delete swimmyModel_;
	delete bearEvent_;
	delete bearModel_;
	delete bearLureModel_;
	delete weatherEvent_;
	delete weatherModel_;
	for (auto& eventFish : events_) {

		delete eventFish;
	}
	events_.clear();

	for (int i = 0; i < 3; i++) {

		delete numSprite_[i];
		numSprite_[i] = nullptr;
	}
}

void GameScene::Update() {

	fade_->Update();
	switch (phase_) {
	case GameScene::Phase::kFadeIn:

		if (fade_->isFinished()) {
			phase_ = Phase::kMain; // フェードイン完了 -> メインフェーズへ
		}
		break;
	case GameScene::Phase::kMain: {

		player_->Update();
		// 小さい魚
		for (auto& fish : fishes_) {
			fish->Update();
		}

		// 大きい魚
		for (auto& bigFish : BigFishes_) {
			bigFish->Update();
		}

		// ゴミ
		for (auto& rubbishs : rubbishes_) {
			rubbishs->Update();
		}

		for (auto& eventFish : events_) {

			eventFish->Update();
		}

		// 群れの更新
		if (swimmyEvent_) {
			swimmyEvent_->Update();
		}

		// クマの更新
		if (bearEvent_) {
			bearEvent_->Update();
		}

		// 天気の更新
		if (weatherEvent_) {
			weatherEvent_->Update();
		}
		

		float currentSpeedMultiplier = weatherEvent_->GetFishSpeedMultiplier();

		// --- すでにいる全ての魚に速度補正を再適用 ---
		for (auto& fish : fishes_) {
			fish->SetSpeedMultiplier(currentSpeedMultiplier);
		}

		for (auto& big : BigFishes_) {
			big->SetSpeedMultiplier(currentSpeedMultiplier);
		}

		for (auto& rub : rubbishes_) {
			rub->SetSpeedMultiplier(currentSpeedMultiplier);
		}

		for (auto& eventFish : events_) {
			eventFish->SetSpeedMultiplier(currentSpeedMultiplier);
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

		// ゴミが釣れた時
		rubbishes_.remove_if([&caughtFishCount](Rubbish* rubbish) {
			if (rubbish->IsLureCheck()) {
				delete rubbish;
				caughtFishCount++;
				return true;
			}
			return false;
		});

		// イベント魚が取れた時
		events_.remove_if([&](EventFish* eventFish) {
			if (eventFish->IsLureCheck()) {
				Vector3 centerPos = eventFish->GetWorldPosition();

				EventFish::FishEventType type = eventFish->GetEventType();

				switch (type) {
				case EventFish::FishEventType::SwimmyGroup:
					// 通常の魚を消す
					ClearAllFish();
					// --- イベント群れを生成 ---
					swimmyEvent_->SpawnFishGroup(centerPos, 8, 3.0f);
					break;
				case EventFish::FishEventType::BearHelp:
					bearEvent_->isBearEvent_ = true;
					break;
				case EventFish::FishEventType::WeatherChange:

					if (weatherEvent_) {

						weatherEvent_->TriggerRandomWeather();
					}

					break;
				default:
					break;
				}

				delete eventFish;
				caughtFishCount++;
				return true;
			}
			return false;
		});

		// 捕まえた数だけ再生成
		for (int i = 0; i < caughtFishCount; i++) {
			SpawnFish();
		}

	if (Input::GetInstance()->TriggerKey(DIK_S)) {
		isFinish = true;
		score_->FileWrite();
	}
		CheckAllCollisions();
		CheckBearCollisions();

	// タイマー処理
	if (isGame_) {
		if (gameTimer_ > 0) {
			gameTimer_--;
		}
		if (gameTimer_ <= 0) {
			gameTimer_ = 0;
			isGame_ = false;
			isFinish = true;
			score_->FileWrite();
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
				score_->FileWrite();
			}
		}

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_B)) {
	  bearEvent_->isBearEvent_ = true;
  }
	
		ImGui::Begin("Game Scene");

		int index = 0;
		for (auto& fish : fishes_) {
			const Vector3& pos = fish->GetWorldPosition();

			ImGui::Text("Fish %d", index);
			ImGui::SameLine();
			ImGui::Text("Pos: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);

			ImGui::Text("fishAABB min x,%f y,%f z,%f", fish->GetAABB().min.x, fish->GetAABB().min.y, fish->GetAABB().min.z);
			ImGui::Text("fishAABB max x,%f y,%f z,%f", fish->GetAABB().max.x, fish->GetAABB().max.y, fish->GetAABB().max.z);

			ImGui::Text("playerPos %d", fish->fishHit_);
			ImGui::Text("GetTimer %d", fish->fishGetTimer_);
			index++;
		}

		for (auto& BigFish : BigFishes_) {
			const Vector3& pos = BigFish->GetWorldPosition();

			ImGui::Text("BigFish %d", index);
			ImGui::SameLine();
			ImGui::Text("Pos: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
			ImGui::Text("fishAABB min x,%f y,%f z,%f", BigFish->GetAABB().min.x, BigFish->GetAABB().min.y, BigFish->GetAABB().min.z);
			ImGui::Text("fishAABB max x,%f y,%f z,%f", BigFish->GetAABB().max.x, BigFish->GetAABB().max.y, BigFish->GetAABB().max.z);
			ImGui::Text("GetTimer %d", BigFish->fishGetTimer_);
			index++;
		}

		for (auto& Rubbishs : rubbishes_) {
			const Vector3& pos = Rubbishs->GetWorldPosition();

			ImGui::Text("Rubbish %d", index);
			ImGui::SameLine();
			ImGui::Text("Pos: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
			ImGui::Text("GetTimer %d", Rubbishs->fishGetTimer_);
			index++;
		}

		ImGui::Text("Spawned %d small fish and %d big fish \nand rubbish %d\n", smallCount, bigCount, rubbishCount);

		ImGui::Text("playerPos %f,%f,%f", player_->GetPlayerPos().x, player_->GetPlayerPos().y, player_->GetPlayerPos().z);
		ImGui::Text("lurePos %f,%f,%f", player_->GetLurePos().x, player_->GetLurePos().y, player_->GetLurePos().z);

		ImGui::End();
		ImGui::Begin("Event Fish ");
		int eventIndex = 0;
		for (auto& eventFish : events_) {
			const Vector3& pos = eventFish->GetWorldPosition();
			ImGui::Separator();
			ImGui::Text("EventFish %d", eventIndex);
			ImGui::SameLine();
			ImGui::Text("Pos: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
			ImGui::Text("GetTimer: %d", eventFish->fishGetTimer_);
			ImGui::Text("EventType: %d", static_cast<int>(eventFish->GetEventType())); // ← イベント種類確認用
			eventIndex++;
		}
		ImGui::End();

		ImGui::Begin("playerAABB ");

		ImGui::Text("playerAABB min x,%f y,%f z,%f", player_->GetAABB().min.x, player_->GetAABB().min.y, player_->GetAABB().min.z);
		ImGui::Text("playerAABB max x,%f y,%f z,%f", player_->GetAABB().max.x, player_->GetAABB().max.y, player_->GetAABB().max.z);
		ImGui::Text("playerPos %d", player_->playerHit_);

		ImGui::End();
#endif
	} break;

	case GameScene::Phase::kfadeOut: {
		timer++;
		if (timer > 120) {
			isFinish = true;
		}
	} break;
	default:
		break;
	}
}

void GameScene::Draw() {

	

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	backgroundModel_->Draw(backgroundTransform_, camera_);

	weatherEvent_->Draw();

	// 魚の描画
	for (auto& fish : fishes_) {
		fish->Draw();
	}

	for (auto& bigFish : BigFishes_) {
		bigFish->Draw();
	}

	for (auto& rubbishs : rubbishes_) {
		rubbishs->Draw();
	}

	for (auto& eventFish : events_) {

		eventFish->Draw();
	}

	// プレイヤーをフェード完了後に描画
	if (phase_ == Phase::kMain) {
		player_->Draw();
	}

	// 群れの描画
	if (swimmyEvent_) {
		swimmyEvent_->Draw();
	}

	// クマの描画
	if (bearEvent_) {
		bearEvent_->Draw();
	}

	

		
	

	Model::PostDraw();

	// 2d描画
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

	score_->Draw();

	fade_->Draw(dxCommon->GetCommandList());

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

		// プレイヤー
		// ルアーと魚が当たっているとき
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(fish);
			player_->playerHit_ = true;
			fish->fishHit_ = true;
			fish->OnCollision(player_);
		}
		// ルアーと魚が当たってないとき
		else {
			player_->playerHit_ = false;
			fish->fishHit_ = false;
			fish->OutCollision();
		}
	}

	// 自キャラと大きい魚全ての当たり判定
	for (BigFish* Bigfish : BigFishes_) {
		aabb2 = Bigfish->GetAABB();

		// プレイヤー
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

	// 自キャラとゴミ全ての当たり判定
	for (Rubbish* Rubbishs : rubbishes_) {
		aabb2 = Rubbishs->GetAABB();

		// ルアーと魚が当たっているとき
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(Rubbishs);

			Rubbishs->OnCollision(player_);
		}
		// ルアーと魚が当たってないとき
		else {
			Rubbishs->OutCollision();
		}
	}

	// 自キャラとイベント魚全ての当たり判定
	for (EventFish* evenet : events_) {
		aabb2 = evenet->GetAABB();

		// ルアーと魚が当たっているとき
		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(evenet);

			evenet->OnCollision(player_);
		}
		// ルアーと魚が当たってないとき
		else {
			evenet->OutCollision();
		}
	}
}

void GameScene::CheckBearCollisions() 
{ 
	AABB aabb1, aabb2; 
	// 熊の座標
	aabb1 = bearEvent_->GetAABB();

	for (Fish* fish : fishes_)
	{
		aabb2 = fish->GetAABB();

		// 熊
		// ルアーと魚が当たっているとき
		if (bearEvent_->isBearEvent_) {
			if (IsCollision(aabb1, aabb2)) {
				bearEvent_->OnCollision(fish);

				fish->OnCollisionBear(bearEvent_);
			}
			// ルアーと魚が当たってないとき
			else {
				fish->OutCollisionBear();
			}
		}
	}

	// 自キャラと大きい魚全ての当たり判定
	for (BigFish* Bigfish : BigFishes_) 
	{
		aabb2 = Bigfish->GetAABB();
		// 熊
		// ルアーと魚が当たっているとき
		if (bearEvent_->isBearEvent_) {
			if (IsCollision(aabb1, aabb2)) {
				bearEvent_->OnCollision(Bigfish);

				Bigfish->OnCollisionBear(bearEvent_);
			}
			// ルアーと魚が当たってないとき
			else {
				Bigfish->OutCollisionBear();
			}
		}
	}
}

void GameScene::SpawnFish() {
	bool moveRight = (rand() % 2 == 0);
	Vector3 fishPos;
	bool setPos = false;

	// === 位置を決定 ===
	for (int confirmation = 0; confirmation < 50 && !setPos; confirmation++) {
		fishPos = {0.0f, static_cast<float>(rand() % 60) / 10.0f - 2.0f, 0.0f};
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

		// ゴミと距離確認
		for (auto& Rubbish : rubbishes_) {
			float distanceY = fabs(fishPos.y - Rubbish->GetWorldPosition().y);
			if (distanceY < 2.5f) {
				setPos = false;
				break;
			}
		}
	}

	if (!setPos) {
		fishPos = {0.0f, static_cast<float>((rand() % 40) / 10.0f + 1.0f), 0.0f};
	}

	// === 天候補正を取得 ===
	float bigFishChance = weatherEvent_->GetBigFishSpawnRate();      // 虹で上がる
	float rubbishChance = weatherEvent_->GetRubbishSpawnRate();      // 隕石で上がる
	float speedMultiplier = weatherEvent_->GetFishSpeedMultiplier(); // 雨・雲で変化

	// === 正規化（合計が1を超えないように）===
	float total = bigFishChance + rubbishChance;
	if (total > 1.0f) {
		bigFishChance /= total;
		rubbishChance /= total;
	}

	// === 出現確率を決定 ===
	float r = static_cast<float>(rand()) / RAND_MAX;

	// --- イベント魚出現チェック（独立確率） ---
	float eventChance = 0.05f; // 5%の確率でイベント魚出現
	if (r < eventChance && events_.empty()) {
		auto* eventFish = new EventFish();
		eventFish->Initialize(swimmyModel_, &camera_, nullptr, fishPos, moveRight);

		eventFish->SetOnTriggered([this](const Vector3& centerPos, EventFish::FishEventType type) {
			switch (type) {
			case EventFish::FishEventType::SwimmyGroup:
				swimmyEvent_->SpawnFishGroup(centerPos, 8, 3.0f);
				break;
			case EventFish::FishEventType::BearHelp:
				break;
			case EventFish::FishEventType::WeatherChange:
				if (weatherEvent_) {
					// 天候イベント発動！
					weatherEvent_->TriggerRandomWeather();
				}
				break;
			}
		});

		events_.push_back(eventFish);
		return; // イベント魚を出したら他は生成しない
	}

	// === 通常の魚生成（天候補正を反映） ===
	if (r < rubbishChance) {
		// 🗑️ ゴミを生成
		auto* rub = new Rubbish();
		rub->Initialize(rubbishModel_, &camera_, score_, fishPos, moveRight);
		rub->SetSpeedMultiplier(speedMultiplier);
		rubbishes_.push_back(rub);
	} else if (r < rubbishChance + bigFishChance) {
		// 🐋 大魚を生成
		auto* big = new BigFish();
		big->Initialize(bigFishModel_, &camera_, score_, fishPos, moveRight);
		big->SetSpeedMultiplier(speedMultiplier);
		BigFishes_.push_back(big);
	} else {
		// 🐠 小魚を生成
		auto* fish = new Fish();
		fish->Initialize(fishModel_, &camera_, score_, fishPos, moveRight, getTimer_);
		fish->SetSpeedMultiplier(speedMultiplier);
		fishes_.push_back(fish);
	}
}

void GameScene::ClearAllFish() {
	// 小さい魚を削除
	for (auto& fish : fishes_) {
		delete fish;
	}
	fishes_.clear();
	smallCount = 0;

	// 大きい魚を削除
	for (auto& bigFish : BigFishes_) {
		delete bigFish;
	}
	BigFishes_.clear();
	bigCount = 0;

	// ゴミを削除
	for (auto& rubbish : rubbishes_) {
		delete rubbish;
	}
	rubbishes_.clear();
	rubbishCount = 0;
}

Vector3 GameScene::GetRandomPos() {
	Vector3 pos;
	bool setPos = false;
	for (int attempt = 0; attempt < 50 && !setPos; attempt++) {
		pos = {0.0f, static_cast<float>(rand() % 60) / 10.0f - 2.0f, 0.0f};
		setPos = true;
		// 他の魚と距離チェック（小魚・大魚・ゴミ）
		for (auto& fish : fishes_) {
			if (fabs(pos.y - fish->GetWorldPosition().y) < 1.5f) {
				setPos = false;
				break;
			}
		}
		for (auto& big : BigFishes_) {
			if (fabs(pos.y - big->GetWorldPosition().y) < 2.5f) {
				setPos = false;
				break;
			}
		}
		for (auto& rub : rubbishes_) {
			if (fabs(pos.y - rub->GetWorldPosition().y) < 2.5f) {
				setPos = false;
				break;
			}
		}
	}
	if (!setPos)
		pos = {0.0f, static_cast<float>((rand() % 40) / 10.0f + 1.0f), 0.0f};
	return pos;
}

void GameScene::AddFish(Fish* fish) {
	fishes_.push_back(fish);
	smallCount++;
}

GameScene* GameScene::instance_ = nullptr;