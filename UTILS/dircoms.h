#ifndef _DIRCOMS
#define _DIRCOMS

int printDirectory(char args[][ACOLS]);
void parseDirectory(unsigned long sector);
void printDirVerbose(struct directory dir);

#endif
