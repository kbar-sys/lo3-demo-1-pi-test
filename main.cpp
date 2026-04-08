//#include <irrlicht.h>


#include "irrlicht.h"
#include <iostream>

#include "include/lo3items.hpp"
#include "include/GolemBase.hpp"

using namespace irr;



struct A
{
    irr::IrrlichtDevice * device;
    irr::video::IVideoDriver * driver;
    irr::gui::IGUIEnvironment * guienv;
    irr::scene::ISceneManager * smgr;

    irr::scene::ICameraSceneNode * camera;
    irr::scene::ICameraSceneNode * halt_camera;
    irr::core::vector3df camera_fixed;
    irr::core::vector3df camera_distance_from_fixed;
    bool player_halt;
    bool player_action;
};

struct Context
{
    bool quit_game;
    bool viewing_cam;
};

struct M
{
    irr::video::ITexture* golem_spr[LO3_SPR_GOLEM];

} monsters_tex;

struct SMap
{
    irr::scene::ISceneNode * current_map;
    irr::core::array<GolemBase> golems_in_map;
};




class MyEventReceiver : public IEventReceiver
{
public:
    // give input a gui quit button
    MyEventReceiver(Context & c) : context(c) { }


	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == EET_KEY_INPUT_EVENT)
        {
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

            // special up key scenario
            if(KeyIsDown[event.KeyInput.Key] == 0 && KeyIsUp[event.KeyInput.Key] == 0)
                ToggleView();

            KeyIsUp[event.KeyInput.Key] = !event.KeyInput.PressedDown;

            // EVERY key released?
            //KeyIsReleased[event.KeyInput.Key] = 0;
        }

        if(event.EventType == EET_MOUSE_INPUT_EVENT)
        {
            RightMouse = event.MouseInput.isRightPressed();

            mouse_xy.X = event.MouseInput.X;
            mouse_xy.Y = event.MouseInput.Y;
        }


        if(event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
            RightMouse = false;





        // GUI
        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();


            switch(event.GUIEvent.EventType)
            {
            case gui::EGET_BUTTON_CLICKED:
                switch(id)
                {
                case GUI_ID_QUIT:
                    context.quit_game = true;
                    return true;
                }
//                case EGET_TABLE_SELECTED_AGAIN:
//                case EGET_TREEVIEW_NODE_DESELECT:
//                case EGET_TREEVIEW_NODE_SELECT:
//                case EGET_TREEVIEW_NODE_EXPAND:
//                case EGET_TREEVIEW_NODE_COLLAPSE:
//                case EGET_TREEVIEW_NODE_COLLAPS:
                default:
                {
                    return true;
                }
            }
        }


		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	virtual bool IsKeyUp(EKEY_CODE keyCode) const
	{
	    return KeyIsUp[keyCode];
	}


	virtual bool IsRightClick() const
	{
	    return RightMouse;
	}

	virtual bool IsQuit()
	{
	    return context.quit_game;
	}

	virtual core::position2d<s32> getXY()
	{
	    return mouse_xy;
	}

	virtual void ToggleView()
	{
	    context.viewing_cam = !context.viewing_cam;
	    std::cout << "?: " << context.viewing_cam;
	}

	virtual bool getView()
	{
	    return context.viewing_cam;
	}

	// stops key being jammed after halt camera active
	virtual void receiverResetKeys()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

	// init

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;

        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsUp[i] = true;

        context.quit_game = false;
        context.viewing_cam = false;
        RightMouse = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool KeyIsUp[KEY_KEY_CODES_COUNT];

	bool RightMouse;

	int event_tot;;
	core::position2d<s32> mouse_xy;
	Context context;
};



