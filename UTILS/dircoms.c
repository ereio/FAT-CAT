
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
		status = printDirectoryCurrent(*fatcat.curDir);
	}

	return status;
}

void ChangeDirectory(char args[][ACOLS]){
	int result = -2;
	char * name = malloc(sizeof(char) * strlen(args[1]));

	strcpy(name, args[1]);
	if(strlen(name) == 0){
		printf("\nUsage: cd <directory_name>");
		return;
	}

	for(int i=0; i < strlen(name); i++){
		if(name[i] == '/') name[i] = '\0';
		name[i] = toupper(name[i]);
	}

	result = findDirectory(*fatcat.curDir, name);

	if(!result){
		printf("\n%s was found as entry", name);

	} else {
		printf("\n%s is not a directory", name);
	}

	free(name);
}

// CHANGE TO DIRECTORY, ABSTRACT THE CLUSTERS
int findDirectory(struct directory current, char * name){
	int endofdir = 0;
	unsigned long byte_addr = 0;
	struct directory dir;
	char dirName[12];

	for(int i=0; i < current.cluster->clusterNum; i++){
		byte_addr = current.cluster->firstSectors[i] * BPB_BytesPerSec;

		while(!endofdir){
			dir = parseDirectoryEntry(byte_addr, 0);
			if(ErrorCheckDirectory(dir)) return -1;

			if(dir.Attr & ATTR_DIRECTORY){
				ConvertDirName(dir, dirName);
				if(!strcmp(dirName, name)){
					return 0;
				}
			}
			byte_addr += DIR_SIZE;
			endofdir = dir.name[0] == 0x00 ? 1 : 0;
		}
	}

	return 1;
}

struct cluster findCluster(struct directory dir){
		struct cluster newclus;

		return newclus;
}

int printDirectoryCurrent(struct directory current){
	int endofdir = 0;
	int linecount = 1;
	struct directory curdir;
	unsigned long byte_addr = 0;

	for(int i=0; i < current.cluster->clusterNum; i++){
		byte_addr = current.cluster->firstSectors[i] * BPB_BytesPerSec;

		while(!endofdir){
			linecount %= 8;
			curdir = parseDirectoryEntry(byte_addr, 1);
			if(ErrorCheckDirectory(curdir)) return -1;
			if(linecount % 8 == 0) printf("\n");
			byte_addr += 32;
			linecount++;
			endofdir = curdir.name[0] == 0x00 ? 1 : 0;
		}
	}

	return 0;
}

struct directory parseDirectoryEntry(unsigned long byte_addr, int print_values){
	struct directory dir;

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

#ifdef  _DEBUGGING
	printf("\nByte Address Read: 0x%07lx", byte_addr);
	PrintDirVerbose(dir);
#endif
	if (print_values) PrintDirStandard(dir);

	return dir;
}

int ErrorCheckDirectory(struct directory dir){
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

// Needed for comparisons in cd
void ConvertDirName(struct directory dir, char * name){
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
}

void PrintDirStandard(struct directory dir){
	char name[12] = {"\0"};
	ConvertDirName(dir, name);
	printf("%s", name);
	if(dir.Attr & ATTR_DIRECTORY) printf("/");
	printf("\t");
}


