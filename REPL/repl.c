/*
 * http://unix.stackexchange.com/questions/139222/why-is-the-pgid-of-my-child-processes-not-the-pid-of-the-parent
 * http://linux.die.net/man/2/waitpid
 *
 * */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

#include "global.h"
#include "UTILS/dircoms.h"
#include "UTILS/filecoms.h"
#include "repl.h"

int _setup(char cmd[ACOLS][ACOLS])
{
	for(int i=0; i < ACOLS; i++)
		strcpy(cmd[i], "\0");

	margc = 0;
	exec = 1;
	return 0;
}

int _prompt(char** settings)
{
	char* machine = getenv("MACHINE");

	if(machine == NULL) {
		machine = malloc(128 * sizeof(char));
		strcpy(machine, "local\0");
	}

	printf("%s@%s: %s => ", cuser, machine, fatcat.dirName);

	return 0;
}

int _read(char* line)
{
	char* str = malloc(255 * sizeof(char*));

	fgets(str, 255, stdin);
	strcpy(line, str);
	fflush(stdin);

	if(str != NULL) free(str);

	return line == NULL ? 0 : 1;
}

int _execute(char args[][ACOLS])
{
	if(!strcmp(EXIT, args[0]))
		run = 0;
	else if(!strcmp(OPEN, args[0]))
		openFile(args);
	else if(!strcmp(CLOSE, args[0]))
		template(args);
	else if(!strcmp(SIZE, args[0]))
		sizeFile(args);
	else if(!strcmp(CREATE, args[0]))
		template(args);
	else if(!strcmp(RM, args[0]))
		template(args);
	else if(!strcmp(CD, args[0]))
		ChangeDirectory(args);
	else if(!strcmp(LS, args[0]))
		PrintDirectory(args);
	else if(!strcmp(MKDIR, args[0]))
		CreateDirectory(args);
	else if(!strcmp(RMDIR, args[0]))
		template(args);
	else if(!strcmp(READ, args[0]))
		template(args);
	else if(!strcmp(WRITE, args[0]))
		template(args);
	else
		invalid_command();

	return 0;
}

int template(char args[][ACOLS]){
	return 0;
}

void invalid_command(){
	printf("%s", "Invalid command: Type help for all commands");
}
