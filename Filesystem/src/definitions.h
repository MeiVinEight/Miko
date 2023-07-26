#ifndef FSDEF_H
#define FSDEF_H

#include <filesystem.h>

#define MAX_PATH					260

#define FILE_ATTRIBUTE_DIRECTORY	0x00000010
#define FILE_ATTRIBUTE_NORMAL		0x00000080

#define FILE_TYPE_DISK				0x0001

#define ERROR_FILE_NOT_FOUND		2L
#define ERROR_PATH_NOT_FOUND		3L
#define ERROR_FILE_EXISTS			80L

#define STD_INPUT_HANDLE    ((DWORD)-10)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)
#define STD_ERROR_HANDLE    ((DWORD)-12)

Memory::string cstring(const String::string &);

#endif //FSDEF_H
