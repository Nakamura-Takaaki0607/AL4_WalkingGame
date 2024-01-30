#pragma once

#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:
	enum {
		kModelIndexBody,
		kModelIndexL_arm,
		kModelIndexR_arm,
	};

	// 座標の獲得
	float GetX() { return worldTransform_.matWorld_.m[3][0]; }
	float GetY() { return worldTransform_.matWorld_.m[3][1]; }
	float GetZ() { return worldTransform_.matWorld_.m[3][2]; }

	void EnemyStart();

	float floatingParameter = 0.0f;

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void UpdateGimmick();

protected:
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
};
