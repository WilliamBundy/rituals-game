/* This is free and unencumbered software released into the public domain. */
/* Check the bottom of this file for the remainder of the unlicense */

/* alloc.h
 * 
 * Three custom allocators that can (hopefully) safely allocate a very large
 * amount of memory for you. Check the warnings below for an explanation
 * 
 * Version 0.1 Beta
 */

/* Author: William Bundy 
 * Written in November of 2017
 * 
 * A big thanks to @alexkelbo for helping me get this working on macOS
 *
 * williambundy.xyz
 * github.com/williambundy
 * Twitter: @William_Bundy
 */ 

/* ===========================================================================
 * WARNING -- BETA SOFTWARE
 * ===========================================================================
 * At this point, I can guarantee that most things seem to work without error.
 * I haven't pushed the limits or tested every configuration possible.
 *
 * If the application compiles but segfaults, try including the requisite
 * OS headers!
 *
 * #include <Windows.h>
 *
 * or
 *
 * #include <sys/mman.h>
 * #include <unistd.h>
 *
 * I embed sections of these headers in the document, but they may not match 
 * what your OS expects. (This is likely especially true of BSD)
 *
 * Please report all bugs to the github.
 * ===========================================================================
 *
 * Also: Warning -- Virtual Memory abuse
 *
 * The way the core allocator here works, the MemoryArena, is by reserving
 * a ton of virtual memory up front and committing out of it as necessary.
 *
 * By "a ton" I mean an amount equal to the physical amount of ram on your
 * machine. I have 16 gb in my tower, so I reserve 16 gb. 
 *
 * This is probably fine, but...
 * 
 * This might be a bad idea!
 *
 * It's also a windows-centric design, so it may work poorly on POSIX systems
 * that only have mmap. 
 *
 * I've used this quite a bit in my own code, and it's worked fine there, 
 * but you might find you have problems.
 */

/* To use this library, include it as
 * #define ALLOC_IMPLEMENTATION
 * #include "alloc.h"
 *
 * It probably only works from C right now, sorry!
 * 
 * In a single translation unit, otherwise it behaves as a header
 *
 * Only minimal documentation is available right now, but check line 317
 * for some details.
 *
 * There is also a short demo at the bottom of the file.
 *
 * Options:
 *
 * #define ALLOC_API
 * #define ALLOC_BACKEND_API
 * This will apply to all functions in the library. By default, without
 * ALLOC_IMPLEMENTATION, it is extern, and with it, it is blank
 * The internal virtual memory wrapping functions are defined with 
 * ALLOC_BACKEND_API, which defaults to ALLOC_API.
 *
 * #ifndef ALLOC_CUSTOM_INTEGER_TYPES
 * The library uses usize, isize, and isize. These are typedef'd
 * size_t, ptrdiff_t, and int, respectively. Typedef these on your own and 
 * define ALLOC_CUSTOM_INTEGERTYPES to ignore them.
 *
 * #define ALLOC_STACK_PTR usize
 * #define ALLOC_EXTENDED_INFO isize
 * These are options for modes of MemoryArena. By default they are 8 byte
 * integers.
 *
 * #define ALLOC_MEMSET memset
 * #define ALLOC_MEMCPY memcpy
 * alloc uses memset and memcpy. If neither of these are present, the 
 * library includes string.h. You may define your own, and it will not.
 *
 * #define ALLOC_TAGGEDHEAP_MAX_TAG_COUNT 64
 * This defines the total number of tags available to a tagged heap. If you 
 * need more than 64, or far fewer, redefine it as you need.
 *
 * #define ALLOC_NO_ZERO_ON_INIT
 * Whenever you call allocatorInit(allocator*, ...) we zero the pointer 
 * you give, unless this flag is set.
 *
 * #define ALLOC_CPLUSPLUS_FEATURES
 * If you are using C++, there are some "features" of C that are not available,
 * first and foremost, automatic void* coercion to other pointer types. To save 
 * your fingers, I've added templated versions of the regular allocation 
 * functions, which use the provided type to determine the size and return
 * type. Too, you can use arenaPush<MyType, 100>(arena) to allocate room for
 * 100 objects (this exists on a few, search for ALLOC_CPLUSPLUS_FEATURES
 * to find the actual prototypes).
 */

/* Things I've borrowed that influence this:
 * http://blog.nervus.org/managing-virtual-address-spaces-with-mmap/
 * Jason Gregory's trick where you embed the free list into the pool slots!
 * That Naughty Dog GDC talk about fibers where he mentions the tagged heap
 */

/*
 * Roadmap:
 * 	- Testing
 * 	- C++ integration features
 * 	- Fixed-size only version that doesn't define anything to do 
 * 		with virtual memory
 * 	- A simple malloc-backed memory arena for projects that really don't
 * 		need anything special at all
 */

#ifndef ALLOC_NO_DISABLE_STUPID_MSVC_WARNINGS
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:201 204 28 244 706)
#endif
#endif


#ifndef ALLOC_ERROR_HANDLER
#define ALLOC_ERROR_HANDLER(message, object, name) fprintf(stderr, \
		"wbAlloc error: [%s] %s\n", name, message)
#endif

#ifdef ALLOC_CPLUSPLUS_FEATURES
#ifndef __cplusplus
#undef ALLOC_CPLUSPLUS_FEATURES
#endif
#endif

#if !(defined(ALLOC_POSIX) || defined(ALLOC_WINDOWS))
#ifdef _MSC_VER
#define ALLOC_WINDOWS 
#endif
#ifdef __unix__
#define ALLOC_POSIX
#endif
#ifdef __APPLE__
#define ALLOC_POSIX
#endif
#endif 

#ifndef ALLOC_API
#ifdef ALLOC_IMPLEMENTATION
#define ALLOC_API 
#else
#define ALLOC_API extern
#endif
#endif

#ifndef ALLOC_BACKEND_API
#define ALLOC_BACKEND_API ALLOC_API
#endif


