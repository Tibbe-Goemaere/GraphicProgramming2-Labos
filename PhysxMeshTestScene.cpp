#include "stdafx.h"
#include "PhysxMeshTestScene.h"

#include "MeshObject.h"
#include "ContentManager.h"
#include "Logger.h"

enum InputIds
{
	Left,
	Right
};

void PhysxMeshTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = pPhysX->createMaterial(0.5, 0.5f, 0.2f);

	//Ground plane
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo, {0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Convex Chair
	m_pChairConvex = new MeshObject(L"Resources/Meshes/Chair.ovm");
	AddGameObject(m_pChairConvex);

	const auto pConvexMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc");
	const auto pConvexActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pConvexActor, PxConvexMeshGeometry{ pConvexMesh }, *pDefaultMaterial);

	//PxVec3 massLocalPose{0.f,4.f,0.f};
	//PxRigidBodyExt::updateMassAndInertia(*pConvexActor, 10.f,&massLocalPose);
	
	m_pChairConvex->AttachRigidActor(pConvexActor);

	m_pChairConvex->Translate(0, 100.f, 0);
	m_pChairConvex->RotateDegrees(45.f, 45.f, 45.f);

	//Triangle Chair
	m_pChairTriangle = new MeshObject(L"Resources/Meshes/Chair.ovm");
	AddGameObject(m_pChairTriangle);

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Chair.ovpt");
	const auto pTriangleActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	m_pChairTriangle->AttachRigidActor(pTriangleActor);

	m_pChairTriangle->Translate(10.f, 0.f, 0);
	m_KinematicPosition = { 10.f,0.f,0.f };

	//Input
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Right, InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Left, InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT });
}

void PhysxMeshTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pChairConvex->Translate(0, 100, 0);

		m_pChairTriangle->Translate(10.f, 0.f, 0);
		m_KinematicPosition = { 10.f,0.f,0.f };
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(Left))
	{
		m_KinematicPosition.x -= 5.f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{m_KinematicPosition});
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Right))
	{
		m_KinematicPosition.x += 5.f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPosition });
	}
}

void PhysxMeshTestScene::Draw() const
{

}

void PhysxMeshTestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void PhysxMeshTestScene::OnSceneDeactivated()
{
}
