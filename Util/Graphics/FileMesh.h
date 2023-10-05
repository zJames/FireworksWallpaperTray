//------------------------------------------------------------------------------
//  Copyright : (c) 2010
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Pointers/Ptr.h"
#include "../Pointers/ScopedPtr.h"
#include "../Pointers/SharedPtr.h"

#include <WTypes.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	File Mesh
//------------------------------------------------------------------------------
namespace gfx3d
{

class MeshBase;

SharedPtr<MeshBase> loadMesh(const char* fileName);
bool saveMesh(Ptr<const MeshBase>, const char* fileName);

class MeshBase
{
public:
	enum Flags
	{
		eFlag_splitStreams,
		eFlag_index32,		//32bit indices
		eFlag_materialIds,	//Contains material ids for each vertex
		eFlag_position3,	//3 component position
		eFlag_position4,	//4 component position
		eFlag_positionT,	//already transformed
		eFlag_normal,		//3 component normal
		eFlag_texS,			//3 component texture coordinate
		eFlag_tex0,			//2 component texture coordinate
		eFlag_tex1,			//2 component texture coordinate
		eFlag_tex2,			//2 component texture coordinate
		eFlag_tex3,			//2 component texture coordinate
		eFlag_tex4,			//2 component texture coordinate
		eFlag_tex5,			//2 component texture coordinate
		eFlag_tex6,			//2 component texture coordinate
		eFlag_tex7,			//2 component texture coordinate
		eFlag_tex8,			//2 component texture coordinate
		eFlag_diffuse,		//4 byte ARGB value
		eFlag_specular,		//4 byte ARGB value
		eFlag_boneId,		//4 byte DWORD
		eFlag_boneWeights,	//4 32bit floats

		eFlag_num
	};

	struct Header
	{
		char signature[4];
		unsigned int version;
		unsigned int flags;
		unsigned int numVertices;
		unsigned int numIndices;
	};

	struct StreamInfo
	{
		Ptr<const BYTE> data;
		unsigned int size;
		unsigned int flags;
	};

	MeshBase(SharedArray<BYTE> fileData, size_t size);

	const BYTE* data() const;
	size_t dataSize() const;
	bool isValid() const;
	const Header& header() const;
	void* indices() const;
	Ptr<const int> materialIds() const;
	int indexArraySize() const;
	int materialArraySize() const;

	static int flagSize(Flags);

	virtual int numStreams() const = 0;
	virtual StreamInfo stream(int) const = 0;

protected:
	SharedArray<BYTE> mFileData;
	size_t mSize;
};

}