#ifndef ALLOC_CUSTOM_INTEGER_TYPES
#include <stddef.h>
typedef size_t usize;
typedef ptrdiff_t isize;
typedef isize isize;
#endif


#ifndef ALLOC_STACK_PTR
#define ALLOC_STACK_PTR usize
#endif

#ifndef ALLOC_EXTENDED_INFO
#define ALLOC_EXTENDED_INFO isize
#endif

#if !defined(ALLOC_MEMSET) && !defined(ALLOC_MEMCPY)
/* NOTE(will): if the user hasn't provided their own functions, we want
 * 	to use the CRT ones automatically, even if they don't have string.h
 * 	included
 */
#include <string.h>
#endif

#ifndef ALLOC_MEMSET
#define ALLOC_MEMSET memset
#endif

#ifndef ALLOC_MEMCPY
#define ALLOC_MEMCPY memcpy
#endif

#ifndef ALLOC_TAGGEDHEAP_MAX_TAG_COUNT
/* NOTE(will): if you listen to the Naughty Dog talk the tagged heap is based 
 * on, it seems like they only have ~4 tags? Something like "game", "render",
 * "physics", "anim", kinda thing. 64 should be way more than enough if that
 * is the average use case
 */
#define ALLOC_TAGGEDHEAP_MAX_TAG_COUNT 64
#endif

#define CalcKilobytes(x) (((usize)x) * 1024)
#define CalcMegabytes(x) (CalcKilobytes((usize)x) * 1024)
#define CalcGigabytes(x) (CalcMegabytes((usize)x) * 1024)

/* These are equivalent to the PROT_READ values and friends */
#define None 0
#define Read 1
#define Write 2
#define Execute 4

#define FlagArenaNormal 0
#define FlagArenaFixedSize 1
#define FlagArenaStack 2
#define FlagArenaExtended 4
#define FlagArenaNoZeroMemory 8
#define FlagArenaNoRecommit 16 

#define FlagPoolNormal 0
#define FlagPoolFixedSize 1
#define FlagPoolCompacting 2
#define FlagPoolNoZeroMemory 4
#define FlagPoolNoDoubleFreeCheck 8

#define FlagTaggedHeapNormal 0
#define FlagTaggedHeapFixedSize 1
#define FlagTaggedHeapNoZeroMemory 2
#define FlagTaggedHeapNoSetCommitSize 4
#define FlagTaggedHeapSearchForBestFit 8
#define wbi__TaggedHeapSearchSize 8

/* Struct Definitions */

typedef struct MemoryInfo MemoryInfo;
struct MemoryInfo
{
	usize totalMemory, commitSize, pageSize;
	isize commitFlags;
};

typedef struct MemoryArena MemoryArena;
struct MemoryArena
{
	const char* name;
	void *start, *head, *end;
	void *tempStart, *tempHead;
	MemoryInfo info;
	isize align;
	isize flags;
};

typedef struct MemoryPool MemoryPool;
struct MemoryPool
{
	usize elementSize; 
	isize count, capacity;
	void* slots;
	const char* name;
	void** freeList;
	MemoryArena* alloc;
	isize lastFilled;
	isize flags;
};

typedef struct wbi__TaggedHeapArena wbi__TaggedHeapArena;
struct wbi__TaggedHeapArena
{
	isize tag;
	wbi__TaggedHeapArena *next;
	void *head, *end;
	char buffer;
};

typedef struct TaggedHeap TaggedHeap;
struct TaggedHeap
{
	const char* name;
	MemoryPool pool;
	wbi__TaggedHeapArena* arenas[ALLOC_TAGGEDHEAP_MAX_TAG_COUNT];
	MemoryInfo info;
	usize arenaSize, align;
	isize flags;
};

/* Function Prototypes */

/* arenaPush and arenaPushEx increment the head pointer of the provided
 * arena and return the old one. It is safe to write information within
 * the size provided to the function.
 *
 * The Ex version also takes a ALLOC_EXTENDED_INFO (defaults to a ptrdiff_t-
 * sized int; you may wish to redefine it before including the file), which, 
 * if you're using an arena with the ArenaExtended flag enabled, will store
 * that information before your allocation.
 */ 

ALLOC_API 
void* arenaPushEx(MemoryArena* arena, 
		isize size, 
		ALLOC_EXTENDED_INFO extended);

ALLOC_API 
void* arenaPush(MemoryArena* arena, isize size);

/* poolRetrieve gets the next element out of the pool. If the pool hasn't
 * been used yet, it simply pulls the next item out at the correct location.
 * Otherwise, it checks a free list of empty slots.
 *
 * poolRelease attaches the pointer to the free list. By default, it also
 * checks whether the pointer is already on the free list, to help prevent
 * double-free bugs. Use the PoolNoDoubleFreeCheck flag to disable this.
 *
 * If your pool is compacting (PoolCompacting flag), poolRelease will instead
 * copy the last element of the pool into the slot pointed at by ptr.
 * TODO(will): uh, maybe do some safety checks there...
 * This means that you can treat the pool's slots field as an array of your
 * struct/union and iterate over it without expecting holes; however, any 
 * retrieve/release operations can invalidate your pointers
 */
ALLOC_API 
void* poolRetrieve(MemoryPool* pool);
ALLOC_API 
void poolRelease(MemoryPool* pool, void* ptr);

/* taggedAlloc behaves much like arenaPush, returning a pointer to a segment
 * of memory that is safe to write to. However, you cannot allocate more than
 * the arenaSize field of the heap at once; eg: if arenaSize is 1 megabyte, 
 * you cannot allocate anything larger than that in that heap, ever. 
 *
 * Internally, a tagged heap is a memory pool of arenas (simlified ones rather
 * than a full MemoryArena). It, by default, selects the first arena in the
 * list, but you can set the TaggedHeapSearchForBestFit flag, which will search
 * for (the first eight or so) arenas that can fit the object, then put it into
 * the one with the smallest space remaining. 
 *
 * taggedFree allows you to free all allocations on a single tag at once. 
 * If you do not specify TaggedHeapNoZeroMemory, it will also memset everything
 * to zero.
 *
 */
