/* References
 * http://stackoverflow.com/questions/8390356/c-command-line-arguments-eclipse-cdt
 *
 *
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "global.h"
#include "REPL/repl.h"
#include "REPL/parse.h"
#include "UTILS/memmy.h"
#include "UTILS/utils.h"


/* Commands */
const char* OPEN  = "exit";
const char* CLOSE  = "close";
const char* CREATE  = "create";
const char* RM  = "rm";
const char* CD = "cd";
const char* LS = "ls";
const char* MKDIR = "mkdir";
const char* RMDIR = "rmdir";
const char* READ = "read";
const char* WRITE = "write";
const char* EXIT = "exit";

/* Global const defines */
const char* _DELIMS = " \n";
const char* PREV_DIR = "..";
const char* CUR_DIR = ".";
const char* ARGS = "ARGS";
const int ACOLS = 255;

/* Globals */
char* cuser;
int margc = 1;
int run = 1;
int exec = 1;

struct fatcat fatcat;

/* File System Globals */
unsigned int BPB_BytesPerSec = 0;
unsigned char BPB_SecPerClus  = 0;
unsigned short BPB_RsvdSecCnt  = 0;
unsigned char BPB_NumFATs  = 0;
unsigned short BPB_RootEntCnt  = 0;
unsigned short BPB_TotSec16 = 0;
unsigned int BPB_HiddSec = 0;
unsigned int BPB_TotSec32 = 0;

unsigned int BPB_FATSz32 = 0;
unsigned short BPB_ExtFlags = 0;
unsigned int BPB_FSVer = 0;
unsigned int BPB_RootClus = 0;
unsigned int BPB_FSInfo = 0;
unsigned int BPB_BkBootSec = 0;
unsigned int BPB_Reserved[3] = {0,0,0};
unsigned int BS_DrvNum = 0;
unsigned int BS_Reserved1 = 0;

unsigned int BS_BootSig = 0;
unsigned int BS_VolID = 0;
unsigned char BS_VolLab[11] = {0};
unsigned char BS_FilSysType[8] = {0};

/* ALL EXITING TASKS DONE HERE */
int exit_shell(){
	return purge_memmy();
}

int main(int argc, char* args[])
{
	char *line;
	char cmd[255][255];
	FILE * img;

	img = fopen(args[1], "r");	/* opens file system*/

 	if(!init_memmy()) return 1;	/* Address Mem initialization*/
 	if(!LoadImage(img)) return 2; /* File system initialization*/

 	cuser = set_string(255);	/* User handle */
 	cuser = getenv("USER");

	line = set_string(255);		/* current command line init */

	while(run) shell_loop(line, cmd);
	
	return exit_shell();
}

int shell_loop(char * line, char cmd[255][255]) {
        _setup(cmd);
        _prompt();

        if(_read(line)){
                _parse(line, cmd);
                if (exec) _execute(cmd);
        } else {
	      	run = 0;
        }
	return 0;
}


