#pragma once
#include "KamataEngine.h"
#include "math.h"

using namespace KamataEngine;

class Player;


enum class FishEventType {
	None,            //通常
	swmmyFish        //swmmy
};

class Score;


enum class FishState {
	Appear, //登場中
	Normal, //通常行動
};

class Fish {
public:
	// 初期化（座標と速度方向を指定）

	void Initialize(Model* model, Camera* camera, Score* score, const Vector3& targetPos, bool moveRight, int getTimer);

	// 更新
	virtual void Update();

	// 描画
	void Draw();

	Vector3 GetWorldPosition() const { return worldTransform_.translation_; }

	KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();

	// ルアーと当たっている
	virtual void OnCollision(Player* player);
	// ルアーと当たってない
	void OutCollision();

	// フラグ
	// 魚がルアーの範囲内にいるか
	bool isLureCheck_ = false;
	bool IsLureCheck() const { return isLureCheck_; }

	// 魚がゲットできる時間
	int fishGetTimer_;
	// タイマーのリセット
	int resetTimer_;

	//イベントタイプを設定
	void SetEventType(FishEventType type) { eventType_ = type; }

	//イベントタイプを取得
	FishEventType GetEventType() const { return eventType_; }

	//魚の見た目を変える
	void SetTexture(const std::string& filePath);

	float GetMoveDirectionY() const;

	void SetWorldPosition(const Vector3& pos);

	// 移動方向（角度）を設定
	void SetMoveDirectionY(float dir) { direction_.y = dir; }

	// 右向き／左向きフラグを設定
	void SetIsMoveRight(bool flag) { moveRight_ = flag; }

	bool GetIsMoveRight() const { return moveRight_; }
	
private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	//カメラ
	Camera* camera_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Score* score_;

	//向き
	Vector3 direction_;
	
	// 速度
	Vector3 velocity_ = {};

	static inline const float speed_ = 0.01f;
	float radius_ = 1.0f;

	float leftLimit_ = 0.0f;
	float rigdhtLimit_ = 0.0f;

	FishState state_ = FishState::Appear;

	 // 目的位置
	Vector3 targetPos_;

	//登場演出タイマー
	float appearTimer_ = 0.0f;

	//約1秒で登場
	float appearDuration_= 60.0f;

	bool moveRight_ = true;

	Vector3 finalScale_;


	//最初は普通の魚を出す
	FishEventType eventType_ = FishEventType::None;

	const int point_ = 100;
	

};
