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

/* File System Globals */
int BPB_BytesPerSec = -1;
int BPB_SecPerClus = -1;
int BPB_RsvdSecCnt = -1;
int BPB_NumFATS = -1;
int BPB_FATSz32 = -1;
int BPB_RootClus = -1;

/* ALL EXITING TASKS DONE HERE */
int exit_shell(){
	return purge_memmy();
}

int main(int argc, char* args[])
{
	char *line;
	char cmd[255][255];

 	if(!init_memmy()) return 1;	/* Address Mem intialization*/

 	cuser = set_string(255);	/* User handle */
 	cuser = getenv("USER");

	line = set_string(255);		/* current command line init */

	run = LoadImage(args);

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


