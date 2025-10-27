#include "Score.h"
#include "KamataEngine.h"
#include <stdio.h>

using namespace KamataEngine;

Score::~Score() 
{
	for (int i = 0; i < 10; i++) 
	{
		delete sprites_[i];
		sprites_[i] = nullptr;
	}
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 10; j++)
		{
			delete rankingSprites[i][j];
		}
	}
}

void Score::Initialize()
{
	
	Vector4 collar = {1, 1, 1, 1};
	tex_[0] = TextureManager::Load("num/0.png");
	tex_[1] = TextureManager::Load("num/1.png");
	tex_[2] = TextureManager::Load("num/2.png");
	tex_[3] = TextureManager::Load("num/3.png");
	tex_[4] = TextureManager::Load("num/4.png");
	tex_[5] = TextureManager::Load("num/5.png");
	tex_[6] = TextureManager::Load("num/6.png");
	tex_[7] = TextureManager::Load("num/7.png");
	tex_[8] = TextureManager::Load("num/8.png");
	tex_[9] = TextureManager::Load("num/9.png");

	sprites_[0] = Sprite::Create(tex_[0], {970, 10}, collar, {0.0f, 0.0f}, false, false);
	sprites_[1] = Sprite::Create(tex_[0], {1000, 10}, collar, {0.0f, 0.0f}, false, false);
	sprites_[2] = Sprite::Create(tex_[0], {1030, 10}, collar, {0.0f, 0.0f}, false, false);
	sprites_[3] = Sprite::Create(tex_[0], {1060, 10}, collar, {0.0f, 0.0f}, false, false);
	sprites_[4] = Sprite::Create(tex_[0], {1090, 10}, collar, {0.0f, 0.0f}, false, false);
	sprites_[5] = Sprite::Create(tex_[0], {1120, 10}, collar, {0.0f, 0.0f}, false, false);
	sprites_[6] = Sprite::Create(tex_[0], {1150, 10}, collar, {0.0f, 0.0f}, false, false);
	sprites_[7] = Sprite::Create(tex_[0], {1180, 10}, collar, {0.0f, 0.0f}, false, false);
	sprites_[8] = Sprite::Create(tex_[0], {1210, 10}, collar, {0.0f, 0.0f}, false, false);
	sprites_[9] = Sprite::Create(tex_[0], {1240, 10}, collar, {0.0f, 0.0f}, false, false);

	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 10; j++)
		{
			rankingSprites[i][j] = Sprite::Create(tex_[0], {float(970 + (30 * j)), float(60 + (40 * i))}, collar, {0.0f, 0.0f}, false, false);
		}
	}

	score_ = 0;

	FileOpen();

	isScoreBossClear = false;

}

void Score::AddScore(int score)
{
	score_ += score; 
	combo_ = 0;
}

void Score::AddScoreBear(int score) 
{ 
	score_ += score; 
}

void Score::AddScoreCombo(int score) 
{ 
	if (combo_ < 6) 
	{
		combo_ += 1;
	}
	score_ += score + (40 * (combo_ - 1));
}

void Score::SubtractScore(int score)
{
	combo_ = 0;
	if (score_ > 0)
	{
		if (score_ - score <= 0)
		{
			score_ = 0;
		} 
		else 
		{
			score_ -= score;
		}
	}
}

void Score::ResetScore()
{ 
	score_ = 0; 
}

void Score::Update()
{
	for (int i = 0; i < 10; i++) 
	{
		delete sprites_[i];
		sprites_[i] = nullptr;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 10; j++) {
			delete rankingSprites[i][j];
		}
	}
}

void Score::Draw()
{
	int ketaNum = 10; // 桁数
	std::array<int, 10> keta = {0};
	int viewScore = score_;
	for (int i = 0; i < ketaNum; i++) 
	{
		keta[ketaNum - i - 1] = viewScore % 10;
		viewScore /= 10;
	}
	for (int i = 0; i < ketaNum; i++)
	{
		sprites_[i]->SetTextureHandle(tex_[keta[i]]);
		sprites_[i]->Draw();
	}
}

void Score::RankingDraw()
{
	int ketaNum = 10; // 桁数
	std::array<std::array<int, 10>,3> keta = {0};
	int viewScore[3];

	for (int i = 0; i < 3; i++) 
	{
		viewScore[i] = rankingScore_[i];
	}
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < ketaNum; j++) 
		{
			keta[i][ketaNum - j - 1] = viewScore[i] % 10;
			viewScore[i] /= 10;
		}
		
	}
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < ketaNum; j++)
		{
			rankingSprites[i][j]->SetTextureHandle(tex_[keta[i][j]]);
			rankingSprites[i][j]->Draw();
		}
	}
}

void Score::FileOpen()
{
	FILE* fp;
	char scoreFile[] = "score.txt";
	int err;
	err = fopen_s(&fp, scoreFile, "r");
	int readScore = {};
	if (err != 0) 
	{
		//return err;
	} 
	else
	{
		for (int i = 0; i < 3; i++)
		{
			while (fscanf_s(fp, "%d", &readScore) == 10) {
				fscanf_s(fp, "%d", &readScore);
			}
			rankingScore_[i] = readScore;
		}

		fclose(fp);
	}
}

void Score::FileWrite() {
	FILE* fp;
	char scoreFile[] = "score.txt";
	int err;
	err = fopen_s(&fp, scoreFile, "r");
	int readScore = {};
	if (err != 0) {
		// return err;
	} else {
		for (int i = 0; i < 3; i++) {
			while (fscanf_s(fp, "%d", &readScore) == 10) {
				fscanf_s(fp, "%d", &readScore);
			}
			rankingScore_[i] = readScore;
		}
		fclose(fp);
	}
	err = fopen_s(&fp, scoreFile, "w");
	if (err != 0) {
		// return err;
	}
	else
	{
		if (score_ > rankingScore_[0]) 
		{
			while (1) {
				fprintf_s(fp, "%d\n", score_);
				fprintf_s(fp, "%d\n", rankingScore_[0]);
				fprintf_s(fp, "%d\n", rankingScore_[1]);
				break;
			}
		} 
		else if (score_ > rankingScore_[1])
		{
			while (1) {
				fprintf_s(fp, "%d\n", rankingScore_[0]);
				fprintf_s(fp, "%d\n", score_);
				fprintf_s(fp, "%d\n", rankingScore_[1]);
				break;
			}
		} 
		else if (score_ > rankingScore_[2])
		{
			while (1) {
				fprintf_s(fp, "%d\n", rankingScore_[0]);
				fprintf_s(fp, "%d\n", rankingScore_[1]);
				fprintf_s(fp, "%d\n", score_);
				break;
			}
		}
		else
		{
			while (1) {
				fprintf_s(fp, "%d\n", rankingScore_[0]);
				fprintf_s(fp, "%d\n", rankingScore_[1]);
				fprintf_s(fp, "%d\n", rankingScore_[2]);
				break;
			}
		}

		fclose(fp);
	}
}

void Score::scoreBossClear()
{
	if (score_ >= 200000)
	{
		isScoreBossClear = true;
	}
}
