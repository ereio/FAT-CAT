#ifndef _DIRCOMS
#define _DIRCOMS

int printdir(struct directory current);
struct directory finddir(struct directory current, unsigned int attr, char * name);
struct directory finddir_noattr(struct directory current, char * name);
struct directory parsedir(unsigned long sector, int print);


int checkdirerr(struct directory dir);
void convertdirname(struct directory dir, char * name);
unsigned int setclus(struct directory * dir);

int PrintDirectory(char args[][ACOLS]);
void ChangeDirectory(char args[][ACOLS]);
void PrintDirVerbose(struct directory dir);
void PrintDirStandard(struct directory dir);

#endif
