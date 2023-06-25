#ifndef CMAKE_BUILD

#include "ExceptionHandler.h"
#include "definitions.h"
#include "crt.h"

extern "C"
{

#pragma region MACROS

#define STATUS_UNWIND_CONSOLIDATE        (0x80000029UL)
#define STATUS_ACCESS_VIOLATION          (0xC0000005UL)

#define EXCEPTION_NONCONTINUABLE      0x01        // Noncontinuable exception
#define EXCEPTION_UNWINDING           0x02        // Unwind is in progress
#define EXCEPTION_EXIT_UNWIND         0x04        // Exit unwind is in progress
#define EXCEPTION_STACK_INVALID       0x08        // Stack out of limits or unaligned
#define EXCEPTION_NESTED_CALL         0x10        // Nested exception handler call
#define EXCEPTION_TARGET_UNWIND       0x20        // Target unwind in progress
#define EXCEPTION_COLLIDED_UNWIND     0x40        // Collided exception handler call
#define EXCEPTION_SOFTWARE_ORIGINATE  0x80        // Exception originated in software
#define EXCEPTION_UNWIND (EXCEPTION_UNWINDING | EXCEPTION_EXIT_UNWIND | EXCEPTION_TARGET_UNWIND | EXCEPTION_COLLIDED_UNWIND)

#define TI_IsConst          0x00000001 // thrown object has const qualifier
#define TI_IsVolatile       0x00000002 // thrown object has volatile qualifier
#define TI_IsUnaligned      0x00000004 // thrown object has unaligned qualifier
#define TI_IsPure           0x00000008 // object thrown from a pure module
#define TI_IsWinRT          0x00000010 // object thrown is a WinRT Exception

#define CT_IsSimpleType     0x00000001 // type is a simple type
#define CT_ByReferenceOnly  0x00000002 // type must be caught by reference
#define CT_HasVirtualBase   0x00000004 // type is a class with virtual bases
#define CT_IsWinRTHandle    0x00000008 // type is a winrt handle
#define CT_IsStdBadAlloc    0x00000010 // type is a std::bad_alloc

#define HT_IsConst          0x00000001 // type referenced is 'const' qualified
#define HT_IsVolatile       0x00000002 // type referenced is 'volatile' qualified
#define HT_IsUnaligned      0x00000004 // type referenced is 'unaligned' qualified
#define HT_IsReference      0x00000008 // catch type is by reference
#define HT_IsResumable      0x00000010 // the catch may choose to resume (Reserved)
#define HT_IsStdDotDot      0x00000040 // the catch is std C++ catch(...) which is supposed to catch only C++ exceptions
#define HT_IsBadAllocCompat 0x00000080 // the WinRT type can catch a std::bad_alloc
#define HT_IsComplusEh      0x80000000 // Is handling within complus EH

#define EXCEPTION_EXECUTE_HANDLER      1
#define EXCEPTION_CONTINUE_SEARCH      0
#define EXCEPTION_CONTINUE_EXECUTION (-1)

#define EXCEPTION_MAXIMUM_PARAMETERS 15 // maximum number of exception parameters
#define EH_MAGIC_NUMBER1        0x19930520
#define EH_MAGIC_NUMBER3        0x19930522
#define EH_EXCEPTION_NUMBER (0xE06D7363) // The NT Exception # that we use
#define EH_EMPTY_STATE (-1)

#define SCOPE_INDEX_SHIFT_VALUE 2

#pragma endregion

#pragma region TYPEDEF
typedef void (__stdcall *PFNPREPARE_FOR_THROW)(void *);
typedef unsigned long long EHRegistrationNode;
typedef PVOID (*EXCEPTION_HANDLER)(void *, EHRegistrationNode, ULONG, QWORD);
#pragma endregion

#pragma region STRUCTS
typedef const struct _s_ThrowInfo
{
	unsigned int attributes;            // Throw Info attributes (Bit field)
	int          pmfnUnwind;            // Destructor to call when exception has been handled or aborted
	int          pForwardCompat;        // Image relative offset of Forward compatibility frame handler
	int          pCatchableTypeArray;   // Image relative offset of CatchableTypeArray
} ThrowInfo;

#ifndef COMPILE
typedef ThrowInfo _ThrowInfo;
#endif

typedef struct WinRTExceptionInfo
{
	void*                description;
	void*                restrictedErrorString;
	void*                restrictedErrorReference;
	void*                capabilitySid;
	long                 hr;
	void*                restrictedInfo;
	ThrowInfo*           throwInfo;
	unsigned int         size;
	PFNPREPARE_FOR_THROW PrepareThrow;
} WINRTEXCEPTIONINFO;
typedef struct _EXCEPTION_RECORD
{
	DWORD ExceptionCode;
	DWORD ExceptionFlags;
	struct _EXCEPTION_RECORD *ExceptionRecord;
	PVOID ExceptionAddress;
	DWORD NumberParameters;
	ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD;
typedef struct EHExceptionRecord
{
	unsigned long     ExceptionCode;    // The code of this exception. (= EH_EXCEPTION_NUMBER)
	unsigned long     ExceptionFlags;   // Flags determined by NT
	EXCEPTION_RECORD *ExceptionRecord;  // An extra exception record (not used)
	void *            ExceptionAddress; // Address at which exception occurred
	unsigned long     NumberParameters; // Number of extended parameters. (= EH_EXCEPTION_PARAMETERS)
	struct EHParameters
	{
		unsigned long magicNumber;          // = EH_MAGIC_NUMBER1
		void *        pExceptionObject;     // Pointer to the actual object thrown
		ThrowInfo*    pThrowInfo;	        // Description of thrown object
		void *        pThrowImageBase;      // Image base of thrown object
		QWORD         tryDisplacement;      // Try displacement for block to skip
		QWORD         tryBlock;             // Try blocks to skip
	} params;
} EHExceptionRecord;
typedef struct _IMAGE_RUNTIME_FUNCTION_ENTRY
{
	DWORD BeginAddress;
	DWORD EndAddress;
	union
	{
		DWORD UnwindInfoAddress;
		DWORD UnwindData;
	};
} _IMAGE_RUNTIME_FUNCTION_ENTRY, *_PIMAGE_RUNTIME_FUNCTION_ENTRY;
typedef struct _IMAGE_RUNTIME_FUNCTION_ENTRY RUNTIME_FUNCTION, *PRUNTIME_FUNCTION;
typedef struct _xDISPATCHER_CONTEXT
{
	unsigned long long ControlPc;
	unsigned long long ImageBase;
	PRUNTIME_FUNCTION  FunctionEntry;
	ULONG_PTR          EstablisherFrame;
	QWORD              TargetIp;
	void              *ContextRecord;
	PVOID              LanguageHandler;
	PVOID              HandlerData;
	void              *HistoryTable;
	DWORD              ScopeIndex;
} DispatcherContext;
//
// PMD - Pointer to Member Data: Generalized pointer-to-member descriptor
//
typedef struct PMD
{
	int	mdisp;	// Offset of intended data within base
	int	pdisp;	// Displacement to virtual base pointer
	int	vdisp;	// Index within vbTable to offset of base
} PMD;
typedef struct EXCEPTION_CONTEXT
{
	EHExceptionRecord  *pExcept        = nullptr;
	EHRegistrationNode  RN             = 0;
	void               *pContext       = nullptr;
	DispatcherContext  *pDC            = nullptr;
	FuncInfo           *pFuncInfo      = nullptr;
	QWORD               ImageBase      = 0;
	QWORD               ThrowImageBase = 0;
} EXCEPTION_CONTEXT;
typedef struct _EXCEPTION_POINTERS
{
	PEXCEPTION_RECORD ExceptionRecord;
	void *ContextRecord;
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
typedef const struct _s_CatchableType
{
	DWORD properties;
	int pType;
	PMD thisDisplacement;
	int sizeOrOffset;
	unsigned int copyFunction;
} CatchableType;
typedef struct TypeDescriptor
{
	const void *pVFTable;
	void *spare;
	char name[];
} TypeDescriptor;
typedef const struct _s_CatchableTypeArray
{
	int nCatchableTypes;
	int arrayOfCatchableTypes[];
} CatchableTypeArray;
#pragma endregion

#pragma region GLOBALVAR
constexpr signed char s_negLengthTab[16] = {
	-1,    // 0
	-2,    // 1
	-1,    // 2
	-3,    // 3

	-1,    // 4
	-2,    // 5
	-1,    // 6
	-4,    // 7

	-1,    // 8
	-2,    // 9
	-1,    // 10
	-3,    // 11

	-1,    // 12
	-2,    // 13
	-1,    // 14
	-5,    // 15
};
constexpr BYTE s_shiftTab[16] = {
	32 - 7 * 1,    // 0
	32 - 7 * 2,    // 1
	32 - 7 * 1,    // 2
	32 - 7 * 3,    // 3

	32 - 7 * 1,    // 4
	32 - 7 * 2,    // 5
	32 - 7 * 1,    // 6
	32 - 7 * 4,    // 7

	32 - 7 * 1,    // 8
	32 - 7 * 2,    // 9
	32 - 7 * 1,    // 10
	32 - 7 * 3,    // 11

	32 - 7 * 1,    // 12
	32 - 7 * 2,    // 13
	32 - 7 * 1,    // 14
	0,             // 15
};
#pragma endregion

#pragma region KERNELFUN
void __stdcall RaiseException(DWORD, DWORD, DWORD, void *);
void *__stdcall RtlPcToFileHeader(void *, void **);
void __stdcall RtlUnwindEx(void *, void *, void *, void *, void *, void *);
#pragma endregion

#pragma region FUNCTION
unsigned long __cdecl _exception_code(void);
void *        __cdecl _exception_info(void);
int           __cdecl _abnormal_termination(void);
bool MSVCEH(EHExceptionRecord *pExcept)
{
	return pExcept && (pExcept->ExceptionCode == EH_EXCEPTION_NUMBER) && (pExcept->params.magicNumber == EH_MAGIC_NUMBER1);
}
int ReadInt(BYTE **buffer)
{
	int value = *(reinterpret_cast<int *>(*buffer));
	*buffer += sizeof(int);
	return value;
}
// TRANSITION: make sure our overflow read is covered by xdata$aa so if we
// end up reading the first entry we don't go over a section.
unsigned int ReadUnsigned(BYTE **pbEncoding)
{
	unsigned int lengthBits = **pbEncoding & 0x0F;

	signed char negLength = s_negLengthTab[lengthBits];
	unsigned int shift = s_shiftTab[lengthBits];
	unsigned int result = *(reinterpret_cast<unsigned int *>(*pbEncoding - negLength - 4));

	result >>= shift;
	*pbEncoding -= negLength;

	return result;
}
unsigned char *imageRelToByteBuffer(unsigned long long imageBase, unsigned int disp)
{
	return reinterpret_cast<unsigned char *>(imageBase + disp);
}
unsigned long long RVA2RO(DispatcherContext *pDC, DWORD RVA)
{
	return pDC->ImageBase + RVA;
}
unsigned long long FuncRelToRealOffset(DispatcherContext *pDC, unsigned int funcRelOffset)
{
	return (unsigned long long) (pDC->ImageBase + pDC->FunctionEntry->BeginAddress + funcRelOffset);
}
unsigned long long DecompFuncInfo(BYTE *buffer, FuncInfo &FuncInfoDe, QWORD imageBase, DWORD functionStart)
{
	BYTE *buffer_start = buffer;
	FuncInfoDe.header.value = buffer[0];
	++buffer;

	if (FuncInfoDe.header.BBT)
	{
		FuncInfoDe.bbtFlags = ReadUnsigned(&buffer);
	}

	if (FuncInfoDe.header.UnwindMap)
	{
		FuncInfoDe.dispUnwindMap = ReadInt(&buffer);
	}

	if (FuncInfoDe.header.TryBlockMap)
	{
		FuncInfoDe.dispTryBlockMap = ReadInt(&buffer);
	}

	// Find the correct one if this is a separated segment
	if (FuncInfoDe.header.isSeparated)
	{
		int dispToSegMap = ReadInt(&buffer);
		unsigned char *segBuffer = imageRelToByteBuffer(imageBase, dispToSegMap);
		unsigned int numSegEntries = ReadUnsigned(&segBuffer);

		// By default, an entry not found in the table corresponds to no
		// states associated with the segment
		FuncInfoDe.dispIPtoStateMap = 0;
		for (unsigned int i = 0; i < numSegEntries; i++)
		{
			int segRVA = ReadInt(&segBuffer);
			int dispSegTable = ReadInt(&segBuffer);
			if (segRVA == functionStart)
			{
				FuncInfoDe.dispIPtoStateMap = dispSegTable;
				break;
			}
		}
	}
	// Otherwise, the table is directly encoded in the function info
	else
	{
		FuncInfoDe.dispIPtoStateMap = ReadInt(&buffer);
	}

	if (FuncInfoDe.header.isCatch)
	{
		FuncInfoDe.dispFrame = ReadUnsigned(&buffer);
	}

	return buffer - buffer_start;
}
void *AdjustPointer(void *pThis, const PMD &pmd)
{
	char *pRet = (char *) pThis + pmd.mdisp;
	if (pmd.pdisp >= 0)
	{
		char *add = (char *) *((long long *) ((char *) pThis + pmd.pdisp));
		add += (unsigned int) pmd.vdisp;
		pRet += *((int *) add);
		pRet += pmd.pdisp;
	}
	return pRet;
}
void DestructExceptionObject(EHExceptionRecord *pExcept)
{
	if (pExcept->params.pThrowInfo)
	{
		if (pExcept->params.pThrowInfo->pmfnUnwind)
		{
			__try
			{
				void *pmfn = (void *) (((QWORD) pExcept->params.pThrowImageBase) + pExcept->params.pThrowInfo->pmfnUnwind);
				((void (*)(void *)) pmfn)(pExcept->params.pExceptionObject);
			}
			__except(EXCEPTION_CONTINUE_SEARCH)
			{
			}
		}
	}
}
int ExFilterRethrow(EXCEPTION_POINTERS *pExPtrs, EHExceptionRecord *pOldExcept, int *destruct)
{
	*destruct = false;
	EHExceptionRecord *pExcept = (EHExceptionRecord *) pExPtrs->ExceptionRecord;
	if (MSVCEH(pExcept))
	{
		if (pExcept->params.pExceptionObject != pOldExcept->params.pExceptionObject)
		{
			DestructExceptionObject(pOldExcept);
		}
		pExcept->params.tryDisplacement = pOldExcept->params.tryDisplacement;
		pExcept->params.tryBlock = pOldExcept->params.tryBlock;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
int BuildCatchObjectHelperInternal(void **pCatchBuffer, EHExceptionRecord *pExcept, HandlerType *pCatch, CatchableType *pConv)
{
	int retval = 0;
	__try
	{
		if (pCatch->adjectives & HT_IsReference)
		{
			*pCatchBuffer = pExcept->params.pExceptionObject;
			*pCatchBuffer = AdjustPointer(*pCatchBuffer, pConv->thisDisplacement);
		}
		else if (pConv->properties & CT_IsSimpleType)
		{
			memcpy(pCatchBuffer, pExcept->params.pExceptionObject, pConv->sizeOrOffset);
			if (pConv->sizeOrOffset == sizeof(void *) && *pCatchBuffer)
			{
				*pCatchBuffer = AdjustPointer(*pCatchBuffer, pConv->thisDisplacement);
			}
		}
		else
		{
			if (pConv->copyFunction)
			{
				retval = 1 + !!(pConv->properties & CT_HasVirtualBase);
			}
			else
			{
				memcpy(pCatchBuffer, AdjustPointer(pExcept->params.pExceptionObject, pConv->thisDisplacement), pConv->sizeOrOffset);
			}
		}
	}
	__except(EXCEPTION_CONTINUE_SEARCH)
	{
		// abort
	}
	return retval;
}
int StateFromIp(FuncInfo *pFuncInfo, DispatcherContext *pDC, unsigned long long Ip)
{
	DWORD indx;
	DWORD nIPMapEntry;

	if (!pFuncInfo->dispIPtoStateMap)
	{
		return EH_EMPTY_STATE;
	}

	unsigned char *buffer = (unsigned char *) RVA2RO(pDC, pFuncInfo->dispIPtoStateMap);
	nIPMapEntry = ReadUnsigned(&buffer);

	int prevState = EH_EMPTY_STATE;
	DWORD funcRelIP = 0;
	for (indx = 0; indx < nIPMapEntry; indx++)
	{
		funcRelIP += ReadUnsigned(&buffer);
		if (Ip < FuncRelToRealOffset(pDC, funcRelIP))
			break;

		prevState = (int) (ReadUnsigned(&buffer) - 1);
	}

	if (indx == 0)
	{
		return EH_EMPTY_STATE;
	}
	return prevState;
}
int StateFromControlPc(FuncInfo *pFuncInfo, DispatcherContext *pDC)
{
	unsigned long long ip = pDC->ControlPc;
	return StateFromIp(pFuncInfo, pDC, ip);
}
int GetHandlerSearchState(DispatcherContext *pDC, FuncInfo *pFuncInfo)
{
	int curState = StateFromControlPc(pFuncInfo, pDC);
	if (pDC->ScopeIndex)
	{
		curState = (int) (pDC->ScopeIndex - SCOPE_INDEX_SHIFT_VALUE);
	}
	return curState;
}
bool TypeMatch(EXCEPTION_CONTEXT *pEC, HandlerType *pCatch, CatchableType *pCatchable)
{
	ThrowInfo *pThrowInfo = pEC->pExcept->params.pThrowInfo;
	unsigned int dispType = pCatch->dispType;
	TypeDescriptor *HT_PTD = dispType ? ((TypeDescriptor *) (pEC->ImageBase + dispType)) : nullptr;
	TypeDescriptor *CT_PTD = (TypeDescriptor *) (pEC->ThrowImageBase + pCatchable->pType);
	if ((!HT_PTD) || (HT_PTD->name[0] == 0))
	{
		return true;
	}
	if ((pCatch->adjectives & HT_IsBadAllocCompat) && (pCatchable->properties & CT_IsStdBadAlloc))
	{
		return true;
	}
	if ((HT_PTD != CT_PTD) && (strcmp(HT_PTD->name, CT_PTD->name) != 0))
	{
		return false;
	}
	bool result = (!(pCatchable->properties & CT_ByReferenceOnly)) || (pCatch->adjectives & HT_IsReference);
	result &= (!(pThrowInfo->attributes & TI_IsConst)) || (pCatch->adjectives & HT_IsConst);
	result &= (!(pThrowInfo->attributes & TI_IsUnaligned)) || (pCatch->adjectives & HT_IsUnaligned);
	result &= (!(pThrowInfo->attributes & TI_IsVolatile)) || (pCatch->adjectives & HT_IsVolatile);
	return result;
}
EHRegistrationNode *GetEstablisherFrame(const EHRegistrationNode *pRN, FuncInfo *pFuncInfo, EHRegistrationNode *pEstablisher)
{
	*pEstablisher = *pRN;
	if (pFuncInfo->header.isCatch)
	{
		*pEstablisher = *(EHRegistrationNode *) (((char *) *pRN) + pFuncInfo->dispFrame);
	}
	return pEstablisher;
}
void BuildCatchObject(EXCEPTION_CONTEXT *pEC, void *pRN, HandlerType *pCatch, CatchableType *pConv)
{
	void **pCatchBuffer;
	unsigned int dispType = pCatch->dispType;
	TypeDescriptor *HT_PTD = dispType ? ((TypeDescriptor *) (pEC->ImageBase + dispType)) : nullptr;
	if (((!HT_PTD) || (!HT_PTD->name[0])) || ((!pCatch->dispCatchObj) && (!(pCatch->adjectives & HT_IsComplusEh))))
	{
		return;
	}


	if (pCatch->adjectives & HT_IsComplusEh)
	{
		pCatchBuffer = (void **) pRN;
	}
	else
	{
		pCatchBuffer = (void **) ((*((char **) pRN)) + pCatch->dispCatchObj);
	}

	__try
	{
		void (*COPYFUNC)(void *) = pConv->copyFunction ? (((void (*)(void *)) ((pEC->ThrowImageBase + pConv->copyFunction)))) : nullptr;
		void *pthat = AdjustPointer(pEC->pExcept->params.pExceptionObject, pConv->thisDisplacement);
		switch (BuildCatchObjectHelperInternal(pCatchBuffer, pEC->pExcept, pCatch, pConv))
		{
			case 1:
			{
				// CallMemberFunction1((char *) pCatchBuffer, (void *) COPYFUNC, pthat);
				((void (__thiscall *)(void *, void *)) COPYFUNC)(pCatchBuffer, pthat);
				break;
			}
			case 2:
			{
				// CallMemberFunction2((char *) pCatchBuffer, (void *) COPYFUNC, pthat, 1);
				((void (__thiscall *)(void *, void *, int)) COPYFUNC)(pCatchBuffer, pthat, 1);
				break;
			}
		}
	}
	__except(EXCEPTION_CONTINUE_SEARCH)
	{
		// abort();
	}
}
void *CxxCallCatchBlock(EXCEPTION_RECORD* pExcept)
{
	int destruct = 1;
	void *handlerAddress;
	void *continuationAddress = nullptr;
	EHRegistrationNode *pEstablisherFrame;
	EHExceptionRecord *pThisException;
	void *continuationAddresses[MAX_CONT_ADDRESSES];
	memset(continuationAddresses, 0, sizeof(continuationAddresses));
	pThisException = (EHExceptionRecord *)pExcept->ExceptionInformation[6];
	handlerAddress = (void *)pExcept->ExceptionInformation[2];
	pEstablisherFrame = (EHRegistrationNode *)pExcept->ExceptionInformation[1];

	continuationAddresses[0] = (unsigned char *) pExcept->ExceptionInformation[5];
	continuationAddresses[1] = (unsigned char *) pExcept->ExceptionInformation[9];

	__try
	{
		EXCEPTION_HANDLER handler = (EXCEPTION_HANDLER) handlerAddress;
		continuationAddress = handler(handlerAddress, *pEstablisherFrame, 0x100, EH_MAGIC_NUMBER3);

		QWORD continuationIndex = (QWORD) (continuationAddress);
		if (continuationIndex < MAX_CONT_ADDRESSES)
		{
			continuationAddress = continuationAddresses[continuationIndex];
		}
	}
	__except(ExFilterRethrow((EXCEPTION_POINTERS *) _exception_info(), pThisException, &destruct))
	{
	}
	if (destruct)
	{
		DestructExceptionObject(pThisException);
	}

	return continuationAddress;
}
void UnwindNestedFrames(EXCEPTION_CONTEXT *pEC, const EHRegistrationNode *pEstablisher, void *handler, int TargetUnwindState, HandlerType *pCatch)
{
	EXCEPTION_RECORD record = {
		STATUS_UNWIND_CONSOLIDATE,
		EXCEPTION_NONCONTINUABLE,
		nullptr,
		nullptr,
		15,
		{
			EH_MAGIC_NUMBER1,
			0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0
		}
	};
	record.ExceptionInformation[0] = (QWORD) &CxxCallCatchBlock;
	record.ExceptionInformation[1] = (QWORD) pEstablisher;
	record.ExceptionInformation[2] = (QWORD) handler;
	record.ExceptionInformation[3] = (QWORD) TargetUnwindState;
	record.ExceptionInformation[4]=  (QWORD) pEC->pContext;
	record.ExceptionInformation[5] = pCatch->continuationAddress[0] + pEC->pDC->ImageBase;
	record.ExceptionInformation[6] = (QWORD) pEC->pExcept;
	record.ExceptionInformation[8] = EH_MAGIC_NUMBER1;
	record.ExceptionInformation[9] = pCatch->continuationAddress[1] + pEC->pDC->ImageBase;
	RtlUnwindEx((void *) pEC->RN, (void *) pEC->pDC->ControlPc, &record, nullptr, pEC->pDC->ContextRecord, pEC->pDC->HistoryTable);
}
void Catch(EXCEPTION_CONTEXT *pEC, HandlerType *pCatch, CatchableType *pConv, int curState)
{
	EHRegistrationNode *pEstablisher;
	EHRegistrationNode EstablisherFramePointers;
	pEstablisher = GetEstablisherFrame(&pEC->RN, pEC->pFuncInfo, &EstablisherFramePointers);
	if (pConv)
	{
		BuildCatchObject(pEC, pEstablisher, pCatch, pConv);
	}
	UnwindNestedFrames(pEC, pEstablisher, (void *) (pEC->ImageBase + pCatch->dispOfHandler), curState, pCatch);
}
void InternalCxxFrameHandler(EXCEPTION_CONTEXT *pEC)
{
	EXCEPTION_RECORD *record = (EXCEPTION_RECORD *) pEC->pExcept;
	if (record->ExceptionCode == STATUS_UNWIND_CONSOLIDATE && record->NumberParameters == 15 && record->ExceptionInformation[8] == EH_MAGIC_NUMBER1)
		return;

	if (MSVCEH(pEC->pExcept))
	{
		TryBlockMap tryBlockMap(pEC->pFuncInfo, pEC->pDC->ImageBase);
		if (tryBlockMap.getNumTryBlocks())
		{
			int curState = GetHandlerSearchState(pEC->pDC, pEC->pFuncInfo);
			TryBlockMap::IRange range = {tryBlockMap.begin(), tryBlockMap.end()};
			for (; range.first < range.second; ++range.first)
			{
				TryBlockMapEntry tryBlock = *range.first;
				const int *ppCatchable;
				CatchableType *pCatchable;

				bool skipTryBlock = false;
				skipTryBlock |= curState < tryBlock.tryLow || tryBlock.tryHigh < curState;
				skipTryBlock |=
					pEC->pFuncInfo->dispTryBlockMap == pEC->pExcept->params.tryDisplacement &&
					range.first.currBlock < pEC->pExcept->params.tryBlock;
				if (skipTryBlock)
				{
					continue;
				}

				HandlerMap handlerMap(&tryBlock, pEC->pDC->ImageBase, pEC->pDC->FunctionEntry->BeginAddress);
				for (HandlerType handler : handlerMap)
				{
					CatchableTypeArray *typeArray = (CatchableTypeArray *) (pEC->ThrowImageBase + pEC->pExcept->params.pThrowInfo->pCatchableTypeArray);
					ppCatchable = typeArray->arrayOfCatchableTypes;
					int catchables = typeArray->nCatchableTypes;
					for (; catchables > 0; catchables--, ppCatchable++)
					{
						pCatchable = (CatchableType *) (pEC->ThrowImageBase + *ppCatchable);
						if (!TypeMatch(pEC, &handler, pCatchable))
						{
							continue;
						}
						pEC->pExcept->params.tryDisplacement = pEC->pFuncInfo->dispTryBlockMap;
						pEC->pExcept->params.tryBlock = range.first.currBlock + 1;
						Catch(pEC, &handler, pCatchable, curState);
						goto NextTryBlock;
					}
				}
				NextTryBlock:;
			}
		}
	}
}
#pragma endregion


#pragma comment(linker, "/EXPORT:_CxxThrowException")
#pragma comment(linker, "/EXPORT:__CxxFrameHandler4")
void __stdcall _CxxThrowException(void *pExceptionObject, _ThrowInfo *pThrowInfo)
{
	ThrowInfo *pTI = (ThrowInfo *) pThrowInfo;
	unsigned long long magicNumber = EH_MAGIC_NUMBER1;
	if (pTI && (pTI->attributes & TI_IsWinRT))
	{
		WINRTEXCEPTIONINFO **ppWei = *((WINRTEXCEPTIONINFO ***) pExceptionObject);
		--ppWei;
		WINRTEXCEPTIONINFO *pWei = *ppWei;
		pTI = (ThrowInfo *) pWei->throwInfo;
		pWei->PrepareThrow(ppWei);
	}

	void *throwImageBase = RtlPcToFileHeader((void *) pTI, &throwImageBase);
	if (pTI && ((pTI->attributes & TI_IsPure) || !throwImageBase))
	{
		RaiseException(STATUS_ACCESS_VIOLATION, 0, 0, nullptr);
	}

	const unsigned long long parameters[] = {
		magicNumber,
		(unsigned long long) pExceptionObject,
		(unsigned long long) pTI,
		(unsigned long long) throwImageBase,
		0,
		0
	};
	unsigned long long paraCount = sizeof(parameters) / sizeof(parameters[0]);
	RaiseException(EH_EXCEPTION_NUMBER, EXCEPTION_NONCONTINUABLE, paraCount, (void *) parameters);
}
EXCEPTION_DISPOSITION __cdecl __CxxFrameHandler4(EHExceptionRecord *pExcept, EHRegistrationNode RN, void *pContext, DispatcherContext *pDC)
{
	FuncInfo funcInfo = {};
	EXCEPTION_CONTEXT ec = {};
	ec.pExcept = pExcept;
	ec.RN = RN;
	ec.pContext = pContext;
	ec.pDC = pDC;
	ec.pFuncInfo = &funcInfo;
	ec.ImageBase = pDC->ImageBase;
	ec.ThrowImageBase = (QWORD) pExcept->params.pThrowImageBase;
	unsigned char *buffer = (unsigned char *) (ec.ImageBase + *((unsigned long *) pDC->HandlerData));

	DecompFuncInfo(buffer, funcInfo, pDC->ImageBase, pDC->FunctionEntry->BeginAddress);
	InternalCxxFrameHandler(&ec);
	return ExceptionContinueSearch;
}

}

#endif