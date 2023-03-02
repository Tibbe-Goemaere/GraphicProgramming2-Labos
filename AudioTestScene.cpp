#include "stdafx.h"
#include "AudioTestScene.h"

#include "MeshObject.h"
#include "ContentManager.h"
#include "SoundManager.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"

void AudioTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = pPhysX->createMaterial(0.5, 0.5f, 0.2f);

	//Ground plane
	auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo, {0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Blue});
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);

	//Link sphere with sphere actor
	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0.f, 2.f, 0.f);

	//Input
	const auto pInput = m_SceneContext.GetInput();
	pInput->AddInputAction({ Play2D,InputTriggerState::pressed,VK_SPACE });
	pInput->AddInputAction({ Play3D,InputTriggerState::pressed,'B' });
	pInput->AddInputAction({ IncreaseVolume,InputTriggerState::pressed,VK_UP });
	pInput->AddInputAction({ DecreaseVolume,InputTriggerState::pressed,VK_DOWN });

	const auto pFmod = SoundManager::GetInstance()->GetSystem();
	//Sound 2D
	FMOD::Sound* pSound2D{ nullptr };
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/wave.mp3",FMOD_2D | FMOD_LOOP_NORMAL,nullptr,&pSound2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound2D,nullptr,true,&m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	//Sound 3D
	FMOD::Sound* pSound3D{ nullptr };
	result = pFmod->createStream("Resources/Sounds/thrones.mp3", FMOD_3D | FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound3D, nullptr, true, &m_pChannel3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	m_pChannel3D->set3DMinMaxDistance(0.f,20.f);
}

inline FMOD_VECTOR ToFMod(XMFLOAT3 v) //DirectX
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

inline FMOD_VECTOR ToFMod(PxVec3 v) //PhysX
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

void AudioTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{

	}

	const float force{ 10.f };
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,-force });
	}
	
	if (m_SceneContext.GetInput()->IsActionTriggered(Play2D))
	{
		bool bPaused{ false };
		m_pChannel2D->getPaused(&bPaused);
		m_pChannel2D->setPaused(bPaused);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(Play3D))
	{
		bool bPaused{ false };
		m_pChannel3D->getPaused(&bPaused);
		m_pChannel3D->setPaused(bPaused);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(IncreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);
		volume += 0.1f;
		Clamp(volume,10.f,0.f);
		m_pChannel2D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", volume);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(DecreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);
		volume -= 0.1f;
		Clamp(volume, 10.f, 0.f);
		m_pChannel2D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", volume);
	}

	//3D Sound Attributes
	auto pCam = m_SceneContext.GetCamera();
	auto pos = ToFMod(pCam->GetPosition());
	auto forward = ToFMod(pCam->GetForward());
	auto up = ToFMod(pCam->GetUp());

	FMOD_VECTOR vel{};
	const float deltaT = m_SceneContext.GetGameTime()->GetElapsed();
	vel.x = (pos.x - m_prevCamPos.x) / deltaT;
	vel.y = (pos.y - m_prevCamPos.y) / deltaT;
	vel.z = (pos.z - m_prevCamPos.z) / deltaT;
	m_prevCamPos = pos;

	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos,&vel,&forward,&up );

	auto spherePos = ToFMod(m_pSphere->GetPosition());
	auto sphereVel = ToFMod(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity());
	m_pChannel3D->set3DAttributes( &spherePos, &sphereVel);
}

void AudioTestScene::Draw() const
{

}

void AudioTestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void AudioTestScene::OnSceneDeactivated()
{
}
