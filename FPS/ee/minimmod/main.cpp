#include<irrlicht.h>
#include<iostream>


#include "irrKlang-64bit-1.6.0/include/irrKlang.h"
using namespace irrklang;

#include "FollowingCamera.hpp"
#include "Personnage.hpp"
 
using namespace irr;           
using namespace core;                               
using namespace scene;                                                           
using namespace video;                                                 
using namespace gui;                                         
using namespace io;                                         
         
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")


ISceneNode* player[2] = {0,0};     
   
//==========================================================================

class MonEcouteur : public IEventReceiver
{
public:

  struct SMouseState
    {
        core::position2di Position;
        bool LeftButtonDown;
        SMouseState() : LeftButtonDown(false) { }
    } MouseState;

  
   virtual bool OnEvent(const SEvent& event)   
   {

      if (event.EventType == irr::EET_KEY_INPUT_EVENT)
         KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

       if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch(event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN:
                MouseState.LeftButtonDown = true;
                break;

            case EMIE_LMOUSE_LEFT_UP:
                MouseState.LeftButtonDown = false;
                break;

            case EMIE_MOUSE_MOVED:
                MouseState.Position.X = event.MouseInput.X;
                MouseState.Position.Y = event.MouseInput.Y;
                break;

            default:
                // Nous n'utiliserons pas la roulette.
                break;
            }
        }

       if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
            && event.JoystickEvent.Joystick == 2)
        {
            JoystickState = event.JoystickEvent;
        }

      /* if (event.KeyInput.Key == KEY_KEY_A && event.KeyInput.PressedDown)
      { 
           player->setRotation( vector3df(0, player->getRotation().Y - 5.0f, 0) );
           return true;   
	   }*/      
     
      return false;
   }

   const SEvent::SJoystickEvent & GetJoystickState(void) const
    {
        return JoystickState;
    }

    const SMouseState & GetMouseState(void) const
    {
        return MouseState;
    }

      virtual bool IsKeyDown(EKEY_CODE keyCode) const
      {
	return KeyIsDown[keyCode];
      }

      MonEcouteur()
	{
	  for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
	    KeyIsDown[i] = false;
	}
  
private:
  bool KeyIsDown[KEY_KEY_CODES_COUNT];
  SEvent::SJoystickEvent JoystickState;

};

 void LookAt(ISceneNode* Spaceship, ISceneNode* Planet)
    {
       vector3df origin, destin, result;
       vector3df rot;
       origin = Spaceship->getAbsolutePosition();
       destin = Planet->getAbsolutePosition();
       result = destin - origin;
       result = result.getHorizontalAngle();
       rot = result;
       Spaceship->setRotation(vector3df(rot.X,rot.Y + 270,rot.Z));
    }

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



//==============================================================================

