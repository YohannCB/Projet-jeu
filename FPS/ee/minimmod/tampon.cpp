#include <string>
#include <iostream>

#include "Personnage.hpp"

#include <irrlicht.h>

#include "driverChoice.h"

#include <IGUIContextMenu.h>  

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

enum
{
    ID_IsNotPickable = 0,
    IDFlag_IsPickable = 1 << 0,
    IDFlag_IsHighlightable = 1 << 1
};

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace gui;

const int ResX=1680;
const int ResY=1050;
const bool fullScreen=false;

bool SplitScreen=false;

ICameraSceneNode *camera[2]={0,0};

class MyEventReceiver : public IEventReceiver
{
 public:
  virtual bool OnEvent(const SEvent& event)
  {
       if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	 KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
       
       if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_KEY_F && event.KeyInput.PressedDown)
            {
                SplitScreen = !SplitScreen;
                return true;
            }
       
            if (camera[1])
                return camera[1]->OnEvent(event);
            return false;

	    if(camera[0])
	      return camera[0]->OnEvent(event);
	    return false;
	    
  }

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
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
  
};


int menu()
{
  IrrlichtDevice *device=createDevice(EDT_SOFTWARE, dimension2d<u32>(512, 384), 16, false, false, false, 0);

  device->setWindowCaption(L"HELLO WORLD!");
  
  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();

  guienv->addStaticText(L"HELLO WORLD! THIS IS THE IRRLICHT SOFTWARE ENGINE!",rect<int>(10,10,200,22), true);

  ITexture *imageTexture = device->getVideoDriver()->addTexture(dimension2d<u32>((u32) cameraImageMat->cols,(u32) cameraImageMat->rows), "cameraImage");

  auto *tex_buf = (unsigned char *) imageTexture->lock();
  unsigned char *frame_buf = cameraImageMat->data;

  for (int j = 0; j < cameraImageMat->rows * cameraImageMat->cols; j++) {
    *(tex_buf) = *(frame_buf + 2);
    *(tex_buf + 1) = *(frame_buf + 1);
    *(tex_buf + 2) = * (frame_buf);
    *(tex_buf +3) = 225;
    frame_buf += 3;
    tex_buf += 4;
  }

  imageTexture->unlock();
}





