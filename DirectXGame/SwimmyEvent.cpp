#include "SwimmyEvent.h"

void SwimmyEvent::Initialize(Model* fishModel, Model* leaderModel, Camera* camera) {

	fishGroupModel_ = fishModel;
	leaderModel_ = leaderModel;
	camera_ = camera;

	isActive_ = false;
	groupTimer_ = 0;
}

void SwimmyEvent::Update() {

	//魚の群れが存在しなければ何もしない
	if (fishes_.empty()) {
	
		return;
	}

	groupTimer_++;

	// リーダー魚を先頭として扱う
	Fish* leader = fishes_.front();

	leader->Update();
	Vector3 leaderPos = leader->GetWorldPosition();

	// 群れ全体の移動方向（リーダーに合わせる）
	float leaderDir = leader->GetMoveDirectionY(); 

	bool leaderMoveRight = leader->GetIsMoveRight();

	// 他の魚をリーダーに追従させる
	for (size_t i = 1; i < fishes_.size(); i++) {
		Fish* fish = fishes_[i];

		// リーダーと同じ方向に向かせる
		fish->SetMoveDirectionY(leaderDir); 
		fish->SetIsMoveRight(leaderMoveRight);

		// リーダーの周囲に留まるように補間移動
		Vector3 targetPos = leaderPos;
		targetPos.x += cosf(i * 0.8f) * 2.0f;
		targetPos.y += sinf(i * 0.8f) * 2.0f;

		// 魚の現在位置を取得
		Vector3 current = fish->GetWorldPosition();

		// 少しずつ追従する
		Vector3 toTarget = targetPos - current;

		//リーダーの向きに応じて、少し方向を傾ける（自然な追従効果）
		current.x += cosf(leaderDir) * 0.1f;
		current.y += sinf(leaderDir) * 0.1f;

		// 追従スピード
		current += toTarget * 0.05f; 

		fish->SetWorldPosition(current);
		fish->Update();
	}

	//群れの寿命を過ぎたら削除
	if (groupTimer_ > groupLifetime_) {
	
		Reset();
	}

}

void SwimmyEvent::Draw() {

	for (auto& fish : fishes_) {
	
		fish->Draw();
	}

}

void SwimmyEvent::SpawnFishGroup(const Vector3& centerPos, int count, float spreadRadius) { 
	fishes_.clear();
	groupTimer_ = 0;
	isActive_ = true;

	// 魚の移動方向をランダムで決める(true=右方向, false=左方向)
	bool moveRight = (rand() % 2 == 0);

	//----------------中心の赤い魚----------------

	

	Fish* leaderFish = new Fish();
	leaderFish->Initialize(leaderModel_, camera_,nullptr, centerPos, moveRight, 30);

	//イベント魚として設定
	leaderFish->SetEventType(FishEventType::swmmyFish);

	//群れのリストに追加
	fishes_.push_back(leaderFish);

	 //リーダーの方向を取得
	float leaderDir = leaderFish->GetMoveDirectionY();

	 //リーダーの向きに合わせる（右向きならtrue, 左向きならfalse）
	bool sameDir = (leaderDir > 0.0f);

	//群れ全体の方向をリーダーに統一
	moveRight = sameDir;

	for (int i=0;i<count;++i){

		//ランダムな位置オフセットを生成
		Vector3 offset = {
			(float(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadRadius, 
			(float(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadRadius, 
			(float(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadRadius
		};

		//群れの中心にオフセットを加えた座標が魚の出現位置
		Vector3 spawnPos = centerPos + offset;

		
		// リーダーと同じ向きで初期化
		Fish* fish = new Fish();
		fish->Initialize(fishGroupModel_, camera_,nullptr, spawnPos, moveRight,30);

		//イベント用魚
		fish->SetEventType(FishEventType::swmmyFish);

		//群れリスト
		fishes_.push_back(fish);
	}
}

void SwimmyEvent::Reset() {
	for (auto& fish : fishes_) {

		delete fish;
	}

	fishes_.clear();
	isActive_ = false;
	groupTimer_ = 0;

	// ★ イベント終了通知
	if (onEventEnd_) {
		onEventEnd_();
	}
}