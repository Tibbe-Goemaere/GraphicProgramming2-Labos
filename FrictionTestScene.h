#pragma once
#include "GameScene.h"
class FrictionTestScene : public GameScene
{
public:
	FrictionTestScene() :GameScene(L"FrictionTestScene") {}
	~FrictionTestScene() override = default;
	FrictionTestScene(const FrictionTestScene& other) = delete;
	FrictionTestScene(FrictionTestScene&& other) noexcept = delete;
	FrictionTestScene& operator=(const FrictionTestScene& other) = delete;
	FrictionTestScene& operator=(FrictionTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	//Boxes
	GameObject* m_pCube1{ nullptr };
	GameObject* m_pCube2{ nullptr };
	GameObject* m_pCube3{ nullptr };

	//Ramps
	GameObject* m_pRamp1{ nullptr };
	GameObject* m_pRamp2{ nullptr };
	GameObject* m_pRamp3{ nullptr };
};

