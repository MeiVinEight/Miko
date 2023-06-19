#include "SHAC.h"

QWORD Ch(QWORD x, QWORD y, QWORD z) { return (x & y) ^ (~x & z); }
QWORD Maj(QWORD x, QWORD y, QWORD z) { return (x & y) ^ (x & z) ^ (y & z); }