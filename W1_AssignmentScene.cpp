#include "stdafx.h"
#include "W1_AssignmentScene.h"

#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"
#include "Logger.h"

enum InputIds
{
	Up,
	Down,
	Left,
	Right,
	Jump
};

void W1_AssignmentScene::Initialize()
{

	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pSphereMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.f);
	const PxMaterial* pFloorMaterial = pPhysX->createMaterial(0.f, 0.f, 0.f);
	

	//Sphere
	const float radius{ 1.f };
	const int parts{ 20 };
	m_pSphere = new SpherePosColorNorm(radius, parts, parts,XMFLOAT4(Colors::Yellow));
	m_pSphere->Translate(0.f, 1.f, -40.f);
	AddGameObject(m_pSphere);

	//Sphere actor
	m_pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxSphereGeometry sphereGeometry = PxSphereGeometry{ radius };
	PxRigidActorExt::createExclusiveShape(*m_pSphereActor, sphereGeometry, *pSphereMaterial);
	m_pSphereActor->setMass(100);
	m_pSphereActor->setMassSpaceInertiaTensor({10,10,10});
	

	//Link sphere with sphere actor
	m_pSphere->AttachRigidActor(m_pSphereActor);

	//Floor
	const XMFLOAT3 floorDimensions = { 100.f,1.f,100.f };
	auto pFloor = new CubePosColorNorm(floorDimensions.x,floorDimensions.y,floorDimensions.z,XMFLOAT4(Colors::LightGray));
	pFloor->Translate(0.f, -0.5f, 0.f);
	AddGameObject(pFloor);

	//Floor actor
	auto pFloorActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	PxBoxGeometry floorGeometry = PxBoxGeometry{ floorDimensions.x / 2.f,floorDimensions.y / 2.f,floorDimensions.z / 2.f };
	PxRigidActorExt::createExclusiveShape(*pFloorActor, floorGeometry, *pSphereMaterial);

	//Link floor to floor actor
	pFloor->AttachRigidActor(pFloorActor);

	//Wall
	PlaceWall();

	//Input
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Down, InputTriggerState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Up, InputTriggerState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Right, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Left, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Jump, InputTriggerState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A });

	//Camera
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ -15.f,20.f,-60.f });
	m_SceneContext.GetCamera()->SetForward({ 0.5f,-0.5f,1.f });
}

void W1_AssignmentScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pSphere->Translate(0.f, 1.f, -40.f);
		m_pSphere->RotateDegrees(0.f, 0.f, 0.f);
		ResetWall();
	}
	const float force{1000};
	const float jumpForce{ 30000 };
	if (m_SceneContext.GetInput()->IsActionTriggered(Left))
	{
		m_pSphereActor->addTorque(PxVec3{ force * m_SceneContext.GetCamera()->GetForward().x,0,force * m_SceneContext.GetCamera()->GetForward().z });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Right))
	{
		m_pSphereActor->addTorque(PxVec3{ -force * m_SceneContext.GetCamera()->GetForward().x,0,-force * m_SceneContext.GetCamera()->GetForward().z });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Up))
	{
		m_pSphereActor->addTorque(PxVec3{ force * m_SceneContext.GetCamera()->GetRight().x,0,force* m_SceneContext.GetCamera()->GetRight().z });
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Down))
	{
		m_pSphereActor->addTorque(PxVec3{ -force * m_SceneContext.GetCamera()->GetRight().x,0,-force * m_SceneContext.GetCamera()->GetRight().z });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, ' '))
	{
		
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Jump))
	{
		m_pSphereActor->addForce(PxVec3{ 0,jumpForce,0 });
	}
}

void W1_AssignmentScene::Draw() const
{

}

void W1_AssignmentScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}

void W1_AssignmentScene::PlaceWall()
{
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pWallMaterial = pPhysX->createMaterial(1.f, 1.f, 0.f);

	const float maxWiggleSpace{ 1.15f * m_BlockSize };
	
	const float startPos{ -maxWiggleSpace * m_NrCols / 2.f };
	m_pWall.reserve(m_NrCols * m_NrRows);
	for (int i = 0; i < m_NrCols * m_NrRows; i++)
	{
		m_pWall.push_back(new CubePosColorNorm(0,0,0));
	}

	for (int row = 0; row < m_NrRows; row++)
	{
		for (int col = 0; col < m_NrCols; col++)
		{
			const int idx{ row * m_NrRows + col };
			//Cube
			const XMFLOAT3 actorDimensions{ 1.f,1.f,1.f };
			m_pWall[idx] = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
			AddGameObject(m_pWall[idx]);

			//Cube actor
			auto pBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
			PxBoxGeometry boxGeometry = PxBoxGeometry{ actorDimensions.x / 2.f,actorDimensions.y / 2.f,actorDimensions.z / 2.f };
			PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pWallMaterial);

			m_pWall[idx]->AttachRigidActor(pBoxActor);
			m_pWall[idx]->Translate(startPos + col * maxWiggleSpace, row * m_BlockSize * 1.08f, 0.f);
			m_pWall[idx]->RotateDegrees(0.f, static_cast<float>(rand() % 20 - 10), 0.f);
		}
	}
}

void W1_AssignmentScene::ResetWall()
{
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pWallMaterial = pPhysX->createMaterial(1.f, 1.f, 0.f);

	const float maxWiggleSpace{ 1.15f * m_BlockSize };

	const float startPos{ -maxWiggleSpace * m_NrCols / 2.f };
	for (int row = 0; row < m_NrRows; row++)
	{
		for (int col = 0; col < m_NrCols; col++)
		{
			const int idx{ row * m_NrRows + col };
			m_pWall[idx]->Translate(startPos + col * maxWiggleSpace, row * m_BlockSize * 1.08f, 0.f);
			m_pWall[idx]->RotateDegrees(0.f, static_cast<float>(rand() % 20 - 10), 0.f);
		}
	}
}
