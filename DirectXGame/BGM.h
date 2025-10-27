#pragma once
#include <array>

namespace KamataEngine 
{
	class Audio;
}

class BGM
{
public:

	~BGM();

	void Initialize();

	// タイトル
	// BGMスタート
	void titleBGMPlay();
	// BGMストップ
	void titleBGMStop();
	// BGMスタート
	void gamePlayBGMPlay();
	// BGMストップ
	void gamePlayBGMStop();
	// BGMスタート
	void gameClearBGMPlay();
	// BGMストップ
	void gameClearBGMStop();

	//void BGMPlay(uint32_t bgmHandle);

	//void BGMStop(uint32_t bgmHandle);

private:

	uint32_t bgmHandle_; // BGMを入れる箱

	uint32_t titleBgmHandle_; // タイトルのBGM

	uint32_t gamePlayBgmHandle_; // プレイ中のBGM

	uint32_t gameClearBgmHandle_; // クリアしたときのBGM

};
