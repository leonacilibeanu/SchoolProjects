/**
 * Operating Sytems 2013 - Assignment 1
 *
 */

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>


#include "utils.h"

#define IN		0
#define OUT		1
#define APPEND		2

static char *value;
static char *var;



/**
 * Concatenate parts of the word to obtain the command
 */
static char *get_word(word_t *s)
{
	int string_length = 0;
	int substring_length = 0;

	char *string = NULL;
	char *substring = NULL;

	while (s != NULL) {
		substring = strdup(s->string);

		if (substring == NULL) {
			return NULL;
		}

		if (s->expand == true) {
			char *aux = substring;
			substring = getenv(substring);

			/* prevents strlen from failing */
			if (substring == NULL) {
				substring = calloc(1, sizeof(char));
				if (substring == NULL) {
					free(aux);
					return NULL;
				}
			}

			free(aux);
		}

		substring_length = strlen(substring);

		string = realloc(string, string_length + substring_length + 1);
		if (string == NULL) {
			if (substring != NULL)
				free(substring);
			return NULL;
		}

		memset(string + string_length, 0, substring_length + 1);

		strcat(string, substring);
		string_length += substring_length;

		if (s->expand == false) {
			free(substring);
		}

		s = s->next_part;
	}

	return string;
}

/**
 * Concatenate command arguments in a NULL terminated list in order to pass
 * them directly to execv.
 */
static char **get_argv(simple_command_t *command, int *size)
{
	char **argv;
	word_t *param;

	int argc = 0;
	argv = calloc(argc + 1, sizeof(char *));
	assert(argv != NULL);

	argv[argc] = get_word(command->verb);
	assert(argv[argc] != NULL);

	argc++;

	param = command->params;
	while (param != NULL) {
		argv = realloc(argv, (argc + 1) * sizeof(char *));
		assert(argv != NULL);

		argv[argc] = get_word(param);
		assert(argv[argc] != NULL);

		param = param->next_word;
		argc++;
	}

	argv = realloc(argv, (argc + 1) * sizeof(char *));
	assert(argv != NULL);

	argv[argc] = NULL;
	*size = argc;

	return argv;
}

/**
 * @dir: Structure containing the name of the directory
 *
 * Internal change-directory command.
 *
 * @return: On success, 0 is returned and on error -1 is returned
 */
static int shell_cd(word_t *dir)
{
	const char *path = get_word(dir);
	int rc = 0;

	rc = chdir(path);

	return rc;
}

/**
 * Internal exit/quit command.
 */
static int shell_exit()
{

	exit(0);

	return 0;
}

/**
 * @filedes  - file descriptor to be redirected
 * @filename - filename used for redirection
 * @type     - type of the redirection
 *
 * Redirects filedes to the specified file.
 *
 */
static void do_redirect(int filedes, const char *filename, int type)
{
	int fd;
	int rc = 0;

	switch (type){

	case IN:
		fd = open(filename, O_RDWR | O_CREAT, 0644);
		DIE(fd < 0, "Open file error");
		break;

	case OUT:
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
		DIE(fd < 0, "Open file error");
		break;

	case APPEND:
		fd = open(filename, O_RDWR | O_CREAT, 0644);
		DIE(fd < 0, "Open file error");

		rc = lseek(fd, 0L, SEEK_END);
		DIE(rc < 0, "lseek error");
		break;

	default:
		assert(false);
	}

	dup2(fd, filedes);

	close(fd);
}

/**
 * @filename - filename used for redirection
 *
 * Redirects standard output and standard error to the specified file.
 *
 */
static void do_redirect_out_err(const char *filename)
{
	int fd;

	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);

	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);

	close(fd);
}

/**
 * @s      - simple command to be executed
 * @level  - level of the command tree
 * @father - points to the command_t that points to s structure
 * 
 * Parse a simple command.
 * 
 * Environment variable assignment - will parse the value
 * and add the variable name and value to the environment.
 *
 * Internal command -execute and return the exit code.
 *
 * External command - fork a new process to perform redirections
 * and execute command.
 * It will verify what is the type of the redirection using the io_flags.
 * An IO_REGULAR flag will redirect output, input or
 * error to file by rewriting the file from beginning.
 * An IO_OUT_APPEND or IO_ERR_APPEND flag will redirect 
 * output or error to file in append mode.
 *
 * @return - On success, returns 0 and on error returns -1 or other value but 0.
 */

