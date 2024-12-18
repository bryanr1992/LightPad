#include <Windows.h>
#include "TextDocumentInternal.h"


bool TextDocument::init(char* filename)
{
	HANDLE hFile;
	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (hFile == INVALID_HANDLE_VALUE) { return false; }

	return init(hFile);
}

bool TextDocument::init(HANDLE hfile)
{
	ULONG numread;

	if ((length = GetFileSize(hfile, 0)) == 0) { return false; }

	// allocate a new buffer for the file
	if ((buffer = new char[length]) == 0) { return false; }

	// read file into memory
	ReadFile(hFile, buffer, length, &numread, 0);

	// Work out where each line starts
	init_linebuffer();

	CloseHandle(hFile);
	return true;
}