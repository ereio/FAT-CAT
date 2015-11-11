#ifndef _REPL
#define _REPL

/* REPL - Parsing*/
int _setup();
int _prompt();
int _read(char* line);
int _execute(char args[][ACOLS]);

int template(char args[][ACOLS]);
void invalid_command();

#endif