void loadGolemTex(video::IVideoDriver * driver)
{
	monsters_tex.golem_spr[0] = driver->getTexture("../../media_folders/Golem1Sprites/0.0/0007.png");
	monsters_tex.golem_spr[1] = driver->getTexture("../../media_folders/Golem1Sprites/0.0/0014.png");
	monsters_tex.golem_spr[2] = driver->getTexture("../../media_folders/Golem1Sprites/0.0/0021.png");
	monsters_tex.golem_spr[3] = driver->getTexture("../../media_folders/Golem1Sprites/0.0/0028.png");
	monsters_tex.golem_spr[4] = driver->getTexture("../../media_folders/Golem1Sprites/0.0/0035.png");
	monsters_tex.golem_spr[5] = driver->getTexture("../../media_folders/Golem1Sprites/0.0/0042.png");
	monsters_tex.golem_spr[6] = driver->getTexture("../../media_folders/Golem1Sprites/0.0/0049.png");

	monsters_tex.golem_spr[7] = driver->getTexture("../../media_folders/Golem1Sprites/26.75/0007.png");
	monsters_tex.golem_spr[8] = driver->getTexture("../../media_folders/Golem1Sprites/26.75/0014.png");
	monsters_tex.golem_spr[9] = driver->getTexture("../../media_folders/Golem1Sprites/26.75/0021.png");
	monsters_tex.golem_spr[10] = driver->getTexture("../../media_folders/Golem1Sprites/26.75/0028.png");
	monsters_tex.golem_spr[11] = driver->getTexture("../../media_folders/Golem1Sprites/26.75/0035.png");
	monsters_tex.golem_spr[12] = driver->getTexture("../../media_folders/Golem1Sprites/26.75/0042.png");
	monsters_tex.golem_spr[13] = driver->getTexture("../../media_folders/Golem1Sprites/26.75/0049.png");

	monsters_tex.golem_spr[14] = driver->getTexture("../../media_folders/Golem1Sprites/-26.75/0007.png");
	monsters_tex.golem_spr[15] = driver->getTexture("../../media_folders/Golem1Sprites/-26.75/0014.png");
	monsters_tex.golem_spr[16] = driver->getTexture("../../media_folders/Golem1Sprites/-26.75/0021.png");
	monsters_tex.golem_spr[17] = driver->getTexture("../../media_folders/Golem1Sprites/-26.75/0028.png");
	monsters_tex.golem_spr[18] = driver->getTexture("../../media_folders/Golem1Sprites/-26.75/0035.png");
	monsters_tex.golem_spr[19] = driver->getTexture("../../media_folders/Golem1Sprites/-26.75/0042.png");
	monsters_tex.golem_spr[20] = driver->getTexture("../../media_folders/Golem1Sprites/-26.75/0049.png");

	monsters_tex.golem_spr[21] = driver->getTexture("../../media_folders/Golem1Sprites/45.0/0007.png");
	monsters_tex.golem_spr[22] = driver->getTexture("../../media_folders/Golem1Sprites/45.0/0014.png");
	monsters_tex.golem_spr[23] = driver->getTexture("../../media_folders/Golem1Sprites/45.0/0021.png");
	monsters_tex.golem_spr[24] = driver->getTexture("../../media_folders/Golem1Sprites/45.0/0028.png");
	monsters_tex.golem_spr[25] = driver->getTexture("../../media_folders/Golem1Sprites/45.0/0035.png");
	monsters_tex.golem_spr[26] = driver->getTexture("../../media_folders/Golem1Sprites/45.0/0042.png");
	monsters_tex.golem_spr[27] = driver->getTexture("../../media_folders/Golem1Sprites/45.0/0049.png");

	monsters_tex.golem_spr[28] = driver->getTexture("../../media_folders/Golem1Sprites/-45.0/0007.png");
	monsters_tex.golem_spr[29] = driver->getTexture("../../media_folders/Golem1Sprites/-45.0/0014.png");
	monsters_tex.golem_spr[30] = driver->getTexture("../../media_folders/Golem1Sprites/-45.0/0021.png");
	monsters_tex.golem_spr[31] = driver->getTexture("../../media_folders/Golem1Sprites/-45.0/0028.png");
	monsters_tex.golem_spr[32] = driver->getTexture("../../media_folders/Golem1Sprites/-45.0/0035.png");
	monsters_tex.golem_spr[33] = driver->getTexture("../../media_folders/Golem1Sprites/-45.0/0042.png");
	monsters_tex.golem_spr[34] = driver->getTexture("../../media_folders/Golem1Sprites/-45.0/0049.png");

	monsters_tex.golem_spr[35] = driver->getTexture("../../media_folders/Golem1Sprites/63.5/0007.png");
	monsters_tex.golem_spr[36] = driver->getTexture("../../media_folders/Golem1Sprites/63.5/0014.png");
	monsters_tex.golem_spr[37] = driver->getTexture("../../media_folders/Golem1Sprites/63.5/0021.png");
	monsters_tex.golem_spr[38] = driver->getTexture("../../media_folders/Golem1Sprites/63.5/0028.png");
	monsters_tex.golem_spr[39] = driver->getTexture("../../media_folders/Golem1Sprites/63.5/0035.png");
	monsters_tex.golem_spr[40] = driver->getTexture("../../media_folders/Golem1Sprites/63.5/0042.png");
	monsters_tex.golem_spr[41] = driver->getTexture("../../media_folders/Golem1Sprites/63.5/0049.png");

	monsters_tex.golem_spr[42] = driver->getTexture("../../media_folders/Golem1Sprites/-63.5/0007.png");
	monsters_tex.golem_spr[43] = driver->getTexture("../../media_folders/Golem1Sprites/-63.5/0014.png");
	monsters_tex.golem_spr[44] = driver->getTexture("../../media_folders/Golem1Sprites/-63.5/0021.png");
	monsters_tex.golem_spr[45] = driver->getTexture("../../media_folders/Golem1Sprites/-63.5/0028.png");
	monsters_tex.golem_spr[46] = driver->getTexture("../../media_folders/Golem1Sprites/-63.5/0035.png");
	monsters_tex.golem_spr[47] = driver->getTexture("../../media_folders/Golem1Sprites/-63.5/0042.png");
	monsters_tex.golem_spr[48] = driver->getTexture("../../media_folders/Golem1Sprites/-63.5/0049.png");

	monsters_tex.golem_spr[49] = driver->getTexture("../../media_folders/Golem1Sprites/90.0/0007.png");
	monsters_tex.golem_spr[50] = driver->getTexture("../../media_folders/Golem1Sprites/90.0/0014.png");
	monsters_tex.golem_spr[51] = driver->getTexture("../../media_folders/Golem1Sprites/90.0/0021.png");
	monsters_tex.golem_spr[52] = driver->getTexture("../../media_folders/Golem1Sprites/90.0/0028.png");
	monsters_tex.golem_spr[53] = driver->getTexture("../../media_folders/Golem1Sprites/90.0/0035.png");
	monsters_tex.golem_spr[54] = driver->getTexture("../../media_folders/Golem1Sprites/90.0/0042.png");
	monsters_tex.golem_spr[55] = driver->getTexture("../../media_folders/Golem1Sprites/90.0/0049.png");

	monsters_tex.golem_spr[56] = driver->getTexture("../../media_folders/Golem1Sprites/-90.0/0007.png");
	monsters_tex.golem_spr[57] = driver->getTexture("../../media_folders/Golem1Sprites/-90.0/0014.png");
	monsters_tex.golem_spr[58] = driver->getTexture("../../media_folders/Golem1Sprites/-90.0/0021.png");
	monsters_tex.golem_spr[59] = driver->getTexture("../../media_folders/Golem1Sprites/-90.0/0028.png");
	monsters_tex.golem_spr[60] = driver->getTexture("../../media_folders/Golem1Sprites/-90.0/0035.png");
	monsters_tex.golem_spr[61] = driver->getTexture("../../media_folders/Golem1Sprites/-90.0/0042.png");
	monsters_tex.golem_spr[62] = driver->getTexture("../../media_folders/Golem1Sprites/-90.0/0049.png");

	monsters_tex.golem_spr[63] = driver->getTexture("../../media_folders/Golem1Sprites/116.5/0007.png");
	monsters_tex.golem_spr[64] = driver->getTexture("../../media_folders/Golem1Sprites/116.5/0014.png");
	monsters_tex.golem_spr[65] = driver->getTexture("../../media_folders/Golem1Sprites/116.5/0021.png");
	monsters_tex.golem_spr[66] = driver->getTexture("../../media_folders/Golem1Sprites/116.5/0028.png");
	monsters_tex.golem_spr[67] = driver->getTexture("../../media_folders/Golem1Sprites/116.5/0035.png");
	monsters_tex.golem_spr[68] = driver->getTexture("../../media_folders/Golem1Sprites/116.5/0042.png");
	monsters_tex.golem_spr[69] = driver->getTexture("../../media_folders/Golem1Sprites/116.5/0049.png");

	monsters_tex.golem_spr[70] = driver->getTexture("../../media_folders/Golem1Sprites/-116.5/0007.png");
	monsters_tex.golem_spr[71] = driver->getTexture("../../media_folders/Golem1Sprites/-116.5/0014.png");
	monsters_tex.golem_spr[72] = driver->getTexture("../../media_folders/Golem1Sprites/-116.5/0021.png");
	monsters_tex.golem_spr[73] = driver->getTexture("../../media_folders/Golem1Sprites/-116.5/0028.png");
	monsters_tex.golem_spr[74] = driver->getTexture("../../media_folders/Golem1Sprites/-116.5/0035.png");
	monsters_tex.golem_spr[75] = driver->getTexture("../../media_folders/Golem1Sprites/-116.5/0042.png");
	monsters_tex.golem_spr[76] = driver->getTexture("../../media_folders/Golem1Sprites/-116.5/0049.png");

	monsters_tex.golem_spr[77] = driver->getTexture("../../media_folders/Golem1Sprites/135.0/0007.png");
	monsters_tex.golem_spr[78] = driver->getTexture("../../media_folders/Golem1Sprites/135.0/0014.png");
	monsters_tex.golem_spr[79] = driver->getTexture("../../media_folders/Golem1Sprites/135.0/0021.png");
	monsters_tex.golem_spr[80] = driver->getTexture("../../media_folders/Golem1Sprites/135.0/0028.png");
	monsters_tex.golem_spr[81] = driver->getTexture("../../media_folders/Golem1Sprites/135.0/0035.png");
	monsters_tex.golem_spr[82] = driver->getTexture("../../media_folders/Golem1Sprites/135.0/0042.png");
	monsters_tex.golem_spr[83] = driver->getTexture("../../media_folders/Golem1Sprites/135.0/0049.png");

	monsters_tex.golem_spr[84] = driver->getTexture("../../media_folders/Golem1Sprites/-135.0/0007.png");
	monsters_tex.golem_spr[85] = driver->getTexture("../../media_folders/Golem1Sprites/-135.0/0014.png");
	monsters_tex.golem_spr[86] = driver->getTexture("../../media_folders/Golem1Sprites/-135.0/0021.png");
	monsters_tex.golem_spr[87] = driver->getTexture("../../media_folders/Golem1Sprites/-135.0/0028.png");
	monsters_tex.golem_spr[88] = driver->getTexture("../../media_folders/Golem1Sprites/-135.0/0035.png");
	monsters_tex.golem_spr[89] = driver->getTexture("../../media_folders/Golem1Sprites/-135.0/0042.png");
	monsters_tex.golem_spr[90] = driver->getTexture("../../media_folders/Golem1Sprites/-135.0/0049.png");

	monsters_tex.golem_spr[91] = driver->getTexture("../../media_folders/Golem1Sprites/153.5/0007.png");
	monsters_tex.golem_spr[92] = driver->getTexture("../../media_folders/Golem1Sprites/153.5/0014.png");
	monsters_tex.golem_spr[93] = driver->getTexture("../../media_folders/Golem1Sprites/153.5/0021.png");
	monsters_tex.golem_spr[94] = driver->getTexture("../../media_folders/Golem1Sprites/153.5/0028.png");
	monsters_tex.golem_spr[95] = driver->getTexture("../../media_folders/Golem1Sprites/153.5/0035.png");
	monsters_tex.golem_spr[96] = driver->getTexture("../../media_folders/Golem1Sprites/153.5/0042.png");
	monsters_tex.golem_spr[97] = driver->getTexture("../../media_folders/Golem1Sprites/153.5/0049.png");

	monsters_tex.golem_spr[98] = driver->getTexture("../../media_folders/Golem1Sprites/-153.5/0007.png");
	monsters_tex.golem_spr[99] = driver->getTexture("../../media_folders/Golem1Sprites/-153.5/0014.png");
	monsters_tex.golem_spr[100] = driver->getTexture("../../media_folders/Golem1Sprites/-153.5/0021.png");
	monsters_tex.golem_spr[101] = driver->getTexture("../../media_folders/Golem1Sprites/-153.5/0028.png");
	monsters_tex.golem_spr[102] = driver->getTexture("../../media_folders/Golem1Sprites/-153.5/0035.png");
	monsters_tex.golem_spr[103] = driver->getTexture("../../media_folders/Golem1Sprites/-153.5/0042.png");
	monsters_tex.golem_spr[104] = driver->getTexture("../../media_folders/Golem1Sprites/-153.5/0049.png");

	monsters_tex.golem_spr[105] = driver->getTexture("../../media_folders/Golem1Sprites/-180.0/0007.png");
	monsters_tex.golem_spr[106] = driver->getTexture("../../media_folders/Golem1Sprites/-180.0/0014.png");
	monsters_tex.golem_spr[107] = driver->getTexture("../../media_folders/Golem1Sprites/-180.0/0021.png");
	monsters_tex.golem_spr[108] = driver->getTexture("../../media_folders/Golem1Sprites/-180.0/0028.png");
	monsters_tex.golem_spr[109] = driver->getTexture("../../media_folders/Golem1Sprites/-180.0/0035.png");
	monsters_tex.golem_spr[110] = driver->getTexture("../../media_folders/Golem1Sprites/-180.0/0042.png");
	monsters_tex.golem_spr[111] = driver->getTexture("../../media_folders/Golem1Sprites/-180.0/0049.png");
}