ALLOC_API 
void* taggedAlloc(TaggedHeap* heap, isize tag, usize size);
ALLOC_API 
void taggedFree(TaggedHeap* heap, isize tag);

#ifdef ALLOC_CPLUSPLUS_FEATURES
template<typename T, int n>
ALLOC_API 
T* arenaPushEx(MemoryArena* arena, 
		ALLOC_EXTENDED_INFO extended);

template<typename T, int n>
ALLOC_API 
T* arenaPush(MemoryArena* arena);

template<typename T>
ALLOC_API 
T* arenaPush(MemoryArena* arena, isize n = 1);

template<typename T>
ALLOC_API 
T* poolRetrieve(MemoryPool* pool);

template<typename T>
ALLOC_API 
void poolRelease(MemoryPool* pool, T* ptr);


template<typename T>
ALLOC_API 
void poolInit(MemoryPool* pool, MemoryArena* alloc, 
		isize flags);

template<typename T>
ALLOC_API 
MemoryPool* poolBootstrap(MemoryInfo info,
		isize flags);

template<typename T>
ALLOC_API 
MemoryPool* poolFixedSizeBootstrap(
		void* buffer, usize size,
		isize flags);


template<typename T, int n = 1>
ALLOC_API 
T* taggedAlloc(TaggedHeap* heap, isize tag);
#endif


/* TODO(will): Write per-function documentation */

ALLOC_BACKEND_API void* wbi__allocateVirtualSpace(usize size);
ALLOC_BACKEND_API void* wbi__commitMemory(void* addr, usize size, 
		isize flags);
ALLOC_BACKEND_API void wbi__decommitMemory(void* addr, usize size);
ALLOC_BACKEND_API void wbi__freeAddressSpace(void* addr, usize size);
ALLOC_BACKEND_API MemoryInfo getMemoryInfo();

ALLOC_API 
isize alignTo(usize x, usize align);

ALLOC_API 
void arenaInit(MemoryArena* arena, MemoryInfo info, isize flags);
ALLOC_API 
MemoryArena* arenaBootstrap(MemoryInfo info, isize flags);

ALLOC_API 
void arenaFixedSizeInit(MemoryArena* arena, 
		void* buffer, isize size,
		isize flags);
ALLOC_API 
MemoryArena* arenaFixedSizeBootstrap(
		void* buffer, usize size,
		isize flags);


ALLOC_API 
void arenaPop(MemoryArena* arena);

ALLOC_API 
void arenaStartTemp(MemoryArena* arena);
ALLOC_API 
void arenaEndTemp(MemoryArena* arena);

ALLOC_API 
void arenaClear(MemoryArena* arena);
ALLOC_API 
void arenaDestroy(MemoryArena* arena);


ALLOC_API 
void poolInit(
		MemoryPool* pool,
		MemoryArena* alloc, 
		usize elementSize, 
		isize flags);

ALLOC_API 
MemoryPool* poolBootstrap(
		MemoryInfo info,
		isize elementSize, 
		isize flags);

ALLOC_API 
MemoryPool* poolFixedSizeBootstrap(
		isize elementSize, 
		void* buffer, usize size,
		isize flags);


ALLOC_API isize calcTaggedHeapSize(
		isize arenaSize, isize arenaCount, 
		isize bootstrapped);

ALLOC_API 
void taggedInit(
		TaggedHeap* heap,
		MemoryArena* arena,
		isize internalArenaSize, 
		isize flags);

ALLOC_API 
TaggedHeap* taggedBootstrap(MemoryInfo info, 
		isize arenaSize, 
		isize flags);

ALLOC_API 
TaggedHeap* taggedFixedSizeBootstrap(isize arenaSize, 
		void* buffer, isize bufferSize, 
		isize flags);

ALLOC_API 
void wbi__taggedArenaInit(TaggedHeap* heap, 
		wbi__TaggedHeapArena* arena, 
		isize tag);

ALLOC_API 
void wbi__taggedArenaSortBySize(wbi__TaggedHeapArena** array, isize count);


/* Platform-Specific Code */

#ifndef ALLOC_CUSTOM_BACKEND

#ifdef __cplusplus
#define wbi__SystemExtern extern "C"
#else
#define wbi__SystemExtern extern
#endif

#ifdef ALLOC_WINDOWS
#ifndef _WINDOWS_
#ifndef WINAPI
#define WINAPI __stdcall 
#endif
#ifndef _In_
#define _In_
#define _Out_
#define _In_opt_
#endif
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef DWORD* DWORD_PTR;
typedef void* LPVOID;
typedef int BOOL;
typedef unsigned long long* PULONGLONG;

typedef struct _SYSTEM_INFO {
  union {
    DWORD  dwOemId;
    struct {
      WORD wProcessorArchitecture;
      WORD wReserved;
    };
  };
  DWORD     dwPageSize;
  LPVOID    lpMinimumApplicationAddress;
  LPVOID    lpMaximumApplicationAddress;
  DWORD_PTR dwActiveProcessorMask;
  DWORD     dwNumberOfProcessors;
  DWORD     dwProcessorType;
  DWORD     dwAllocationGranularity;
  WORD      wProcessorLevel;
  WORD      wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;

wbi__SystemExtern
DWORD WINAPI GetLastError(void);

wbi__SystemExtern
void WINAPI GetSystemInfo(
  _Out_ LPSYSTEM_INFO lpSystemInfo
);

wbi__SystemExtern
BOOL WINAPI GetPhysicallyInstalledSystemMemory(
  _Out_ PULONGLONG TotalMemoryInKilobytes
);

wbi__SystemExtern
LPVOID WINAPI VirtualAlloc(
  _In_opt_ LPVOID lpAddress,
  _In_     usize dwSize,
  _In_     DWORD  flAllocationType,
  _In_     DWORD  flProtect
);

wbi__SystemExtern
BOOL WINAPI VirtualFree(
  _In_ LPVOID lpAddress,
  _In_ usize dwSize,
  _In_ DWORD  dwFreeType
);

#define MEM_COMMIT 0x1000
#define MEM_RESERVE 0x2000
#define PAGE_EXECUTE 0x10
#define PAGE_EXECUTE_READ 0x20
#define PAGE_EXECUTE_READWRITE 0x40
#define PAGE_EXECUTE_WRITECOPY 0x80
#define PAGE_NOACCESS 0x1
#define PAGE_READONLY 0x2
#define PAGE_READWRITE 0x4
#define PAGE_WRITECOPY 0x8
#define MEM_DECOMMIT 0x4000
#define MEM_RELEASE 0x8000
#endif

#ifdef ALLOC_IMPLEMENTATION

ALLOC_BACKEND_API
void* wbi__allocateVirtualSpace(usize size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS);
}
 
