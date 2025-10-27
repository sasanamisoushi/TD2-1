#define NOMINMAX
#include <algorithm>
#include "SwimmyEvent.h"
#include "Player.h"
#include <numbers>

static float VecLength(const KamataEngine::Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

static KamataEngine::Vector3 VecNormalize(const KamataEngine::Vector3& v) {
	float len = VecLength(v);
	if (len <= 1e-6f)
		return {0.0f, 0.0f, 0.0f};
	return {v.x / len, v.y / len, v.z / len};
}


void SwimmyEvent::Initialize(Model* fishModel, Model* leaderModel, Camera* camera, Score* score) {

	fishGroupModel_ = fishModel;
	leaderModel_ = leaderModel;
	camera_ = camera;
	score_ = score;
	

	isActive_ = false;
	groupTimer_ = 0;
}

void SwimmyEvent::Update(Player* player) {
	// 死んだ魚を削除
	fishes_.erase(
	    std::remove_if(
	        fishes_.begin(), fishes_.end(),
	        [](Fish* fish) {
		        if (!fish)
			        return true;
		        if (!fish->IsAlive()) {
			        delete fish;
			        return true;
		        }
		        return false;
	        }),
	    fishes_.end());

	//魚の群れが存在しなければ何もしない
	if (fishes_.empty()) {
	
		return;
	}

	groupTimer_ += 1.0f / 60.0f;

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

		/*fish->SetMoveDirectionY(newDir);
		fish->SetIsMoveRight(leaderMoveRight);*/

		fish->SetIsMoveRight(leaderMoveRight);

		if (leaderMoveRight) {
			// 右向きに設定
			fish->SetMoveDirectionX(1.0f);                        // 魚の内部 direction_.x を 1.0f に設定
			fish->SetRotationY(std::numbers::pi_v<float> / 2.0f); // 右向きの回転
		} else {
			// 左向きに設定
			fish->SetMoveDirectionX(-1.0f);                              // 魚の内部 direction_.x を -1.0f に設定
			fish->SetRotationY(std::numbers::pi_v<float> * 3.0f / 2.0f); // 左向きの回転
		}

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

	
	// === 当たり判定処理 ===
	for (auto& fish : fishes_) {

		if (!fish->IsAlive()) {
			continue;
		}

		fish->Update();

		// --- AABB衝突判定 ---
		AABB playerAABB = player->GetAABB();
		AABB fishAABB = fish->GetAABB();

		if (IsCollision(playerAABB, fishAABB)) {
			// 衝突した場合の処理
			if (fish->GetScore()) {
				// イベント魚は特別ボーナスをつける
				int eventBonus = 100; // ←好きな値に変更
				fish->GetScore()->AddScore(fish->GetPoint() + eventBonus);
			}

			// 魚を消す
			fish->SetIsAlive(false);
		}

		if (!fish ) {
			continue;
		}


		//Vector3 fishPos = fish->GetWorldPosition();
		//Vector3 playerPos = player->GetWorldPosition(); // Playerに関数がある前提

		//float dx = fishPos.x - playerPos.x;
		//float dy = fishPos.y - playerPos.y;
		//float dz = fishPos.z - playerPos.z;
		//float distanceSq = dx * dx + dy * dy + dz * dz;

		//const float collisionRadius = 0.8f; // ぶつかったとみなす距離
		//if (distanceSq < collisionRadius * collisionRadius) {
		//	// ★ Fish側の反応を呼ぶ
		//	fish->OnCollision(player);

		//	// ★ SwimmyEvent魚専用スコア処理
		//	if (fish->isSwimmyFish_) {

		//		 // イベント魚は通常より高い得点
		//		int eventBonus = 200;

		//		if(fish->GetScore()) {
		//			// イベントボーナス込みで単発加算
		//			fish->GetScore()->AddScore(fish->GetPoint() + eventBonus);
		//		}
		//		// イベント魚を消す
		//		fish->SetIsAlive(false);

		//		// 特殊演出やイベント終了をトリガー
		//		if (fish == fishes_.front() && onEventEnd_) {
		//			onEventEnd_();
		//		}
		//	}
		//}

		//
	}

	if (!fishes_.empty() && !fishes_.front()->IsAlive()) {
		Reset(); // または onEventEnd_() を呼ぶ
		return;
	}
	
	//群れの寿命を過ぎたら削除
	if (groupTimer_ > groupLifetime_) {
		
		Reset();
	}
	const float OUT_LIMIT_X = 17.0f; // 画面外とみなすX範囲
	const float OUT_LIMIT_Y = 4.0f; // Yの範囲（上下）
	const float OUT_LIMIT_Z = 25.0f; // 奥行き範囲

	// どれか一方向でも範囲外なら強制リセット
	if (fabs(leaderPos.x) > OUT_LIMIT_X || fabs(leaderPos.y) > OUT_LIMIT_Y || fabs(leaderPos.z) > OUT_LIMIT_Z) {
#ifdef _DEBUG 
			OutputDebugStringA("[SwimmyEvent] Leader out of screen -> Reset()\n");
#endif
		Reset();
		return; // 以降処理しない
	}

	#ifdef _DEBUG
	ImGui::Begin("Swimmy Event Debug");

	ImGui::Text("IsActive: %s", isActive_ ? "true" : "false");
	ImGui::Text("GroupTimer: %.2f / %.2f", groupTimer_, groupLifetime_);
	ImGui::Text("Fish Count: %d", static_cast<int>(fishes_.size()));

	int index = 0;
	for (auto& fish : fishes_) {
		if (!fish)
			continue;

		const Vector3& pos = fish->GetWorldPosition();
		ImGui::Separator();
		ImGui::Text("Fish[%d]", index++);
		ImGui::Text("Pos: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
		ImGui::Text("DirY: %.2f", fish->GetMoveDirectionY());
		ImGui::Text("MoveRight: %s", fish->GetIsMoveRight() ? "true" : "false");
		ImGui::Text("fishGetTimer_: %d", fish->fishGetTimer_);
		ImGui::Text("isSwimmyFish: %s", fish->isSwimmyFish_ ? "true" : "false");
		ImGui::Text("Alive: %s", fish->IsAlive() ? "true" : "false");
	}

	ImGui::End();
#endif
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

	  // 画面端の X 座標を決定
	const float OUT_LIMIT_X = 17.0f;                       // 画面端X
	float spawnX = moveRight ? -OUT_LIMIT_X : OUT_LIMIT_X; // 右向きなら左端から、左向きなら右端から
	Vector3 spawnPos = centerPos;
	spawnPos.z = 0.0f;
	spawnPos.x = spawnX;

	leaderFish->Initialize(leaderModel_, camera_, score_, spawnPos, moveRight, 30);

	//イベント魚として設定
	leaderFish->SetEventType(FishEventType::swmmyFish);

	leaderFish->isSwimmyFish_ = true;

	leaderFish->SetInitialOffset({0, 0, 0});

	

	//群れのリストに追加
	fishes_.push_back(leaderFish);

	
	for (int i = 0; i < count; ++i) {
		// X・Y は spreadRadius 範囲でばらける、Z は ±2 の範囲
		float offsetX = (float(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadRadius;
		float offsetY = (float(rand()) / RAND_MAX - 0.5f) * 2.0f * spreadRadius;
		float offsetZ = (float(rand()) / RAND_MAX - 0.5f) * 4.0f; // ±2の範囲

		Vector3 offset = {offsetX, offsetY, offsetZ};
		spawnPos = {0.0f + offsetX, centerPos.y + offsetY, 0.0f + offsetZ};
		spawnPos.x = spawnX;

		Fish* fish = new Fish();
		fish->Initialize(fishGroupModel_, camera_, score_, spawnPos, moveRight, 30);
		fish->SetEventType(FishEventType::swmmyFish);
		fish->SetInitialOffset(offset); // ★ オフセットを記憶
		fish->SetInEvent(true);
		fish->isSwimmyFish_ = true;
		leaderFish->SetInEvent(true);
		fishes_.push_back(fish);
	}
}

void SwimmyEvent::Reset() {
	if (!isActive_) {
		return;
	}

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

SwimmyEvent::~SwimmyEvent() {
// リストに残っている全ての Fish オブジェクトを削除
    for (Fish* fish : fishes_) {
        delete fish;
    }
    fishes_.clear();

}
