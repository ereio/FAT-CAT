#ifndef _DIRCOMS
#define _DIRCOMS

int parseContents(unsigned long sector);
struct directory parseDirectory(unsigned long sector);
int ErrorCheckDirectory(struct directory dir);

int printDirectory(char args[][ACOLS]);
void PrintDirVerbose(struct directory dir);
void PrintDirStandard(struct directory dir);

#endif