int main()
 {

				       
//CREATION MOTEUR,DRIVER ET SCENEMANAGER

  
    video::E_DRIVER_TYPE driverType=driverChoiceConsole();
    if (driverType==video::EDT_COUNT)
        return 1;
   
    MyEventReceiver receiver;

    IrrlichtDevice *device = createDevice(driverType,
            dimension2du(ResX,ResY), 32, fullScreen,
            false, false, &receiver);
    if (!device)
        return 1;

    ISceneManager *smgr = device->getSceneManager();
    ISceneManager *smgr2 = device->getSceneManager();
    IVideoDriver *driver = device->getVideoDriver();



    
 //CREATION MODELES 3D

scene::ITriangleSelector* selector = 0;    
    IAnimatedMesh *model = smgr->getMesh("media/sydney.md2");
    if (!model)
        return 1;

    video::SMaterial material;
    
    IAnimatedMeshSceneNode *model_node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/sydney.md2"), 0, IDFlag_IsPickable | IDFlag_IsHighlightable);

if (model_node)
    {

      ITexture *texture = driver->getTexture("media/sydney.bmp");
        model_node->setMaterialTexture(0,texture);
       	model_node->setMD2Animation(scene::EMAT_RUN);
        model_node->setMaterialFlag(EMF_LIGHTING,false);
    }

 selector = smgr->createTriangleSelector(model_node);
 model_node->setTriangleSelector(selector);
 selector->drop();

 	IAnimatedMeshMD2* weaponMesh = (IAnimatedMeshMD2*) smgr->getMesh("media/gun.md2");
	IAnimatedMeshSceneNode* WeaponNode;
	if ( weaponMesh->getMeshType() == EAMT_MD2 )
	{
		s32 count = weaponMesh->getAnimationCount();
		for ( s32 i = 0; i != count; ++i )
		{
		        //snprintf ( buf, 64, "Animation: %s", weaponMesh->getAnimationName(i) );
			//device->getLogger()->log(buf, ELL_INFORMATION);
		}
	}

	WeaponNode = smgr->addAnimatedMeshSceneNode(weaponMesh, smgr->getActiveCamera(), 10, vector3df( 0, 0, 0), vector3df(-90,-90,90));
	WeaponNode->setMaterialFlag(EMF_LIGHTING, false);
	WeaponNode->setMaterialTexture(0, driver->getTexture( "media/gun.jpg"));
	WeaponNode->setLoopMode ( false );
	WeaponNode->setName ( "tommi the gun man" );



 
// CREATION CARTE & SES COLLISIONS.

 
	device->getFileSystem()->addFileArchive("media/map-20kdm2.pk3");
	//IAnimatedMesh *map = smgr->getMesh("20kdm2.bsp");
			IAnimatedMesh *map = smgr->getMesh("Wii+-+Naruto+Shippuden+Clash+of+Ninja+Revolution+3+-+Konoha.skp");
	
    IMeshSceneNode* map_node = 0;
    
    if (map)
    {
        map_node = smgr->addOctreeSceneNode(map->getMesh(0), 0, IDFlag_IsPickable);
    }
    
    
    if(map_node)
    {
      map_node->setPosition(core::vector3df(-1350,-130,-1400));

        selector = smgr->createOctreeTriangleSelector(
                map_node->getMesh(), map_node, 128);
        map_node->setTriangleSelector(selector);
    }
    map_node->setMaterialFlag(EMF_LIGHTING,false);

    
    
//CREATION COLLISIONS


    


    
    
// CREATION CAMERAS.

    
    device->getCursorControl()->setVisible(false);
    
    camera[0] = smgr->addCameraSceneNodeFPS(0, 30.0f, 30.0f, ID_IsNotPickable, 0, 0, true, 3.f);
    camera[1] = smgr->addCameraSceneNodeFPS(0, 30.0f, 30.0f, ID_IsNotPickable, 0, 0, true, 3.f);

     if (selector)
    {
        ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
            selector, camera[1], vector3df(30,50,30),
            vector3df(0,-10,0), vector3df(0,30,0));
        selector->drop(); // As soon as we're done with the selector, drop it.
        camera[1]->addAnimator(anim);
        anim->drop();  // And likewise, drop the animator when we're done referring to it.
    }

     /*scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
    bill->setMaterialTexture(0, driver->getTexture("../../media/particle.bmp"));
    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialFlag(video::EMF_ZBUFFER, false);
    bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
    bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it*/


    int lastFPS = -1;



