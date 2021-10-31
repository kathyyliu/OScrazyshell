// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<fcntl.h>
#include<dirent.h>
#include<time.h>


#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

int reboot(int);

// Greeting shell during startup
void init_shell()
{
    clear();
    printf("\n\n\n\n******************"
           "************************");
    printf("\n\n\n\t****CRAZY SHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
           "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}

// Function to take input
int takeInput(char* str)
{
	char* buf;

	buf = readline("\n>>> ");
	if (strlen(buf) != 0) {
		add_history(buf);
		strcpy(str, buf);
		return 0;
	} else {
		return 1;
	}
    char* buf;

    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}

// Function to print Current Directory.
void printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\nDir: %s", cwd);
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

// Function where the system command is executed
void execArgs(char** parsed)
{
	// Forking a child
	pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	} else if (pid == 0) {
		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command..");
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
	// 0 is read end, 1 is write end
	int pipefd[2];
	pid_t p1, p2;

	if (pipe(pipefd) < 0) {
		printf("\nPipe could not be initialized");
		return;
	}
	p1 = fork();
	if (p1 < 0) {
		printf("\nCould not fork");
		return;
	}

	if (p1 == 0) {
		// Child 1 executing..
		// It only needs to write at the write end
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command 1..");
			exit(0);
		}
	} else {
		// Parent executing
		p2 = fork();

		if (p2 < 0) {
			printf("\nCould not fork");
			return;
		}

		// Child 2 executing..
		// It only needs to read at the read end
		if (p2 == 0) {
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(parsedpipe[0], parsedpipe) < 0) {
				printf("\nCould not execute command 2..");
				exit(0);
			}
		} else {
			// parent executing, waiting for two children
			wait(NULL);
			wait(NULL);
		}
	}
}

// Help command builtin
void openHelp()
{
    puts("\n***WELCOME TO THE CRAZY SHELL HELP PAGE***"
         "\n-Use the shell at your own risk..."
         "\nList of Commands supported:"
         "\n>cd"
         "\n>ls"
         "\n>boscoe"
         "\n>gparent"
         "\n>fwrite"
         "\n>freeze"
         "\n>randir"
         "\n>exit"
         "\n>all other general commands available in UNIX shell"
         "\nBased on code from Suprotik Dey");
    return;
}

