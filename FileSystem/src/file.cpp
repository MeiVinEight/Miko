#include "fsdef.h"


FileSystem::File::File(const void *path): file(FileSystem::open(path, OF_READWRITE))
{
}

FileSystem::File::File(FileSystem::FD fdVal): file(fdVal)
{
}

FileSystem::File::File(FileSystem::File &&another) noexcept: file(another.file)
{
	if (~this->file)
	{
		CloseHandle((HANDLE) this->file);
	}
	this->file = another.file;
	another.file = HFILE_ERROR;
}

FileSystem::File::~File()
{
	this->close();
}

FileSystem::File &FileSystem::File::operator=(FileSystem::File &&fVal) noexcept
{
	if (this->file)
	{
		CloseHandle((HANDLE)this->file);
	}
	this->file = fVal.file;
	fVal.file = HFILE_ERROR;
	return *this;
}

DWORD FileSystem::File::read(void *b, DWORD len)
{
	if (~this->file)
	{
		return FileSystem::read(this->file, b, len);
	}
	throw Exception::exception(Exception::exception::EXTERNAL, FileSystem::FILE_CLOSED);
}

DWORD FileSystem::File::write(void *b, DWORD len)
{
	if (~this->file)
	{
		return FileSystem::write(this->file, b, len);
	}
	throw Exception::exception(Exception::exception::EXTERNAL, FileSystem::FILE_CLOSED);
}

QWORD FileSystem::File::available()
{
	if (~this->file)
	{
		HANDLE handle = (HANDLE) this->file;
		DWORD type = GetFileType(handle);
		switch (type)
		{
			case FILE_TYPE_CHAR:
			case FILE_TYPE_PIPE:
			{
				HANDLE stdInHandle = GetStdHandle(STD_INPUT_HANDLE);
				if (stdInHandle == handle)
				{
					BOOL noerr;
					DWORD events;
					noerr = GetNumberOfConsoleInputEvents(stdInHandle, &events);
					if (noerr)
					{
						DWORD bufferSize = events + sizeof(INPUT_RECORD);
						bufferSize = bufferSize ? bufferSize : 1;
						INPUT_RECORD *lpBuffer = (INPUT_RECORD *) Memory::allocate(bufferSize);
						if (lpBuffer)
						{
							DWORD eventsRead;
							noerr = PeekConsoleInput(handle, lpBuffer, events, &eventsRead);
							if (noerr)
							{
								DWORD curLength = 0;
								DWORD actLength = 0;
								for (DWORD i = 0; i < events; i++)
								{
									if (lpBuffer[i].EventType == 1)
									{
										KEY_EVENT_RECORD *keyRecord = (KEY_EVENT_RECORD *) &(lpBuffer[i].Event);
										if (keyRecord->bKeyDown == 1)
										{
											CHAR *keyPressed = (CHAR *) &(keyRecord->uChar);
											curLength++;
											if (*keyPressed == '\r')
											{
												actLength = curLength;
											}
										}
									}
								}
								Memory::free(lpBuffer);
								return actLength;
							}
							Memory::free(lpBuffer);
						}
						break;
					}
					goto NOSEEK;
				}
				else
				{
					NOSEEK:;
					DWORD ret = 0;
					if (!PeekNamedPipe(handle, NULL, 0, NULL, &ret, NULL))
					{
						DWORD err = GetLastError();
						if (err != ERROR_BROKEN_PIPE)
						{
							goto AVA_ERROR;
						}
						ret = 0;
					}
					return ret;
				}
			}
			case FILE_TYPE_DISK:
			{
				LARGE_INTEGER distance, pos;
				distance.QuadPart = 0;
				if (SetFilePointerEx(handle, distance, &pos, FILE_CURRENT))
				{
					QWORD current = pos.QuadPart;
					LARGE_INTEGER filesize;
					if (GetFileSizeEx(handle, &filesize))
					{
						return filesize.QuadPart - current;
					}
				}
				break;
			}
		}
	}
	AVA_ERROR:
	return -1;
}

void FileSystem::File::seek(QWORD offset) const
{
	FileSystem::seek(this->file, offset, FILE_BEGIN);
}

void FileSystem::File::close()
{
	if (~this->file)
	{
		FileSystem::close(this->file);
		this->file = HFILE_ERROR;
	}
}