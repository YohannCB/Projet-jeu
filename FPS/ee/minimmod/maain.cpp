#include<irrlicht.h>
#include<iostream>


#include "irrKlang-64bit-1.6.0/include/irrKlang.h"
using namespace irrklang;

#include"FollowingCamera.hpp"
 
using namespace irr;           
using namespace core;                               
using namespace scene;                                                           
using namespace video;                                                 
using namespace gui;                                         
using namespace io;                                         
         
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib") 

ISceneNode* player[2] = {0,0};     
IrrlichtDevice* device = 0;
   
//==========================================================================

class MonEcouteur : public IEventReceiver
{
public:   
   virtual bool OnEvent(const SEvent& event)   
   {

      if (event.EventType == irr::EET_KEY_INPUT_EVENT)
         KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

      /* if (event.KeyInput.Key == KEY_KEY_A && event.KeyInput.PressedDown)
      { 
           player->setRotation( vector3df(0, player->getRotation().Y - 5.0f, 0) );
           return true;   
      }               
 
      if (event.KeyInput.Key == KEY_KEY_E && event.KeyInput.PressedDown)
      {
           player->setRotation( vector3df(0, player->getRotation().Y + 5.0f, 0) ); 
           return true;   

      } 

      if (event.KeyInput.Key == KEY_KEY_Z && event.KeyInput.PressedDown)
      {                       
           vector3df facing( cos( player->getRotation().Y * PI/180.0f ), 0, -sin( player->getRotation().Y * PI/180.0f ) );
           facing.normalize();
           vector3df newPos = player->getPosition() + (facing*5.0f); 
           player->setPosition( newPos );
           return true;
	   }
     
      if (event.KeyInput.Key == KEY_KEY_S && event.KeyInput.PressedDown)       
      {
           vector3df facing( cos( player->getRotation().Y * PI/180.0f ), 0, -sin( player->getRotation().Y * PI/180.0f ) );
           facing.normalize();
           vector3df newPos = player->getPosition() - (facing*5.0f)  ;
           player->setPosition( newPos );
           return true;
	   } */    
      return false;
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

};

//==============================================================================

int main()
{
    MonEcouteur receiver;
    IrrlichtDevice *device =
        createDevice( video::EDT_OPENGL, dimension2d<u32>(1920, 1080), 16,
            false, false, false, &receiver);
              
    device->setWindowCaption(L"Following Camera"); 
   
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

     ISoundEngine* engine = createIrrKlangDevice();

     engine->play2D("irrKlang-64bit-1.6.0/media/naruto.mp3", true);
    
//=================================== objet ====================================

     device->getFileSystem()->addZipFileArchive("media/mp_alienbunker.pk3");
   
    IAnimatedMesh* q3levelmesh = smgr->getMesh("mp_alienbunker.bsp");
    ISceneNode* q3node = 0;
   
    if (q3levelmesh)
        q3node = smgr->addOctTreeSceneNode(q3levelmesh->getMesh(0));

    ITriangleSelector* selector = 0;
   
    if (q3node)
    {       
        q3node->setPosition(vector3df(1450,100,-1500));
        selector = smgr->createOctTreeTriangleSelector(q3levelmesh->getMesh(0), q3node, 128);
        q3node->setTriangleSelector(selector);
        selector->drop();       
    }   

    player[0] = smgr->addAnimatedMeshSceneNode(smgr->               
    getMesh("media/sydney.md2"));     
    player[0]->setRotation(irr::core::vector3df(0,0,0) );
    player[0]->setMaterialTexture(0, driver->getTexture("media/sydney.bmp"));
    player[0]->setMaterialFlag(video::EMF_LIGHTING , false);
    player[0]->setPosition(vector3df(1030,150,-1500));   
    player[0]->setScale(core::vector3df(1.5,1.5,1.5));
       
    ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
        selector, player[0], vector3df(15,40,20),
        vector3df(0,-3,0),     
        vector3df(0,0,0)); 
    player[0]->addAnimator(anim);     
    anim->drop();

      player[1] = smgr->addAnimatedMeshSceneNode(smgr->               
    getMesh("media/sydney.md2"));     
    player[1]->setRotation(irr::core::vector3df(0,0,0) );
    player[1]->setMaterialTexture(0, driver->getTexture("media/sydney.bmp"));
    player[1]->setMaterialFlag(video::EMF_LIGHTING , false);
    player[1]->setPosition(vector3df(1720,150,-1500));   
    player[1]->setScale(core::vector3df(1.5,1.5,1.5));
       
     ISceneNodeAnimator* anim2 = smgr->createCollisionResponseAnimator(
        selector, player[1], vector3df(15,40,20),
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

      if (receiver.IsKeyDown(KEY_KEY_M))
	{
	   player[0]->setPosition(player[1]->getPosition());
	}

      if (receiver.IsKeyDown(KEY_KEY_H))
	{
	  MaCamera->Target(player[1]->getPosition());
	}
      
        driver->beginScene(true, true, SColor(255,100,101,140));
       
        smgr->drawAll(); 
        guienv->drawAll();
	 if (receiver.IsKeyDown(KEY_KEY_H))
	{
	  MaCamera->Target(player[1]->getPosition());
	  MaCamera->Unfocused();  
	}
	 else
	{
	 MaCamera->Update();
	}       
       
        driver->endScene();
            
    }           
    if(MaCamera) {delete MaCamera;}
    engine->drop();
    device->drop();

    return 0;
}
