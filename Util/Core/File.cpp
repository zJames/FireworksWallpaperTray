//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "File.h"

#include <stdio.h>
////////////////////////////////////////////////////////////////////////////////


#define NULL 0


//------------------------------------------------------------------------------
//	Local Const Data
//------------------------------------------------------------------------------
static const char kOptionText[][4] =
{
	"r",	//eOption_Read,
	"w",	//eOption_Write,
	"a",	//eOption_Append,
	"rb",	//eOption_Read_Binary,
	"rt",	//eOption_Read_Text,
	"wb",	//eOption_Write_Binary,
	"wt",	//eOption_Write_Text,
	"ab",	//eOption_Append_Binary,
	"at",	//eOption_Append_Text,
};


static const int kSeekOrigin[] =
{
	SEEK_CUR,
	SEEK_SET,
	SEEK_END,
};


//------------------------------------------------------------------------------
//	File Implementation
//------------------------------------------------------------------------------
File::File() : mFile(NULL)
{

}


File::File(const char* fileName, eOption options) : mFile(NULL)
{
	open(fileName, options);
}


File::~File()
{
	close();
}


bool File::open(const char* fileName, eOption option)
{
	close();

	if (fileName == NULL)
		return false;

#ifdef MOBILE_APP
	mFile = fopen(fileName, kOptionText[option]);
#else
	fopen_s(&mFile, fileName, kOptionText[option]);
#endif

	return mFile != NULL;
}


void File::close()
{
	if (mFile == NULL)
		return;

	fclose(mFile);
	mFile = NULL;
}


bool File::eof() const
{
	return isValid() ? feof(mFile) != 0 : true;
}


bool File::isValid() const
{
	return mFile != 0;
}


File::operator FILE* ()
{
	return mFile;
}


bool File::read(void* dest, size_t size)
{
	if (mFile == NULL)
	{
		return false;
	}

	return fread(dest, size, 1, mFile) == 1;
}


bool File::write(const void* source, size_t size)
{
	if (!isValid())
	{
		return false;
	}

	return fwrite(source, size, 1, mFile) == 1;
}


bool File::seek(int offset, SeekType st)
{
	if (!isValid() || st >= eSeekType_num || st < 0)
	{
		return false;
	}

	return fseek(mFile, offset, kSeekOrigin[st]) == 0;
}


size_t File::position() const
{
	return isValid() ? ftell(mFile) : 0;
}


size_t File::size() const
{
	if (!isValid())
	{
		return 0;
	}

	const long curPos = ftell(mFile);

	fseek(mFile, 0, SEEK_END);

	const long size = ftell(mFile);

	fseek(mFile, curPos, SEEK_SET);

	return size;
}