#ifndef _UTILS
#define _UTILS

int LoadImage(FILE * img);
int LoadBPB(FILE * img);
int PrintBootSectInfo();
int SetRootDir();
unsigned int FindFirstSector(unsigned int cluster);
struct cluster FindClusterInfo(unsigned int cluster);

#endif
