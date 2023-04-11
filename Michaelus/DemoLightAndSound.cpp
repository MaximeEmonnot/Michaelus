#include "DemoLightAndSound.h"

#include "LightManager.h"
#include "SceneComponent.h"
#include "SoundSystem.h"
#include "Timer.h"

// Constructeur définissant le nom de l'acteur
DemoLightAndSound::DemoLightAndSound(const std::string& name)
{
	// COMPONENTS CREATION
	pRoot = CreateComponent<SceneComponent>();
	pRootComponent = pRoot;

	pLightRoot = CreateComponent<SceneComponent>();
	pLightRoot->AttachTo(pRoot);

	pLightPoint1 = CreateComponent<SceneComponent>();
	pLightPoint1->AttachTo(pLightRoot);
	pLightPoint2 = CreateComponent<SceneComponent>();
	pLightPoint2->AttachTo(pLightRoot);
	pLightPoint3 = CreateComponent<SceneComponent>();
	pLightPoint3->AttachTo(pLightRoot);
	pLightPoint4 = CreateComponent<SceneComponent>();
	pLightPoint4->AttachTo(pLightRoot);
	pLightPoint5 = CreateComponent<SceneComponent>();
	pLightPoint5->AttachTo(pLightRoot);
	pLightPoint6 = CreateComponent<SceneComponent>();
	pLightPoint6->AttachTo(pLightRoot);
	pLightPoint7 = CreateComponent<SceneComponent>();
	pLightPoint7->AttachTo(pLightRoot);
	pLightPoint8 = CreateComponent<SceneComponent>();
	pLightPoint8->AttachTo(pLightRoot);

	pSoundRoot = CreateComponent<SceneComponent>();
	pSoundRoot->AttachTo(pRoot);
	pSoundSource = CreateComponent<SceneComponent>();
	pSoundSource->AttachTo(pSoundRoot);

	// COMPONENTS INITIALIZATION
	pLightPoint1->AddRelativeLocation({ 2.f, 0.f, 0.f });
	pLightPoint2->AddRelativeLocation({ 2.f, 2.f, 0.f });
	pLightPoint3->AddRelativeLocation({ 0.f, 2.f, 0.f });
	pLightPoint4->AddRelativeLocation({ -2.f, 2.f, 0.f });
	pLightPoint5->AddRelativeLocation({ -2.f, 0.f, 0.f });
	pLightPoint6->AddRelativeLocation({ -2.f, -2.f, 0.f });
	pLightPoint7->AddRelativeLocation({ 0.f, -2.f, 0.f });
	pLightPoint8->AddRelativeLocation({ 2.f, -2.f, 0.f });
	pointLightLocations.emplace_back(pLightPoint1);
	pointLightLocations.emplace_back(pLightPoint2);
	pointLightLocations.emplace_back(pLightPoint3);
	pointLightLocations.emplace_back(pLightPoint4);
	pointLightLocations.emplace_back(pLightPoint5);
	pointLightLocations.emplace_back(pLightPoint6);
	pointLightLocations.emplace_back(pLightPoint7);
	pointLightLocations.emplace_back(pLightPoint8);

	pSoundSource->AddRelativeLocation({ 25.f, 0.f, 0.f });

	// Lights Creation
	pointLights.push_back(LIGHT_SYSTEM.CreatePointLight(pLightPoint1->GetWorldLocation(), { 1.f, 0.f, 0.f }, 1.f));
	pointLights.push_back(LIGHT_SYSTEM.CreatePointLight(pLightPoint2->GetWorldLocation(), { 0.f, 1.f, 0.f }, 1.f));
	pointLights.push_back(LIGHT_SYSTEM.CreatePointLight(pLightPoint3->GetWorldLocation(), { 0.f, 0.f, 1.f }, 1.f));
	pointLights.push_back(LIGHT_SYSTEM.CreatePointLight(pLightPoint4->GetWorldLocation(), { 1.f, 1.f, 0.f }, 1.f));
	pointLights.push_back(LIGHT_SYSTEM.CreatePointLight(pLightPoint5->GetWorldLocation(), { 1.f, 0.f, 1.f }, 1.f));
	pointLights.push_back(LIGHT_SYSTEM.CreatePointLight(pLightPoint6->GetWorldLocation(), { 0.f, 1.f, 1.f }, 1.f));
	pointLights.push_back(LIGHT_SYSTEM.CreatePointLight(pLightPoint7->GetWorldLocation(), { 1.f, 1.f, 1.f }, 1.f));
	pointLights.push_back(LIGHT_SYSTEM.CreatePointLight(pLightPoint8->GetWorldLocation(), { 0.25f, 0.5f, 0.25f }, 1.f));

	// Sound Creation
	soundSourceIndex = SFX.PlaySound<Effect>("Sounds/free_bird.wav", true, 1.f, 1.f, pSoundSource->GetWorldLocation());
}

// Méthode Update() de l'acteur, surchargée
void DemoLightAndSound::Update()
{
	Actor::Update();

	// Rotations
	pLightRoot->AddRelativeRotation(FQuaternion(0.f, 0.f, MMath::Rad(30.f) * DELTATIME));
	pSoundRoot->AddRelativeRotation(FQuaternion(0.f, 0.f, -MMath::Rad(30.f) * DELTATIME));

	// Lights and Sound Update
	for (size_t i = 0; i < pointLights.size(); i++)
		LIGHT_SYSTEM.SetPointLight(static_cast<int>(pointLights.at(i)), pointLightLocations.at(i)->GetWorldLocation());

	SFX.UpdatePlayingSound(soundSourceIndex, true, 1.f, 1.f, pSoundSource->GetWorldLocation());
}