ALLOC_BACKEND_API
void* wbi__commitMemory(void* addr, usize size, isize flags)
{
	DWORD newFlags = 0;
	if(flags & Read) {
		if(flags & Write) {
			if(flags & Execute) {
				newFlags = PAGE_EXECUTE_READWRITE;
			}
			newFlags = PAGE_READWRITE;
		} else if(flags & Execute) {
			newFlags = PAGE_EXECUTE_READ;
		} else {
			newFlags = PAGE_READONLY;
		}
	} else if(flags & Write) {
		if(flags & Execute) {
			newFlags = PAGE_EXECUTE_READWRITE;
		} else {
			newFlags = PAGE_READWRITE;
		}
	} else if(flags & Execute) {
		newFlags = PAGE_EXECUTE;
	} else {
		newFlags = PAGE_NOACCESS;
	}

    return VirtualAlloc(addr, size, MEM_COMMIT, newFlags);
}
 
ALLOC_BACKEND_API
void wbi__decommitMemory(void* addr, usize size)
{
    VirtualFree((void*)addr, size, MEM_DECOMMIT);
}
 
ALLOC_BACKEND_API
void wbi__freeAddressSpace(void* addr, usize size)
{
	/* This is a stupid hack, but blame clang/gcc with -Wall; not me */
	/* In any kind of optimized code, this should just get removed. */
	usize clangWouldWarnYouAboutThis = size;
	clangWouldWarnYouAboutThis++;
    VirtualFree((void*)addr, 0, MEM_RELEASE);
}

ALLOC_BACKEND_API
MemoryInfo getMemoryInfo()
{
	SYSTEM_INFO systemInfo;
	MemoryInfo info;
	GetSystemInfo(&systemInfo);
	usize pageSize, localMem, totalMem;
	int ret;

	pageSize = systemInfo.dwPageSize;

	localMem = 0;
	totalMem = 0;
	ret = GetPhysicallyInstalledSystemMemory(&localMem);
	if(ret) {
		totalMem = localMem * 1024;
	}
	
	info.totalMemory = totalMem;
	info.commitSize = CalcMegabytes(1);
	info.pageSize = pageSize;
	info.commitFlags = Read | Write;
	return info;

}
#endif
#endif

#ifdef ALLOC_POSIX
#ifndef __APPLE__
#ifndef PROT_NONE
#define PROT_NONE 0
#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define PROT_EXEC 0x4
#define MAP_SHARED 0x1
#define MAP_PRIVATE 0x2
#define MAP_FIXED 0x10
#define MAP_ANON 0x20
#define MS_SYNC 1
#define MS_ASYNC 4
#define MS_INVALIDATE 2
#endif

#ifndef _SC_PAGESIZE
#define _SC_PAGESIZE 30
#endif

#ifndef __off_t_defined
typedef usize off_t;
#endif

#else
#ifndef PROT_NONE
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4
#define PROT_NONE 0
#define MAP_SHARED 1
#define MAP_PRIVATE 2
#define MAP_FIXED 16
#define MAP_ANON 4096
#define MS_SYNC 16
#define MS_ASYNC 1
#define MS_INVALIDATE 2
#endif

#ifndef _SC_PAGESIZE
#define _SC_PAGESIZE 29
#endif
#endif
 
#ifndef _SYS_SYSINFO_H
typedef short unsigned int wbi__u16;
typedef unsigned int wbi__u32;
typedef long int wbi__i64;
typedef unsigned long int wbi__u64;
struct sysinfo 
{
	wbi__i64 uptime;
	wbi__u64 loads[3];
	wbi__u64 totalram;
	wbi__u64 freeMemory;
	wbi__u64 sharedMemory;
	wbi__u64 bufferMemory;
	wbi__u64 totalSwap;
	wbi__u64 freeSwap;
	wbi__u16 procs;
	wbi__u16 pad;
	wbi__u64 totalHigh;
	wbi__u64 freeHigh;
	wbi__u32 memUnit;
#ifdef ALLOC_POSIX_SYSINFO_PADDING
	char _f[20-2*sizeof(long)-sizeof(int)];
#endif
};
wbi__SystemExtern
int sysinfo(struct sysinfo* info);

#ifdef __APPLE__
#ifndef CTL_HW
#define CTL_HW 6
#define HW_PAGESIZE 7
#define HW_PHYSMEM 5
#define HW_MEMSIZE 24

wbi__SystemExtern
int sysctl(int *name, 
		unsigned int namelen, 
		void *oldp, size_t *oldlenp, 
		void *newp, size_t newlen);

#endif
ALLOC_BACKEND_API
int __cdecl sysinfo(struct sysinfo* info)
{
	int mib[2];
	usize size, eightByte;

	size = sizeof(usize);
	mib[0] = CTL_HW;
	mib[1] = HW_MEMSIZE;
	sysctl(mib, 2, &eightByte, &size, NULL, 0);
	info->totalram = eightByte;

#if 0
	mib[0] = CTL_HW;
	mib[1] = HW_PAGESIZE;
	size = sizeof(int);
	sysctl(mib, 2, &fourByte, &size, NULL, 0);
	info->freeMemory = (wbi__u64)fourByte;
#endif
	return 0;
}

