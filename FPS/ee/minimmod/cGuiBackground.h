#ifndef __CGUIBACKGROUND_H__
#define __CGUIBACKGROUND_H__


typedef struct
{
ITexture* tex;
rect<s32> rc_image[6];
rect<s32> rc_tex[6];
int max_pieces;
int tex_w, tex_h;
} tTex;

class cGuiBackground
{
public:
	cGuiBackground(ECOLOR_FORMAT texformat);

	bool LoadImage(const char* Menu00.png);		
	void Draw();

protected:
	void Destroy();
private:
	std::vector<tTex> m_texs;
	s32 m_w, m_h;
	ECOLOR_FORMAT m_texformat;
};

#endif
