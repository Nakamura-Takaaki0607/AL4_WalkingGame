#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
// #include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "AxisIndicator.h"
#include "DebugCamera.h"
#include "FollowCamera.h"

#include "Skydome.h"
#include "Ground.h"
#include "Player.h"
#include "Enemy.h"

#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>

class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	// タイトル
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;

	uint32_t textureHandleMove_ = 0;
	Sprite* spriteMove_ = nullptr;

	// 説明画面
	uint32_t textureHandleSetumei_ = 0;
	Sprite* spriteSetumei_ = nullptr;

	// ゲームオーバー
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;

	// ゲームクリア
	uint32_t textureHandleGameClear_ = 0;
	Sprite* spriteGameClear_ = nullptr;

	// ゲームクリア
	uint32_t textureHandleBlack_ = 0;
	Sprite* spriteBlack_ = nullptr;

	// タイム
	uint32_t textureHandleTime0_ = 0;
	Sprite* spriteTime0_ = nullptr;

	uint32_t textureHandleTime1_ = 0;
	Sprite* spriteTime1_ = nullptr;

	uint32_t textureHandleTime2_ = 0;
	Sprite* spriteTime2_ = nullptr;

	uint32_t textureHandleTime3_ = 0;
	Sprite* spriteTime3_ = nullptr;

	uint32_t textureHandleTime4_ = 0;
	Sprite* spriteTime4_ = nullptr;

	uint32_t textureHandleTime5_ = 0;
	Sprite* spriteTime5_ = nullptr;

	uint32_t textureHandleTime6_ = 0;
	Sprite* spriteTime6_ = nullptr;

	uint32_t textureHandleTime7_ = 0;
	Sprite* spriteTime7_ = nullptr;

	uint32_t textureHandleTime8_ = 0;
	Sprite* spriteTime8_ = nullptr;

	uint32_t textureHandleTime9_ = 0;
	Sprite* spriteTime9_ = nullptr;

	int sceneMode_ = 1;

	int playerLife_ = 1;

	int titleTimer_ = 0;

	int gameTimer_ = 600;

	int timerSUB_ = 0;

	bool flag_ = false;

	float black_ = 1.0f;

	float dx;

	float dz;

	void GamePlayStart();

	void TitleUpdate();

	void TitleDraw2DNear();

	void MoveDraw2DNear();

	void SetumeiUpdate();

	void SetumeiDraw2DNear();

	void GameOverUpdate();

	void GameOverDraw2DNear();

	void GameClearUpdate();

	void GameClearDraw2DNear();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 3Dモデル
	std::unique_ptr<Model> modelCube_;
	std::unique_ptr<Model> modelSkydome_;
	std::unique_ptr<Model> modelGround_;
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;
	std::unique_ptr<Model> modelEnemyBody_;
	std::unique_ptr<Model> modelEnemyL_arm_;
	std::unique_ptr<Model> modelEnemyR_arm_;

	// 自キャラ
	std::unique_ptr<Player> player_;
	// 敵
	std::unique_ptr<Enemy> enemy_;
	// 天球
	std::unique_ptr<Skydome> skydome_;
	// 地面
	std::unique_ptr<Ground> ground_;

	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
};