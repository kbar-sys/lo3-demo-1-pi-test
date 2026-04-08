#ifndef GOLEMBASE_HPP_INCLUDED
#define GOLEMBASE_HPP_INCLUDED

#include "irrlicht.h"
#include "lo3items.hpp"

struct GolemBase
{
    irr::scene::ICameraSceneNode * camera;
    irr::scene::IBillboardSceneNode * billboard;

	irr::video::ITexture * golem_sprites[LO3_SPR_GOLEM];


    void initMonst();
    void setBillboard(irr::scene::ISceneManager * sm, irr::core::vector3df position);
    void setTextures(irr::video::ITexture ** copy_of_tex, int size);
    void beforeRun(irr::IrrlichtDevice * device);
    void beforeDraw(irr::IrrlichtDevice * device);

    void outsideMonst(irr::scene::ICameraSceneNode * camera);
    void directionMonst();
    void renderMonst();


    irr::core::vector3df forward;

	irr::core::vector3df v3dfCameraPosition;
	irr::core::vector3df v3dfBillboardposition;
	irr::core::vector3df relativeDirection;
	irr::core::vector3df billboardDirection;


	irr::f32 angleY_rad;
    irr::f32 angleY_degrees;
	irr::u32 currentFrame;
	irr::u32 msFrameRate;

	irr::u32 lastTime;
    irr::u32 currentTime;
    irr::u32 elapsedTime;

    GolemBase()
    {
        forward = irr::core::vector3df(0.f, 0.f, -1.f);
        msFrameRate = 124;
        currentFrame = 0;
    }
};

#endif // GOLEMBASE_HPP_INCLUDED
