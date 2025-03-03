/*
 * sbrk.c
 *
 * This code written July 2018 by Chuck McManis (cmcmanis@mcmanis.com)
 * it may be freely used, reused, and modified with our without accrediting the
 * author.
 * 
 * This implements a larger heap than you get by default
 * in newlib. It uses the RAM between the end of the declared
 * data and the bottom of the "largest" stack for the heap.
 *
 * The only user interface to this code is the weakly bound
 * local_heap_setup() which is called the first time malloc
 * has to get more memory for the heap. This function passes
 * pointers to its two controls, the start address and end
 * address of the heap. Once called those are not changed.
 *
 * If the user does not define local_heap_setup in their code,
 * the RAM between _ebss and _stack - MAX_STACK_SIZE is used
 * for the heap. Typically this is RAM that is internal to the
 * SoC.
 *
 * On other processors, additional ram might become available
 * after reset and some peripheral setup (like PSRAM or SDRAM
 * on parts with the FMC peripheral). In that case the user
 * can define their own function local_heap_setup and put
 * the start and end address of heap into that external RAM.
 *
 * Note that memory MUST be contiguous between the start and
 * end point of the heap. If there are gaps in that memory
 * then malloc() will hard fault when it tries to use
 * memory from the gap.
 */
#include <stdint.h>
#include <errno.h>
#include <malloc.h>

#define MAX_STACK_SIZE  1024

void local_heap_setup(uint8_t **start, uint8_t **end);

#pragma weak local_heap_setup = __local_ram

/* these are defined by the linker script */
#ifdef STM32F1
extern uint8_t _ebss, _stack;
#else
extern uint8_t __bss_end__, __stack;
#endif

static uint8_t *_cur_brk = NULL;
static uint8_t *_heap_end = NULL;

/*
 * If not overridden, this puts the heap into the left
 * over ram between the BSS section and the stack while
 * preserving MAX_STACK_SIZE bytes for the stack itself.
 */
static void
__local_ram(uint8_t **start, uint8_t **end)
{
#ifdef STM32F1
    *start = &_ebss;
    *end = (uint8_t *)(&_stack - MAX_STACK_SIZE);
#else
    *start = &__bss_end__;
    *end = (uint8_t *)(&__stack - MAX_STACK_SIZE);
#endif
}


/* prototype to make gcc happy */
void *_sbrk_r(struct _reent *, ptrdiff_t );

void *_sbrk_r(struct _reent *reent, ptrdiff_t diff)
{
    uint8_t *_old_brk;

    if (_heap_end == NULL) {
        local_heap_setup(&_cur_brk, &_heap_end);
    }

    _old_brk = _cur_brk;
    if (_cur_brk + diff > _heap_end) {
        reent->_errno = ENOMEM;
        return (void *)-1;
    }
    _cur_brk += diff;
    return _old_brk;
}

