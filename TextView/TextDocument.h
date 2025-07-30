#include <Windows.h>

class TextDocument
{
public:
	TextDocument();
	~TextDocument();

	bool init(char* filename);
	bool init(HANDLE hFile);

	bool clear();

	ULONG getline(ULONG lineno, char* buf, size_t len);
	
	ULONG longestline(int tabwidth);
	ULONG linecount();

private:
	bool init_linebuffer();

	char* m_buffer;
	ULONG m_length;
	
	ULONG m_numlines;
	ULONG* m_linebuffer;
};
