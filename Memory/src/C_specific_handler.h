#ifndef C_SPECIFIC_HANDLER_H
#define C_SPECIFIC_HANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum _EXCEPTION_DISPOSITION
{
	ExceptionContinueExecution,
	ExceptionContinueSearch,
	ExceptionNestedException,
	ExceptionCollidedUnwind
} EXCEPTION_DISPOSITION;

typedef EXCEPTION_DISPOSITION (__stdcall *C_SPECIFIC_HANDLER)(void *, void *, void *, void *);
extern C_SPECIFIC_HANDLER NTDLL___C_specific_handler;


#ifdef __cplusplus
}
#endif

#endif //C_SPECIFIC_HANDLER_H
