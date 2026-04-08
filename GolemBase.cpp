#include "include/lo3items.hpp"

#include <iostream>
#include <algorithm>
#include "irrlicht.h"

#include "include/GolemBase.hpp"

using namespace irr;

//struct GolemBase
//{
//    scene::ICameraSceneNode * camera;
//    scene::IBillboardSceneNode * billboard;
//    scene::ISceneManager * smgr;
//
//	video::ITexture * golem_sprites[111];
//
//
//    void initMonst();
//    void setBillboard(scene::ISceneManager * sm, core::vector3df position);
//    void setTextures(irr::video::ITexture ** copy_of_tex, int size);
//    void beforeRun(irr::IrrlichtDevice * device);
//    void beforeDraw(irr::IrrlichtDevice * device);
//    void renderMonst();
//    void directionMonst();
//    void outsideMonst(scene::ICameraSceneNode * camera);
//
//    core::vector3df forward{0.f, 0.f, -1.f};
//
//	core::vector3df v3dfCameraPosition;
//	core::vector3df v3dfBillboardposition;
//	core::vector3df relativeDirection;
//	core::vector3df billboardDirection;
//
//
//	irr::f32 angleY_rad;
//    irr::f32 angleY_degrees = irr::f32(angleY_rad * (180.0f / core::PI));
//	irr::u32 currentFrame = 0;
//	irr::u32 msFrameRate = 124; //min 17ms approx.
//
//	irr::u32 lastTime;
//    irr::u32 currentTime;
//    irr::u32 elapsedTime;
//};

void GolemBase::setBillboard(scene::ISceneManager * sm, core::vector3df position)
{
    billboard = sm->addBillboardSceneNode();

    billboard->setSize(core::dimension2d<f32>(25.0f, 25.0f));
	billboard->setPosition(position + core::vector3df(0,25,0));
	billboard->setMaterialFlag(video::EMF_LIGHTING, false);
	billboard->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
}

void GolemBase::setTextures(irr::video::ITexture ** copy_of_tex, int size)
{

    std::copy(copy_of_tex, copy_of_tex + size, golem_sprites);
    //std::begin not working

    for(video::ITexture * t : golem_sprites)
    {
        if(!t)
            std::cout << "Pointer ERR\n";
    }

}

void GolemBase::beforeRun(irr::IrrlichtDevice * device)
{
	lastTime = device->getTimer()->getTime();
}

void GolemBase::beforeDraw(irr::IrrlichtDevice * device)
{
    currentTime = device->getTimer()->getTime();
    elapsedTime = currentTime - lastTime;
}

void GolemBase::outsideMonst(scene::ICameraSceneNode * camera)
{
	v3dfCameraPosition = camera->getPosition();
	v3dfBillboardposition = billboard->getPosition();
	relativeDirection = v3dfBillboardposition - v3dfCameraPosition;
	billboardDirection = v3dfCameraPosition - v3dfBillboardposition;
	billboardDirection.normalize();

	angleY_rad = f32(acos(billboardDirection.dotProduct(forward)));
	angleY_degrees = irr::f32(angleY_rad * (180.0f / core::PI));
}

void GolemBase::directionMonst()
{
	if (relativeDirection.X > 0.0f && relativeDirection.Z > 0.0f) {
		angleY_degrees = -angleY_degrees;
	}
	else if (relativeDirection.X > 0.0f && relativeDirection.Z < 0.0f) {
		angleY_degrees = -angleY_degrees;
	}

	angleY_degrees = irr::core::clamp(angleY_degrees, -180.0f, 180.0f);
}

