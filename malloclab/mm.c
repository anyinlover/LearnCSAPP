/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "best_one",
    /* First member's full name */
    "guhangsong",
    /* First member's email address */
    "@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define WSIZE       4
#define DSIZE       8
#define CHUNKSIZE   512
#define M 100
#define DEBUGx

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word*/
#define PACK(size, prev_alloc, alloc) ((size) | (prev_alloc) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
#define SET_PREALLOC(p) (*(unsigned int *)(p) |= 0x2)
#define UNSET_PREALLOC(p) (*(unsigned int *)(p) &= ~0x2)

/* Read and write a address at address p */
#define GETP(p) (*(void **)(p))
#define PUTP(p, val) (*(void **)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_PREALLOC(p) (GET(p) & 0x2)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* Given free block ptr bp, compute address of its prev free block and next free block */
#define PREV_FP(bp) (GETP(bp))
#define NEXT_FP(bp) (GETP((char *)(bp) + WSIZE))
#define PUTP_PREV(bp, pp) (PUTP(bp, pp))
#define PUTP_NEXT(bp, np) (PUTP((char *)(bp) + WSIZE, np))

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

static char *heap_listp = 0;
static int is_realloc = 0;

static void* sp[M];

static void *extend_heap(size_t words);
static void* place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp);
static int mm_check(void);
static void checkblock(void *bp);

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    memset(sp, 0, sizeof(sp));
    /* Create the initial empty heap*/
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void*)-1)
        return -1;
    PUT(heap_listp, 0); /*Alignment padding*/
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 0, 1)); /* Prologue header */
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 0, 1)); /* Prologue footer */
    PUT(heap_listp + (3*WSIZE), PACK(0, 2, 1)); /* Epilogue header */
    heap_listp += (2*WSIZE);

    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    #ifdef DEBUG
    printf("------------ init\n");
    mm_check();
    #endif  //
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;

    /* $end mmmalloc */
    if (heap_listp == 0){
        mm_init();
    }
    /* $begin mmmalloc */
    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE + WSIZE)                                          //line:vm:mm:sizeadjust1
        asize = 2*DSIZE;                                        //line:vm:mm:sizeadjust2
    else
        asize = DSIZE * ((size + (WSIZE) + (DSIZE-1)) / DSIZE); //line:vm:mm:sizeadjust3

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {  //line:vm:mm:findfitcall
        bp = place(bp, asize);                  //line:vm:mm:findfitplace
        #ifdef DEBUG
        if (!is_realloc) {
            printf(" ------------- malloc by fit %d at %p\n", size, bp);
            mm_check();
        }
        #endif
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize,CHUNKSIZE);                 //line:vm:mm:growheap1
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;                                  //line:vm:mm:growheap2
    bp = place(bp, asize);                                 //line:vm:mm:growheap3
    #ifdef DEBUG
    if (!is_realloc) {
        printf(" ------------- malloc by extend %d at %p\n", size, bp);
        mm_check();
    }
    #endif
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
    if (bp == 0)
        return;
    size_t size = GET_SIZE(HDRP(bp));
    size_t prev_alloc = GET_PREALLOC(HDRP(bp));

    if (heap_listp == 0) {
        mm_init();
    }

    PUT(HDRP(bp), PACK(size, prev_alloc, 0));
    PUT(FTRP(bp), PACK(size, prev_alloc, 0));
    coalesce(bp);
    #ifdef DEBUG
    if (!is_realloc) {
    printf("-----------------------free %p\n", bp);
    mm_check();
    }
    #endif
}

static int find_pos(size_t size)
{
    if (size == 0) {
        printf("empty size");
        return -1;
    }
    size_t i = 4;
    int m = 0;
    while (((size-1) >> i)) {
        i++;
        m++;
        if (m == M - 1)
            break;
    }
    return m;
}

static void add_link(void *bp)
{
    int m;
    void *p, *op;
    size_t size = GET_SIZE(HDRP(bp));
    m = find_pos(size);
    if (m < 0)
        return;
    p = sp[m];
    if (p == NULL) {
        PUTP_PREV(bp, NULL);
        PUTP_NEXT(bp, NULL);
        sp[m] = bp;
    } else if (bp <= p) {
        PUTP_PREV(bp, NULL);
        PUTP_NEXT(bp, p);
        PUTP_PREV(p, bp);
        sp[m] = bp;
    } else {
        while (1) {
            op = p;
            p = NEXT_FP(p);
            if (p == NULL || p > bp) {
                PUTP_PREV(bp, op);
                PUTP_NEXT(bp, p);
                PUTP_NEXT(op, bp);
                if (p)
                    PUTP_PREV(p, bp);
                break;
            }
        }
    }
}

