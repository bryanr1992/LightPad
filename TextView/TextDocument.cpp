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

// Init method for the text doc
bool TextDocument::init(char* filename)
{
	HANDLE hFile;
	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (hFile == INVALID_HANDLE_VALUE) { return false; }

	return init(hFile);
}

// Helper function that uses the HANDLE returned by CreateFile
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

//Takes a line numer to retrieve
ULONG TextDocument::getline(ULONG lineno, char* buf, size_t len)
{
	char* lineptr;
	ULONG linelen;

	//locate the start of the specified line
	lineptr = m_buffer + m_linebuffer[lineno];

	//find the lenght of the line by subtracting "how long the current line is" 
	//we look that up in the next position of our line buffer as the next position will have the
	//value of where the previous line (the line we want to find the length of) finishes
	// minus the value of the start of the current line
	//TLDR subtract next line's offset from the current line's
	linelen = m_linebuffer[lineno + 1] - m_linebuffer[lineno];

	//Make sure we do not overflow caller's buffer
	linelen = min(len, linelen);

	memcpy(buf, lineptr, linelen);

	return linelen;
}

/*
* Return number of lines
*/

ULONG TextDocument::linecount()
{
	return m_numlines;
}

/*
* Return length of longest line
*/

ULONG TextDocument::longestline(int tabwidth)
{
	ULONG i;
	ULONG xpos = 0;
	ULONG longest = 0;

	// use linebuffer to work out which line is the longest line

	for (i = 0; i < m_length; i++)
	{
		if (m_buffer[i] == '\r')
		{
			if (m_buffer[i + 1] == '\n') { i++; }

			longest = max(longest, xpos);
			xpos = 0;
		}
		else if (m_buffer[i] == '\t')
		{
			xpos += tabwidth - (xpos % tabwidth);
		}
		else {
			xpos++;
		}

	}

	longest = max(longest, xpos);
	return longest;
}

/*
* For now only used as class destructor
*
*/
bool TextDocument::clear()
{
	if (m_buffer)
	{
		delete m_buffer;
		m_length = 0;
		m_buffer = 0;
	}

	if (m_linebuffer)
	{
		delete m_linebuffer;
		m_linebuffer = 0;
		m_numlines = 0;
	}

	return true;
	

	return true;
}