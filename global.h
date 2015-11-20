/* Global References for Command Strings, Deliminators, etc. */
#ifndef _SIZE_T
#include <stddef.h>
#include <stdio.h>
#endif

#include <sys/types.h>
/* Defines  */
#define MAX_HEAP_SIZE 255
#define MAX_ADDR_NAME 16
#define DEFAULT_ALLOC 255

/* Structures  */
struct address{
	int* loc;
	size_t type;
	char name[16];
};
extern pid_t bgproc;

/* Commands  */
extern const char* EXIT;
extern const char* OPEN;
extern const char* CLOSE;
extern const char* CREATE;
extern const char* RM;
extern const char* CD;
extern const char* LS;
extern const char* MKDIR;
extern const char* RMDIR;
extern const char* READ;
extern const char* WRITE;

/* Constant and Labels */
extern const char* PREV_DIR;
extern const char* CUR_DIR;
extern const char* _DELIMS;

/* MAX var amounts */
extern const char* ARGS;
extern const int ACOLS;
extern char* cuser;

/* Globals  */
extern struct address * ADDRS;
extern int margc;
extern int run;
extern int exec;
extern int runbg;

/* Boot Globals */
extern int BPB_BytesPerSec;
extern int BPB_SecPerClus;
extern int BPB_RsvdSecCnt;
extern int BPB_NumFATS;
extern int BPB_FATSz32;
extern int BPB_RootClus;
