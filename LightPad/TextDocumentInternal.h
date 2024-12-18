#include <Windows.h>

class TextDocument
{
public:
	bool init(TCHAR* filename);
	bool init(HANDLE hFile);

	ULONG getline(ULONG lineno, char* buf, size_t len);
	ULONG linecount();

private:
	bool init_linebuffer();
	char* buffer;
	int length;
};
