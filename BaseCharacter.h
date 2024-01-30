#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class BaseCharacter {
public:
	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(const ViewProjection& viewProjection);

	void SetLocalPosition(const Vector3& position);

	void SetLocalRotation(const Vector3& rotation);

	const Vector3& GetLocalPosition() const { return worldTransform_.translation_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

protected:
	std::vector<Model*> models_;
	WorldTransform worldTransform_;
};
