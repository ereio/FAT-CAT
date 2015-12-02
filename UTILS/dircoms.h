#ifndef _DIRCOMS
#define _DIRCOMS

int printDirectoryCurrent(struct directory current);
int printDirectory(struct directory current, char * name);
int findDirectory(struct directory current, char * name);

struct directory parseDirectoryEntry(unsigned long sector, int print);
struct cluster findCluster(struct directory dir);


int ErrorCheckDirectory(struct directory dir);

void ConvertDirName(struct directory dir, char * name);

int PrintDirectory(char args[][ACOLS]);
void ChangeDirectory(char args[][ACOLS]);
void PrintDirVerbose(struct directory dir);
void PrintDirStandard(struct directory dir);

#endif
