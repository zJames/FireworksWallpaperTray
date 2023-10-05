//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <iosfwd>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	File
//------------------------------------------------------------------------------
class File
{
public:
	enum eOption
	{
		eOption_Read,
		eOption_Write,
		eOption_Append,
		eOption_Read_Binary,
		eOption_Read_Text,
		eOption_Write_Binary,
		eOption_Write_Text,
		eOption_Append_Binary,
		eOption_Append_Text,

		eOption_num
	};

	enum SeekType
	{
		eSeekType_current,
		eSeekType_direct,
		eSeekType_end,

		eSeekType_num
	};

	File();
	File(const char* fileName, eOption);
	~File();

	bool open(const char* fileName, eOption);
	void close();
	bool eof() const;
	bool isValid() const;

	operator FILE* ();

	bool read(void*, size_t size);
	bool write(const void*, size_t size);

	template<class T>
	bool read(T* target)
	{
		return read(target, sizeof(T));
	}

	template<class T>
	bool write(const T* input)
	{
		return write(input, sizeof(T));
	}

	bool seek(int offset, SeekType = eSeekType_current);

	size_t position() const;
	size_t size() const;

private:
	FILE* mFile;
};
