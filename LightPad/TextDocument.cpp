#include <Windows.h>
#include "TextDocumentInternal.h"


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

	if ((TextDocument::length = GetFileSize(hFile, 0)) == 0) { return false; }

	// allocate a new buffer for the file
	if ((TextDocument::buffer = new char[TextDocument::length]) == 0) { return false; }

	// read file into memory
	ReadFile(hFile, buffer, length, &numread, 0);

	// Work out where each line starts
	init_linebuffer();

	CloseHandle(hFile);
	return true;
}