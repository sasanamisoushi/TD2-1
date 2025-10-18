#define NOMINMAX
#include <algorithm>
#include "SwimmyEvent.h"

static float VecLength(const KamataEngine::Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

static KamataEngine::Vector3 VecNormalize(const KamataEngine::Vector3& v) {
	float len = VecLength(v);
	if (len <= 1e-6f)
		return {0.0f, 0.0f, 0.0f};
	return {v.x / len, v.y / len, v.z / len};
}


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

		// 方向をスムーズに補間
		float currentDir = fish->GetMoveDirectionY();
		float newDir = currentDir + (leaderDir - currentDir) * 0.05f;
		// 少しランダムに揺らぎを入れる
		newDir += (float(rand()) / RAND_MAX - 0.5f) * 0.05f;

		fish->SetMoveDirectionY(newDir);
		fish->SetIsMoveRight(leaderMoveRight);

		  // 固定オフセットを使って追従
		Vector3 targetPos = leaderPos + fish->GetInitialOffset();
		Vector3 current = fish->GetWorldPosition();

		// 目標までのベクトル
		Vector3 toTarget = {targetPos.x - current.x, targetPos.y - current.y, targetPos.z - current.z};
		// 距離と正規化（自前のユーティリティを使用）
		float distance = VecLength(toTarget);
		if (distance > 0.01f) {
			Vector3 dir = VecNormalize(toTarget);


			// 離れてる魚ほど速く追従
			float followSpeed = 0.02f + std::min(distance * 0.01f, 0.04f);

			current.x += dir.x * followSpeed;
			current.y += dir.y * followSpeed;
			current.z += dir.z * followSpeed;
		}

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

	leaderFish->SetInitialOffset({0, 0, 0});

	//群れのリストに追加
	fishes_.push_back(leaderFish);

	
	for (int i = 0; i < count; ++i) {
		Vector3 offset = {(float(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadRadius, (float(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadRadius, (float(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadRadius};

		Vector3 spawnPos = centerPos + offset;

		Fish* fish = new Fish();
		fish->Initialize(fishGroupModel_, camera_, nullptr, spawnPos, moveRight, 30);
		fish->SetEventType(FishEventType::swmmyFish);
		fish->SetInitialOffset(offset); // ★ オフセットを記憶
		fish->SetInEvent(true);
		leaderFish->SetInEvent(true);
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