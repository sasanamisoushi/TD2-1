#include "BGM.h"
#include "KamataEngine.h"
#include <stdio.h>

using namespace KamataEngine;

BGM::~BGM() 
{ 
	// デリート

	// nullprt

}

void BGM::Initialize()
{
	bgmHandle_ = 0; 
	
	titleBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/In the Sweet By and By.mp3");
	gamePlayBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/All the Fixings.mp3");
	gameClearBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/All the Fixings.mp3");

}

void BGM::titleBGMPlay()
{ 
	if (!Audio::GetInstance()->IsPlaying(bgmHandle_))
	{
		bgmHandle_ = Audio::GetInstance()->PlayWave(titleBgmHandle_, true);
	}
	
}

void BGM::titleBGMStop()
{
	if (Audio::GetInstance()->IsPlaying(bgmHandle_))
	{
		Audio::GetInstance()->StopWave(bgmHandle_);
	}
}

void BGM::gamePlayBGMPlay() 
{
	if (!Audio::GetInstance()->IsPlaying(bgmHandle_)) {
		bgmHandle_ = Audio::GetInstance()->PlayWave(gamePlayBgmHandle_, true);
	}
}

void BGM::gamePlayBGMStop()
{
	if (Audio::GetInstance()->IsPlaying(bgmHandle_)) 
	{
		Audio::GetInstance()->StopWave(bgmHandle_);
	}
}

void BGM::gameClearBGMPlay()
{
	if (!Audio::GetInstance()->IsPlaying(bgmHandle_)) 
	{
		bgmHandle_ = Audio::GetInstance()->PlayWave(gameClearBgmHandle_, true);
	}
}

void BGM::gameClearBGMStop() 
{
	if (Audio::GetInstance()->IsPlaying(bgmHandle_)) 
	{
		Audio::GetInstance()->StopWave(bgmHandle_);
	}
}

/* void BGM::BGMPlay(uint32_t bgmHandle) 
{
	if (!Audio::GetInstance()->IsPlaying(bgmHandle_))
	{
		bgmHandle_ = Audio::GetInstance()->PlayWave(bgmHandle, true);
	}
}
*/

/*void BGM::BGMStop(uint32_t bgmHandle)
{
	if (Audio::GetInstance()->IsPlaying(bgmHandle_))
	{
		Audio::GetInstance()->StopWave(bgmHandle_);
	}
}
*/