static void remove_link(void *bp)
{
    void *pp, *np;
    pp = PREV_FP(bp);
    np = NEXT_FP(bp);
    if (pp)
        PUTP_NEXT(pp, np);
    else {
        int m;
        size_t size = GET_SIZE(HDRP(bp));
        m = find_pos(size);
        if (m < 0)
            return;
        sp[m] = np;
    }
    if (np)
        PUTP_PREV(np, pp);
}


static void* coalesce(void *bp)
{
    size_t prev_alloc = GET_PREALLOC(HDRP(bp));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {
        ;
    }

    else if (prev_alloc && !next_alloc) {
        remove_link(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 2, 0));
        PUT(FTRP(bp), PACK(size, 2, 0));
    }

    else if (!prev_alloc && next_alloc) {
        remove_link(PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 2, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 2, 0));
        bp = PREV_BLKP(bp);
    }

    else {
        remove_link(NEXT_BLKP(bp));
        remove_link(PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 2, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 2, 0));
        bp = PREV_BLKP(bp);
    }
    add_link(bp);
    UNSET_PREALLOC(HDRP(NEXT_BLKP(bp)));
    return bp;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    is_realloc = 1;
    size_t asize;
    if (size <= DSIZE + WSIZE)                                          //line:vm:mm:sizeadjust1
        asize = 2*DSIZE;                                        //line:vm:mm:sizeadjust2
    else
        asize = DSIZE * ((size + (WSIZE) + (DSIZE-1)) / DSIZE);
    #ifdef  DEBUG
    printf("realloc asize: %d, pos: %p\n", asize, ptr);
    #endif  //DEBUG
    size_t oldsize, lsize;
    void *newptr, *nextptr;

    if (size == 0) {
        mm_free(ptr);
        return 0;
    }

    if (ptr == NULL) {
        return mm_malloc(size);
    }

    oldsize = GET_SIZE(HDRP(ptr));

    if (asize <= oldsize) {
        size_t lsize = oldsize - asize;
        size_t prev_alloc = GET_PREALLOC(HDRP(ptr));
        if (lsize >= 2 * DSIZE) {
            PUT(HDRP(ptr), PACK(asize, prev_alloc, 1));
            PUT(HDRP(NEXT_BLKP(ptr)), PACK(lsize, 2, 0));
            PUT(FTRP(NEXT_BLKP(ptr)), PACK(lsize, 2, 0));
            add_link(NEXT_BLKP(ptr));
        }
    } else {
        nextptr = NEXT_BLKP(ptr);
        if (!GET_ALLOC(HDRP(nextptr)) && GET_SIZE(HDRP(nextptr)) + oldsize >= asize) {
            remove_link(nextptr);
            lsize = GET_SIZE(HDRP(nextptr)) + oldsize - asize;
            size_t prev_alloc = GET_PREALLOC(HDRP(ptr));
            if (lsize >= 2 * DSIZE) {
                PUT(FTRP(nextptr), PACK(lsize, 2, 0));
                PUT(HDRP(ptr), PACK(asize, prev_alloc, 1));
                PUT(HDRP(NEXT_BLKP(ptr)), PACK(lsize, 2, 0));
                add_link(NEXT_BLKP(ptr));
            } else {
                PUT(HDRP(ptr), PACK(GET_SIZE(HDRP(nextptr)) + oldsize, prev_alloc, 1));
                SET_PREALLOC(HDRP(NEXT_BLKP(ptr)));
            }
        } else {
            if (!GET_SIZE(HDRP(nextptr))) {
                size_t extend_size = asize - oldsize;
                if ((long)(mem_sbrk(extend_size)) == -1)
                    return NULL;
                size_t prev_alloc = GET_PREALLOC(HDRP(ptr));
                PUT(HDRP(ptr), PACK(asize, prev_alloc, 1));
                PUT(HDRP(NEXT_BLKP(ptr)), PACK(0, 2, 1));
            } else {
                #ifdef DEBUG
                printf("------------------realloc by move %d\n", size);
                #endif
                newptr = mm_malloc(size);
                if (!newptr) {
                    return 0;
                }
                memcpy(newptr, ptr, oldsize);
                mm_free(ptr);
                #ifdef DEBUG
                mm_check();
                #endif
                is_realloc = 0;
                return newptr;
            }
        }
    }

    #ifdef DEBUG
    printf("----------------realloc by extend %d\n", size);
    mm_check();
    #endif

    is_realloc = 0;
    return ptr;
}

