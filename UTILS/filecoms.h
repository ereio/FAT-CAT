#ifndef _FILECOMS
#define _FILECOMS

int openFile(char args[][ACOLS]);
int checkFile(struct directory dir);
int sizeFile(char args[][ACOLS]);
void removeFile(struct directory dir);
void closeFile(char args[][ACOLS]);

#endif
