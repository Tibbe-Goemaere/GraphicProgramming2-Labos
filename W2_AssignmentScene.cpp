#include "stdafx.h"
#include "W2_AssignmentScene.h"

#include "MeshObject.h"
#include "ContentManager.h"
#include "SoundManager.h"
#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"
#include "Logger.h"

void W2_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = pPhysX->createMaterial(0.5, 0.5f, 0.1f);
	auto pMovingCubeMaterial = pPhysX->createMaterial(0.5, 0.5f, 0.5f);

	//Ground plane
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo, {0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);

	m_pPhysxScene->addActor(*pGroundActor);

	//Level
	m_pLevel = new MeshObject(L"Resources/Meshes/Level.ovm");
	AddGameObject(m_pLevel);

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");
	const auto pTriangleActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	m_pLevel->AttachRigidActor(pTriangleActor);


	//Sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::IndianRed });
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);

	m_pSphere->AttachRigidActor(pSphereActor);
	m_pSphere->Translate(0.f, 15.f, 0.f);
	pSphereActor->setMass(100.f);
	pSphereActor->setMassSpaceInertiaTensor(PxVec3{ 20.f,20.f,20.f });

	//Joint to lock position in xy plane
	auto d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pSphereActor, PxTransform{ PxIdentity });
	//joint->setMotion(PxD6Axis::eZ, PxD6Motion::eLIMITED);
	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	//Trigger left
	auto pos = PxTransform{ -7.3f,2.5f,0 };
	auto dimensions = PxBoxGeometry{ 1.f,0.4f,3.f };
	m_pTriggerLeft = pPhysX->createRigidStatic(pos);
	auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, dimensions, *pDefaultMaterial);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerLeft);

	//Trigger right visual
	auto triggerLeftBox = new CubePosColorNorm(dimensions.halfExtents.x * 2, dimensions.halfExtents.y * 2, dimensions.halfExtents.z * 2, XMFLOAT4{ Colors::DarkBlue });
	triggerLeftBox->Translate(pos.p.x,pos.p.y,pos.p.z);
	AddGameObject(triggerLeftBox);

	//Trigger right
	pos = PxTransform{ 6.7f,2.5f,0 };
	dimensions = PxBoxGeometry{ 1.f,0.4f,3.f };
	m_pTriggerRight = pPhysX->createRigidStatic(pos);
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight,dimensions , *pDefaultMaterial);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerRight);

	//Trigger left visual
	triggerLeftBox = new CubePosColorNorm(dimensions.halfExtents.x * 2, dimensions.halfExtents.y * 2, dimensions.halfExtents.z * 2, XMFLOAT4{ Colors::DarkRed });
	triggerLeftBox->Translate(pos.p.x, pos.p.y, pos.p.z);
	AddGameObject(triggerLeftBox);

	//Left cube
	const XMFLOAT3 actorCubeDimensions{ 1.5f,1.5f,1.5f };
	m_pCubeLeft = new CubePosColorNorm(actorCubeDimensions.x, actorCubeDimensions.y, actorCubeDimensions.z, XMFLOAT4{Colors::Blue});
	AddGameObject(m_pCubeLeft);

	auto pBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeometry = PxBoxGeometry{ actorCubeDimensions.x / 2.f,actorCubeDimensions.y / 2.f,actorCubeDimensions.z / 2.f };
	PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pDefaultMaterial);

	m_pCubeLeft->AttachRigidActor(pBoxActor);
	m_pCubeLeft->Translate(-2.f,5.f, 0.f);

	//Joint to lock position in xy plane
	d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pBoxActor, PxTransform{ PxIdentity });
	//joint->setMotion(PxD6Axis::eZ, PxD6Motion::eLIMITED);
	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	//Right cube
	m_pCubeRight = new CubePosColorNorm(actorCubeDimensions.x, actorCubeDimensions.y, actorCubeDimensions.z, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pCubeRight);

	pBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pDefaultMaterial);

	m_pCubeRight->AttachRigidActor(pBoxActor);
	m_pCubeRight->Translate(2.f, 5.f, 0.f);

	//Joint to lock position in xy plane
	d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pBoxActor, PxTransform{ PxIdentity });
	//joint->setMotion(PxD6Axis::eZ, PxD6Motion::eLIMITED);
	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	//Bell sound
	const auto pFmod = SoundManager::GetInstance()->GetSystem();
	FMOD_RESULT result = pFmod->createSound("Resources/Sounds/bell.mp3", FMOD_2D, nullptr, &m_pBellSound);
	SoundManager::GetInstance()->ErrorCheck(result);

	//Left hatch
	auto actorHatchDimensions = XMFLOAT3{ 2.f,0.2f,5.f };
	m_pHatchLeft = new CubePosColorNorm(actorHatchDimensions.x, actorHatchDimensions.y, actorHatchDimensions.z, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pHatchLeft);

	auto pHatchActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	boxGeometry = PxBoxGeometry{ actorHatchDimensions.x / 2.f,actorHatchDimensions.y / 2.f,actorHatchDimensions.z / 2.f };
	PxRigidActorExt::createExclusiveShape(*pHatchActor, boxGeometry, *pDefaultMaterial);
	pHatchActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	m_pHatchLeft->AttachRigidActor(pHatchActor);
	m_pHatchLeft->Translate(-9.f, 17.f, 0.f);

	//Right hatch
	m_pHatchRight = new CubePosColorNorm(actorHatchDimensions.x, actorHatchDimensions.y, actorHatchDimensions.z, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pHatchRight);

	pHatchActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pHatchActor, boxGeometry, *pDefaultMaterial);
	pHatchActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	m_pHatchRight->AttachRigidActor(pHatchActor);
	m_pHatchRight->Translate(9.f, 17.f, 0.f);
	
	//Input
	const auto pInput = m_SceneContext.GetInput();

}

