
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../global.h"
#include "UTILS/dircoms.h"
#include "UTILS/utils.h"

#define DIR_Name 11
#define DIR_Attr 1
#define DIR_NTRes 1
#define DIR_CrtTimeTenth 1
#define DIR_CrtTime 2
#define DIR_CrtDate 2
#define DIR_LstAccDate 2
#define DIR_FstClusHI 2
#define DIR_WrtTime 2
#define DIR_WrtDate 2
#define DIR_FstClusLO 2
#define DIR_FileSize 4
#define DIR_SIZE 32
#define EXT 8
#define NAMELEN 11

int PrintDirectory(char args[][ACOLS]){
	int status = 0;

	if(args[1][0] != 0){
		printf("Args[1] == %d\n", args[1][0]);
		return 0;
	} else {
		status = printdir(*fatcat.curDir);
	}

	return status;
}

void ChangeDirectory(char args[][ACOLS]){
	struct directory dest;
	char * name = malloc(sizeof(char) * strlen(args[1]));
	dest.name[0] = 0x00;	// Allows distinction if dir is set

	strcpy(name, args[1]);
	if(strlen(name) == 0){
		printf("\nUsage: cd <directory_name>");
		return;
	}

	for(int i=0; i < strlen(name); i++){
		if(name[i] == '/') name[i] = '\0';
		name[i] = toupper(name[i]);
	}

	// add name parsing by slashes, right now depth is 1
	dest = finddir(*fatcat.curDir, name);

	if(dest.name[0] != DIR_EMPTY && dest.name[0] != DIR_ERROR){
		printf("\n%s was found as entry\n", name);
		memcpy(fatcat.curDir, &dest, sizeof(struct directory));
	} else {
		printf("\n%s is not a directory", name);
	}

	free(name);
}

int printdir(struct directory current){
	int endofdir = 0;
	int linecount = 1;
	struct directory curdir;
	unsigned long byte_addr = 0;

	for(int i=0; i < current.cluster->clusterNum; i++){
		byte_addr = current.cluster->firstSectors[i] * BPB_BytesPerSec;

		while(!endofdir){
			linecount %= 8;
			curdir = parsedir(byte_addr, 1);
			if(curdir.name[0] == DIR_ERROR){
				printf("\nAn invalid directory was encountered");
				return -1;
			}
			if(linecount % 8 == 0) printf("\n");
			byte_addr += 32;
			linecount++;
			endofdir = curdir.name[0] == 0x00 ? 1 : 0;
		}
	}

	return 0;
}

// Will return a directory regardless of what is found but if the directory
// is not found, it will return an empty directory with a name value of
// 0x00000000. <dir_var>.name[0] == 0x00 can determine if not found
struct directory finddir(struct directory current, char * name){
	int endofdir = 0;
	unsigned long byte_addr = 0;
	struct directory dir;
	char dirName[12];

	for(int i=0; i < current.cluster->clusterNum; i++){
		byte_addr = current.cluster->firstSectors[i] * BPB_BytesPerSec;

		while(!endofdir){
			dir = parsedir(byte_addr, 0);
			if(dir.name[0] == DIR_ERROR) printf("\nAn invalid directory was encountered");

			if(dir.Attr & ATTR_DIRECTORY){
				convertdirname(dir, dirName);
				if(!strcmp(dirName, name)){
					return dir;
				}
			}
			byte_addr += DIR_SIZE;
			endofdir = dir.name[0] == 0x00 ? 1 : 0;
			if(dir.cluster != NULL) free(dir.cluster);
		}
	}

	return dir;
}

struct directory parsedir(unsigned long byte_addr, int print_values){
	struct directory dir;
	dir.cluster = NULL;

	fseek(fatcat.img, byte_addr, SEEK_SET);

	fread(&dir.name, DIR_Name, 1, fatcat.img);
	fread(&dir.Attr, DIR_Attr, 1, fatcat.img);
	fread(&dir.NTRes, DIR_NTRes, 1, fatcat.img);
	fread(&dir.CrtTimeTenth, DIR_CrtTimeTenth, 1, fatcat.img);
	fread(&dir.CrtTime, DIR_CrtTime, 1, fatcat.img);
	fread(&dir.CrtDate, DIR_CrtDate, 1, fatcat.img);
	fread(&dir.LstAccDate, DIR_LstAccDate, 1, fatcat.img);
	fread(&dir.FstClusHi, DIR_FstClusHI, 1, fatcat.img);
	fread(&dir.WrtTime, DIR_WrtTime, 1, fatcat.img);
	fread(&dir.WrtDate, DIR_WrtDate, 1, fatcat.img);
	fread(&dir.FstClusLO, DIR_FstClusLO, 1, fatcat.img);
	fread(&dir.FileSize, DIR_FileSize, 1, fatcat.img);

