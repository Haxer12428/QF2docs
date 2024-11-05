#include "../QF.h"

QF::Utils::Image::Image(const std::filesystem::path& _PathToImage, bool _ModifyColor, const ImColor& _ImageColor)
	: m_Path{ _PathToImage }, m_ModifyColor{ _ModifyColor }, m_Color{ _ImageColor } {
	/* Load texture */
	glewInit();
	func_LoadTexture();
} 

void QF::Utils::Image::func_LoadTexture()
{
	m_Texture = SOIL_load_OGL_texture(
		m_Path.string().c_str(),
		SOIL_LOAD_AUTO, // Automatically detect the format
		SOIL_CREATE_NEW_ID, // Create a new OpenGL texture ID
		SOIL_FLAG_INVERT_Y // Invert the Y axis to match OpenGL's texture coordinates
	);
}

ImTextureID QF::Utils::Image::g_GLTexture() const
{
	return reinterpret_cast<ImTextureID>(m_Texture);
}