inline FMOD_VECTOR ToFMod(XMFLOAT3 v) //DirectX
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

inline FMOD_VECTOR ToFMod(PxVec3 v) //PhysX
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

void W2_AssignmentScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pSphere->Translate(0.f, 10.f, 0.f);
		m_pSphere->RotateDegrees(0.f, 0.f, 0.f);
	}

	const float force{ 300.f }, jumpForce{50000.f};
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,-force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, VK_SPACE))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0,jumpForce,0 });
	}


	const float speed{100}, maxRotation{90};
	//Trigger check
	if (m_IsTriggeredLeft)
	{
		if (m_LeftRotateAngle < 90)
		{
			m_LeftRotateAngle += m_SceneContext.GetGameTime()->GetElapsed() * speed;
			m_pHatchLeft->RotateDegrees(0.f,0.f,m_LeftRotateAngle);
		}
	}

	if (m_IsTriggeredRight)
	{
		if (m_RightRotateAngle > -90)
		{
			m_RightRotateAngle -= m_SceneContext.GetGameTime()->GetElapsed() * speed;
			m_pHatchRight->RotateDegrees(0.f, 0.f, m_RightRotateAngle);
		}
	}
}

void W2_AssignmentScene::Draw() const
{

}

void W2_AssignmentScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void W2_AssignmentScene::OnSceneDeactivated()
{
}

void W2_AssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i{ 0 }; i < count; ++i)
	{
		//ignore deleted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		const PxTriggerPair& pair = pairs[i];

		if (pair.triggerActor == m_pTriggerLeft && pair.otherActor == m_pCubeLeft->GetRigidActor())
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //entered
			{
				m_IsTriggeredLeft = true;
				SoundManager::GetInstance()->GetSystem()->playSound(m_pBellSound,nullptr,false,&m_pChannel2D);
			}
		}

		if (pair.triggerActor == m_pTriggerRight && pair.otherActor == m_pCubeRight->GetRigidActor())
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //entered
			{
				m_IsTriggeredRight = true;
				SoundManager::GetInstance()->GetSystem()->playSound(m_pBellSound, nullptr, false, &m_pChannel2D);
			}
		}
	}
}