#endif
wbi__SystemExtern
void* mmap(void* addr, usize len, int prot,
		int flags, int fd, off_t offset);
wbi__SystemExtern
int munmap(void* addr, usize len);
wbi__SystemExtern
int msync(void* addr, usize len, int flags);
wbi__SystemExtern
long sysconf(int name);

#ifdef ALLOC_IMPLEMENTATION
ALLOC_BACKEND_API
void* wbi__allocateVirtualSpace(usize size)
{
    void * ptr = mmap((void*)0, size, PROT_NONE, MAP_PRIVATE|MAP_ANON, -1, 0);
    msync(ptr, size, MS_SYNC|MS_INVALIDATE);
    return ptr;
}
 
ALLOC_BACKEND_API
void* wbi__commitMemory(void* addr, usize size, isize flags)
{
    void * ptr = mmap(addr, size, flags, MAP_FIXED|MAP_SHARED|MAP_ANON, -1, 0);
    msync(addr, size, MS_SYNC|MS_INVALIDATE);
    return ptr;
}
 
ALLOC_BACKEND_API
void wbi__decommitMemory(void* addr, usize size)
{
    mmap(addr, size, PROT_NONE, MAP_FIXED|MAP_PRIVATE|MAP_ANON, -1, 0);
    msync(addr, size, MS_SYNC|MS_INVALIDATE);
}
 
ALLOC_BACKEND_API
void wbi__freeAddressSpace(void* addr, usize size)
{
    msync(addr, size, MS_SYNC);
    munmap(addr, size);
}

ALLOC_BACKEND_API
MemoryInfo getMemoryInfo()
{
	struct sysinfo si;
	usize totalMem, pageSize;
	MemoryInfo info;
	sysinfo(&si);
	totalMem = si.totalram;
	pageSize = sysconf(_SC_PAGESIZE);

	info.totalMemory = totalMem;
	info.commitSize = CalcMegabytes(1);
	info.pageSize = pageSize;
	info.commitFlags = Read | Write;
	return info;

}
#endif
#endif
#endif
#endif

/* ===========================================================================
 * 		Main library -- Platform non-specific code
 * ===========================================================================
 */

#ifdef ALLOC_IMPLEMENTATION
ALLOC_API
isize alignTo(usize x, usize align)
{
	usize mod = x & (align - 1);
	return mod ? x + (align - mod) : x;
}

/* Memory Arena */

ALLOC_API 
void arenaFixedSizeInit(MemoryArena* arena, 
		void* buffer, isize size, 
		isize flags)
{
#ifndef ALLOC_NO_ZERO_ON_INIT
	ALLOC_MEMSET(arena, 0, sizeof(MemoryArena));
#endif

	arena->name = "arena";

	arena->flags = flags | FlagArenaFixedSize;
	arena->align = 8;

	arena->start = buffer;
	arena->head = buffer;
	arena->end = (void*)((isize)arena->start + size);
	arena->tempStart = NULL;
	arena->tempHead = NULL;
}


ALLOC_API 
void arenaInit(MemoryArena* arena, MemoryInfo info, isize flags)
{
	void* ret;
#ifndef ALLOC_NO_ZERO_ON_INIT
	ALLOC_MEMSET(arena, 0, sizeof(MemoryArena));
#endif

#ifndef ALLOC_NO_FLAG_CORRECTNESS_CHECKS
	if(flags & FlagArenaFixedSize) {
		ALLOC_ERROR_HANDLER(
				"can't create a fixed-size arena with arenaInit\n"
				"use arenaFixedSizeInit instead.",
				arena, "arena");
		return;
	}
#endif

	arena->flags = flags;
	arena->name = "arena";
	arena->info = info;
	arena->start = wbi__allocateVirtualSpace(info.totalMemory);
	ret = wbi__commitMemory(arena->start,
			info.commitSize,
			info.commitFlags);
	if(!ret) {
		ALLOC_ERROR_HANDLER("failed to commit inital memory", 
				arena, arena->name);
		return;
	}
	arena->head = arena->start;
	arena->end = (char*)arena->start + info.commitSize;
	arena->tempStart = NULL;
	arena->tempHead = NULL;
	arena->align = 8;
}

ALLOC_API 
void* arenaPushEx(MemoryArena* arena, isize size, 
		ALLOC_EXTENDED_INFO extended)
{
	void *oldHead, *ret;
	usize newHead, toExpand;

	if(arena->flags & FlagArenaStack) {
		size += sizeof(ALLOC_STACK_PTR);
	}

	if(arena->flags & FlagArenaExtended) {
		size += sizeof(ALLOC_EXTENDED_INFO);
	}

	oldHead = arena->head;
	newHead = alignTo((isize)arena->head + size, arena->align);

	if(newHead > (usize)arena->end) {
		if(arena->flags & FlagArenaFixedSize) {
			ALLOC_ERROR_HANDLER(
					"ran out of memory",
					arena, arena->name);
			return NULL;
		}

		toExpand = alignTo(size, arena->info.commitSize);
		ret = wbi__commitMemory(arena->end, toExpand, arena->info.commitFlags);
		if(!ret) {
			ALLOC_ERROR_HANDLER("failed to commit memory in arenaPush",
					arena, arena->name);
			return NULL;
		}
		arena->end = (char*)arena->end + toExpand;
	}

	if(arena->flags & FlagArenaStack) {
		ALLOC_STACK_PTR* head;
		head = (ALLOC_STACK_PTR*)newHead;
		
		head--;
		*head = (ALLOC_STACK_PTR)oldHead;
	}
	
	if(arena->flags & FlagArenaExtended) {
		ALLOC_EXTENDED_INFO* head;
		head = (ALLOC_EXTENDED_INFO*)oldHead;
		*head = extended;
		head++;
		oldHead = (void*)head;
	}

	arena->head = (void*)newHead;

	return oldHead;
}

