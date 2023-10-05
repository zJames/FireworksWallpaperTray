//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "SaveImage.h"

#include "../Graphics.h"
#include "../Misc/TextManip.h"
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//	Graphics
//------------------------------------------------------------------------------
namespace Graphics
{

//------------------------------------------------------------------------------
//	SaveImage
//------------------------------------------------------------------------------
void saveImage(const Bitmap& input, const char* fileName)
{
	const RefString& ext = getFileExtension(fileName);

	if (_strcmpi("bmp", ext) == 0)
	{
		input.saveBMPFile(fileName);
	}
	else if (_strcmpi("png", ext) == 0)
	{
		SavePNG(input, fileName);
	}
	else if (_strcmpi("pcx", ext) == 0)
	{
		//loadPCX(txtr, image);
	}
	else if (_strcmpi("lbm", ext) == 0)
	{
		//loadILBM(txtr, image);
	}
}

} //end namespace Graphics