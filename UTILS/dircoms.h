#ifndef _DIRCOMS
#define _DIRCOMS


// High level functions for shell
int PrintDirectory(char args[][ACOLS]);
void ChangeDirectory(char args[][ACOLS]);
void CreateDirectory(char args[][ACOLS]);

// Low level operations
int removedir();
struct directory makedir();
int printdir(struct directory current);
struct directory finddir(struct directory current, unsigned int attr,  char * name);
struct directory parsedir(unsigned long sector);

int checkdirerr(struct directory dir);
void convertdirname(struct directory dir, char * name);

void PrintDirVerbose(struct directory dir);
void PrintDirStandard(struct directory dir);

#endif
