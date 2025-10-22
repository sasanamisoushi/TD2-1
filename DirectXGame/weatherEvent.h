#pragma once
#include "Fish.h"
#include "KamataEngine.h"
#include "math.h"

class weatherEvent {
public:

	//天気
	enum class WeatherType {
		Clear,      //通常状態
		Rain,       //雨
		Rainbow,    //虹
		Cloud,      //雲
		Meteor      //隕石
	};


	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	//イベント魚を釣ったら呼ぶ
	void TriggerRandomWeather();

	//天気情報
	WeatherType GetWeatherType() const { return currentWeather_; }

	//天気がアクティブかどうか
	bool IsActive() const { return isActive_; }

	//魚出現・挙動補正用
	float GetFishSpeedMultiplier() const;
	float GetBigFishSpawnRate() const;
	float GetRubbishSpawnRate() const;

	

private:
	// 天気変更
	void ChangeWeather(WeatherType newWeather);
	// 元に戻す
	void ResetWeather();

	// カメラ
	Camera* camera_ = nullptr;

	bool isActive_ = false;

	//最初は雨
	WeatherType currentWeather_ = WeatherType::Clear;
	float timer_ = 0.0f;

	//天気の持続時間
	float duration_ = 60.0f;
};