static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;
    size_t prev_alloc;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;


    prev_alloc = GET_PREALLOC(HDRP(bp));
    PUT(HDRP(bp), PACK(size, prev_alloc, 0));
    PUT(FTRP(bp), PACK(size, prev_alloc, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 0, 1));

    /* Coalesce if the previous block was free*/
    return coalesce(bp);
}


static void* place(void *bp, size_t asize)
{
    remove_link(bp);
    size_t lsize;
    lsize = GET_SIZE(HDRP(bp)) - asize;

    if (lsize < 2 * DSIZE) {
        PUT(HDRP(bp), PACK(GET_SIZE(HDRP(bp)), 2, 1));
        SET_PREALLOC(HDRP(NEXT_BLKP(bp)));
    } else {
        if (asize <= 64) {
            PUT(FTRP(bp), PACK(lsize, 2, 0));
            PUT(HDRP(bp), PACK(asize, 2, 1));
            PUT(HDRP(NEXT_BLKP(bp)), PACK(lsize, 2, 0));
            add_link(NEXT_BLKP(bp));
        } else {
            SET_PREALLOC(HDRP(NEXT_BLKP(bp)));
            PUT(HDRP(bp), PACK(lsize, 2, 0));
            PUT(FTRP(bp), PACK(lsize, 2, 0));
            add_link(bp);
            bp = NEXT_BLKP(bp);
            PUT(HDRP(bp), PACK(asize, 0, 1));
        }
    }
    return bp;
}


static void *find_fit(size_t asize)
{
    int m;
    m = find_pos(asize);
    if (m < 0)
        return NULL;
    void *bp;
    while (m < M) {
        bp = sp[m];
        while (bp) {
            if (GET_SIZE(HDRP(bp)) >= asize)
                return bp;
            bp = NEXT_FP(bp);
        }
        m++;
    }
    return NULL;
}

static void printblock(void *bp)
{
    size_t hsize, halloc, fsize, falloc, hpalloc, fpalloc;

    hsize = GET_SIZE(HDRP(bp));
    halloc = GET_ALLOC(HDRP(bp));
    hpalloc = GET_PREALLOC(HDRP(bp));
    if (hsize == 0) {
        printf("%p: EOL: [%d:%c:%c]\n", bp,
            hsize, (hpalloc ? 'a' : 'f'), (halloc ? 'a' : 'f'));
        return;
    }

    if (!halloc) {
        fsize = GET_SIZE(FTRP(bp));
        falloc = GET_ALLOC(FTRP(bp));
        fpalloc = GET_PREALLOC(FTRP(bp));

        printf("%p: header: [%d:%c:%c] footer: [%d:%c:%c]\n", bp,
                hsize, (hpalloc ? 'a' : 'f'), (halloc ? 'a' : 'f'),
                fsize, (fpalloc ? 'a' : 'f'), (falloc ? 'a' : 'f'));
    } else {
        printf("%p: header: [%d:%c:a]\n", bp, hsize, (hpalloc ? 'a' :
        'f'));
    }
}

static void checkblock(void *bp)
{
    if ((size_t)bp % 8)
        printf("Error: %p is not doubleword aligned\n", bp);
    if (!GET_ALLOC(HDRP(bp)) && GET(HDRP(bp)) != GET(FTRP(bp)))
        printf("Error: header does not match footr\n");
}

static void printlink(void *bp)
{
    while (bp) {
        printf("%p ", bp);
        bp = NEXT_FP(bp);
    }
    printf("NULL\n");
}

static int mm_check(void)
{
    char *bp = heap_listp;

    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
        printf("Bad prologue header\n");
    checkblock(heap_listp);

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        printblock(bp);
        checkblock(bp);
    }

    printblock(bp);
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
        printf("Bad epilogue footer %d : %d\n", GET_SIZE(HDRP(bp)), GET_ALLOC(HDRP(bp)));

    #ifdef DEBUGLINK
    for (int i = 0; i < M; i++) {
        if (i == 0)
            printf("1-16: ");
        else
            printf("%d-%d: ", (2 << (i+2)) + 1, 2 << (i+3));
        printlink(sp[i]);
    }
    #endif

    printf("-------------------------------------\n");
    return 0;
}