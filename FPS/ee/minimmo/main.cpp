
#include<irrlicht.h>
#include<iostream>
#include<driverChoice.h>


#include "irrKlang-64bit-1.6.0/include/irrKlang.h"
using namespace irrklang;

#include"FollowingCamera.hpp"
#include "Personnage.hpp"
 
using namespace irr;           
using namespace core;                               
using namespace scene;                                                           
using namespace video;                                                 
using namespace gui;                                         
using namespace io;                                         
         
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")


IAnimatedMeshSceneNode* player[2] = {0,0};     
   
//==========================================================================

class MonEcouteur : public IEventReceiver
{
public:

   // Nous créons une structure pour enregistrer les informations sur l'état de la souris.
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

       // Se rappelle de l'état de la souris.
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
      
        // L'état de chaque manette connectée nous est envoyé 
        // à chaque run() du moteur irrlicht. Stockons l'état
        // de la première manette, et ignorons les autres.
        // Ceci n'est actuellement supporté que sous Windows et Linux.
	
        if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
	    && event.JoystickEvent.Joystick == 3)
	  {
	  JoystickState = event.JoystickEvent;
	    }


	if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
	     && event.JoystickEvent.Joystick == 2)
	   {
	    JoystickState2 = event.JoystickEvent;
	     }


	
	
      return false;
   }


       const SEvent::SJoystickEvent & GetJoystickState(void) const
    {
        return JoystickState;
    }

   const SEvent::SJoystickEvent & GetJoystickState2(void) const
  {
  return JoystickState2;
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
  SEvent::SJoystickEvent JoystickState2;

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


   // Demande à l'utilisateur un pilote.
    video::E_DRIVER_TYPE driverType=driverChoiceConsole();
    if (driverType==video::EDT_COUNT)
        return 1;

    // Crée le moteur.
    MonEcouteur receiver;
  
  Sasuke Sasuke1;
  Naruto Naruto1;


  
    IrrlichtDevice *device =
      createDevice( driverType, dimension2d<u32>(1920, 1080), 16,
          false, false, false, &receiver);
     if (device == 0)
        return 1; // Ne peut pas créer le pilote sélectionné.


     ITimer *timer = device->getTimer();
  u32 temps_actuel, temps_precedent;

    temps_precedent = timer->getTime(); 

  
     core::array<SJoystickInfo> joystickInfo;
    if(device->activateJoysticks(joystickInfo))
    {
        std::cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << std::endl;

        for(u32 joystick = 0; joystick < joystickInfo.size(); ++joystick)
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

    core::stringw tmp = L"Irrlicht Joystick Example (";
    tmp += joystickInfo.size();
    tmp += " joysticks)";
    device->setWindowCaption(tmp.c_str());
              
    IGUIEnvironment* env = device->getGUIEnvironment();
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
     ISoundEngine* engine = createIrrKlangDevice();

     scene::ISceneNode * node = smgr->addMeshSceneNode(
        smgr->addArrowMesh( "Arrow",
                video::SColor(255, 255, 0, 0),
                video::SColor(255, 0, 255, 0),
                16,16,
                2.f, 1.3f,
                0.1f, 0.6f
                )
        );
     node->setPosition(vector3df(-890,0,-1395));
    node->setMaterialFlag(video::EMF_LIGHTING, false);


    // Comme dans l'exemple 04, nous allons utiliser un mouvement indépendant du nombre d'images par seconde.
    u32 then = device->getTimer()->getTime();
    const f32 MOVEMENT_SPEED = 20.f;

        
	

     engine->play2D("media2/menu.ogg", true);




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
    getMesh("media/narut.b3d"));     
    player[0]->setRotation(irr::core::vector3df(0,180,0) );
    player[0]->setMaterialTexture(1, driver->getTexture("media/naruto2.jpg"));
    player[0]->setMaterialFlag(video::EMF_LIGHTING , false);
    player[0]->setPosition(vector3df(-890,0,-1395));   
    player[0]->setScale(core::vector3df(0.3,0.3,0.3));
     player[0]->setFrameLoop(0, 0);
    player[0]->setAnimationSpeed(15);
       
    ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
        selector, player[0], vector3df(15,1,20),
        vector3df(0,-3,0),     
        vector3df(0,0,0)); 
    player[0]->addAnimator(anim);     
    anim->drop();

      player[1] = smgr->addAnimatedMeshSceneNode(smgr->               
    getMesh("media/narut.b3d"));     
    player[1]->setRotation(irr::core::vector3df(0,0,0) );
    player[1]->setMaterialTexture(0, driver->getTexture("media/naruto1.png"));
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
                                                  false,           // Pour utiliser une position initiale.
                                                  selector,       // Pour les collisions.
                                                  100,            // Distance.
                                                  50,             // Hauteur.
                                                  30,             // Delta cible.
                                                  vector3df(-50,200,-120));   // Position initiale.


    
    smgr->setActiveCamera( MaCamera->getCam() );
     