static int parse_simple(simple_command_t *s, int level, command_t *father)
{
	char *command;
	char **args;
	int *size;
	int rc = 0;
	int rd = 0;

	pid_t pid;
	int status;

	size = malloc(sizeof(int));
	DIE(size == NULL, "Error malloc");

	command = get_word(s->verb);
	args = get_argv(s,size);

	/* environment variable assignment */
	if (strchr(command, '=') != 0) {
		char *delim = "=\n";
		char *token;

		token = strtok(command, delim);

		if (token == NULL)
			return -1;

		var = strdup(token);
		DIE(var == NULL, "strdup error");

		token = strtok(NULL, delim);
		if (token == NULL)
			return -1;

		value = strdup(token);
		DIE(value == NULL, "strdup error");
		
		rc = setenv(var, value, 1);

		return rc;
	}

	free(size);

	/* internal command */
	if ((strncmp(command, args[0], sizeof(command)) == 0) && (sizeof(args) == 1)){
		rc  = execvp(command, (char* const*)args);
		fflush(stdout);
		return rc;
	}
	else{
		/* external command */
		pid = fork();

		switch (pid){
		case -1: /* error */
			exit(EXIT_FAILURE);

		case 0: /* child process */

			if (s->io_flags == IO_REGULAR){
					if (s->in != NULL)
						do_redirect(STDIN_FILENO, get_word(s->in), IN);

					if (s->out == s->err)
						do_redirect_out_err(get_word(s->out));

					if (s->out != s->err){
						if (s->err != NULL)
							do_redirect(STDERR_FILENO, get_word(s->err), OUT);

						if (s->out != NULL)
							do_redirect(STDOUT_FILENO, get_word(s->out), OUT);
					}
			}

			if (s->io_flags == IO_OUT_APPEND)
					if (s->out != NULL)
						do_redirect(STDOUT_FILENO, get_word(s->out), APPEND);

			if (s->io_flags == IO_ERR_APPEND)
					if (s->err != NULL)
						do_redirect(STDERR_FILENO, get_word(s->err), APPEND);
			/* execute command */
			rc = execvp(command, (char* const*) args);

			rd = fprintf(stderr, "Execution failed for '%s'\n", args[0]);
			DIE(rd < 0, "fprintf error");
			fflush(stdout);

			/* verify exit code */
			if (rc != 0)
				exit(EXIT_FAILURE);
			else
				exit(EXIT_SUCCESS);

		default:	/* parent process */
			waitpid(pid, &status, 0);

			return status;
		}
	}
}

/**
 * @cmd1   - first command to be executed in parallel
 * @cmd2   - second command to be executed in parallel
 * @level  - level of the command tree
 * @father - points to the command_t that points to cmd1 and cmd2 structures
 *
 * Process two commands in parallel, by creating two children.
 * The parent process of the first child will not wait for the the first child
 * process to finish and creates a second child in order to run simulaneously.
 * Then, it will wait for both of them to finish.
 *
 * @return - exit code of running both, cmd1 and cmd2, in parallel
 */
static int do_in_parallel(command_t *cmd1, command_t *cmd2, int level, command_t *father)
{
	pid_t chld1, chld2;
	int status1, status2;
	int rc1 = 0;
	int rc2 = 0;

	chld1 = fork();

	switch (chld1){

	case -1: /* error*/
		exit(EXIT_FAILURE);
		
	case 0: /* child1 process */
		rc1 = parse_command(cmd1, level+1, father);

		if (rc1 != 0)
				exit(EXIT_FAILURE);
			else
				exit(EXIT_SUCCESS);
	default: /* parent process */
		chld2 = fork();
		switch (chld2){

		case -1: /* error */
			exit(EXIT_FAILURE);

		case 0: /* second child process */

			rc2 = parse_command(cmd2, level+1, father);

			if (rc2 != 0)
				exit(EXIT_FAILURE);
			else
				exit(EXIT_SUCCESS);

		default: /* parent process */

			/* wait for the second child to finish */
			waitpid(chld2, &status2, 0);
			break;
		}

		/* wait for the first child to finish */
		waitpid(chld1, &status1, 0);

		if ((status1 == status2) && (status1 == 0))
			return 0;
		else
			return -1;
	}
}

/**
 * @cmd1   - first command to be executed
 * @cmd2   - second command to be executed
 * @level  - level of the command tree
 * @father - points to the command_t that points to cmd1 and cmd2 structures
 *
 * Run commands by creating an anonymous pipe (cmd1 | cmd2).
 * It will fork a new process.
 * The child will close the reading end of pipe and send stdout and stderr
 * to the writing end of pipe. Then, execute first command.
 * Parent process will close the writing end of pipe and send the reading end
 * to stdin.
 * The parent will not wait for the child to finish and will execute
 * second command in order to ensure the communication between the two
 * processes.
 *
 * @return - exit code
 */
