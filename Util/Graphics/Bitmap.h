//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <windows.h>
////////////////////////////////////////////////////////////////////////////////


class File;


//------------------------------------------------------------------------------
//	Bitmap Class
//------------------------------------------------------------------------------
class Bitmap
{
public:
	enum Format
	{
		F_INDEXED,
		F_R5G5B5,
		F_R5G6B5,
		F_R8G8B8,
		F_A8R8G8B8,

#ifndef MOBILE_APP
#endif

		F_NUM,
	};

	inline			Bitmap() { initBitmap(); }
	inline			~Bitmap() { freeBitmap(); }
					Bitmap(const Bitmap&);
	inline			operator HDC() const { return mBitmapDC; }
	void			clear(DWORD color);
	void			draw(HDC destDC, int destx, int desty) const;
	void			draw(HDC destDC, int destx, int desty, int destwidth, int destheight, int sourcex, int sourcey) const;
	void			drawAll(Bitmap& dest) const;
	void			drawAll(HDC destDC) const;
	void			freeBitmap();
	inline int		channels() const		{ return mChannels; }
	inline int		height() const		{ return mHeight; }
	inline BYTE*	dataPtr() const { return mSurfaceBits; }
	inline HBITMAP	bitmapHandle() const	{ return mBitmapNew; }
	inline HDC		drawSurface() const	{ return mBitmapDC; }
	unsigned char*	linePtr(int y) const;
	bool			palette(int StartColor, int NumColors, RGBQUAD* pColors) const;
	unsigned char*	pixelPtr(int x, int y) const;
	inline int		stride() const { return mStride; }
	inline int		width() const { return mWidth; }
	inline bool		hasBitmap() const { return mBitmapDC != 0; }
	bool			loadBMPFile(const char* szFileName);
	bool			loadData(const char* data);
	bool			loadResource(LPCSTR name, HINSTANCE hInst);
	void			rgbaFpxToBgra();
	bool			saveBMPFile(const char* szFileName) const;
	void			setPalette(int StartColor, int NumColors, const RGBQUAD* pColors);
	bool			setSize(int width, int height, Format fmt);	// Returns true for success.
	inline Format	format() const { return mFormat; }
	void			swap(Bitmap& other);

	static Format	format(int channels);

	Bitmap& operator=(const Bitmap& other);

private:
	void			initBitmap();
	int				decompressToBitmap(const char *CompressedData, int DataLength);
	void			initDerivedData();
	bool			internalLoadBMPFile(const BYTE* pData);
	bool			writeBMPLine(File& file, int y) const;
	bool			internalSaveBMPFile(File& file) const;
	int				readColorMap(const BYTE* pFile, const BITMAPINFOHEADER *infoheader);
	bool			readBMPLine(const BYTE* pData, int y, int* offset);

	int				mBytesPerLine;		// number of bytes per data line
	int				mChannels;			// number of channels/planes
	int				mHeight;			// height of image
	int				mStride;			// number of bytes per line
	int				mWidth;				// width of image
	unsigned char*	mSurfaceBits;		// pointer to the bottom of the image
	unsigned char*	mLineZeroPointer;	// pointer to the first line
	HDC				mBitmapDC;			// HDC of the image
	HBITMAP			mBitmapNew;			// new bitmap we put on the HDC
	HBITMAP			mBitmapMonochrome;	// old bitmap that was on the HDC
	Format			mFormat;
};
