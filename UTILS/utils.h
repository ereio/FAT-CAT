#ifndef _UTILS
#define _UTILS

int LoadImage(FILE * img);
int LoadFSInfo(FILE * img);
int LoadBPB(FILE * img);
int PrintBootSectInfo();
int SetRootDir();
struct cluster FindClusterInfo(unsigned int cluster);

struct cluster getfreeclus();
unsigned int setdirclus(struct directory * dir);
struct cluster freecluschain(struct directory dir);
void nametofat(char * name);

#endif