ALLOC_API
void* arenaPush(MemoryArena* arena, isize size)
{
	return arenaPushEx(arena, size, 0);
}

ALLOC_API 
void arenaPop(MemoryArena* arena)
{
	usize prevHeadPtr;
	void* newHead;
#ifndef ALLOC_NO_FLAG_CORRECTNESS_CHECKS
	if(!(arena->flags & FlagArenaStack)) {
		ALLOC_ERROR_HANDLER(
				"can't use arenaPop with non-stack arenas",
				arena, arena->name);
		return;
	}
#endif

	
	prevHeadPtr = (isize)arena->head - sizeof(ALLOC_STACK_PTR);
	newHead = (void*)(*(ALLOC_STACK_PTR*)prevHeadPtr);
	if((isize)newHead <= (isize)arena->start) {
		arena->head = arena->start;
		return;
	}

	if(!(arena->flags & FlagArenaNoZeroMemory)) {
		usize size;
		size = (isize)arena->head - (isize)newHead;
		if(size > 0) {
			ALLOC_MEMSET(newHead, 0, size);
		}
	}

	arena->head = newHead;
}

ALLOC_API 
MemoryArena* arenaBootstrap(MemoryInfo info, isize flags)
{
	MemoryArena arena, *strapped;
#ifndef ALLOC_NO_FLAG_CORRECTNESS_CHECKS
	if(flags & FlagArenaFixedSize) {
		ALLOC_ERROR_HANDLER(
				"can't create a fixed-size arena with arenaBootstrap\n"
				"use arenaFixedSizeBootstrap instead.",
				NULL, "arena");
		return NULL;
	}
#endif

	arenaInit(&arena, info, flags);
	strapped = (MemoryArena*)
		arenaPush(&arena, sizeof(MemoryArena) + 16);
	*strapped = arena;
	if(flags & FlagArenaStack) {
		arenaPushEx(strapped, 0, 0);
		*((ALLOC_STACK_PTR*)(strapped->head) - 1) = 
			(ALLOC_STACK_PTR)strapped->head;
	}
	
	return strapped;
}

ALLOC_API 
MemoryArena* arenaFixedSizeBootstrap(void* buffer, usize size,
		isize flags)
{
	MemoryArena arena, *strapped;
	arenaFixedSizeInit(&arena, buffer, size, flags | FlagArenaFixedSize);
	strapped = (MemoryArena*)
		arenaPush(&arena, sizeof(MemoryArena) + 16);
	*strapped = arena;
	if(flags & FlagArenaStack) {
		arenaPushEx(strapped, 0, 0);
		*((ALLOC_STACK_PTR*)(strapped->head) - 1) = 
			(ALLOC_STACK_PTR)strapped->head;
	}
	return strapped;
}

ALLOC_API 
void arenaStartTemp(MemoryArena* arena)
{
	if(arena->tempStart) return;
	arena->tempStart = (void*)alignTo((isize)arena->head, 
			arena->info.pageSize);
	arena->tempHead = arena->head;
	arena->head = arena->tempStart;
}

ALLOC_API 
void arenaEndTemp(MemoryArena* arena)
{
	isize size;
	if(!arena->tempStart) return;
	arena->head = (void*)alignTo((isize)arena->head, arena->info.pageSize);
	size = (isize)arena->head - (isize)arena->tempStart;

	/* NOTE(will): if you have an arena with flags 
	 * 	ArenaNoRecommit | ArenaNoZeroMemory
	 * This just moves the pointer, which might be something you want to do.
	 */
	if(!(arena->flags & FlagArenaNoRecommit)) {
		wbi__decommitMemory(arena->tempStart, size);
		wbi__commitMemory(arena->tempStart, size, arena->info.commitFlags);
	} else if(!(arena->flags & FlagArenaNoZeroMemory)) {
		ALLOC_MEMSET(arena->tempStart, 0,
				(isize)arena->head - (isize)arena->tempStart);
	}

	arena->head = arena->tempHead;
	arena->tempHead = NULL;
	arena->tempStart = NULL;
}

ALLOC_API 
void arenaClear(MemoryArena* arena)
{
	MemoryArena local = *arena;
	isize size = (isize)arena->end - (isize)arena->start;
	wbi__decommitMemory(local.start, size);
	wbi__commitMemory(local.start, size, local.info.commitFlags);
	*arena = local;
}

ALLOC_API
void arenaDestroy(MemoryArena* arena)
{
	wbi__freeAddressSpace(arena->start, 
			(isize)arena->end - (isize)arena->start);
}

/* Memory Pool */
ALLOC_API
void poolInit(MemoryPool* pool, MemoryArena* alloc, 
		usize elementSize,
		isize flags)
{
#ifndef ALLOC_NO_ZERO_ON_INIT
	ALLOC_MEMSET(pool, 0, sizeof(MemoryPool));
#endif

	pool->alloc = alloc;
	pool->flags = flags;
	pool->name = "pool";
	pool->elementSize = elementSize < sizeof(void*) ?
		sizeof(void*) :
		elementSize;
	pool->count = 0;
	pool->lastFilled = -1;
	pool->capacity = (isize)
		((char*)alloc->end - (char*)alloc->head) / elementSize;

	pool->slots = alloc->head;
	pool->freeList = NULL;
}

ALLOC_API
MemoryPool* poolBootstrap(MemoryInfo info, 
		isize elementSize,
		isize flags)
{
	MemoryArena* alloc;
	MemoryPool* pool;

	isize arenaFlags = 0;
	if(flags & FlagPoolFixedSize) {
		arenaFlags = FlagArenaFixedSize;
	}
	
	alloc = arenaBootstrap(info, arenaFlags);
	pool = (MemoryPool*)arenaPush(alloc, sizeof(MemoryPool));

	poolInit(pool, alloc, elementSize, flags);
	return pool;
}