// GESTION DEPLACEMENTS
    u32 then = device->getTimer()->getTime(); 
    const f32 MOVEMENT_SPEED =1.0f;


    
 ISceneNode* highlightedSceneNode = 0;
 ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

 map_node->setPosition(vector3df(-1000.0f,-40.0f,-1000.0f));
        model_node->setPosition(vector3df(1000.0f,200.0f,1000.0f));
    
     while(device->run())
	{
	  

	  if (device->isWindowActive())
    {
        driver->beginScene(true, true, 0);
        smgr->drawAll();

        // Unlight any currently highlighted scene node
        if (highlightedSceneNode)
        {
            highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
            highlightedSceneNode = 0;
        }

        // All intersections in this example are done with a ray cast out from the camera to
        // a distance of 1000.  You can easily modify this to check (e.g.) a bullet
        // trajectory or a sword's position, or create a ray from a mouse click position using
        // ISceneCollisionManager::getRayFromScreenCoordinates()
        core::line3d<f32> ray;
        ray.start = camera[1]->getPosition();
        ray.end = ray.start + (camera[1]->getTarget() - ray.start).normalize() * 1000.0f;

        // Tracks the current intersection point with the level or a mesh
        core::vector3df intersection;
        // Used to show with triangle has been hit
        core::triangle3df hitTriangle;

        // This call is all you need to perform ray/triangle collision on every scene node
        // that has a triangle selector, including the Quake level mesh.  It finds the nearest
        // collision point/triangle, and returns the scene node containing that point.
        // Irrlicht provides other types of selection, including ray/triangle selector,
        // ray/box and ellipse/triangle selector, plus associated helpers.
        // See the methods of ISceneCollisionManager
        scene::ISceneNode * selectedSceneNode =
            collMan->getSceneNodeAndCollisionPointFromRay(
                    ray,
                    intersection, // This will be the position of the collision
                    hitTriangle, // This will be the triangle hit in the collision
                    IDFlag_IsPickable, // This ensures that only nodes that we have
                            // set up to be pickable are considered
                    0); // Check the entire scene (this is actually the implicit default)

        // If the ray hit anything, move the billboard to the collision position
        // and draw the triangle that was hit.
        if(selectedSceneNode)
        {
	  // bill->setPosition(intersection);

            // We need to reset the transform before doing our own rendering.
            driver->setTransform(video::ETS_WORLD, core::matrix4());
            driver->setMaterial(material);
            driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));

            // We can check the flags for the scene node that was hit to see if it should be
            // highlighted. The animated nodes can be highlighted, but not the Quake level mesh
            if((selectedSceneNode->getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable)
            {
                highlightedSceneNode = selectedSceneNode;

                // Highlighting in this case means turning lighting OFF for this node,
                // which means that it will be drawn with full brightness.
                highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
            }
        }
	  
//DEPLACEMENTS


	//const u32 now = device->getTimer()->getTime();
	//const f32 frameDeltaTime = (f32)(now - then) / 1000.f;
        //then = now;
	
	 vector3df sydneyPos(model_node->getPosition().X, model_node->getPosition().Y - 60, model_node->getPosition().Z);

	 vector3df sydneyOrient = model_node->getRotation();

	 vector3df camOrient = camera[1]->getRotation();

	 vector3df camOnSydney(sydneyPos.X, sydneyPos.Y +25, sydneyPos.Z);

	 vector3df replaceCam(0 , camera[1]->getRotation().Y - 90, camera[1]->getRotation().Z);

	 vector3df camVec = camera[1]->getPosition();
	 
	 f32 angleCam;

	 //if(!SplitScreen)
	 //{
	 
        if(receiver.IsKeyDown(KEY_KEY_Z))
	  {
	    camVec.Z -= MOVEMENT_SPEED * sin(sydneyOrient.Y / 180 * 3.14);
	  camVec.X += MOVEMENT_SPEED * cos(sydneyOrient.Y / 180 * 3.14);  
	  }
        else if(receiver.IsKeyDown(KEY_KEY_S))
	  {
	    camVec.Z += MOVEMENT_SPEED * sin(sydneyOrient.Y / 180 * 3.14);
	  camVec.X -= MOVEMENT_SPEED * cos(sydneyOrient.Y / 180 * 3.14);
	  }
	if(receiver.IsKeyDown(KEY_KEY_Q))
	  {
	     camVec.Z += MOVEMENT_SPEED * sin(camOrient.Y / 180 * 3.14);
	  camVec.X -= MOVEMENT_SPEED * cos(camOrient.Y / 180 * 3.14);
	  }
	else if(receiver.IsKeyDown(KEY_KEY_D))
	  {
	    camVec.Z -= MOVEMENT_SPEED * sin(camOrient.Y / 180 * 3.14);
	  camVec.X += MOVEMENT_SPEED * cos(camOrient.Y / 180 * 3.14); 
	  }
	/*	else if(receiver.IsKeyDown(KEY_KEY_D))
	  {
	    camVec.Z -= MOVEMENT_SPEED * sin(camOrient.Y / 180 * 3.14);
	  camVec.X += MOVEMENT_SPEED * cos(camOrient.Y / 180 * 3.14); 
	  }*/

	vector3df SydneyVec(camVec.X, camVec.Y - 25, camVec.Z);

	vector3df WeaponVec(camVec.X, camVec.Y, camVec.Z);

	vector3df Void(-100,-100,-100);

	camera[1]->setPosition(camVec);
	
	model_node->setPosition(SydneyVec);
	model_node->setRotation(replaceCam);

	WeaponNode->setPosition(WeaponVec);
	WeaponNode->setRotation(replaceCam);

	//}
        driver->setViewPort(rect<s32>(0,0,ResX,ResY));
        driver->beginScene(true,true,SColor(255,100,100,100));




	
// DESSIN DES CAMERAS


	smgr->drawAll();
	
        if (SplitScreen)
	  {
        smgr->setActiveCamera(camera[0]);
	  }
	
	if(!SplitScreen)
	  {
        smgr->setActiveCamera(camera[1]);
	  }
	
	driver->endScene();



	
// AFFICHAGE FPS

	
        if (driver->getFPS() != lastFPS)
        {
            lastFPS = driver->getFPS();
            stringw tmp = L"SUPER JEU DE TIR 3D - BETA TEST  (FPS: ";
            tmp += lastFPS;
            tmp += ")";
            device->setWindowCaption(tmp.c_str());
        }
    }
	}

      
    device->drop();
    return 0;
}