void GolemBase::renderMonst()
{

	irr::u32 billboardStartFrameLoop = 0;
	irr::u32 billboardEndFrameLoop = 6;


    if (elapsedTime >= msFrameRate)
    {
        if (angleY_degrees > 13.375f && angleY_degrees < 35.875)
        {
            billboardStartFrameLoop = 7;
            billboardEndFrameLoop = 13;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //26.75(7-13)
        }
        else if (angleY_degrees < -13.375f && angleY_degrees > -35.875)
        {
            billboardStartFrameLoop = 14;
            billboardEndFrameLoop = 20;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //-26.75(14-20)
        }
        else if (angleY_degrees >= 35.875f && angleY_degrees < 54.25)
        {
            billboardStartFrameLoop = 21;
            billboardEndFrameLoop = 27;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //45.0(21-27)
        }
        else if (angleY_degrees <= -35.875f && angleY_degrees > -54.25)
        {
            billboardStartFrameLoop = 28;
            billboardEndFrameLoop = 34;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //-45.0(28-34)
        }
        else if (angleY_degrees >= 54.25f && angleY_degrees < 78.75)
        {
            billboardStartFrameLoop = 35;
            billboardEndFrameLoop = 41;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //63.5(35-41)
        }
        else if (angleY_degrees <= -54.25f && angleY_degrees > -78.75)
        {
            billboardStartFrameLoop = 42;
            billboardEndFrameLoop = 48;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //-63.5(42-48)
        }
        else if (angleY_degrees >= 78.75f && angleY_degrees < 103.25)
        {
            billboardStartFrameLoop = 49;
            billboardEndFrameLoop = 55;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //90.5(49-55)
        }
        else if (angleY_degrees <= -78.75f && angleY_degrees > -103.25)
        {
            billboardStartFrameLoop = 56;
            billboardEndFrameLoop = 62;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //-90.5(56-62)
        }
        else if (angleY_degrees >= 103.25f && angleY_degrees < 125.75)
        {
            billboardStartFrameLoop = 63;
            billboardEndFrameLoop = 69;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //116.5(63-69)
        }
        else if (angleY_degrees <= -103.25f && angleY_degrees > -125.75)
        {
            billboardStartFrameLoop = 70;
            billboardEndFrameLoop = 76;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //-116.5(70-76)
        }
        else if (angleY_degrees >= 125.75f && angleY_degrees < 144.125)
        {
            billboardStartFrameLoop = 77;
            billboardEndFrameLoop = 83;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //135.0(77-83)
        }
        else if (angleY_degrees <= -125.75f && angleY_degrees > -144.125)
        {
            billboardStartFrameLoop = 84;
            billboardEndFrameLoop = 90;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //-135.0(84-90)
        }
        else if (angleY_degrees >= 144.125f && angleY_degrees < 166.625)
        {
            billboardStartFrameLoop = 91;
            billboardEndFrameLoop = 97;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //153.5(91-96)
        }
        else if (angleY_degrees <= -144.125f && angleY_degrees > -166.625)
        {
            billboardStartFrameLoop = 98;
            billboardEndFrameLoop = 104;
            if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                currentFrame = billboardStartFrameLoop;
            }
            //-153.5(97-104)
        }
        else if (angleY_degrees <= -166.625f && angleY_degrees >= -180.0f || angleY_degrees >= 166.625f && angleY_degrees <= 180.0f)
        {
            if (relativeDirection.Z < 0.0f)
            {
                billboardStartFrameLoop = 105;
                billboardEndFrameLoop = 111;
                if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                    currentFrame = billboardStartFrameLoop;
                }
                //180.0(105-111)
            }
            if (relativeDirection.Z > 0.0f)
            {
                billboardStartFrameLoop = 0;
                billboardEndFrameLoop = 6;
                if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                    currentFrame = billboardStartFrameLoop;
                }
                //0.0(0-6)
            }
        }
        else if (angleY_degrees >= -13.375f && angleY_degrees <= 13.375f)
        {
            if (relativeDirection.Z < 0.0f)
            {
                billboardStartFrameLoop = 105;
                billboardEndFrameLoop = 111;
                if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                    currentFrame = billboardStartFrameLoop;
                }
                //180.0(105-111)
            }
            if (relativeDirection.Z > 0.0f)
            {
                billboardStartFrameLoop = 0;
                billboardEndFrameLoop = 6;
                if (currentFrame < billboardStartFrameLoop || currentFrame > billboardEndFrameLoop) {
                    currentFrame = billboardStartFrameLoop;
                }
                //0.0(0-6)
            }
        }

        billboard->setMaterialTexture(0, golem_sprites[currentFrame]);
        currentFrame = (currentFrame + 1) % (billboardEndFrameLoop + 1);
        lastTime = currentTime;
    }
}
