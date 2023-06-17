#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#include "C_specific_handler.h"

#ifdef __cplusplus
extern "C"
{
#endif


void *__cdecl memset(void *, int, unsigned long long);
void *__cdecl memcpy(void *, void const *, unsigned long long);
int __cdecl strcmp(const char *, const char *);
int ReadInt(unsigned char **);
unsigned int ReadUnsigned(unsigned char **);
unsigned char *imageRelToByteBuffer(unsigned long long, unsigned int);


#pragma intrinsic(memset)
#pragma intrinsic(memcpy)
#pragma intrinsic(strcmp)

#pragma region STRUCTS
typedef struct FuncInfoHeader
{
	union
	{
		struct
		{
			unsigned char isCatch        : 1;  // 1 if this represents a catch funclet, 0 otherwise
			unsigned char isSeparated    : 1;  // 1 if this function has separated code segments, 0 otherwise
			unsigned char BBT            : 1;  // Flags set by Basic Block Transformations
			unsigned char UnwindMap      : 1;  // Existence of Unwind Map RVA
			unsigned char TryBlockMap    : 1;  // Existence of Try Block Map RVA
			unsigned char EHs            : 1;  // EHs flag set
			unsigned char NoExcept       : 1;  // NoExcept flag set
			unsigned char reserved       : 1;  // Have to be 0
		};
		unsigned char value;
	};
} FuncInfoHeader;
typedef struct FuncInfo
{
	FuncInfoHeader header{};
	unsigned int   bbtFlags;         // flags that may be set by BBT processing
	int            dispUnwindMap;    // Image relative offset of the unwind map
	unsigned int   dispTryBlockMap;  // Image relative offset of the handler map
	unsigned int   dispIPtoStateMap; // Image relative offset of the IP to state map
	unsigned int   dispFrame;        // displacement of address of function frame wrt establisher frame, only used for catch funclets

	FuncInfo()
	{
		header.value = 0;
		bbtFlags = 0;
		dispUnwindMap = 0;
		dispTryBlockMap = 0;
		dispIPtoStateMap = 0;
		dispFrame = 0;
	}
} FuncInfo;
typedef struct TryBlockMapEntry
{
	unsigned int tryLow;             // Lowest state index of try
	unsigned int tryHigh;            // Highest state index of try
	unsigned int catchHigh;          // Highest state index of any associated catch
	unsigned int dispHandlerArray;     // Image relative offset of list of handlers for this try
} TryBlockMapEntry;
struct HandlerTypeHeader
{
	// See contAddr for description of these values
	enum contType
	{
		NONE = 0b00,
		ONE = 0b01,
		TWO = 0b10,
		RESERVED = 0b11
	};
	union
	{
		struct
		{
			unsigned char adjectives   : 1; // Existence of Handler Type adjectives (bitfield)
			unsigned char dispType     : 1; // Existence of Image relative offset of the corresponding type descriptor
			unsigned char dispCatchObj : 1; // Existence of Displacement of catch object from base
			unsigned char contIsRVA    : 1; // Continuation addresses are RVAs rather than function relative, used for separated code
			unsigned char contAddr     : 2; // 1.   00: no continuation address in metadata, use what the catch funclet returns
			// 2.   01: one function-relative continuation address
			// 3.   10: two function-relative continuation addresses
			// 4.   11: reserved
			unsigned char unused       : 2;
		};
		unsigned char value;
	};
};
constexpr unsigned int MAX_CONT_ADDRESSES = 2;
class HandlerType
{
	public:
	HandlerTypeHeader     header{};
	unsigned int          adjectives{};                              // Handler Type adjectives (bitfield)
	unsigned int          dispType{};                                // Image relative offset of the corresponding type descriptor
	unsigned int          dispCatchObj{};                            // Displacement of catch object from base
	unsigned int          dispOfHandler{};                           // Image relative offset of 'catch' code
	unsigned long long    continuationAddress[MAX_CONT_ADDRESSES]{}; // Continuation address(es) of catch funclet
	void reset()
	{
		header.value = 0;
		adjectives = 0;
		dispType = 0;
		dispCatchObj = 0;
		dispOfHandler = 0;
		memset(continuationAddress, 0, sizeof(continuationAddress));
	}
	HandlerType()
	{
		reset();
	}
};
#pragma endregion


#ifdef __cplusplus
}
#endif

#pragma region CLASS
class TryBlockMap
{
	public:
	class iterator
	{
		public:
		TryBlockMap &tryBlockMap;
		unsigned int currBlock;
		iterator(TryBlockMap &, unsigned int);
		TryBlockMap::iterator &operator++();
		TryBlockMapEntry operator*() const;
		bool operator!=(const TryBlockMap::iterator &) const;
		bool operator<(const TryBlockMap::iterator &) const;
		TryBlockMap::iterator &operator=(const TryBlockMap::iterator &);
	};
	class IRange
	{
		public:
		TryBlockMap::iterator first;
		TryBlockMap::iterator second;
		IRange(TryBlockMap::iterator, TryBlockMap::iterator);
	};
	unsigned int numTryBlocks = 0;
	unsigned char *buffer = nullptr;
	unsigned char *bufferStart = nullptr;
	TryBlockMapEntry tryBlock = {};
	TryBlockMap(const FuncInfo *, unsigned long long);
	TryBlockMap::iterator begin();
	TryBlockMap::iterator end();
	unsigned int getNumTryBlocks() const;
	void DecompTryBlock();
};
class HandlerMap
{
	public:
	class iterator
	{
		public:
		HandlerMap &handlerMap;
		unsigned int currBlock;
		iterator(HandlerMap &, unsigned int);
		HandlerMap::iterator &operator++();
		HandlerType operator*() const;
		bool operator==(const HandlerMap::iterator &) const;
		bool operator!=(const HandlerMap::iterator &) const;
	};
	unsigned int numHandlers = 0;
	unsigned char *buffer = nullptr;
	unsigned char *bufferStart = nullptr;
	HandlerType handler;
	unsigned long long imageBase;
	unsigned int functionStart;
	HandlerMap(const TryBlockMapEntry *, unsigned long long, unsigned int);
	HandlerMap::iterator begin();
	HandlerMap::iterator end();
	void setBuffer(unsigned int);
	void resetBuffer();
	void DecompHandler();
};
#pragma endregion


#endif //EXCEPTIONHANDLER_H