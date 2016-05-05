#ifndef ASSERT 
//! ASSERT macro for debugging. ATMEL specific.
#ifndef RELEASE
#define ASSERT(x) if(!(x)) asm("break")
#else
#define ASSERT(x) if(!(x)) asm("jmp 0")
#endif
#endif
