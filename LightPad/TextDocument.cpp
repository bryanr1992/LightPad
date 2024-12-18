#include <Windows.h>
#include "TextDocument.h"


/*
* TextDocument constructor
*/
TextDocument::TextDocument()
{
	m_buffer = 0;
	m_length = 0;
}

/*
* TextDocument destructor
*/
TextDocument::~TextDocument()
{
	clear();
}

bool TextDocument::init(TCHAR* filename)
{
	HANDLE hFile;
	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (hFile == INVALID_HANDLE_VALUE) { return false; }

	return init(hFile);
}

bool TextDocument::init(HANDLE hFile)
{
	ULONG numread;

	if ((m_length = GetFileSize(hFile, 0)) == 0) { return false; }

	// allocate a new buffer for the file
	if ((m_buffer = new char[m_length]) == 0) { return false; }

	// read file into memory
	//Not a smart way to load file into mem, but we will improve this when I know how
	ReadFile(hFile, m_buffer, m_length, &numread, 0);

	// Work out where each line starts
	init_linebuffer();

	CloseHandle(hFile);
	return true;
}

/*
* TODO: Finish implementing
*/
bool TextDocument::init_linebuffer()
{
	return true;
}

/*
* For now only used as class destructor
*
*/
bool TextDocument::clear()
{
	m_length = 0;
	m_buffer = 0;

	return true;
}