//------------------------------------------------------------------------------
//  Copyright : (c) 2010
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "FileMesh.h"

#include "FileMeshInternal.h"
#include "../Util.h"
#include "../Containers/BitSet.h"
#include "../Core/File.h"
#include "../Math/RoundUp.h"
////////////////////////////////////////////////////////////////////////////////


namespace gfx3d
{

//------------------------------------------------------------------------------
//	Local Const Data
//------------------------------------------------------------------------------
static const int kFlagSizes[/*MeshBase::eFlag_num*/] =
{
	0,					//eFlag_splitStreams,	//invalid for table
	0,					//eFlag_index32,		//invalid for table
	0,					//eFlag_materialIds,	//invalid for table
	3 * sizeof(float),	//eFlag_position3,		//3 component position
	4 * sizeof(float),	//eFlag_position4,		//4 component position
	4 * sizeof(float),	//eFlag_positionT,		//already transformed
	3 * sizeof(float),	//eFlag_normal,			//3 component normal
	3 * sizeof(float),	//eFlag_texS,			//3 component texture coordinate
	2 * sizeof(float),	//eFlag_tex0,			//2 component texture coordinate
	2 * sizeof(float),	//eFlag_tex1,			//2 component texture coordinate
	2 * sizeof(float),	//eFlag_tex2,			//2 component texture coordinate
	2 * sizeof(float),	//eFlag_tex3,			//2 component texture coordinate
	2 * sizeof(float),	//eFlag_tex4,			//2 component texture coordinate
	2 * sizeof(float),	//eFlag_tex5,			//2 component texture coordinate
	2 * sizeof(float),	//eFlag_tex6,			//2 component texture coordinate
	2 * sizeof(float),	//eFlag_tex7,			//2 component texture coordinate
	2 * sizeof(float),	//eFlag_tex8,			//2 component texture coordinate
	sizeof(DWORD),		//eFlag_diffuse,		//4 byte ARGB value
	sizeof(DWORD),		//eFlag_specular,		//4 byte ARGB value
	sizeof(DWORD),		//eFlag_boneId,			//4 byte DWORD
	4 * sizeof(float),	//eFlag_boneWeights,	//4 32bit floats
};


//------------------------------------------------------------------------------
//	loadMesh()
//------------------------------------------------------------------------------
SharedPtr<MeshBase> loadMesh(const char* fileName)
{
	File file(fileName, File::eOption_Read_Binary);

	if (!file.isValid())
	{
		return SharedPtr<MeshBase>();
	}

	const size_t fileSize = file.size();

	if (fileSize < sizeof(MeshBase::Header))
	{
		return SharedPtr<MeshBase>();
	}

	SharedArray<BYTE> fileData(new BYTE[fileSize]);

	if (!file.read(fileData.get(), fileSize))
	{
		return SharedPtr<MeshBase>();
	}

	const Ptr<const MeshBase::Header> header = (MeshBase::Header*)fileData.get();

	return ((header->flags & bit(MeshBase::eFlag_splitStreams)) != 0)
		? SharedPtr<MeshBase>(new SplitMesh(fileData, fileSize))
		: SharedPtr<MeshBase>(new CombinedMesh(fileData, fileSize));
}


bool saveMesh(Ptr<const MeshBase> model, const char* fileName)
{
	File file(fileName, File::eOption_Write_Binary);

	if (!file.isValid())
	{
		return false;
	}

	return file.write(model->data(), model->dataSize());
}


//------------------------------------------------------------------------------
//	Mesh Base
//------------------------------------------------------------------------------
MeshBase::MeshBase(SharedArray<BYTE> fileData, size_t size)
	:
	mFileData(fileData),
	mSize(size)
{
}


const BYTE* MeshBase::data() const
{
	return mFileData.get();
}


size_t MeshBase::dataSize() const
{
	return mSize;
}


bool MeshBase::isValid() const
{
	return (mFileData.get() != NULL);
}


const MeshBase::Header& MeshBase::header() const
{
	assert(isValid());

	return *(Header*)mFileData.get();
}


void* MeshBase::indices() const
{
	return (BYTE*)mFileData.get() + sizeof(Header);
}


Ptr<const int> MeshBase::materialIds() const
{
	if ((header().flags & bit(eFlag_materialIds)) == 0)
	{
		return NULL;
	}

	const int idxSize = indexArraySize();

	return (int*)((BYTE*)indices() + roundUp(idxSize, 4));
}


int MeshBase::indexArraySize() const
{
	const Header hdr = header();
	const int idxSize = ((hdr.flags & bit(eFlag_index32)) != 0) ? sizeof(int) : sizeof(short);

	return hdr.numIndices * idxSize;
}


int MeshBase::materialArraySize() const
{
	const Header hdr = header();

	if ((hdr.flags & bit(eFlag_materialIds)) == 0)
	{
		return 0;
	}

	return (hdr.numIndices / 3) * sizeof(int);
}


int MeshBase::flagSize(Flags flag)
{
	if (flag < 0 || flag >= eFlag_num)
	{
		return 0;
	}

	return kFlagSizes[flag];
}


//------------------------------------------------------------------------------
//	Combined Stream Mesh
//------------------------------------------------------------------------------
CombinedMesh::CombinedMesh(SharedArray<BYTE> fileData, size_t size)
	:
	MeshBase(fileData, size)
{
}


int CombinedMesh::numStreams() const
{
	return 1;
}


MeshBase::StreamInfo CombinedMesh::stream(int streamId) const
{
	assert(streamId == 0);

	StreamInfo si;

	si.data = (BYTE*)mFileData.get() + sizeof(Header) + indexArraySize() + materialArraySize();
	si.size = (unsigned int)mSize;
	si.size -= sizeof(Header);
	si.size -= indexArraySize();
	si.size -= materialArraySize();
	si.flags = header().flags;

	return si;
}


//------------------------------------------------------------------------------
//	Split Stream Mesh
//------------------------------------------------------------------------------
SplitMesh::SplitMesh(SharedArray<BYTE> fileData, size_t size)
	:
	MeshBase(fileData, size)
{
	C_ASSERT(ARRAY_LENGTH(kFlagSizes) == eFlag_num);

	const Header fileHeader = header();

	int offset = sizeof(Header) + indexArraySize() + materialArraySize();

	for (int i = 0; i < eFlag_num; ++i)
	{
		if (kFlagSizes[i] < 0)
		{
			continue;
		}

		if (fileHeader.flags & bit(i))
		{
			mStreamOffsets.push_back(Offset(offset, bit(i)));
			offset += kFlagSizes[i] * fileHeader.numVertices;
		}
	}
}


int SplitMesh::numStreams() const
{
	return (int)mStreamOffsets.size();
}


MeshBase::StreamInfo SplitMesh::stream(int streamId) const
{
	const int nStreams = numStreams();
	assert(streamId >= 0 && streamId < nStreams);

	StreamInfo info;

	info.data = (BYTE*)mFileData.get() + mStreamOffsets[streamId].range;
	info.flags = mStreamOffsets[streamId].flags;

	info.size = (streamId == (nStreams - 1))
		? unsigned int(mSize - mStreamOffsets[nStreams - 1].range)
		: mStreamOffsets[streamId + 1].range - mStreamOffsets[streamId].range;

	return info;
}

} //end namespace gfx3d