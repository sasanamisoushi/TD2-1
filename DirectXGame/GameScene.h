#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Fish.h"
#include "BigFish.h"
#include "Rubbish.h"
#include "SwimmyEvent.h"
#include "EventFish.h"
#include "SwimmyEvent.h"



class GameScene 
{
public:

	//初期化
	void Initialize();
	~GameScene();
	//更新
	void Update();

	//描画
	void Draw();

	// 全ての当たり判定
	void CheckAllCollisions();

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
	
	static GameScene* GetInstance();
	
private:

	Player* player_ = nullptr;
	Model* model_ = nullptr;
	Model* playerModel_ = nullptr;
	Camera camera_;

	
	
	 

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

	//イベント魚
	Model* swimmyModel_ = nullptr;
	std::list<EventFish*> swimmys_;

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

	//群れのイベント管理用
	SwimmyEvent swimmyEvent_;
};
