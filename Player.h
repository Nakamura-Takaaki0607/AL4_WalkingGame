#pragma once

#include "BaseCharacter.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player : public BaseCharacter {
public:
	enum {
		kModelIndexBody,
		kModelIndexHead,
		kModelIndexL_arm,
		kModelIndexR_arm,
	};
	// 座標の獲得
	float GetX() { return worldTransform_.matWorld_.m[3][0]; }
	float GetY() { return worldTransform_.matWorld_.m[3][1]; }
	float GetZ() { return worldTransform_.matWorld_.m[3][2]; }

	void PlayerStart();

	// 初期化
	void Initialize(const std::vector<Model*>& models) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(const ViewProjection& viewProjection) override;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

private:
	// 浮遊移動のサイクル<sec>
	int32_t floatingCycle_ = 90;
	// 浮遊振幅
	float floatingAmplitude_ = 0.2f;
	// 待機モーションの腕角度最大値
	float idleArmAngleMax_ = 30.0f;

	// ワールド変換データ
	// WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// ギミック初期化
	void InitializeFloatingGimmick();
	// ギミック更新
	void UpdateFloatingGimmick();

	// モデル
	// Model* modelBody_ = nullptr;
	// Model* modelHead_ = nullptr;
	// Model* modelL_arm_ = nullptr;
	// Model* modelR_arm_ = nullptr;

	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;
	// 入力
	Input* input_ = nullptr;
};
