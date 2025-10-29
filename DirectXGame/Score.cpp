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
	// ===== 数字画像を読み込み =====
	for (int i = 0; i < 10; i++) {
		char path[64];
		sprintf_s(path, "num/%d.png", i);
		tex_[i] = TextureManager::Load(path);
	}

	SetScoreDisplayMode(DisplayMode::Normal);

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

			rankingSprites[i][j]->SetSize({40, 40});
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

void Score::SetScoreDisplayMode(DisplayMode mode) {
	displayMode_ = mode;

	// 共通の色
	Vector4 color = {1, 1, 1, 1};

	// 各モードごとの設定
	float scale = 1.0f; // スプライトの拡大率
	float baseX = 0.0f;
	float baseY = 0.0f;
	float spacing = 10.0f; //行間隔


	if (mode == DisplayMode::Normal) {
		scale = 30.0f;
		baseX = 950.0f;
		baseY = 20.0f;
		spacing = 20.0f ;
	} else if (mode == DisplayMode::Result) {
		scale = 70.0f;
		baseX = 600.0f; // 画面中央寄りに
		baseY = 150.0f;
		spacing = 45.0f ;
	}



	// 一旦既存スプライト削除（再作成するため）
	for (int i = 0; i < 10; i++) {
		delete sprites_[i];
		sprites_[i] = nullptr;
	}

	// 新しいスプライトを再生成
	for (int i = 0; i < 10; i++) {
		Vector2 pos = {baseX + spacing * i, baseY};
		sprites_[i] = Sprite::Create(tex_[0], pos, color, {0.0f, 0.0f}, false, false);

		// スプライトのサイズ変更
		sprites_[i]->SetSize({scale, scale});
	}


}
