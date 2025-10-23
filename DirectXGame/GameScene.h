#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Fish.h"
#include "BigFish.h"
#include "Score.h"
#include "Rubbish.h"
#include "SwimmyEvent.h"
#include "EventFish.h"
#include "bearEvent.h"
#include "weatherEvent.h"
#include "Fade.h"

class GameScene 
{
public:

	//フェード
	enum class Phase {
		kFadeIn,
		kMain,
		kfadeOut

	};

	//初期化
	void Initialize(Score* score);
	~GameScene();
	//更新
	void Update();

	//描画
	void Draw();

	// 全ての当たり判定
	void CheckAllCollisions();

	void CheckBearCollisions();

	//デスフラグのgetter
	bool IsFinished() const { return isFinish; }


	int getTimer_;

	void SpawnFish();

	

	void ClearAllFish();

	Model* GetFishModel() { return fishModel_; }
	Model* GetSwimmyModel() { return swimmyModel_; }

	Camera* GetCamera() { return &camera_; }


	Vector3 GetRandomPos();

	void AddFish(Fish* fish);
	
	
private:

	Player* player_ = nullptr;
	Model* model_ = nullptr;
	Model* playerModel_ = nullptr;
	Camera camera_;
	Score* score_ = nullptr;
	SwimmyEvent* swimmyEvent_=nullptr;
	bearEvent* bearEvent_=nullptr;
	weatherEvent* weatherEvent_=nullptr;

	
	// フェード
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
	 

	//========================================
	//魚
	//========================================
	
	// 魚モデル
	Model* fishModel_ = nullptr;

	std::list<Fish*> fishes_;


	//大きい魚のモデル
	Model* bigFishModel_ = nullptr;
	std::list<BigFish*> BigFishes_;

	//ゴミ
	Model* rubbishModel_ = nullptr;
	std::list<Rubbish*> rubbishes_;

	//========================================
	// イベント魚
	//========================================

	//スイミー
	Model* swimmyModel_ = nullptr;
	std::list<EventFish*> events_;
	

	//ベアー
	Model* bearModel_ = nullptr;
	// 熊のルアー
	Model* bearLureModel_ = nullptr;
	

	//天気
	Model* weatherModel_ = nullptr;

	// 現在数をカウント
	int smallCount = 0;
	int bigCount = 0;
	int rubbishCount = 0;
	int eventCount = 0;

	//終了フラグ
	bool isFinish = false;

	// タイマー
	uint32_t numTexHandles_[10];
	Sprite* numSprite_[3];
	int gameTimer_ = 18000;
	bool isGame_ = true; // ゲーム中か

	static GameScene* instance_;



	//フェード用のタイマー
	float timer;

	

	
};
