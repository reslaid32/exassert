#ifndef ABORT_H
#define ABORT_H

#include <stdlib.h>

#ifdef _NOABORT
#define NOABORT
#endif // _NOABORT

#ifndef ABORT_DEFINED
#define ABORT_DEFINED
#ifdef NOABORT
#define abort_()
#define force_abort_() abort()
#else
#define abort_() abort()
#define force_abort_() abort()
#endif // NOABORT
#endif // ABORT_DEFINED

#endif // ABORT_H