void face()
{
    char* boscoe = "%%%&&&&&&&%#(/***/(((((///((((((((////(((((((#(#######((((((((((((((((((((((////((##(/**/#%%#(/(%%&%\n"
                   "%#####%##((((/****/(((((((((((((((//(((((##((#((((/(###(((((((((((((((((((((////(###(/**/###(/(#%%%%\n"
                   "#(((/((###((/**,,,*/(###((((((((((///(##((/**,**,//*,*,*/(%&%##(((((((((((((////(#%##/*******,/(#%%%\n"
                   "#(((#%%&&%#(////***/(###(//////////##(/*(%(*/*,.,**,,,,,*/&%(#&@#(#((((((((/////(#%%#(*,****,,*(#%%#\n"
                   "##((######((((///***((###(((((((###*,(%///*/*,****////((/*#((@&#@&&(#(((((((////(#%##/*,,***//(#%%%#\n"
                   "((####(//////////*//(####(((((#(#**(///*.. .,,//##////(((##(#%#@@&@&&#((((((///(#%%%#(*,..,,*/(##%##\n"
                   "%%%%%%(//((##(((((((((##((/(#%#,***/*.  ,/#%&@@@@@@@@@%(*../%(%%@@@@@@&((((((//(#%&&&%##(((///(##%##\n"
                   "%%%%%##(##################%#(/,*/*, ./#%&&@@@@@@@@@@@@@@@@#*..(%%%@&&@@@#(((((((#%&@@&&&&&&&%%&&&%%#\n"
                   "((########((((((((((##%%%%#(,/**...(%&&&@@@@@@@@@@@@@@@@@@@@%*,*%&&@&@@@@%#((/((#%&@@@@@@@@@@@@@@&%#\n"
                   "((((((((((((((((((((##%%%(//*,.,.,#%%%&&&&@@@@@@@@@@@@@@@@@@@%/*/%#&@&@@@@@#((((#%&@@@@@@@@@@@@@&&%#\n"
                   "((((((((((((((((((((####(//, .,,*(###%%%&&&&&&&&@@@@@@@@@@@@@&#*./#&%@@@@@@&%#(#%%&@@@@@@@@@@@@@&&%#\n"
                   "((((((((((((((((((###%(//,.,,..*(#####%%%%%&&&%%&&&&&@@@@@@@@@%(*.,(%&@@@@@@@%(#%&&@@@@@@@@@@@@@&&%#\n"
                   "((((((((#(#(((((((##((/,..,,..,/#####################%%%####%&&#/,  (#@@@@@@@@%%%&@@@@@@@@@@@@@@&&%#\n"
                   "((((((((((((((((((((/*,,,.,.  ,/(//*,,,,**///(#(//*,..,,  ...     (*,/#&@@@@@@&@%&&@@@@@@@@@@@@@&&%#\n"
                   "((//(((((((((((((//**,,,,..         .,,..  .     .**,.      ./(  .,. ,((%&@@@@&&&&&@@@@@@@@@@@@&&%%#\n"
                   "((//((((((((((((///**,..,* ./.,*,,        .. .*, *//****////(#%.##(*,,*((%&&@@@&&&@@@@@@@@@@@@@@&%%#\n"
                   "(((((##(#(#####//*/*,.,,**,   */***,,**////  /%&%.*/###%%%%%&&#*&%#/,*,/((#&%@&@&@@@@@@@@@@@@@@@&%%#\n"
                   "(#############(///*,.,,*,*,.   //(((((((((..(%&@@@&, /#%%#((,#&&&%%/,*,//((%&&&@@@@@@@@@@@@@@@@@&%%#\n"
                   "#############((///*,,,,,.**,. ,*,./*,,..*/**(#%&@@&%(,*//(###%%%&%%/,,,**(/#&%%@@@@@@@@@@@@@@@@@&%##\n"
                   "#############(/(((*,,,,,.,,,.  *//(((((/**///*/(#(,*////**//((#####/,.,,,((#%@&%%@@@@@@@@@@@@@@@&%##\n"
                   "###(((######(((((/,,,,,. ,,,..  */////*,**/*,,,,*/((###((//**//####/, ,./*###&&@%&&@@@@&&&&&&&&&&%#(\n"
                   "((#(((########(///***,,,..,,,.. .***,,***//(///(//###%%##(((/(#####*  ..**/%%#&@@@&&@@@@&&&&&&&&&%#(\n"
                   "(((#####%%%####((/*/*,,*,,.,,,.  ,***//*.          .*//(###((#####/,   ,,//(@%#@&&@@@@&@&&&&&&&&&%#(\n"
                   "(((####%#%####%(////*,,*,.,.,...  ,////////*,,,,,,,/(####%%%#####/,     .//((%@@@&&#@@@@@&&&&&&&&%#(\n"
                   "((#####%%###%###((//,**,/,.*,,...   **//(((//////////(#%%%%%#(((*,.      **((%#%@@@@@&@&@&&&&&&&&%#(\n"
                   "######%%###(/##(((/,,***,/.**,,..    .**//((#((##%%%%&&&%###(/***,..   ,*(//(/(#%&@@@&%&&&&&&&&&%%#(\n"
                   "(((######%####(((/,****,*,/*,,,,.      .,***//((((((((((///***/**,,...*//(/////(((#(#%#%&@@@@@@&&%#(\n"
                   "((((#########(/*((((/*/*,*/**,*.        .,,,,,,******,******///**,,,**,,*///*.*(#(#%%#%&@@@@@@@&&%#(\n"
                   "((((########(((///(//***/(*,/*..        .,,*,,**********/////////*,,.,,..,,,..,*(%&%&@@@@@@@@@@&&%#(\n"
                   "(((############(((/*//(///(,..          .************////////////***, .,,,,,*///&@@@@@@@@@@@@@@@@&%#\n"
                   "((###########((((((((/**,,,,...(        .***/*******//////////////*,......,/(/((#,*%@@@@@@@@@@@@@@@@\n"
                   "(##############((((##(,,*(*.           .,***///****////////(((///*,,,,,,,,,,,,*//*%# ...*&@@@@@@@@@@\n"
                   "(((############(((##/.             .,*.,,***//((((///////((((((/****,****,******/(*     ....,*(&@@@@\n"
                   "((((((((#(/*,.                ..,,***//*//**//((((((((((((((/(* ..,.,,*/******,/(         ........,,\n"
                   "##(((/                    .*,********,.(*/((##(//#(((####((/.,   ......,,,.,,..,                   .\n"
                   "(((*                      /*//**//.     .(((####((((((*&#*/,,.   (#/. .,..,,. .                     \n";

    write(1, boscoe, strlen(boscoe));
}

