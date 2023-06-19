#ifndef COMMONMESSAGEDIGEST_H
#define COMMONMESSAGEDIGEST_H

#include <memory.h>

bool Appendix32(QWORD, Memory::string &, QWORD &, void (*)(QWORD, void *, BYTE));
bool Appendix64(QWORD, Memory::string &, QWORD &, void (*)(QWORD, void *, BYTE));
void Transform32(Memory::string &, void (*)(QWORD, void *, BYTE));
void Transform64(Memory::string &, void (*)(QWORD, void *, BYTE));

#endif //COMMONMESSAGEDIGEST_H