//
// main
//


int main(int argc, char** argv)
{

    video::E_DRIVER_TYPE driverType;

    printf("Please select the driver you want for this example:\n"\
        " (a) OpenGL\n"\
        " (b) DirectX 9\n"\
        " (c) Burning's Software Renderer\n"\
        " (d) Software Renderer\n" \
        " (otherKey) exit\n\n");

    char i;
    std::cin >> i;

    switch(i)
    {
        case 'a': driverType = video::EDT_OPENGL;   break;
        case 'b': driverType = video::EDT_DIRECT3D9; break;
        case 'c': driverType = video::EDT_BURNINGSVIDEO;break;
        case 'd': driverType = video::EDT_SOFTWARE; break;
        default: return 1;
    }


    A a;
    SMap m;



    MyEventReceiver receiver;

    a.device =
        createDevice(driverType, core::dimension2d<u32>(800, 600), 16, //BURNINGSVIDEO
            false, false, false, &receiver);

    if(!a.device)
    {
        std::cout << "No compatable device.";
        return 1;
    }
    else
    {
        std::cout << "Display created..." << std::endl;
    }



    a.device->setWindowCaption(L"Hello Lord of 3D!");

    a.driver = a.device->getVideoDriver();
    a.smgr = a.device->getSceneManager();

    if(!a.driver)
    {
        std::cout << "No driver available." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Driver created..." << std::endl;
    }

    a.guienv = a.device->getGUIEnvironment();

    if(!a.guienv)
    {
        std::cout << "WARNING: No GUI." << std::endl;
    }
    else
    {
        std::cout << "GUI created.." << std::endl;
    }


    a.guienv->addButton(core::rect<s32>(10,240,110,240 + 32), 0, GUI_ID_QUIT, L"Quit", L"Exits Program");
    a.guienv->addStaticText(L"HELLO NOIE! RIGHT CLICK! USE WASD!",
        core::rect<int>(10,10,320,22), true);

    gui::IGUIStaticText * fps_mention = a.guienv->addStaticText(L"?", core::rect<s32>(10,10+32,110,10+32+32), true);
    fps_mention->setText(L"FPS");


    gui::IGUISkin * skin = a.guienv->getSkin();
    gui::IGUIFont * font_cour = a.guienv->getFont("../../media/fontcourier.bmp");
    if (font_cour)
        skin->setFont(font_cour);



    //
    // vars
    //


    irr::s32 IDNotPicked = 1 << 0;
    irr::s32 IDIsHUD = 1 << 4;






    //
    // materials
    //

    /*
    video::SMaterial red_wire_mat;
    red_wire_mat.Wireframe = true;
    red_wire_mat.Thickness = 1.f;
    red_wire_mat.BackfaceCulling = false;

    scene::IMeshSceneNode * sphere = smgr->addSphereSceneNode();
    sphere->setPosition(start_position);
    sphere->setScale(vector3df(5.f, 5.f, 5.f));
    red_wire_mat.setTexture(0, driver->getTexture("../../media_folders/bmps/red.bmp"));
    sphere->getMaterial(0) = red_wire_mat;
*/

    // try this
    video::SMaterial red_wire_mat;
    //red_wire_mat.DiffuseColor = SColor(255, 255, 0, 0);
    red_wire_mat.Wireframe = true;
    red_wire_mat.Thickness = 1.f;
    red_wire_mat.BackfaceCulling = false;
    red_wire_mat.setTexture(0, a.driver->getTexture("../../media_folders/bmps/red.bmp"));




//    scene::IMeshManipulator * manip = 0;
//    manip->setVertexColors(sphere->getMesh(), video::SColor(255, 255, 0, 0));




    //
    // ILogger
    //


    //core::stringw messageLog = stringw(playerSmallBlock->getPosition().X);
    //ILogger* logger = device->getLogger();
    //logger->log("playerSmallBlock Position(X) = ",messageLog.c_str(), ELL_INFORMATION);



    //
    // input
    //


    int key_inputs = 9;

    SKeyMap keyMap[key_inputs];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    keyMap[8].Action = EKA_JUMP_UP;
    keyMap[8].KeyCode = KEY_SPACE;


    // set position later on level load
    a.camera = a.smgr->addCameraSceneNodeFPS(0, 50.f, 0.1f, IDNotPicked, keyMap, key_inputs, false, 2.5f);
    a.camera->setName("mycam");


    SKeyMap * no_function = NULL;
    a.halt_camera = a.smgr->addCameraSceneNodeFPS(0, 50.f, 0.1f, IDNotPicked, no_function, 0, true, 0, false, false);
    a.halt_camera->setPosition(core::vector3df(0, 200.f, 0));
    a.halt_camera->setTarget(core::vector3df(0,0,0));
    a.halt_camera->setName("viewcam");


    if(a.camera == 0)
    {
        std::cout << "No camera available.";
        return 1;
    }





    //
    // HUD
    //

    std::cout << "Billboards..." << std::endl;

    // john wtf //scene::ISceneNode * complete_hud(camera);
    scene::ISceneNode * complete_hud = a.smgr->addBillboardSceneNode(a.camera);
    complete_hud->setID(IDNotPicked);
    complete_hud->setName("complete");


    scene::IBillboardSceneNode * hud1 =
        a.smgr->addBillboardSceneNode(complete_hud, core::dimension2d<f32>(10.f, 5.f), core::vector3df(-6.f, 5.f, 15.f), -1, video::SColor(100,0,0,200), video::SColor(100,0,0,200));
    hud1->setMaterialFlag(video::EMF_LIGHTING, false);
    hud1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    hud1->setID(IDNotPicked);
    hud1->setName("gui1");


    scene::IBillboardSceneNode * hud2 =
        a.smgr->addBillboardSceneNode(complete_hud, core::dimension2d<f32>(10.f, 5.f), core::vector3df(6.f, 5.f, 15.f), -1, video::SColor(100,0,0,200), video::SColor(100,0,0,200));
    hud2->setMaterialFlag(video::EMF_LIGHTING, false);
    hud2->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    hud2->setID(IDNotPicked);
    hud2->setName("gui2");

    // title
    scene::IBillboardTextSceneNode * hud1_title = a.smgr->addBillboardTextSceneNode(font_cour, L"MOVE", complete_hud, core::dimension2d<f32>(3.f, 1.5f), core::vector3df(-5.f, 6.5f, 14.f));
    hud1_title->setName("hud1title");
    hud1_title->setID(0);

    // number
    scene::IBillboardTextSceneNode * hud_text = a.smgr->addBillboardTextSceneNode(font_cour, L"3", complete_hud, core::dimension2d<f32>(2.f, 2.f), core::vector3df(-5.f, 5.f, 14.f));
    hud_text->setName("3");
    hud_text->setID(0);

    scene::IBillboardSceneNode * hud_less = a.smgr->addBillboardSceneNode(complete_hud, core::dimension2d<f32>(1.2f, 1.2f), core::vector3df(-8.f, 5.f, 14.f));
    hud_less->setName("less");
    hud_less->setID(IDIsHUD);
    hud_less->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    hud_less->setMaterialTexture(0, a.driver->getTexture("../../media_folders/bmps/less_than.bmp"));
    hud_less->setMaterialFlag(video::EMF_LIGHTING, false);
    // allows a brighter appearance
    //hud_less->setMaterialFlag(video::EMF_ZBUFFER, true);

    scene::IBillboardSceneNode * hud_more = a.smgr->addBillboardSceneNode(complete_hud, core::dimension2d<f32>(1.2f, 1.2f), core::vector3df(-2.f, 5.f, 14.f));
    hud_more->setName("more");
    hud_more->setID(IDIsHUD);
    hud_more->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    hud_more->setMaterialTexture(0, a.driver->getTexture("../../media_folders/bmps/more_than.bmp"));
    hud_more->setMaterialFlag(video::EMF_LIGHTING, false);



    //
    // collision
    //

    std::cout << "Collisions..." << std::endl;

    scene::ISceneCollisionManager* collMan = a.smgr->getSceneCollisionManager();


    // Create a meta triangle selector to hold several triangle selectors.
    scene::IMetaTriangleSelector * meta = a.smgr->createMetaTriangleSelector();

    if(!meta)
    {
        std::cout << "No collisions available.";
        return 1;
    }



    //
    // NOIE billboards
    //


    loadGolemTex(a.driver);
    std::cout << "Loaded GolemBase textures.\n";




    //
    // irr file
    //

    a.smgr->loadScene("../../media_folders/playground3.irr");
    a.device->getCursorControl()->setVisible(false);

    std::cout << "Using IRR file..." << std::endl;

    // bitmasks
    int ids_change = 0;

    core::array<scene::ISceneNode *> nodes;
    a.smgr->getSceneNodesFromType(scene::ESNT_ANY, nodes); // Find all nodes

    for (u32 i=0; i < nodes.size(); ++i)
    {
        scene::ISceneNode * node = nodes[i];
        scene::ITriangleSelector * selector = 0;


        switch(node->getType())
        {
        case scene::ESNT_CUBE:
        case scene::ESNT_ANIMATED_MESH:
            // Because the selector won't animate with the mesh,
            // and is only being used for camera collision, we'll just use an approximate
            // bounding box instead of ((scene::IAnimatedMeshSceneNode*)node)->getMesh(0)
            selector = a.smgr->createTriangleSelectorFromBoundingBox(node);
        break;

        case scene::ESNT_MESH:
        case scene::ESNT_SPHERE: // Derived from IMeshSceneNode
            selector = a.smgr->createTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);
            break;
        case scene::ESNT_TERRAIN:
            selector = a.smgr->createTerrainTriangleSelector((scene::ITerrainSceneNode*)node);
            break;

        case scene::ESNT_OCTREE:
            selector = a.smgr->createOctreeTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);
            break;

        default:
            // Don't create a selector for this node type
            break;
        }


        // extras
            switch(node->getID())
            {
            // remove -1 IDs to stop bitmasks checking
            case LO3_UNINIT:
            {
                node->setID(0);
                ids_change++;
                break;
            }
            case LO3_PLAYER:
            {
                std::cout << std::endl << "Found start origin" << std::endl;
                a.camera->setPosition(node->getPosition());
                break;
            }
            case LO3_GOLEM:
            {

                GolemBase golem_temp;
                golem_temp.setTextures(monsters_tex.golem_spr, LO3_SPR_GOLEM); // setTex (from the loaded Tex)
                golem_temp.setBillboard(a.smgr, node->getPosition());
                m.golems_in_map.push_back(golem_temp);
                break;
            }
            case LO3_HEALTH:
            {
                scene::ISceneNodeAnimator * health_anim = a.smgr->createRotationAnimator(core::vector3df(0,1.2f,0));
                if(health_anim)
                {
                    node->addAnimator(health_anim);
                    health_anim->drop();
                }
                break;
            }
            }




        // free stuff
        if(selector)
        {
            // Add it to the meta selector, which will take a reference to it
            meta->addTriangleSelector(selector);
            // And drop my reference to it, so that the meta selector owns it.
            selector->drop();
        }
    }
    // bitmasks
    std::cout << "Changed " << ids_change << "IDs." << std::endl;


    //
    // models
    //

    //
    // models
    // come after irr loading as we don't want any collisions yet
    //

    scene::IAnimatedMesh* mesh = a.smgr->getMesh("../../media/sydney.md2");
    scene::IAnimatedMeshSceneNode* node = a.smgr->addAnimatedMeshSceneNode( mesh );
    node->setPosition(core::vector3df(0,160,0));

    if (node)
    {
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setFrameLoop(0, 310);
        node->setMaterialTexture( 0, a.driver->getTexture("../../media/sydney.bmp") );
    }


    // try with Irr mesh
    scene::IMeshSceneNode * movement_sphere_irr = a.smgr->addSphereSceneNode();
    movement_sphere_irr->setScale(core::vector3df(5.f, 5.f, 5.f));
    movement_sphere_irr->getMaterial(0) = red_wire_mat;
    // try with obj mesh
    scene::IMesh * movement_mesh_obj = a.smgr->getMesh("../../media_folders/mods/10x_sphere.obj");
    if(!movement_mesh_obj)
        std::cout << "No player movement mesh." << std::endl;
    scene::IMeshSceneNode * movement_sphere_obj = a.smgr->addMeshSceneNode(movement_mesh_obj);
    movement_sphere_obj->setScale(core::vector3df(10.f, 10.f, 10.f));
    movement_sphere_obj->getMaterial(0) = red_wire_mat;


    // the grid lines
    scene::IMesh * helper_grid = a.smgr->getMesh("../../media_folders/help/onlygrid.obj");
    if(!helper_grid)
        std::cout << "No helper grid.\n";
    else
    {
        scene::IMeshSceneNode * helper_node = a.smgr->addMeshSceneNode(helper_grid);
        helper_node->setScale(core::vector3df(1000,1,1000));
        helper_node->setPosition(core::vector3df(0,1,0));
        helper_node->getMaterial(0) = red_wire_mat;
    }







    scene::ISceneNodeAnimator* anim = a.smgr->createCollisionResponseAnimator(meta, a.camera, core::vector3df(10,15,10), core::vector3df(0,-10,0));

    if(!anim)
    {
        std::cout << "No gravity available.";
        return 1;
    }

    meta->drop(); // I'm done with the meta selector now
    a.camera->addAnimator(anim);
    anim->drop(); // I'm done with the animator now


    //
    // camera
    //


    //scene::ISceneNodeFactory factory;




	// In order to do framerate independent movement
    //int lastFPS = -1;
	//u32 then = a.device->getTimer()->getTime();

	// debug start position with line
    core::line3d<f32> my_ray;
    my_ray.start = a.camera->getPosition();
    my_ray.end = my_ray.start + (core::vector3df(0,100.f,0));




    for(u32 i = 0; i < m.golems_in_map.size(); ++i)
        m.golems_in_map[i].beforeRun(a.device);


    // cam
    // player is in free roam
    a.player_halt = false;
    //a.player_action_phase = false;

    //
    // game loop
    //

    while(a.device->run() && !receiver.IsQuit())
    {
        // Work out a frame delta time.
		//const u32 now = a.device->getTimer()->getTime();
		//const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		//then = now;


        // FPS rail gun
		core::line3d<f32> ray;
        ray.start = a.camera->getPosition();
        ray.end = ray.start + (a.camera->getTarget() - ray.start).normalize() * 1000.0f;




        // cam
        // keep fixed cam following player until action phase
        if(!a.player_halt)
            a.camera_fixed = a.camera->getPosition();

        // always keep the current position
        a.camera_distance_from_fixed = a.camera->getPosition();

        // check distance every loop
        float dist = a.camera_distance_from_fixed.getDistanceFrom(a.camera_fixed);

        // when player reaches outer edge halt player, set new cam
        if(dist > 100.f && !a.player_halt)
        {
            std::cout << "Perimeter reached.\n";

            // copy the camera
            a.halt_camera->setPosition(a.camera->getPosition());
            a.halt_camera->setRotation(a.camera->getRotation());


            a.smgr->setActiveCamera(a.halt_camera);

            a.camera->setInputReceiverEnabled(false);

            a.halt_camera->setInputReceiverEnabled(true);

            fps_mention->setText(L"NO FPS");
            a.player_halt = true;

        }

        if(receiver.IsKeyDown(KEY_KEY_2))
        {
            movement_sphere_obj->setPosition(a.camera->getPosition());
            // set where the middle of the sphere is
            a.camera_fixed = a.camera->getPosition();
        }

        if(receiver.IsKeyDown(KEY_KEY_3))
        {
            a.player_halt = false;
            a.smgr->setActiveCamera(a.camera);

            // try to stop the camera
            a.camera->setInputReceiverEnabled(true);
            a.halt_camera->setInputReceiverEnabled(false);

            fps_mention->setText(L"FPS");
            //receiver.receiverResetKeys();
        }

            // no check for halt camera or camera
		if(receiver.IsRightClick())
        {
            a.device->getCursorControl()->setVisible(true);
            complete_hud->setVisible(true);

            // if I comment this line out right click does not stop the camera
            a.camera->setInputReceiverEnabled(false);

            core::position2d<s32> chuck;
            chuck.X = receiver.getXY().X;
            chuck.Y = receiver.getXY().Y;

            scene::ISceneNode * grab2 = collMan->getSceneNodeFromScreenCoordinatesBB(chuck, IDIsHUD);
            if(grab2)
                std::cout << "N: " << grab2->getName();
        }

        // pretty much all the time outside the HUD
        else
        {
            a.device->getCursorControl()->setVisible(false);
            complete_hud->setVisible(false);
            a.camera->setInputReceiverEnabled(true);
        }




        // noie
//        golem1.beforeDraw(a.device);
        for(u32 i = 0; i < m.golems_in_map.size(); ++i)
            m.golems_in_map[i].beforeDraw(a.device);



        //
        // scene
        //

        a.driver->beginScene(true, true, video::SColor(0,0,200,200));



        // monster
        for(u32 i = 0; i < m.golems_in_map.size(); ++i)
        {
            m.golems_in_map[i].outsideMonst(a.camera);
            m.golems_in_map[i].directionMonst();
            m.golems_in_map[i].renderMonst();
        }


        a.smgr->drawAll();
        a.guienv->drawAll();


        // reset transforms
        a.driver->setTransform(video::ETS_WORLD, core::matrix4());
        a.driver->draw3DLine(ray.start, ray.end);
        a.driver->draw3DLine(my_ray.start, my_ray.end);
        a.driver->endScene();




        //
        // fps
        //

        int fps = a.driver->getFPS();
        int lastFPS = -1;
		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
			tmp += a.driver->getName();
			tmp += L"] fps: ";
			tmp += fps;
			a.device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
    }

    // end of game

    m.golems_in_map.clear();
    a.device->drop();

    return 0;
}

