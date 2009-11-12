#ifndef __TERRAINHEIGHTMAP_H__
#define __TERRAINHEIGHTMAP_H__

#include <string>
#include "types.h"

/* Mostly downloaded and hacked to use SDL_image. Based on
 * http://www.ogre3d.org/wiki/index.php/Ogre_Compatible_HeightMap
 * Note that while the original was written for raw, that code has been
 * fiddled with too, but not tested. Beware! ;). */

/// Reads and stores heightmap information for TerrainSceneManager scenes.
class TerrainHeightMap
{
public:
    /**
     * set width = 0 to autodetect. Only works with an image atm, not a raw file
     */
	TerrainHeightMap(int width, float widthScale, float heightScale,
			 const std::string& rawFilename, bool is16bit = true);

	~TerrainHeightMap();

	/**
	 * Print the height map to standard output, useful for debugging
	 */
	void printHeightMap();

	/**
	 * This function is merely a modified form of the function
	 * of the same name in TerrainRenderable.
	 */
	float getHeightAt(float x, float z);

private:
	/**
	 * Simple function for pulling out a value from the array
	 */
	inline float indexHeight(int x, int y);

private:
	float* m_Vertices;
	int m_Width;
	float m_WidthScale;
	float m_HeightScale;
};

#endif