// Create a file with the given filename and write 'msg' into it
int filewrite(char* filename, char* msg)
{
    int fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 00000);
    if (fd == -1) {
        return 0;
    }
    if (write(fd, msg, strlen(msg)) != strlen(msg)) {
        write(2,"There was an error writing to the file\n", strlen("There was an error writing to the file\n"));
        return 0;
    }
    return 1;
}

// Randomly decide to either reboot or print boscoe and then recurse
void freeze()
{
    double random = (double)rand() / (double)RAND_MAX;
    if (random > 0.9) {
//        reboot(LINUX_REBOOT_MAGIC1,
//               LINUX_REBOOT_MAGIC2,
//               LINUX_REBOOT_CMD_POWER_OFF, 0);
        system("shutdown now");
        reboot(0);
    } else {
        face();
        printf("Uh oh, you've really done it this time.\n");
        freeze();
    }
}

// Recurse through dir
char* recurseDir(char* baseDir)
{
    strcat(baseDir, "/");
    printf("Current path: %s\n", baseDir);
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(baseDir);

    if (!dir) {
        return baseDir;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            double random = (double)rand() / (double)RAND_MAX;
            char* dirName = dp->d_name;
            char* newPath = strcat(path, dirName);
            printf("%s : %f\n", dirName, random);
            printf("%s\n", path);
            if (random > 0.9) {
                return newPath;
            } else {
                return recurseDir(newPath);
            }
        }
    }
    closedir(dir);
    return baseDir;
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 9, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "gparent";	// chdir
    ListOfOwnCmds[5] = "boscoe";	// write
    ListOfOwnCmds[6] = "fwrite";	// open, write
    ListOfOwnCmds[7] = "cr";    // chdir
    ListOfOwnCmds[8] = "freeze";    // calloc/malloc
    //TODO: nthparent

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    printf("%d", switchOwnArg);
    switch (switchOwnArg) {
        case 1:
            printf("\nGoodbye\n");
            exit(0);
        case 2:
            chdir(parsed[1]);
            return 1;
        case 3:
            openHelp();
            return 1;
        case 4:
            username = getenv("USER");
            printf("\nHello %s.\nMind that this is "
                   "not a place to play around."
                   "\nUse help to know more..\n",
                   username);
            return 1;
        case 5:
            if (chdir("../..") == 0) {
                return 1;
            }
        case 6:
            face();
            return 1;
        case 7:
            return filewrite(parsed[1], parsed[2]);
        case 8:
            chdir(recurseDir(""));
            return 1;
        case 9:
            freeze();
            return 1;
        default:
            break;
    }
    return 0;
}

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char* str, char** parsed, char** parsedpipe)
{

    char* strpiped[2];
    int piped = 0;

    piped = parsePipe(str, strpiped);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);

    } else {

        parseSpace(str, parsed);
    }

    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + piped;
}

int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    init_shell();

    while (1) {
        // print shell line
        printDir();
        // take input
         if (takeInput(inputString))
         	continue;
        // strcpy(inputString, "filewrite cool.txt hi");
        // process
        execFlag = processString(inputString,
                                 parsedArgs, parsedArgsPiped);
        // execflag returns zero if there is no command
        // or it is a builtin command,
        // 1 if it is a simple command
        // 2 if it is including a pipe.

        // execute
        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    return 0;
}