	if(checkdirerr(dir)){
		dir.name[0] = 0x7C;
		return dir;
	}

	if (print_values)
		PrintDirStandard(dir);
	else if(!(dir.Attr ^ ATTR_DIRECTORY) || !(dir.Attr ^ ATTR_ARCHIVE))
		setclus(&dir);

#ifdef  _DEBUGGING_F
	// printf("\nByte Address Read: 0x%07lx", byte_addr);
#endif
	return dir;
}

unsigned int setclus(struct directory * dir){
		struct cluster temp;
		unsigned int clusval = 0;

		dir->cluster = malloc(sizeof(struct cluster));

		clusval = dir->FstClusHi;
		clusval = clusval << 1;
		clusval = clusval | dir->FstClusLO;

#ifdef  _DEBUGGING_F
		printf("\nCLUSTER VALUE: %d\n", clusval);
#endif
		temp = FindClusterInfo(clusval);
		memcpy(dir->cluster, &temp, sizeof(struct cluster));
		return 0;
}

int checkdirerr(struct directory dir){
	if(dir.name[0] < 0x20 && !(dir.name[0] == 0x00)) return -1;
	for(int i=0; i < 11; i++){
		if( dir.name[i] == 0x22 ||
			dir.name[i] == 0x2A ||
			dir.name[i] == 0x2B ||
			dir.name[i] == 0x2C ||
			dir.name[i] == 0x2E ||
			dir.name[i] == 0x2F ||
			dir.name[i] == 0x3A ||
			dir.name[i] == 0x3B ||
			dir.name[i] == 0x3C ||
			dir.name[i] == 0x3D ||
			dir.name[i] == 0x3E ||
			dir.name[i] == 0x3F ||
			dir.name[i] == 0x5B ||
			dir.name[i] == 0x5C ||
			dir.name[i] == 0x5D ||
			dir.name[i] == 0x7C ){
			printf("\nInvalid Char: %x", dir.name[i]);
			return -2;
		}
	}
	return 0;
}

// Needed for comparisons in cd
void convertdirname(struct directory dir, char * name){
	char * str = dir.name;
	char temp[12] = {"\0"};
	int namepos = 0;
	char ext[3] = {"\0"};

	if (dir.Attr & ATTR_LONG_NAME) return;
	if (dir.name[0] == 0xE5) return;

	for(int i=EXT; i < NAMELEN && !isspace(str[i]); i++)
			ext[i-EXT] = str[i];

	for(int i=0; i < EXT && !isspace(str[i]); i++)
			temp[i] = str[i];

	if (dir.Attr & ATTR_DIRECTORY) {
		if(ext[0] != '\0') strcat(temp, ext);
		sprintf(name, "%s", temp);
	} else {
		namepos = sprintf(name, "%s", temp);
		if (ext[0] != '\0') sprintf(name+namepos, ".%s", ext);
	}
	name[11] = '\0';
}

void PrintDirVerbose(struct directory dir){
	printf("\nDIR_Name:%s", dir.name);
	printf("\nDIR_Attr: 0x%x", dir.Attr);
	printf("\nDIR_NTRes: 0x%x", dir.NTRes);
	printf("\nDIR_CrtTimeTenth:0x%x", dir.CrtTimeTenth);
	printf("\nDIR_CrtTime: 0x%x", dir.CrtTime);
	printf("\nDIR_CrtDate: 0x%x", dir.CrtDate);
	printf("\nDIR_LstAccDate: 0x%x", dir.LstAccDate);
	printf("\nDIR_FstClusHI: 0x%x", dir.FstClusHi);
	printf("\nDIR_WrtTime: 0x%x", dir.WrtTime);
	printf("\nDIR_WrtDate 0x:%x", dir.WrtDate);
	printf("\nDIR_FstClusLO: 0x%x", dir.FstClusLO);
	printf("\nDIR_FileSize:0x%x\n", dir.FileSize);
}

void PrintDirStandard(struct directory dir){
	char name[12] = {"\0"};
	convertdirname(dir, name);
	printf("%s", name);
	if(dir.Attr & ATTR_DIRECTORY) printf("/");
	printf("\t");
}


