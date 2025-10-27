#pragma once
#include "Fish.h"
#include "KamataEngine.h"
#include "math.h"
#include "BGM.h"

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
	void Update(int fishCount, int bigFishCount, int rubbishCount);

	// 描画処理
	void Draw();

	//イベント魚を釣ったら呼ぶ
	void TriggerRandomWeather();

	//天気情報
	WeatherType GetWeatherType() const { return currentWeather_; }

	//天気がアクティブかどうか
	bool IsEventActive() const { return isActive_; }

	//魚出現・挙動補正用
	float GetFishSpeedMultiplier() const;
	float GetBigFishSpawnRate() const;
	float GetRubbishSpawnRate() const;
	float GetFishMaxCountMultiplier() const;

	bool WasRainJustEnded() const;

	bool isActive_ = false;

private:
	// 天気変更
	void ChangeWeather(WeatherType newWeather);
	// 元に戻す
	void ResetWeather();

	// カメラ
	Camera* camera_ = nullptr;

	//最初は晴れ
	WeatherType currentWeather_ = WeatherType::Clear;
	float timer_ = 0.0f;

	//天気の持続時間
	float duration_ = 60.0f;

	// 最大数追跡用
	int maxFishCount_ = 0;
	int maxBigFishCount_ = 0;
	int maxRubbishCount_ = 0;

	bool wasRaining_ = false;
	bool rainJustEnded_ = false;

	BGM* bgm_ = nullptr;

	uint32_t bgmBox_; // 四つのBGMを切り替えて流す箱

	uint32_t rainBgm_; // 雨の時のBGM

	uint32_t rainbowBgm_; // 虹の時

	uint32_t cloudBgm_; // 雲の時

	uint32_t meteorBgm_; // 隕石の時 
};
