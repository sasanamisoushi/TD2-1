#pragma once
#include "Fish.h"
#include "KamataEngine.h"
#include "math.h"
#include <functional>

class SwimmyEvent {
public:
	// 初期化
	void Initialize(Model* fishModel, Model* leaderModel, Camera* camera, Score* score);

	// 更新処理
	void Update(Player* player);

	// 描画処理
	void Draw();

	// 魚の群れを出現させる
	//centerPos    :群れの中心座標
	//count        :生成する魚の数
	//spreadRadius :群れの広がり
	void SpawnFishGroup(const Vector3& centerPos, int count, float spreadRadius);

	//群れが存在するか
	bool HasFish() const { return !fishes_.empty(); }

	//イベント全体をリセット
	void Reset();

	 // ★ イベント終了時に呼ばれるコールバック
	void SetOnEventEnd(std::function<void()> callback) { onEventEnd_ = callback; }

private:

	std::vector<Fish*> fishes_;

	//魚のモデル
	Model* fishGroupModel_ = nullptr;
	Model* leaderModel_ = nullptr;

	//カメラ
	Camera* camera_ = nullptr;
	
	
	//---------群れの状態制御---------
	
	//イベントが発生中かどうか
	bool isActive_ = false;

	//群れの存在時間カウンタ
	float groupTimer_ = 0;

	//群れが存在する最大時間
	float groupLifetime_ = 60.0f;

	std::function<void()> onEventEnd_;

	Score* score_ = nullptr;
};