//==============================================================================

    bool frame;
    int nbr;
    int vitesse = 10;
     irr::u32 thetime = timer->getTime();
    irr::u32 thetime2 = timer->getTime();
    bool animbool = true;
    int tick = 0;
    
   
    
    while(device->run())         
    {
 
      //irr::u32 time = device->getTimer()->getTime () % vitesse;
      //device->sleep (vitesse-time);

      int controle = 1;
      if(receiver.IsKeyDown(irr::KEY_SPACE))
	{

      if(controle == 1)
	{
	  env->clear();
	  env->addImage(driver->getTexture("media/Chat.jpg"),
	  position2d<int>(0,0));
	  controle = 2;
	}

      if(controle == 2)
 	{
          env->clear();
   	  env->addImage(driver->getTexture("media/90.png"),
          position2d<int>(0,0));
	  controle = 3;
					
					
	}
       if(controle == 3)
				{
			                env->clear();
					env->addImage(driver->getTexture("media/80.png"),
					position2d<int>(0,0));
					controle = 4;
				}
				if(controle == 4)
				{
				        
					env->addImage(driver->getTexture("media/70.png"),
					position2d<int>(0,0));	
					controle = 5;
				}
				if(controle == 5)
				{
				        env->clear();
					env->addImage(driver->getTexture("media/60.png"),
					position2d<int>(0,0));
					controle = 6;
				}
				if(controle == 6)
				{
					env->clear();
					env->addImage(driver->getTexture("media/50.png"),
					position2d<int>(0,0));
					controle = 7;
				
				}
				if(controle == 7)
				{
					env->clear();
					env->addImage(driver->getTexture("media/40.png"),
					position2d<int>(0,0));
					controle = 8;
				
				}
				if(controle == 8)
				{
				        
					env->addImage(driver->getTexture("media/30.png"),
					position2d<int>(0,0));
					controle = 9;
				
				}
				if(controle == 9)
				{
				        env->clear();
					env->addImage(driver->getTexture("media/20.png"),
					position2d<int>(0,0));
					controle = 10;
				
				}
				if(controle == 10)
				{
				        env->clear();
					env->addImage(driver->getTexture("media/10.png"),
					position2d<int>(0,0));
					controle = 11;
				
				}
				if(controle == 11)
				{
				        env->clear();
				        env->addImage(driver->getTexture("media/0.png"),
					position2d<int>(0,0));
					controle = 1;
				
				}
				
			        
	}

    
      // Calcul le temps delta d'une image.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Temps en secondes.
        then = now;


        bool movedWithJoystick = true;
        core::vector3df nodePosition = player[0]->getPosition();
	core::vector3df nodeRotation = player[0]->getRotation();
	core::vector3df nodeRotation2 = player[1]->getRotation();

	int michou;
	 const SEvent::SJoystickEvent & joystickData = receiver.GetJoystickState();
	 const SEvent::SJoystickEvent & joystickData2 = receiver.GetJoystickState2();

	   
	  if(joystickData.IsButtonPressed(4))
                        {
			  Sasuke1.recevoirDegats(10); 
			  std::cout << Sasuke1.vie << std::endl;
			   player[0]->setFrameLoop(40, 70);
    player[0]->setAnimationSpeed(15);
                        }
      
	  if(joystickData2.IsButtonPressed(4))
	  {
	    Sasuke1.recevoirDegats(10);
	    std::cout << Sasuke1.vie << std::endl;
	     player[0]->setFrameLoop(30, 70);
    player[0]->setAnimationSpeed(15);
	  }
	    
	  
        if(joystickInfo.size() > 0)
        {
            f32 moveHorizontal = 0.f; // La fourchette va de -1.f (tout à gauche) à 1.f (tout à droite).
            f32 moveVertical = 0.f; // -1.f (tout en bas) à  +1.f (tout en haut).

	    f32 moveHorizontal2 = 0.f; // La fourchette va de -1.f (tout à gauche) à 1.f (tout à droite).
            f32 moveVertical2 = 0.f; // -1.f (tout en bas) à  +1.f (tout en haut).

	    

           

            // Nous recevons une fourchette de valeur analogique complète des axes, et donc nous implémentons notre
            // propre zone morte. Ceci est une valeur empirique, depuis que certaines manettes ont plus de gigue ou de glissement
            // autour du point central que d'autres. Nous utiliserons 5 % de la fourchette comme zone morte,
            // mais généralement, vous voudrez donner à l'utilisateur la possibilité de changer cela.
            const f32 DEAD_ZONE = 0.05f;

	    const f32 DEAD_ZONE2 = 0.15f;

            moveHorizontal =
                (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
            if(fabs(moveHorizontal) < DEAD_ZONE)
                moveHorizontal = 0.f;

            moveVertical =
	      (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
            if(fabs(moveVertical) < DEAD_ZONE)
	      moveVertical = 0.f;


	        moveHorizontal2 =
                (f32)joystickData2.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
            if(fabs(moveHorizontal2) < DEAD_ZONE2)
                moveHorizontal2 = 0.f;

            moveVertical2 =
	      (f32)joystickData2.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
            if(fabs(moveVertical2) < DEAD_ZONE2)
	      moveVertical2 = 0.f;

	    


	    

	    
            // L'information POV du chapeau est supportée uniquement par Windows, mais sa valeur
            // sera obligatoirement 65535 lorsque non-supporté, donc nous pouvons la vérifier.
            const u16 povDegrees = joystickData.POV / 100;
            if(povDegrees < 360)
            {
                if(povDegrees > 0 && povDegrees < 180)
		  {
		  moveHorizontal = 1.f;
		  }
		  
                else if(povDegrees > 180)
		  {
		  moveHorizontal = -1.f;
		  }

                if(povDegrees > 90 && povDegrees < 270)
		  {
		    moveVertical = -5.f;
		  }
		else if(povDegrees > 270 || povDegrees < 90)
		  {
		     moveVertical = +5.f;
		  }
		  
            }
	    if(core::equals(moveVertical, 0.f))
	      {
		animbool = true;
			 player[0]->setFrameLoop(0, 0);
	    player[0]->setAnimationSpeed(100);
	      }
            if(!core::equals(moveHorizontal, 0.f) || !core::equals(moveVertical, 0.f))
	      {

		if(animbool == true)
		  {
		 player[0]->setFrameLoop(3, 37);
	    player[0]->setAnimationSpeed(50);
	    animbool = false;
		  }
		
		
	    nodeRotation.Y += 4 * MOVEMENT_SPEED * frameDeltaTime * moveHorizontal; //Z POS
	    
	    vector3df facing( cos( player[0]->getRotation().Y * PI/180.0f ), 0, -sin( player[0]->getRotatio\
n().Y * PI/180.0f ) );                                                                               
           facing.normalize();


 
    temps_actuel = timer->getTime();
    int passed = nbr * 1000;
    
    //irr::u32 time = device->getTimer()->getTime () % vitesse;
     // device->sleep (vitesse-time);
    irr::u32 time = timer->getTime () % vitesse;
    std::cout << "thetime =" << std::endl;
    std::cout << thetime << std::endl;
    std::cout << "thetime2" << std::endl;
    std::cout << thetime2 << std::endl;
    if (thetime > thetime2) 
    {
      tick = tick + 1;
      if(tick == 1)
	{
	   player[0]->setPosition(player[0]->getPosition() + (6 * facing * moveVertical));
      thetime2 = timer->getTime();
      tick = 0;
	}
    }
    else{
	thetime = timer->getTime();
      }
    
    //vector3df nodePosition = player[0]->getPosition() + (facing*5.0f);
	   
    //nodePosition.X += MOVEMENT_SPEED * frameDeltaTime * -moveVertical;
                movedWithJoystick = true;
		}

	    const u16 povDegrees2 = joystickData2.POV / 100;
            if(povDegrees2 < 360)
            {
                if(povDegrees2 > 0 && povDegrees2 < 180)
		  {
		  moveHorizontal2 = 1.f;
		  }
		  
                else if(povDegrees2 > 180)
		  {
		  moveHorizontal2 = -1.f;
		  }

                if(povDegrees2 > 90 && povDegrees2 < 270)
		  {
		    moveVertical2 = -5.f;
		  }
		else if(povDegrees2 > 270 || povDegrees2 < 90)
		  {
		     moveVertical2 = +5.f;
		  }
		  
            }
            if(!core::equals(moveHorizontal2, 0.f) || !core::equals(moveVertical2, 0.f))
	      {
	    nodeRotation2.Y += 4 * MOVEMENT_SPEED * frameDeltaTime * moveHorizontal2; //Z POS
	    
	    vector3df facing( cos( player[1]->getRotation().Y * PI/180.0f ), 0, -sin( player[1]->getRotatio\
n().Y * PI/180.0f ) );                                                                               
           facing.normalize();

	       player[1]->setPosition(player[1]->getPosition() + (4 * facing * moveVertical2));
	   vector3df nodePosition2 = player[1]->getPosition() + (facing*5.0f);
		       //nodePosition.X += MOVEMENT_SPEED * frameDeltaTime * -moveVertical;
                movedWithJoystick = true;
		}

	    
        }

        // Si le nœud de la flèche n'est pas déplacé avec la manette, alors il suit le curseur de la souris.
        if(!movedWithJoystick)
        {
            // Crée un rayon partant du curseur de la souris.
            core::line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
											      receiver.GetMouseState().Position, MaCamera->getCam());

            // Ainsi qu'une intersection du rayon avec un plan proche d'un nœud faisant face à la caméra.
            core::plane3df plane(nodePosition, core::vector3df(0, 0, -1));
            core::vector3df mousePosition;
            if(plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition))
            {
                // Nous avons maintenant une position de souris dans un espace 3D; déplaçons-la autour.
                core::vector3df toMousePosition(mousePosition - nodePosition);
                const f32 availableMovement = MOVEMENT_SPEED * frameDeltaTime;

                if(toMousePosition.getLength() <= availableMovement)
                    nodePosition = mousePosition; // Saute à la position finale
                else
                    nodePosition += toMousePosition.normalize() * availableMovement; // Move towards it
            }
        }

	player[0]->setRotation(nodeRotation);
	player[1]->setRotation(nodeRotation2);
        //player[0]->setPosition(nodePosition);

        // Active ou désactive l'éclairage selon que le bouton gauche de la souris est pressé ou non.
        player[0]->setMaterialFlag(video::EMF_LIGHTING, receiver.GetMouseState().LeftButtonDown);
		
	/*   

    
    	smgr->addCameraSceneNode(0, vector3df(0,90,-40), vector3df(0,50,0));
    

	*/



	  if(!receiver.IsKeyDown(KEY_KEY_A))
	    {
	      frame = true;
	    }
	
      if (receiver.IsKeyDown(KEY_KEY_A) && frame == true)
      {                                                                                              
            Sasuke1.recevoirDegats(10); 
	    std::cout << Sasuke1.vie << std::endl;
	    frame = false;
	    
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
      
      if (receiver.IsKeyDown(KEY_KEY_M))
	{
	   player[0]->setPosition(player[1]->getPosition());
	}

      if (receiver.IsKeyDown(KEY_KEY_H))
	{
	  LookAt(player[0], player[1]);
	  MaCamera->Vecteur = vector3df(player[1]->getPosition().X, player[1]->getPosition().Y, player[1]->getPosition().Z);

	   vector3df facing( cos( player[0]->getRotation().Y * PI/180.0f ), 0, -sin( player[0]->getRotatio\
n().Y * PI/180.0f ) );                                                                               
           facing.normalize();                                                                       
           vector3df newPos = player[0]->getPosition() + (facing*20.0f)  ;                               
           player[0]->setPosition( newPos );
	   

	   //ATTAQUES

	   if(receiver.IsKeyDown(KEY_KEY_P))
	     {
	       int vie;
	       Sasuke1.recevoirDegats(10);
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
        
	smgr->drawAll();
	env->drawAll();
       
        driver->endScene();
            
    }           
    if(MaCamera) {delete MaCamera;}
    // engine->drop();
    device->drop();
  
}