int main()
{

  
    MonEcouteur receiver;
    IrrlichtDevice *device =
      createDevice( video::EDT_OPENGL, dimension2d<u32>(1920, 1080), 16,
          false, false, false, &receiver);
              
    device->setWindowCaption(L"Following Camera"); 
    IGUIEnvironment* env = device->getGUIEnvironment();
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
     ISoundEngine* engine = createIrrKlangDevice();

        
	

     engine->play2D("media2/menu.ogg", true);


     core::array<SJoystickInfo> joystickInfo;
    if(device->activateJoysticks(joystickInfo))
    {
        std::cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << std::endl;

        for(u32 joystick = 2; joystick < joystickInfo.size(); ++joystick)
        {
            std::cout << "Joystick " << joystick << ":" << std::endl;
            std::cout << "\tName: '" << joystickInfo[joystick].Name.c_str() << "'" << std::endl;
            std::cout << "\tAxes: " << joystickInfo[joystick].Axes << std::endl;
            std::cout << "\tButtons: " << joystickInfo[joystick].Buttons << std::endl;

            std::cout << "\tHat is: ";

            switch(joystickInfo[joystick].PovHat)
            {
            case SJoystickInfo::POV_HAT_PRESENT:
                std::cout << "present" << std::endl;
                break;

            case SJoystickInfo::POV_HAT_ABSENT:
                std::cout << "absent" << std::endl;
                break;

            case SJoystickInfo::POV_HAT_UNKNOWN:
            default:
                std::cout << "unknown" << std::endl;
                break;
            }
        }
    }
    else
    {
        std::cout << "Joystick support is not enabled." << std::endl;
    }


//=================================== objet ====================================

    device->getFileSystem()->addZipFileArchive("media/mp_alienbunker.pk3");

    IAnimatedMesh* q3levelmesh = smgr->getMesh("mp_alienbunker.bsp");
    ISceneNode* q3node = 0;
   
    if (q3levelmesh)
        q3node = smgr->addOctTreeSceneNode(q3levelmesh->getMesh(0));

    ITriangleSelector* selector = 0;
   
    if (q3node)
    {       
        q3node->setPosition(vector3df(-1370,-130,-1400));
        selector = smgr->createOctTreeTriangleSelector(q3levelmesh->getMesh(0), q3node, 128);
        q3node->setTriangleSelector(selector);
        selector->drop();       
    }   

    player[0] = smgr->addAnimatedMeshSceneNode(smgr->               
    getMesh("media/sasuk.obj"));     
    player[0]->setRotation(irr::core::vector3df(0,180,0) );
    player[0]->setMaterialTexture(2, driver->getTexture("media/Sasukcape.BMP"));
    player[0]->setMaterialTexture(0, driver->getTexture("media/sasuk.BMP"));
     player[0]->setMaterialTexture(1, driver->getTexture("media/333.BMP"));
    player[0]->setMaterialFlag(video::EMF_LIGHTING , false);
    player[0]->setPosition(vector3df(-890,0,-1395));   
    player[0]->setScale(core::vector3df(0.3,0.3,0.3));
       
    ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
        selector, player[0], vector3df(15,1,20),
        vector3df(0,-3,0),     
        vector3df(0,0,0)); 
    player[0]->addAnimator(anim);     
    anim->drop();

      player[1] = smgr->addAnimatedMeshSceneNode(smgr->               
    getMesh("media/naruto.obj"));     
    player[1]->setRotation(irr::core::vector3df(0,0,0) );
    player[1]->setMaterialTexture(0, driver->getTexture("media/naruto1.png"));
    player[1]->setMaterialTexture(1, driver->getTexture("media/naruto2.jpg"));
    player[1]->setMaterialTexture(3, driver->getTexture("media/naruto3.png"));
    player[1]->setMaterialFlag(video::EMF_LIGHTING , false);
    player[1]->setPosition(vector3df(-1880,60,-1395));   
    player[1]->setScale(core::vector3df(0.3,0.3,0.3));
       
     ISceneNodeAnimator* anim2 = smgr->createCollisionResponseAnimator(
        selector, player[1], vector3df(15,1,20),
        vector3df(0,-3,0),     
        vector3df(0,0,0)); 
    player[1]->addAnimator(anim2);     
    anim2->drop();            
      
//==================================Camera======================================
                                         
    FollowingCamera* MaCamera   = new FollowingCamera(player[0], smgr,
                                                  true,           // Pour utiliser une position initiale.
                                                  selector,       // Pour les collisions.
                                                  100,            // Distance.
                                                  50,             // Hauteur.
                                                  30,             // Delta cible.
                                                  vector3df(-50,200,-120));   // Position initiale.


    
    smgr->setActiveCamera( MaCamera->getCam() );
     