ALLOC_API
MemoryPool* poolFixedSizeBootstrap(
		isize elementSize, 
		void* buffer, usize size, 
		isize flags)
{
	MemoryArena* alloc;
	MemoryPool* pool;
	flags |= FlagPoolFixedSize;
	
	alloc = arenaFixedSizeBootstrap(buffer, size, FlagArenaFixedSize);
	pool = (MemoryPool*)arenaPush(alloc, sizeof(MemoryPool));

	poolInit(pool, alloc, elementSize, flags);
	return pool;
}

/* Utility functions not used
isize poolIndex(MemoryPool* pool, void* ptr)
{
	isize diff = (isize)ptr - (isize)pool->slots;
	return diff / pool->elementSize;
}
void* poolFromIndex(MemoryPool* pool, isize index)
{
	return (char*)pool->slots + index * pool->elementSize;
}
*/

ALLOC_API
void* poolRetrieve(MemoryPool* pool)
{
	void *ptr, *ret;
	ptr = NULL;
	if((!(pool->flags & FlagPoolCompacting)) && pool->freeList) {
		ptr = pool->freeList;
		pool->freeList = (void**)*pool->freeList;
		pool->count++;

		if(!(pool->flags & FlagPoolNoZeroMemory)) {
			ALLOC_MEMSET(ptr, 0, pool->elementSize);
		}

		return ptr;
	} 

	if(pool->lastFilled >= pool->capacity - 1) {
		if(pool->flags & FlagPoolFixedSize) {
			ALLOC_ERROR_HANDLER("pool ran out of memory",
					pool, pool->name);
			return NULL;
		}

		ret = arenaPush(pool->alloc, pool->alloc->info.commitSize);
		if(!ret) {
			ALLOC_ERROR_HANDLER("arenaPush failed in poolRetrieve", 
					pool, pool->name);
			return NULL;
		}
		pool->capacity = (isize)
			((char*)pool->alloc->end - (char*)pool->slots) / pool->elementSize;
	}

	ptr = (char*)pool->slots + ++pool->lastFilled * pool->elementSize;
	pool->count++;
	if(!(pool->flags & FlagPoolNoZeroMemory)) {
		ALLOC_MEMSET(ptr, 0, pool->elementSize);
	}
	return ptr;
}

ALLOC_API
void poolRelease(MemoryPool* pool, void* ptr)
{
	pool->count--;

	if(pool->freeList && !(pool->flags & FlagPoolNoDoubleFreeCheck)) {
		void** localList = pool->freeList;
		do {
			if(ptr == localList) {
				ALLOC_ERROR_HANDLER("caught attempting to free previously "
						"freed memory in poolRelease", 
						pool, pool->name);
				return;
			}
		} while((localList = (void**)*localList));
	}

	if(pool->flags & FlagPoolCompacting) {
		ALLOC_MEMCPY(ptr, 
				(char*)pool->slots + pool->count * pool->elementSize,
				pool->elementSize);
		return;
	}

	*(void**)ptr = pool->freeList;
	pool->freeList = (void**)ptr;
}

/*
 * TODO(will): Maybe, someday, have a tagged heap that uses real memoryArenas
 * 	behind the scenes, so that you get to benefit from stack and extended 
 * 	mode for ~free; maybe as a preprocessor flag?
 */ 
ALLOC_API
isize calcTaggedHeapSize(isize arenaSize, isize arenaCount,
		isize bootstrapped)
{
	return arenaCount * (arenaSize + sizeof(wbi__TaggedHeapArena))
		+ sizeof(TaggedHeap) * bootstrapped;
}

ALLOC_API
void taggedInit(TaggedHeap* heap, MemoryArena* arena, 
		isize internalArenaSize, isize flags)
{
#ifndef ALLOC_NO_ZERO_ON_INIT
	ALLOC_MEMSET(heap, 0, sizeof(TaggedHeap));
#endif

	heap->name = "taggedHeap";
	heap->flags = flags;
	heap->align = 8;
	heap->arenaSize = internalArenaSize;
	poolInit(&heap->pool, arena, 
			internalArenaSize + sizeof(wbi__TaggedHeapArena), 
			FlagPoolNormal | FlagPoolNoDoubleFreeCheck | 
			((flags & FlagTaggedHeapNoZeroMemory) ? 
			FlagPoolNoZeroMemory : 
			0));
}

ALLOC_API
TaggedHeap* taggedBootstrap(MemoryInfo info, 
		isize arenaSize,
		isize flags)
{
	TaggedHeap* strapped;
	TaggedHeap heap;
	MemoryArena* arena;
	info.commitSize = calcTaggedHeapSize(arenaSize, 8, 1);
	arena = arenaBootstrap(info, 
			((flags & FlagTaggedHeapNoZeroMemory) ? 
			FlagArenaNoZeroMemory :
			FlagArenaNormal));
	strapped = (TaggedHeap*)arenaPush(arena, sizeof(TaggedHeap) + 16);
	taggedInit(&heap, arena, arenaSize, flags);
	*strapped = heap;
	return strapped;
}

ALLOC_API
TaggedHeap* taggedFixedSizeBootstrap(
		isize arenaSize, 
		void* buffer, isize bufferSize, 
		isize flags)
{
	MemoryArena* alloc;
	TaggedHeap* heap;
	flags |= FlagTaggedHeapFixedSize;
	
	alloc = arenaFixedSizeBootstrap(buffer, bufferSize, 
			FlagArenaFixedSize |
			((flags & FlagTaggedHeapNoZeroMemory) ? 
			FlagArenaNoZeroMemory : 0));
	heap = (TaggedHeap*)arenaPush(alloc, sizeof(TaggedHeap));

	taggedInit(heap, alloc, arenaSize, flags);
	return heap;
	
}

