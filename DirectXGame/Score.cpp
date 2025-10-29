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

	for (int i = 0; i < 5; i++) 
	{
		delete comboNumSprites_[i];
		comboNumSprites_[i] = nullptr;
	}
}

void Score::Initialize()
{
	
	Vector4 collar = {1, 1, 1, 1};
	// ===== 数字画像を読み込み =====
	for (int i = 0; i < 10; i++) {
		char path[64];
		sprintf_s(path, "num/%d.png", i);
		tex_[i] = TextureManager::Load(path);
	}

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

	// ===== ランキング表示位置と大きさを変更 =====
	// ※ここを自由に調整してランキング部分を見やすくできます
	const Vector2 rankBasePos = {430.0f, 360.0f}; // 左上位置（下げたいならYを大きく）
	const float rankDigitSpacing = 20.0f;         // 桁間隔（数字の横幅方向）
	const float rankLineSpacing = 135.0f;          // 行間（数字列の縦方向）
	const float rankScale = 2.0f;                 // 疑似スケール（spacingで調整）

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 10; j++) {
			// spacingをrankScale分だけ広げることで見た目を「拡大」
			Vector2 pos = {rankBasePos.x + j * (rankDigitSpacing * rankScale), rankBasePos.y + i * (rankLineSpacing)};
			rankingSprites[i][j] = Sprite::Create(tex_[0], pos, collar, {0.0f, 0.0f}, false, false);
		}
	}

	score_ = 0;

	FileOpen();

	isScoreBossClear = false;

	combo_ = 0;

	
	comboNumSprites_[0] = Sprite::Create(tex_[1], {970, 50}, collar, {0.0f, 0.0f}, false, false);
	comboNumSprites_[1] = Sprite::Create(tex_[2], {970, 50}, collar, {0.0f, 0.0f}, false, false);
	comboNumSprites_[2] = Sprite::Create(tex_[3], {970, 50}, collar, {0.0f, 0.0f}, false, false);
	comboNumSprites_[3] = Sprite::Create(tex_[4], {970, 50}, collar, {0.0f, 0.0f}, false, false);
	comboNumSprites_[4] = Sprite::Create(tex_[5], {970, 50}, collar, {0.0f, 0.0f}, false, false);
	

	comboGH = TextureManager::Load("num/combo.png");

	comboSprites_ = Sprite::Create(comboGH, {1000, 40}, collar, {0.0f, 0.0f}, false, false);
	


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
	score_ += score + (40 * (combo_));
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
	
	if (combo_ > 0)
	{
		comboNumSprites_[combo_]->Draw();
		comboSprites_->Draw();
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
	if (score_ >= 10)
	{
		isScoreBossClear = true;
	}
}
