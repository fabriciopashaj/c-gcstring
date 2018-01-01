#ifndef _C_GCSTRING_H_
#define _C_GCSTRING_H_
#include <c-string/lib.h>

typedef String* GCString;
// typedef struct StringGC StringGC;
#define GCString_from(str) (StringGC_register(String_from(str)))

GCString StringGC_register(String);
size_t   GCString_use(GCString);
size_t   GCString_dispose(GCString);

#endif /* _C_GCSTRING_H_ */