//==============================================================================



      scene::ISceneNode * node = smgr->addMeshSceneNode(
        smgr->addArrowMesh( "Arrow",
                video::SColor(255, 255, 0, 0),
                video::SColor(255, 0, 255, 0),
                16,16,
                2.f, 1.3f,
                0.1f, 0.6f
                )
        );
    node->setMaterialFlag(video::EMF_LIGHTING, false);

    
     u32 then = device->getTimer()->getTime();
    const f32 MOVEMENT_SPEED = 5.f;

    
    
    while(device->run())         
    {

       // Calcul le temps delta d'une image.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Temps en secondes.
        then = now;
      
    	  bool movedWithJoystick = false;
        core::vector3df nodePosition = node->getPosition();

        if(joystickInfo.size() > 0)
        {
            f32 moveHorizontal = 0.f; // La fourchette va de -1.f (tout à gauche) à 1.f (tout à droite).
            f32 moveVertical = 0.f; // -1.f (tout en bas) à  +1.f (tout en haut).

            const SEvent::SJoystickEvent & joystickData = receiver.GetJoystickState();

            // Nous recevons une fourchette de valeur analogique complète des axes, et donc nous implémentons notre
            // propre zone morte. Ceci est une valeur empirique, depuis que certaines manettes ont plus de gigue ou de glissement
            // autour du point central que d'autres. Nous utiliserons 5 % de la fourchette comme zone morte,
            // mais généralement, vous voudrez donner à l'utilisateur la possibilité de changer cela.
            const f32 DEAD_ZONE = 0.05f;

            moveHorizontal =
                (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
            if(fabs(moveHorizontal) < DEAD_ZONE)
                moveHorizontal = 0.f;

            moveVertical =
                (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
            if(fabs(moveVertical) < DEAD_ZONE)
                moveVertical = 0.f;

            // L'information POV du chapeau est supportée uniquement par Windows, mais sa valeur
            // sera obligatoirement 65535 lorsque non-supporté, donc nous pouvons la vérifier.
            const u16 povDegrees = joystickData.POV / 100;
            if(povDegrees < 360)
            {
                if(povDegrees > 0 && povDegrees < 180)
                    moveHorizontal = 1.f;
                else if(povDegrees > 180)
                    moveHorizontal = -1.f;

                if(povDegrees > 90 && povDegrees < 270)
                    moveVertical = -1.f;
                else if(povDegrees > 270 || povDegrees < 90)
                    moveVertical = +1.f;
            }

            if(!core::equals(moveHorizontal, 0.f) || !core::equals(moveVertical, 0.f))
            {
                nodePosition.X += MOVEMENT_SPEED * frameDeltaTime * moveHorizontal;
                nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime * moveVertical;
                movedWithJoystick = true;
            }
        }

        // Si le nœud de la flèche n'est pas déplacé avec la manette, alors il suit le curseur de la souris.
       


	
	/*   

    
    	smgr->addCameraSceneNode(0, vector3df(0,90,-40), vector3df(0,50,0));
    

	*/


	while(device->run())
	{


      if (receiver.IsKeyDown(KEY_KEY_A))                          
      {                                                                                              
           player[0]->setRotation( vector3df(0, player[0]->getRotation().Y - 5.0f, 0) );                                                                                                 
      }                                                                                              
                                                                                                     
      if (receiver.IsKeyDown(KEY_KEY_E))                             
      {                                                                                              
           player[0]->setRotation( vector3df(0, player[0]->getRotation().Y + 5.0f, 0) );                                                                                                 
      }                                                                                              
                                                                                                     
      if (receiver.IsKeyDown(KEY_KEY_Z))                             
      {                                                                                              
           vector3df facing( cos( player[0]->getRotation().Y * PI/180.0f ), 0, -sin( player[0]->getRotatio\
n().Y * PI/180.0f ) );                                                                               
           facing.normalize();                                                                       
           vector3df newPos = player[0]->getPosition() + (facing*5.0f);                                 
           player[0]->setPosition( newPos );                                                                                                                                          
           }                                                                                         
                                                                                                     
      if (receiver.IsKeyDown(KEY_KEY_S))                             
      {                                                                                              
           vector3df facing( cos( player[0]->getRotation().Y * PI/180.0f ), 0, -sin( player[0]->getRotatio\
n().Y * PI/180.0f ) );                                                                               
           facing.normalize();                                                                       
           vector3df newPos = player[0]->getPosition() - (facing*5.0f)  ;                               
           player[0]->setPosition( newPos );
      }


      /////////////////// J2     

       if (receiver.IsKeyDown(KEY_KEY_J))                          
      {                                                                                              
           player[1]->setRotation( vector3df(0, player[1]->getRotation().Y - 5.0f, 0) );                                                                                                 
      }                                                                                              
                                                                                                     
      if (receiver.IsKeyDown(KEY_KEY_L))                             
      {                                                                                              
           player[1]->setRotation( vector3df(0, player[1]->getRotation().Y + 5.0f, 0) );                                                                                                 
      }                                                                                              
                                                                                                     
      if (receiver.IsKeyDown(KEY_KEY_I))                             
      {                                                                                              
           vector3df facing( cos( player[1]->getRotation().Y * PI/180.0f ), 0, -sin( player[1]->getRotatio\
n().Y * PI/180.0f ) );                                                                               
           facing.normalize();                                                                       
           vector3df newPos = player[1]->getPosition() + (facing*5.0f);                                 
           player[1]->setPosition( newPos );                                                                                                                                          
           }                                                                                        
                                                                                                     
      if (receiver.IsKeyDown(KEY_KEY_K))                             
      {                                                                                              
           vector3df facing( cos( player[1]->getRotation().Y * PI/180.0f ), 0, -sin( player[1]->getRotatio\
n().Y * PI/180.0f ) );                                                                               
           facing.normalize();                                                                       
           vector3df newPos = player[1]->getPosition() - (facing*5.0f)  ;                               
           player[1]->setPosition( newPos );
           }


      /////////////////////////////// OUTILS
      

	   //ATTAQUES

	   if(receiver.IsKeyDown(KEY_KEY_P))
	     {
	       int vie;
	       // gui::IGUIFont *font = env->getFont("media/bigfont.png");
	        gui::IGUIStaticText *texte = env->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
        rect<s32>(10,10,260,22), true);
		//texte->setOverrideFont(font);


	     }

	   
	}
      	 // else
	 //	{
       MaCamera->Update();
	 //	}       
      
        driver->beginScene(true, true, SColor(255,100,101,140));
        
        env->drawAll();
	smgr->drawAll();
       
        driver->endScene();
            
    }           
    if(MaCamera) {delete MaCamera;}
    engine->drop();
    device->drop();
  
    }
}
