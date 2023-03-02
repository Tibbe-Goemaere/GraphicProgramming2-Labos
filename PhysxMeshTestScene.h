#pragma once
#include "GameScene.h"
class PhysxMeshTestScene final : public GameScene
{
public:
	PhysxMeshTestScene() :GameScene(L"PhysxMeshTestScene") {}
	~PhysxMeshTestScene() override = default;
	PhysxMeshTestScene(const PhysxMeshTestScene& other) = delete;
	PhysxMeshTestScene(PhysxMeshTestScene&& other) noexcept = delete;
	PhysxMeshTestScene& operator=(const PhysxMeshTestScene& other) = delete;
	PhysxMeshTestScene& operator=(PhysxMeshTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	GameObject* m_pChairConvex{ nullptr };
	GameObject* m_pChairTriangle{ nullptr };

	PxVec3 m_KinematicPosition{};
	
};

