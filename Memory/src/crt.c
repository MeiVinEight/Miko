void *heap = 0;

#ifndef CMAKE_BUILD

void *__stdcall HeapAlloc(void *, unsigned long, unsigned long long);
void *__stdcall HeapReAlloc(void *, unsigned long, void *, unsigned long long);
int __stdcall HeapFree(void *, unsigned long, void *);
void __stdcall ExitProcess(unsigned int);

typedef struct ExitNode
{
	void (*onexit)(void);
	struct ExitNode *next;
} ExitNode;

ExitNode *exittable = 0;

__declspec(dllexport) void *__cdecl malloc(unsigned long long size)
{
	size += !size;
	return HeapAlloc(heap, 0, size);
}
__declspec(dllexport) void __cdecl free(void *block)
{
	if (block)
	{
		HeapFree(heap, 0, block);
	}
}
__declspec(dllexport) void *__cdecl realloc(void *block, unsigned long long size)
{
	if (!block)
		return malloc(size);
	if (!size)
	{
		free(block);
		return 0;
	}
	return HeapReAlloc(heap, 0, block, size);
}
__declspec(dllexport) int __cdecl _purecall()
{
	ExitProcess(3); // abort
	return 0;
}
__declspec(dllexport) int __cdecl atexit(void (*exit)(void))
{
	if (exit)
	{
		ExitNode *node = (ExitNode *) malloc(sizeof(ExitNode));
		if (node)
		{
			node->onexit = exit;
			node->next = exittable;
			exittable = node;
			return 1;
		}
	}
	return 0;
}
void onexit(void)
{
	ExitNode *node = exittable;
	while (node)
	{
		node->onexit();
		ExitNode *next = node->next;
		free(node);
		node = next;
	}
	exittable = 0;
}

#endif