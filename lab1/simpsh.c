#include <stdlib.h>
#include <stdio.h>
#include <getopt.h> //  parse
#include <fcntl.h>  //  open
#include <unistd.h> //  fork
#include <sys/wait.h>   //  waitpid
#include <string.h>

int check_option(char* opt_name)
{
    return (strcmp(opt_name, "--rdonly") == 0 || 
            strcmp(opt_name, "--wronly") == 0 ||
            strcmp(opt_name, "--command") == 0 ||
            strcmp(opt_name, "--verbose") == 0 ||
            strcmp(opt_name, "--rdwr") == 0 ||
            strcmp(opt_name, "--pipe") == 0 ||
            strcmp(opt_name, "--wait") == 0 ||
            strcmp(opt_name, "--profile") == 0 ||
            strcmp(opt_name, "--abort") == 0 ||
            strcmp(opt_name, "--catch") == 0 ||
            strcmp(opt_name, "--ignore") == 0 ||
            strcmp(opt_name, "--default") == 0 ||
            strcmp(opt_name, "--pause") == 0);
}
int main(int argc, char *argv[])
{
    //int digit_optind = 0;
    int const NUM_OPTIONS = 13;
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
    
    enum OPTIONS {
        RDONLY, //0
        WRONLY, //1
        COMMAND, //2
        VERBOSE, //3
        RDWR, //4
        PIPE, //5
        WAIT, //6
        PROFILE, //7
        ABORT, //8
        CATCH, //9
        IGNORE, //10
        DEFAULT, //11
        PAUSE //12
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
        {"rdwr", required_argument, 0, RDWR},
        {"pipe", no_argument, 0, PIPE},
        {"wait", no_argument, 0, WAIT},
        {"profile", no_argument, 0, PROFILE},
        {"abort", no_argument, 0, ABORT},
        {"catch", required_argument, 0, CATCH},
        {"ignore", required_argument, 0, IGNORE},
        {"default", required_argument, 0, DEFAULT},
        {"pause", no_argument, 0, PAUSE},
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
                    //  child process
                    printf("Create child process number %i\n", cpid_index);
                    exit(0);
                } 
                cpid_index++;
                break;
            case VERBOSE:
                index = optind;
                while(index < argc)
                {
                    if(check_option(argv[index]))
                        profilentf("\n%s ", argv[index]);
                    else 
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

