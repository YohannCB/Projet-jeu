#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "driverChoice.h"
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <stdlib.h>
#include <string>
//#include "Bombe.hpp"



using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


//CLass pour les mouvements
class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


//class pour image de loading

struct SAppContext
{
    IrrlichtDevice *device;
    s32             counter;
    IGUIListBox*    listbox;
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
    GUI_ID_QUIT_BUTTON = 101,
    GUI_ID_NEW_WINDOW_BUTTON,
    GUI_ID_FILE_OPEN_BUTTON,
    GUI_ID_TRANSPARENCY_SCROLL_BAR
};




int main()
{
	
	//création de la clock du temps
	float x,y;
	clock_t time_req; 	
	
	

	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device
	MyEventReceiver receiver;

	IrrlichtDevice* device = createDevice(driverType,
			core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

     
	
	//scene::ISceneNode* n2 = smgr->addCubeSceneNode();
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = 	device->getSceneManager();


	//open le fond d'écran(chargement)
	
	IGUIEnvironment* env = device->getGUIEnvironment();
	
	//env->addImage(driver->getTexture("media/entrer.png"),
        //position2d<int>(0,0));


	
    
    	
    	
        
	

   	 /*
	To be able to look at and move around in this scene, we create a first
	person shooter style camera and make the mouse cursor invisible.
	*/
    
    	smgr->addCameraSceneNode(0, vector3df(0,90,-40), vector3df(0,50,0));
    



	gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(
		L"", core::rect<s32>(10, 10, 400, 20));
	diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));
	
	
        
	
	
	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED = 50.f;
	
	//close le fond d'écran(chargement)

	u32 now = device->getTimer()->getTime();  
	static u32 last = now;
	
	
	//Calcul de la distance entre le cube et la bombe
	// si c'est un node : n->getMesh()->getBoundingBox().getExtent()*2.5f; (pour un personnage-ex : bomberman)
	
	
	
	
	
	ITimer *timer = device->getTimer();
	u32 temps_actuel, temps_precedent;
	 
	temps_precedent = timer->getTime();

	
	int Check = 0;
	int controle = 1;
	int TempsActu = 0;
	int TempsDem = 0;
	


	while(device->run())
	{
	
	  if(receiver.IsKeyDown(irr::KEY_DOWN))
	    {
		if (controle==1)
		  {
		    env->clear();
		    env->addImage(driver->getTexture("media/play.png"),
		    position2d<int>(0,0));
		  }

		if (controle==2)
		  {
		    env->clear();
		    env->addImage(driver->getTexture("media/opt.png"),
		    position2d<int>(0,0));
		  }
		if (controle==3)
		  {
		    env->clear();
		    env->addImage(driver->getTexture("media/quit.png"),
		    position2d<int>(0,0));
		  }
		  }
		  }
		    

	device->drop();
	
	return 0;
}


