#ifndef _DIRCOMS
#define _DIRCOMS

int parseContents(struct cluster cluster);
struct directory parseDirectoryEntry(unsigned long sector, int print);
int ErrorCheckDirectory(struct directory dir);

int PrintDirectory(char args[][ACOLS]);
void PrintDirVerbose(struct directory dir);
void PrintDirStandard(struct directory dir);

#endif
