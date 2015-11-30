#ifndef _DIRCOMS
#define _DIRCOMS

int printContents(struct cluster cluster);
int findDirectory(struct cluster cluster, char * name);
struct cluster findCluster(struct directory dir);

struct directory parseDirectoryEntry(unsigned long sector, int print);
int ErrorCheckDirectory(struct directory dir);

void ConvertDirName(struct directory dir, char * name);

int PrintDirectory(char args[][ACOLS]);
void ChangeDirectory(char args[][ACOLS]);
void PrintDirVerbose(struct directory dir);
void PrintDirStandard(struct directory dir);

#endif
