#include "TerrainHeightMap.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#ifndef WIN32
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif


using namespace std;
using namespace ap;

float max(float a, float b, float c)
{
    if (a > b) {
        if (a > c) return a;
        else return c;
    } else if (b > c) return b;
    return c;
}

TerrainHeightMap::TerrainHeightMap(int width, float widthScale, float heightScale, const std::string& rawFilename, bool is16bit)
		: m_Width(width), m_WidthScale(widthScale), m_HeightScale(heightScale)
{
    std::ifstream ifs;
    ios::fmtflags coutflags = cout.flags();     // Store cout flags..

    SDL_Surface *imageSurface;

    imageSurface = IMG_Load(rawFilename.c_str());

    if (imageSurface)       // IF SDL_image manages to open the image..
    {
        float invScale = heightScale / 256.0f;
        float r, g, b;
        SDL_PixelFormat *pixFormat = imageSurface->format;
        uint32_t pixel;

        if (m_Width == 0) {
            m_Width = imageSurface->w;
            m_Vertices = new float[m_Width * m_Width];
        }

        cout << ios::dec << "[TerrainHeightMap] Horizontal scale: "<<m_WidthScale<<", vertical scale: "<<m_HeightScale<<", map width: "<<m_Width<<endl;

        if (SDL_MUSTLOCK(imageSurface)) SDL_LockSurface(imageSurface);
        if (imageSurface->w != imageSurface->h) { cout << "[TerrainHeightMap] Image is not rectangular!"<<endl; }
        if (imageSurface->w != m_Width) { cout << "[TerrainHeightMap] Image is not of specified size ("<<width<<")!"<<endl; }
        if (pixFormat->palette) { cout << "[TerrainHeightMap] Image has a palette! Possible problem!"<<endl; }

        cout << "[TerrainHeightMap] Image format: ";
        cout << "BytesPerPixel = "<<int(pixFormat->BytesPerPixel)<<"  ";
        cout << "RGBshift = "<<int(pixFormat->Rshift)<<","<<int(pixFormat->Gshift)<<","<<int(pixFormat->Bshift)<<"  ";
        cout << ios::hex << "RGBmask = "<<pixFormat->Rmask<<","<<pixFormat->Gmask<<","<<pixFormat->Bmask <<endl;

        for(int y = 0; y < m_Width; ++y) {                      // loop through and populate the vertices
            for(int x = 0; x < m_Width; ++x) {
                pixel = ((uint8_t *)imageSurface->pixels)[x*pixFormat->BytesPerPixel + y*imageSurface->pitch];
                r = (pixel & pixFormat->Rmask) >> pixFormat->Rshift;
                g = (pixel & pixFormat->Gmask) >> pixFormat->Gshift;
                b = (pixel & pixFormat->Bmask) >> pixFormat->Bshift;
                m_Vertices[y*m_Width + x] = max(r,g,b) * invScale;
            }
        }

        if (SDL_MUSTLOCK(imageSurface)) SDL_UnlockSurface(imageSurface);
        SDL_FreeSurface(imageSurface);
    }
    else                       // Otherwise, loat it as raw
    {
        cout << "Raw load. Image system error: "<<IMG_GetError() <<endl;
        ifs.open(rawFilename.c_str(), std::ios::binary);    // TODO: add some error checking
        m_Vertices = new float[m_Width * m_Width];

        unsigned short tmp = 0;
        float invScale = 1.0f / 65535.0f;

        for(int y = 0; y < m_Width; ++y) {                      // loop through and populate the vertices
            for(int x = 0; x < m_Width; ++x) {
                tmp = ifs.get();				                // get first byte
                if(is16bit)	tmp += ifs.get() << 8;              // if its 16 bit, get the second
                m_Vertices[y*m_Width + x] = float(tmp) * invScale;     // add the current value to the correct position
            }
        }
    }

    cout.flags(coutflags);      // restore cout flags
    ifs.close();
}//end HeightMap()

TerrainHeightMap::~TerrainHeightMap()
{
    delete[] m_Vertices;
}//end ~HeightMap


/**
* Print the height map to standard output, useful for debugging
*/
void TerrainHeightMap::printHeightMap()
{
    // now print out the heightmap
    for(int y = 0; y < m_Width; ++y)
    {
        for(int x = 0; x < m_Width; ++x)
        {
            std::cout << std::setw(2) <<
                static_cast<int>(m_Vertices[y*m_Width + x] * m_HeightScale)<< ",";
        }
        std::cout << std::endl;
    }
}//end printHeightMap


/**
* This function is merely a modified form of the function
* of the same name in TerrainRenderable.
*/
float TerrainHeightMap::getHeightAt(float x, float z)
{
//    float start_y = indexHeight(0,0);
//    float end_y = indexHeight(m_Width,m_Width);

    float start_x = 0;
    float start_z = 0;
//    float end_x = m_Width*m_WidthScale/4;       // That is, end_x = 320625
//    float end_z = m_Width*m_WidthScale/4;
    float end_x = m_WidthScale;
    float end_z = m_WidthScale;

    float x_pct = ( x - start_x ) / ( end_x - start_x );
    float z_pct = ( z - start_z ) / ( end_z - start_z );

    float x_pt = x_pct * ( float ) ( m_Width);
    float z_pt = z_pct * ( float ) ( m_Width);

    int x_index = ( int ) x_pt;
    int z_index = ( int ) z_pt;

    // If we got to the far right / bottom edge, move one back
    if (x_index == m_Width) {
        --x_index;
        x_pct = 1.0f;
    } else {
        // get remainder
        x_pct = x_pt - x_index;
    }
    if (z_index == m_Width) {
        --z_index;
        z_pct = 1.0f;
    } else {
        z_pct = z_pt - z_index;
    }

    //bilinear interpolate to find the height.
    float t1 = indexHeight( x_index, z_index);
    float t2 = indexHeight( x_index + 1, z_index);
    float b1 = indexHeight( x_index, z_index + 1);
    float b2 = indexHeight( x_index + 1, z_index + 1);

    float midpoint = (b1 + t2) / 2.0;

    if (x_pct + z_pct <= 1) {
        b2 = midpoint + (midpoint - t1);
    } else {
        t1 = midpoint + (midpoint - b2);
    }

    float t = ( t1 * ( 1 - x_pct ) ) + ( t2 * ( x_pct ) );
    float b = ( b1 * ( 1 - x_pct ) ) + ( b2 * ( x_pct ) );
    float h = ( t * ( 1 - z_pct ) ) + ( b * ( z_pct ) );

    return h;// * m_HeightScale;
}//end getHeightAt



inline float TerrainHeightMap::indexHeight(int x, int y)
{
	return m_Vertices[m_Width * y + x];
}//end indexHeight
