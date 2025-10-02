#pragma once
class TitleScene {
public:

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();


	//デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:

	//終了フラグ
	bool finished_ = false;



};
