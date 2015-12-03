#ifndef _DIRCOMS
#define _DIRCOMS

int printdir(struct directory current);
struct directory finddir(struct directory current, unsigned int attr,  char * name);
struct directory parsedir(unsigned long sector);

struct directory makedir();
int removedir();

int checkdirerr(struct directory dir);
void convertdirname(struct directory dir, char * name);

int PrintDirectory(char args[][ACOLS]);
void ChangeDirectory(char args[][ACOLS]);
void PrintDirVerbose(struct directory dir);
void PrintDirStandard(struct directory dir);

#endif
