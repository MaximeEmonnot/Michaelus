#pragma once
#include "World.h"

class DemoLightAndSound;
class DemoShrek;
class DemoPlane;
class DemoPawn;
class DemoRat;

class DemoWorld :
    public World
{
public:
    DemoWorld();

    virtual void BeginPlay() override;
    virtual void Update() override;

private:
	std::shared_ptr<DemoRat> pTestActor1;
	std::shared_ptr<DemoRat> pTestActor2;
	std::shared_ptr<DemoRat> pTestActor3;
	std::shared_ptr<DemoRat> pTestActor4;
	std::shared_ptr<DemoRat> pTestActor5;

	std::shared_ptr<DemoPawn> pTestPawn;

	std::shared_ptr<DemoPlane> pDemoPlane;

	std::shared_ptr<DemoShrek> pDemoShrek;

	std::shared_ptr<DemoLightAndSound> pDemoLightSound;
};

