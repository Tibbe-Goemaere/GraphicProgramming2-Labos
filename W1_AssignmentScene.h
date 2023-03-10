#pragma once
#include "GameScene.h"
class W1_AssignmentScene : public GameScene
{
public:
	W1_AssignmentScene() :GameScene(L"W1_AssignmentScene") {}
	~W1_AssignmentScene() override = default;
	W1_AssignmentScene(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene(W1_AssignmentScene&& other) noexcept = delete;
	W1_AssignmentScene& operator=(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene& operator=(W1_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
	
private:
	GameObject* m_pSphere{ nullptr };
	PxRigidDynamic* m_pSphereActor{ nullptr };
	std::vector<GameObject*> m_pWall{ nullptr };
	const int m_NrCols{ 10 }, m_NrRows{ 10 };
	const float m_BlockSize{ 1 };

	void PlaceWall();
	void ResetWall();
};

