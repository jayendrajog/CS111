#include <stdlib.h>
#include <stdio.h>
#include <getopt.h> //  parse
#include <fcntl.h>  //  open
#include <unistd.h> //  fork
#include <sys/wait.h>   //  waitpid
#include <string.h> //  strcpy

int main(int argc, char *argv[])
{
    //int digit_optind = 0;
    int const NUM_OPTIONS = 4;
    int option; //  TODO: don't know what this is for
    int option_index = 0;   //  TODO: might need to be reset to 0 for every iteration of the loop
    int fd_index = 0;
    int count = 0;
    
    int **flags = malloc(sizeof(int*) * NUM_OPTIONS);
    int *fds = malloc(sizeof(int) * argc);    //  TODO: will allocate too much memory
    pid_t *cpids = malloc(sizeof(pid_t) * argc);    //  TODO: will allocate too much memory
    int cpid_index = 0;
    
    int status;
    pid_t pid;
    int n;
    int index;
    int cmd_count;
    int cmd_index;
    
    enum FLAGS {
        RDONLY,
        WRONLY,
        COMMAND,
        VERBOSE
    };

    enum BOOL{
        FALSE,
        TRUE
    };
    
    static struct option the_options[] = {
        {"rdonly", required_argument, 0, RDONLY},
        {"wronly", required_argument, 0, WRONLY},
        {"command", required_argument, 0, COMMAND},
        {"verbose", no_argument, 0, VERBOSE},
        {0, 0, 0, 0}
    };
    
    while ((option = getopt_long(argc, argv, "", the_options, &option_index)) != -1) {
        switch (option) {
            case RDONLY:
                //printf("--rdonly is ON\n");
                //printf("optind is %d and file to open with --rdonly is %s\n", optind, argv[optind-1]);
                if ((fds[fd_index] = open(argv[optind-1], O_RDONLY)) == -1) {
                    fprintf(stderr, "Cannot open %s.\n", argv[optind-1]);
                    //exit(1);
                } else {
                  //  printf("%s is successfully opened with --rdonly option!\n", argv[optind-1]);
                }
                fd_index++; //  go to next place for save
                break;
            case WRONLY:
                //printf("--wronly is ON\n");
                if ((fds[fd_index] = open(argv[optind-1], O_WRONLY)) == -1) {
                    fprintf(stderr, "Cannot open %s.\n", argv[optind-1]);
                    //exit(1);
                } else {
                 //   printf("%s is successfully opened with --wronly option!\n", argv[optind-1]);
                }
                fd_index++; //  go to next place for save
                break;
            case COMMAND:
                printf("--command is ON\n");
                cpids[cpid_index] = fork();
                if (cpids[cpid_index] == 0) {
                    //  create child process
                    printf("Create child process number %i\n", cpid_index);
                    
                    //  count number of arguments after --command
                    cmd_count = 0;
                    cmd_index = optind-1;
                    while (cmd_index < argc) {
                        if (argv[cmd_index][0] == '-' && argv[cmd_index][1] == '-')
                            break;
                        cmd_count++;
                        cmd_index++;
                    }
                    if (cmd_count < 4) {
                        //  TODO: change message
                        fprintf(stderr, "Yo bruh you need at least four args after --command\n");
                        exit(1);
                    }
                    //  TODO: we get an error, and things don't work, if we have -stuff flags for commands
                    
                    //  prepare arg for execvp
                    argv[cmd_index] = '\0';
                    //  NOTE: seems like you can modify argv, which is super sweet, cuz the child get it's own copy
                    
                    //  set fds
                    cmd_index = optind-1;   //  use this to access std i, o, e args

                    //  NOTE: strtol "converts the initial part of the string in nptr to a long integer"
                    dup2(fds[strtol(argv[cmd_index], NULL, 10)], 0);
                    dup2(fds[strtol(argv[cmd_index+1], NULL, 10)], 1);
                    dup2(fds[strtol(argv[cmd_index+2], NULL, 10)], 2);
                    
                    //  TODO: idk why the first argument we pass in is this thing...
                    execvp(argv[cmd_index+3], &argv[cmd_index+3]);  //  cmd_index+3 is where the arg starts
                    
                    //exit(0);  //  for when we don't call execvp
                } 
                cpid_index++;
                break;
            case VERBOSE:
                index = optind - 1;
                while(index < argc)
                {
                    printf("%s ", argv[index]);
                    index++;
                }
                //printf("--verbose is ON\n");
                break;
            default:
               // printf("OOPS\n");
                break;
        }   
    }
    
    n = cpid_index - 1;
    while (n >= 0) {
        pid = waitpid(cpids[n], &status, 0);
        printf("Child number %i (PID %ld) exited with status 0x%x\n", n, (long)pid, status);
        --n;    //  // TODO(pts): Remove pid from the pids array.
    }
}