static int do_on_pipe(command_t *cmd1, command_t *cmd2, int level, command_t *father)
{
	int rc = 0;
	int status;
	int pipefd[2];
	pid_t pid;

	/* create anonymous pipe */
	rc = pipe(pipefd);
	if (rc < 0)
		exit(EXIT_FAILURE);

	pid = fork();

	/* redirect the output of cmd1 to the input of cmd2 */
	switch (pid){

	case -1: /* error */
		exit(EXIT_FAILURE);

	case 0: /* child process */

		rc = close(pipefd[0]);
		DIE(rc < 0, "Close pipe end error");

		rc = dup2(pipefd[1], STDOUT_FILENO); /*send stdout to the pipe*/
		DIE(rc < 0, "Duplicate pipe end error");
    		rc = dup2(pipefd[1], STDERR_FILENO); /*send stderr to the pipe*/
    		DIE(rc < 0, "Duplicate pipe end error");

    		rc = close(pipefd[1]);
    		DIE(rc < 0, "Close pipe end error");

    		/* execute command and return exit code */
		rc = parse_command(cmd1, level+1, father);

		if (rc != 0)
			exit(EXIT_FAILURE);
		else
			exit(EXIT_SUCCESS);

	default: /* parent process */

		rc = close(pipefd[1]);
		DIE(rc < 0, "Close pipe end error");

		/* send child's output to stdin*/
		rc = dup2(pipefd[0], STDIN_FILENO);
		DIE(rc < 0, "Duplicate pipe end error");

		rc = close(pipefd[0]);
		DIE(rc < 0, "Close pipe end error");

		/* execute command and wait for the child to finish */
		rc = parse_command(cmd2, level+1, father);

		waitpid(pid, &status, 0);
		if (status != 0)
			exit(EXIT_FAILURE);

		return rc;
	}
}

/**
 * @c      - the command that will be parsed
 * @level  - level of the command tree
 * @father - points to the command_t that points to c structure
 *
 * Parse and execute a command by evaluating the type of operator_t in command_t
 * structure.
 *
 * @return - exit code
 */
int parse_command(command_t *c, int level, command_t *father)
{
	int rc = 0;
	char *command;

	switch (c->op){

	case OP_NONE:
		command = get_word(c->scmd->verb);
		/* execute cd command */
		if (strncmp(command, "cd", 2) == 0){
			shell_cd(c->scmd->params);
			if ((c->scmd->in != NULL) || (c->scmd->out != NULL) || (c->scmd->err != NULL))
				rc = parse_simple(c->scmd, level, father);
		}
		else	/* execute exit or quit command */
			if ((strncmp(command, "exit", 4) == 0) || (strncmp(command, "quit", 4) == 0))
				rc = shell_exit();
			else	/* execute simple command */
				rc = parse_simple(c->scmd, level, father);

		return rc;

	case OP_SEQUENTIAL:
		/* execute the commands one after the other */
		if (c->cmd1 != NULL)
			rc = parse_command(c->cmd1,level,c);
		if (c->cmd2 != NULL)
			rc = parse_command(c->cmd2,level,c);
		break;

	case OP_PARALLEL:
		/* execute the commands simultaneously */
		rc = do_in_parallel(c->cmd1, c->cmd2, level, c);
		break;

	case OP_CONDITIONAL_NZERO:
		/**
		 * execute the second command only if the first one
                 * returns non zero
                 */
		if (c->cmd1 != NULL)
			rc = parse_command(c->cmd1,level,c);
		if (rc != 0)
			rc = parse_command(c->cmd2,level,c);
		break;

	case OP_CONDITIONAL_ZERO:
		/**
		 * execute the second command only if the first one
                 * returns zero
                 */
		if (c->cmd1 != NULL)
			rc = parse_command(c->cmd1,level,c);
		if (rc == 0)
			rc = parse_command(c->cmd2,level,c);
		break;

	case OP_PIPE:
		/**
		 * redirect the output of the first command to the
		 * input of the second
		 */
		rc = do_on_pipe(c->cmd1, c->cmd2, level, c);
		break;

	default:
		assert(false);
		break;
	}

	return rc;
}

/**
 * Readline from mini-shell.
 */
char *read_line()
{
	char *instr;
	char *chunk;
	char *ret;

	int instr_length;
	int chunk_length;

	int endline = 0;

	instr = NULL;
	instr_length = 0;

	chunk = calloc(CHUNK_SIZE, sizeof(char));
	if (chunk == NULL) {
		fprintf(stderr, ERR_ALLOCATION);
		return instr;
	}

	while (!endline) {
		ret = fgets(chunk, CHUNK_SIZE, stdin);
		if (ret == NULL) {
			break;
		}

		chunk_length = strlen(chunk);
		if (chunk[chunk_length - 1] == '\n') {
			chunk[chunk_length - 1] = 0;
			endline = 1;
		}

		ret = instr;
		instr = realloc(instr, instr_length + CHUNK_SIZE + 1);
		if (instr == NULL) {
			free(ret);
			return instr;
		}
		memset(instr + instr_length, 0, CHUNK_SIZE);
		strcat(instr, chunk);
		instr_length += chunk_length;
	}

	free(chunk);

	return instr;
}
