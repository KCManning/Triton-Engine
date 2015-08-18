//-------------------------------------------------------------------------------------------------
#include "Texture.h"

using namespace Triton;

void Texture::load(const char* textureFile)
{
	// creates a temporary SDL Surface from the image file
	SDL_Surface* tempSurface = IMG_Load(textureFile);
	// checks if an error occurred when creating the surface
	if (tempSurface == NULL)
	{
		std::string errorMsg = "failed to load texture: ";
		throw (errorMsg + '"' + textureFile + '"' + IMG_GetError());
	}
	
	// number of pixels, multiplied by the number of components in each pixel, 
	// can reach over 64,000, the range of a short
	unsigned int pixelDataCount = tempSurface->pitch * tempSurface->h;

	int surfaceWidth = tempSurface->w;

	// pixels = new unsigned char[pixelDataCount];
	pixels.resize(pixelDataCount, 0);
	// iterates through each pixel
	for (int y = 0; y < tempSurface->h; ++y)
	{
		for (int x = 0; x < surfaceWidth; ++x)
		{
			unsigned char r, g, b, a;

			// gets the color information on the pixel and assigns them to the unsigned char values
			SDL_GetRGBA(getPixel(tempSurface, x, y), tempSurface->format, &r, &g, &b, &a);

			// copies the information of each pixel component into the pixel array
			pixels[y * surfaceWidth + x * 4] = r;
			pixels[y * surfaceWidth + x * 4 + 1] = g;
			pixels[y * surfaceWidth + x * 4 + 2] = b;
			pixels[y * surfaceWidth + x * 4 + 3] = a;
		}
	}

	// generates a texture object in the openGL context
	glGenTextures(1, &handle);
	// binds the texture for editting
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// fills the texture object with information from the pixels array
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempSurface->w, tempSurface->h, 0, GL_RGBA, 
		GL_UNSIGNED_BYTE, pixels.data());

	// releases the temporary SDL_surface from memory
	SDL_FreeSurface(tempSurface);
}

void Texture::bind()
{
	// multiple images can be assigned to other texture types such as cube maps, GL_TEXTURE_CUBE
	// this case we only have one image for GL_TEXTURE_2D
	glActiveTexture(GL_TEXTURE0);
	// binds texture for reading and writing
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::unbind()
{
	// unbinds this texture and binds a null texture
	glBindTexture(GL_TEXTURE_2D, NULL);
}

Texture::~Texture()
{
	// deletes texture from openGL context
	glDeleteTextures(1, &handle);
	// releases pixels array from memory
	pixels.clear();
}

Uint32 Triton::getPixel(SDL_Surface* surface, int x, int y)
{
	
	int bpp = surface->format->BytesPerPixel;
	// converts pixel data to a Uint8
	Uint8 * pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	// depending on image formate, each pixel can be made of a different number of components
	switch (surface->format->BytesPerPixel) {
	// 8-bit images
	case 1:
		return *pixel;
		break;
	// 16-bit images
	case 2:
		return *(Uint16 *)pixel;
		break;
	// 24-bit images
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
		else
			return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
		break;
	// 32-bit image, should be default format of texture files, 32-bit pngs
	case 4:
		return *(Uint32 *)pixel;
		break;
	// shouldn't happen, but avoids warnings
	default:
		return 0;      
	}
}