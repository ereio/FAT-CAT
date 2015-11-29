/* Global References for Command Strings, Deliminators, etc. */
#ifndef _SIZE_T
#include <stddef.h>
#include <stdio.h>
#endif

#ifndef _DEBUGGING_BOOT_SECT
#define _DEBUGGING_BOOT_SECT	// Comment out for less output
#endif

//#ifndef _DEBUGGING
//#define _DEBUGGING	// Comment out for less output
//#endif

#include <sys/types.h>
/* Defines  */
#define MAX_HEAP_SIZE 255
#define MAX_ADDR_NAME 16
#define DEFAULT_ALLOC 255
#define EOC 268435448	// End Of Cluster
#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)
#define R 0
#define W 1
#define RW 2

/* Structures  */
struct address{
	int* loc;
	size_t type;
	char name[16];
};

struct cluster{
	unsigned long sectorNum;
	unsigned long entryOffset;
	unsigned int totalClusters;
};

struct directory{
	char name[11];
	char Attr;
	char NTRes;

	char CrtTimeTenth;
	unsigned short CrtTime;
	unsigned short CrtDate;
	unsigned short LstAccDate;
	unsigned short WrtTime;
	unsigned short WrtDate;

	unsigned short FstClusHi;
	unsigned short FstClusLO;
	unsigned int FileSize;
	unsigned int Mode;
};

struct fatcat{
        FILE * img;
        unsigned long rootSector;
        unsigned long currentSector;
        unsigned int currentCluster;
        unsigned int* rootdir;
        unsigned int rootDirSectors;
        unsigned int firstDataSector;
        unsigned int dataSectors;
        unsigned int dataClusters;
        struct cluster root;
	unsigned long currentDirAddr;
	char * dirName;
};

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
extern struct directory * OPENFILES;
extern struct address * ADDRS;
extern struct fatcat fatcat;
extern int margc;
extern int run;
extern int exec;
extern int runbg;

/* Boot Globals */

extern unsigned int BPB_BytesPerSec;
extern unsigned char BPB_SecPerClus;
extern unsigned short BPB_RsvdSecCnt;
extern unsigned char BPB_NumFATs;
extern unsigned short BPB_RootEntCnt;
extern unsigned short BPB_TotSec16;
extern unsigned int BPB_HiddSec;
extern unsigned int BPB_TotSec32;

extern unsigned int BPB_FATSz32;
extern unsigned short BPB_ExtFlags;
extern unsigned int BPB_FSVer;
extern unsigned int BPB_RootClus;
extern unsigned int BPB_FSInfo;
extern unsigned int BPB_BkBootSec;
extern unsigned int BPB_Reserved[3];
extern unsigned int BS_DrvNum;
extern unsigned int BS_Reserved1;

extern unsigned int BS_BootSig;
extern unsigned int BS_VolID;
extern unsigned char BS_VolLab[11];
extern unsigned char BS_FilSysType[8];
