#include <Windows.h>
#include "TextDocument.h"


/*
* TextDocument constructor
*/
TextDocument::TextDocument()
{
	m_buffer = 0;
	m_linebuffer = 0;
	m_length = 0;
	m_numlines = 0;
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
	if ((m_buffer = new char[m_length]) == nullptr) { return false; }

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
	ULONG i = 0;
	ULONG linestart = 0;

	if ((m_linebuffer = new ULONG[m_length]) == nullptr) { return false; }

	m_numlines = 0;

	//Loop through every byte of data in the file (our m_buffer)
	for (; i < m_length;)
	{
		if (m_buffer[i++] == '\r')
		{
			if (m_buffer[i] == '\n') { i++; }

			m_linebuffer[m_numlines++] = linestart;
			linestart = i;
		}
	}

	m_linebuffer[m_numlines++] = m_length;
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