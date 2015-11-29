#ifndef _DIRCOMS
#define _DIRCOMS

int parseContents(unsigned long sector);
struct directory parseDirectoryEntry(unsigned long sector, int print);
int ErrorCheckDirectory(struct directory dir);

int printDirectory(char args[][ACOLS]);
void PrintDirVerbose(struct directory dir);
void PrintDirStandard(struct directory dir);

#endif