ALLOC_API
void wbi__taggedArenaInit(TaggedHeap* heap, 
		wbi__TaggedHeapArena* arena,
		isize tag)
{
#ifndef ALLOC_NO_ZERO_ON_INIT
	ALLOC_MEMSET(arena, 0, sizeof(wbi__TaggedHeapArena));
#endif

	arena->tag = tag;
	arena->head = &arena->buffer;
	arena->end = (void*)((char*)arena->head + heap->arenaSize);
}

ALLOC_API
void wbi__taggedArenaSortBySize(wbi__TaggedHeapArena** array, isize count)
{
#define wbi__arenaSize(arena) ((isize)arena->head - (isize)arena->head)
	isize i, j, minSize;
	for(i = 1; i < count; ++i) {
		j = i - 1;

		minSize = wbi__arenaSize(array[i]);
		if(wbi__arenaSize(array[j]) > minSize) {
			wbi__TaggedHeapArena* temp = array[i];
			while((j >= 0) && (wbi__arenaSize(array[j]) > minSize)) {
				array[j + 1] = array[j];
				j--;
			}
			array[j + 1] = temp;
		}
	}
#undef wbi__arenaSize
}


ALLOC_API
void* taggedAlloc(TaggedHeap* heap, isize tag, usize size)
{
	wbi__TaggedHeapArena *arena, *newArena;
	void* oldHead;
	wbi__TaggedHeapArena* canFit[wbi__TaggedHeapSearchSize];
	isize canFitCount = 0;

	if(size > heap->arenaSize) {
		ALLOC_ERROR_HANDLER("cannot allocate an object larger than the "
				"size of a tagged heap arena.",
				heap, heap->name);
		return NULL;
	}

	if(!heap->arenas[tag]) {
		heap->arenas[tag] = (wbi__TaggedHeapArena*)poolRetrieve(&heap->pool);
		if(!heap->arenas[tag]) {
			ALLOC_ERROR_HANDLER("tagged heap arena retrieve returned null "
					"when creating a new tag",
				heap, heap->name);
			return NULL;
		}
		wbi__taggedArenaInit(heap, heap->arenas[tag], tag);
	}

	arena = heap->arenas[tag];

	if((char*)arena->head + size > (char*)arena->end) {
		/* TODO(will) add a find-better-fit option rather than
		 * allocating new arenas whenever */

		if(heap->flags & FlagTaggedHeapSearchForBestFit) {
			while((arena = arena->next)) {
				if((char*)arena->head + size < (char*)arena->end) {
					canFit[canFitCount++] = arena;
					if(canFitCount > (wbi__TaggedHeapSearchSize - 1)) {
						break;
					}
				}
			}

			if(canFitCount > 0) {
				wbi__taggedArenaSortBySize(canFit, canFitCount);
				arena = canFit[0];
			}
		}

		if(canFitCount == 0) {
			newArena = (wbi__TaggedHeapArena*)poolRetrieve(&heap->pool);
			if(!newArena) {
				ALLOC_ERROR_HANDLER(
						"tagged heap arena retrieve returned null",
						heap, heap->name);
				return NULL;
			}
			wbi__taggedArenaInit(heap, newArena, tag);
			newArena->next = arena;
			arena = newArena;
		}
	}

	oldHead = arena->head;
	arena->head = (void*)alignTo((isize)arena->head + size, heap->align);
	return oldHead;
}

ALLOC_API
void taggedFree(TaggedHeap* heap, isize tag)
{
	wbi__TaggedHeapArena *head;
	if((head = heap->arenas[tag])) do {
		poolRelease(&heap->pool, head);
	} while((head = head->next));
	heap->arenas[tag] = NULL;
}

#ifdef ALLOC_CPLUSPLUS_FEATURES
template<typename T, int n>
ALLOC_API 
T* arenaPushEx(MemoryArena* arena, 
		ALLOC_EXTENDED_INFO extended)
{
	return reinterpret_cast<T*>(arenaPushEx(arena, sizeof(T) * n, extended));
}

template<typename T>
ALLOC_API 
T* arenaPushEx(MemoryArena* arena, 
		ALLOC_EXTENDED_INFO extended)
{
	return reinterpret_cast<T*>(arenaPushEx(arena, sizeof(T) * n, extended));
}

template<typename T, int n>
ALLOC_API 
T* arenaPush(MemoryArena* arena)
{
	return reinterpret_cast<T*>(arenaPush(arena, sizeof(T) * n));
}

template<typename T>
ALLOC_API 
T* arenaPush(MemoryArena* arena, isize n)
{
	return reinterpret_cast<T*>(arenaPush(arena, sizeof(T) * n));
}

template<typename T>
ALLOC_API 
T* poolRetrieve(MemoryPool* pool)
{
	return reinterpret_cast<T*>(poolRetrieve(pool));
}

template<typename T>
ALLOC_API 
void poolRelease(MemoryPool* pool, T* ptr)
{
	poolRelease(pool, reinterpret_cast<void*>(ptr));
}

template<typename T>
ALLOC_API 
void poolInit(
		MemoryPool* pool,
		MemoryArena* alloc, 
		isize flags)
{
	poolInit(pool, alloc, sizeof(T), flags);
}

template<typename T>
ALLOC_API 
MemoryPool* poolBootstrap(MemoryInfo info,
		isize flags)
{
	return poolBootstrap(info, sizeof(T), flags);
}

template<typename T>
ALLOC_API 
MemoryPool* poolFixedSizeBootstrap(
		void* buffer, usize size,
		isize flags)
{
	return poolFixedSizeBootstrap(sizeof(T), buffer, size, flags);
}

template<typename T, int n>
ALLOC_API 
T* taggedAlloc(TaggedHeap* heap, isize tag)
{
	return reinterpret_cast<T*>(taggedAlloc(heap, tag, sizeof(T) * n));
}
#endif
#endif

#ifndef ALLOC_NO_DISABLE_STUPID_MSVC_WARNINGS
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif

/*
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/
