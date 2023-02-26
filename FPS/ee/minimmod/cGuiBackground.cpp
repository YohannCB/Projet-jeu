#include "cGuiBackground.h"

extern IrrlichtDevice* g_device;
extern IVideoDriver* g_driver;

//------------------------------------------------------------------------------
cGuiBackground::cGuiBackground(ECOLOR_FORMAT texformat) :
m_texformat(texformat)
{
}


//------------------------------------------------------------------------------
void cGuiBackground::Destroy()
{
	for (unsigned int i=0; i<m_texs.size(); ++i)
	{
		if (m_texs[i].tex)
			g_driver->removeTexture(m_texs[i].tex);
	}
	m_texs.clear();
}

//------------------------------------------------------------------------------
bool cGuiBackground::LoadImage(const char* filename)
{
	io::IReadFile* file=NULL;
	IImage* image = NULL;
	
	try
	{
		file = g_device->getFileSystem()->createAndOpenFile(filename);
		if (!file) throw "FileError";
		// Load the image into an IImage
		image = g_driver->createImageFromFile(file);	
		if (!image) throw "ImageError";
		// suppported sizes : 640x480, 800x600, 1024x768
		if (!(	(image->getDimension().Width==640 && image->getDimension().Height==480) ||
				(image->getDimension().Width==800 && image->getDimension().Height==600) ||
				(image->getDimension().Width==1024 && image->getDimension().Height==768)
				))
				throw "ImageSizeNotSupported";

		Destroy();

		// set up the tiles
		tTex tex;
		tex.tex = NULL;
		if (image->getDimension().Width==640 && image->getDimension().Height==480)
		{
			tex.max_pieces = 1;
			tex.rc_image[0] = rect<s32>(0,0,512,480);
			tex.rc_tex[0] = rect<s32>(0,0,512,480);
			tex.tex_w = 512;
			tex.tex_h = 512;
			m_texs.push_back(tex);
			tex.rc_image[0] = rect<s32>(512,0,512+128,480);
			tex.rc_tex[0] = rect<s32>(0,0,128,480);
			tex.tex_w = 128;
			tex.tex_h = 512;
			m_texs.push_back(tex);
			m_w = 640;
			m_h = 480;
		} else if (image->getDimension().Width==800 && image->getDimension().Height==600)
		{
			tex.max_pieces = 1;
			tex.rc_image[0] = rect<s32>(0, 0, 512, 512);
			tex.rc_tex[0] = rect<s32>(0, 0, 512, 512);
			tex.tex_w = 512;
			tex.tex_h = 512;
			m_texs.push_back(tex);
			tex.max_pieces = 6;
			tex.rc_image[0] = rect<s32>(0, 512, 512, 512+88);
			tex.rc_tex[0] = rect<s32>(0, 0, 512, 88);			
			tex.rc_image[1] = rect<s32>(512, 0, 512+288, 424);
			tex.rc_tex[1] = rect<s32>(0, 88, 288, 88+424);
			tex.rc_image[2] = rect<s32>(512, 424, 512+144, 424+88);
			tex.rc_tex[2] = rect<s32>(288, 88, 288+144, 88+88);
			tex.rc_image[3] = rect<s32>(656, 424, 656+144, 424+88);
			tex.rc_tex[3] = rect<s32>(288, 176, 288+144, 176+88);
			tex.rc_image[4] = rect<s32>(512, 512, 512+144, 512+88);
			tex.rc_tex[4] = rect<s32>(288, 264, 288+144, 264+88);
			tex.rc_image[5] = rect<s32>(656, 512, 656+144, 512+88);
			tex.rc_tex[5] = rect<s32>(288, 352, 288+144, 352+88);
			m_texs.push_back(tex);
			m_w = 800;
			m_h = 600;
		} else if (image->getDimension().Width==1024 && image->getDimension().Height==768)
		{
			tex.max_pieces = 1;
			tex.rc_image[0] = rect<s32>(0, 0, 512, 512);
			tex.rc_tex[0] = rect<s32>(0, 0, 512, 512);
			tex.tex_w = 512;
			tex.tex_h = 512;
			m_texs.push_back(tex);			
			tex.rc_image[0] = rect<s32>(512, 0, 512+512, 512);
			tex.rc_tex[0] = rect<s32>(0, 0, 512, 512);
			tex.tex_w = 512;
			tex.tex_h = 512;
			m_texs.push_back(tex);
			tex.max_pieces = 2;
			tex.rc_image[0] = rect<s32>(0, 512, 512, 512+256);
			tex.rc_tex[0] = rect<s32>(0, 0, 512, 256);
			tex.rc_image[1] = rect<s32>(512, 512, 512+512, 512+256);
			tex.rc_tex[1] = rect<s32>(0, 256, 512, 256+256);
			tex.tex_w = 512;
			tex.tex_h = 512;
			m_texs.push_back(tex);
			m_w = 1024;
			m_h = 768;
		}

		// turn off mipmapping
		bool bmipmap = g_driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
		g_driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

		// create textures and put tiles into them
		s32* imgpix = (s32*)image->lock();
		char texname[8];
		for (unsigned int i=0; i<m_texs.size(); ++i)
		{
			sprintf(texname, "BTEX%d", i);
			m_texs[i].tex = g_driver->addTexture(dimension2d<s32>(m_texs[i].tex_w, m_texs[i].tex_h), texname, m_texformat);
			s32* texpix = (s32*)m_texs[i].tex->lock();
			for (int j=0; j<m_texs[i].max_pieces; ++j)
			{				
				s32 texofs = m_texs[i].rc_tex[j].UpperLeftCorner.Y * m_texs[i].tex_w + m_texs[i].rc_tex[j].UpperLeftCorner.X;
				s32 pixels = m_texs[i].rc_tex[j].getWidth()+1;
				s32 imgofs = m_texs[i].rc_image[j].UpperLeftCorner.Y * image->getDimension().Width;
												
				// 32-Bit Image -> 32-Bit Texture (fast)
				if (image->getColorFormat() == ECF_A8R8G8B8 && m_texs[i].tex->getColorFormat() == ECF_A8R8G8B8)
				{					
					for (int y=m_texs[i].rc_tex[j].UpperLeftCorner.Y; y<=m_texs[i].rc_tex[j].LowerRightCorner.Y; ++y)
					{
						memcpy(texpix+texofs, imgpix+imgofs, pixels);
						texofs += m_texs[i].tex_w;
						imgofs += image->getDimension().Width;
					}				
				} else if (m_texs[i].tex->getColorFormat() ==  ECF_A8R8G8B8) // Image -> 32-Bit Texture (slow)
				{
					for (int y=0; y<m_texs[i].rc_tex[j].getHeight()+1; ++y)
					{
						for (int x=0; x<m_texs[i].rc_tex[j].getWidth()+1; ++x)
						{
							texpix[texofs+x] = image->getPixel(	m_texs[i].rc_image[j].UpperLeftCorner.X+x,
																m_texs[i].rc_image[j].UpperLeftCorner.Y+y).color;
						}
						texofs += m_texs[i].tex_w;
					}
				} else if (m_texs[i].tex->getColorFormat() ==  ECF_A1R5G5B5) // Image -> 16-Bit Texture (slow)
				{
					s16* texpix16 = (s16*)texpix;
					for (int y=0; y<m_texs[i].rc_tex[j].getHeight()+1; ++y)
					{
						for (int x=0; x<m_texs[i].rc_tex[j].getWidth()+1; ++x)
						{
							texpix16[texofs+x] = image->getPixel(	m_texs[i].rc_image[j].UpperLeftCorner.X+x,
																m_texs[i].rc_image[j].UpperLeftCorner.Y+y).toA1R5G5B5();
						}
						texofs += m_texs[i].tex_w;
					}

				}

			}
			m_texs[i].tex->unlock();
		}

		// restore mipmapping-state
		g_driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, bmipmap);

		image->unlock();
		file->drop();
	} catch (const char*)
	{
		if (image) image->drop();
		if (file) file->drop();
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
void cGuiBackground::Draw()
{
	int sw = g_driver->getScreenSize().Width;
	int sh = g_driver->getScreenSize().Height;
	float scale_w = 1.0f;
	float scale_h = 1.0f;
	bool scale = false;
	if (m_w != sw) 
	{
		scale_w = float(sw) / float(m_w);
		scale = true;
	}
	if (m_h != sh) 
	{
		scale_h = float(sh) / float(m_h);
		scale = true;
	}

	for (unsigned int i=0; i<m_texs.size(); ++i)
	{
		for (int j=0; j<m_texs[i].max_pieces; ++j)
		{						
			if (scale)
			{
				rect<s32> irc = m_texs[i].rc_image[j];
				if (scale_w != 1.0f)
					irc = rect<s32>( int(float(irc.UpperLeftCorner.X)*scale_w),
									irc.UpperLeftCorner.Y,
									int(float(irc.LowerRightCorner.X)*scale_w),
									irc.LowerRightCorner.Y);
				if (scale_h != 1.0f)
					irc = rect<s32>( irc.UpperLeftCorner.X,
									int(float(irc.UpperLeftCorner.Y)*scale_h),
									irc.LowerRightCorner.X,
									int(float(irc.LowerRightCorner.Y)*scale_h));
				rect<s32> trc = m_texs[i].rc_tex[j];
				trc.LowerRightCorner.X--;
				trc.LowerRightCorner.Y--;
				g_driver->draw2DImage(m_texs[i].tex, irc, trc, 0, 0, false);
			} else
			{
				g_driver->draw2DImage(	m_texs[i].tex, 
										m_texs[i].rc_image[j].UpperLeftCorner,
										m_texs[i].rc_tex[j],
										0,
										SColor(255,255,255,255),
										false);

			}
			
		}
	}
}
