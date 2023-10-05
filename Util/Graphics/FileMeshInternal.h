//------------------------------------------------------------------------------
//  Copyright : (c) 2010
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <vector>
////////////////////////////////////////////////////////////////////////////////


namespace gfx3d
{

//------------------------------------------------------------------------------
//	Combined Stream Mesh
//------------------------------------------------------------------------------
class CombinedMesh: public MeshBase
{
public:
	CombinedMesh(SharedArray<BYTE> fileData, size_t size);

	virtual int numStreams() const;
	virtual StreamInfo stream(int) const;
};

	
//------------------------------------------------------------------------------
//	Split Stream Mesh
//------------------------------------------------------------------------------
class SplitMesh: public MeshBase
{
public:
	SplitMesh(SharedArray<BYTE> fileData, size_t size);

	virtual int numStreams() const;
	virtual StreamInfo stream(int) const;

private:
	struct Offset
	{
		Offset(int range, unsigned int flags): range(range), flags(flags) {}

		int range;
		int flags;
	};

	std::vector<Offset> mStreamOffsets;
};

}