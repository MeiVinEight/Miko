#include "definitions.h"


Filesystem::FileStream::FileStream(const void *path): file(Filesystem::open(path, OF_READWRITE))
{
}

Filesystem::FileStream::FileStream(Filesystem::FD fdVal): file(fdVal)
{
}

Filesystem::FileStream::FileStream(Filesystem::FileStream &&another) noexcept: file(another.file)
{
	if (~this->file)
	{
		CloseHandle((HANDLE) this->file);
	}
	this->file = another.file;
	another.file = HFILE_ERROR;
}

Filesystem::FileStream::~FileStream()
{
	this->close();
}

Filesystem::FileStream &Filesystem::FileStream::operator=(Filesystem::FileStream &&fVal) noexcept
{
	if (this->file)
	{
		CloseHandle((HANDLE)this->file);
	}
	this->file = fVal.file;
	fVal.file = HFILE_ERROR;
	return *this;
}

void Filesystem::FileStream::read(void *b, DWORD len)
{
	if (~this->file)
	{
		char *buf = (char *)b;
		while (len)
		{
			DWORD readed = Filesystem::read(this->file, buf, len);
			buf += readed;
			len -= readed;
		}
		return;
	}
	throw Exception::exception("File closed");
}

void Filesystem::FileStream::write(const void *b, DWORD len)
{
	if (~this->file)
	{
		char *buf = (char *)b;
		while (len)
		{
			DWORD written = Filesystem::write(this->file, buf, len);
			buf += written;
			len -= written;
		}
		return;
	}
	throw Exception::exception("File closed");
}

QWORD Filesystem::FileStream::available()
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
							noerr = PeekConsoleInputA(handle, lpBuffer, events, &eventsRead);
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

void Filesystem::FileStream::seek(QWORD offset) const
{
	Filesystem::seek(this->file, offset, FILE_BEGIN);
}

void Filesystem::FileStream::close()
{
	if (~this->file)
	{
		Filesystem::close(this->file);
		this->file = HFILE_ERROR;
	}
}