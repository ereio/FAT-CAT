#ifndef _FILECOMS
#define _FILECOMS

int openFile(char args[][ACOLS]);
int checkFile(struct directory dir);
int sizeFile(char args[][ACOLS]);
void removeOpenFile(struct directory dir);
int rmFile(char args[][ACOLS]);
void closeFile(char args[][ACOLS]);

#endif
