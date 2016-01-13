#include <stdlib.h>
#include <stdio.h>
#include <getopt.h> //  parse
#include <fcntl.h>  //  open
#include <unistd.h> //  fork
#include <sys/wait.h>   //  waitpid
#include <string.h> //  str stuff
#include <errno.h>  //  errno

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
            strcmp(opt_name, "--pause") == 0 ||
            strcmp(opt_name, "--append") == 0 ||
            strcmp(opt_name, "--cloexec") == 0 ||
            strcmp(opt_name, "--creat") == 0 ||
            strcmp(opt_name, "--directory") == 0 ||
            strcmp(opt_name, "--dsync") == 0 ||
            strcmp(opt_name, "--excl") == 0 ||
            strcmp(opt_name, "--nofollow") == 0 ||
            strcmp(opt_name, "--nonblock") == 0 ||
            strcmp(opt_name, "--rsync") == 0 ||
            strcmp(opt_name, "--sync") == 0 ||
            strcmp(opt_name, "--trunc") == 0 );
}

int main(int argc, char *argv[])
{
    //int digit_optind = 0;
    int const NUM_OPTIONS = 13;
    int option; //  TODO: don't know what this is for
    int option_index = 0;   //  TODO: might need to be reset to 0 for every iteration of the loop
    int fd_index = 0;
    int count = 0;
    
    char* verbose_strings = malloc(argc * 10 * sizeof(char));
    int verbose_string_index = 0;
    //int **flags = malloc(sizeof(int*) * NUM_OPTIONS);
    int *fds = malloc(sizeof(int) * argc);    //  TODO: will allocate too much memory
    pid_t *cpids = malloc(sizeof(pid_t) * argc);    //  TODO: will allocate too much memory
    int cpid_index = 0;
    
    int status;
    pid_t pid;
    int n;
    int index;
    int cmd_count;
    int cmd_index;
    int old_optind;
    
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
        PAUSE, //12

        //file flags
        APPEND, //13
        CLOEXEC, //14
        CREAT, //15
        DIRECTORY, //16
        DSYNC, //17
        EXCL, //18
        NOFOLLOW, //19
        NONBLOCK, //20
        RSYNC, //21
        SYNC, //22
        TRUNC //23
    };

    enum BOOL{
        FALSE,
        TRUE
    } Verbose_ON = FALSE;

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
        {"append", optional_argument, 0, APPEND},
        {"cloexec", optional_argument, 0, CLOEXEC},
        {"creat", optional_argument, 0, CREAT},
        {"directory", optional_argument, 0, DIRECTORY},
        {"dsync", optional_argument, 0, DSYNC},
        {"excl", optional_argument, 0, EXCL},
        {"nofollow", optional_argument, 0, NOFOLLOW},
        {"nonblock", optional_argument, 0, NONBLOCK},
        {"rsync", optional_argument, 0, RSYNC},
        {"sync", optional_argument, 0, SYNC},
        {"trunc", optional_argument, 0, TRUNC},
        {0, 0, 0, 0}
    };
    
    while ((option = getopt_long(argc, argv, "", the_options, &option_index)) != -1) {
        switch (option) {
            case RDONLY:
                //printf("--rdonly is ON\n");
                //printf("optind is %d and file to open with --rdonly is %s\n", optind, argv[optind-1]);
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index-1]);
                    strcat(verbose_strings, " ");
                    strcat(verbose_strings, argv[index]);
                    printf("%s\n", verbose_strings);
                    memset(verbose_strings, 0, argc * 10 * sizeof(char));
                }
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
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index-1]);
                    strcat(verbose_strings, " ");
                    strcat(verbose_strings, argv[index]);
                    printf("%s\n", verbose_strings);
                    memset(verbose_strings, 0, argc * 10 * sizeof(char));
                }
                if ((fds[fd_index] = open(argv[optind-1], O_WRONLY)) == -1) {
                    fprintf(stderr, "Cannot open %s.\n", argv[optind-1]);
                    //exit(1);
                } else {
                 //   printf("%s is successfully opened with --wronly option!\n", argv[optind-1]);
                }
                fd_index++; //  go to next place for save
                break;
            case COMMAND:
        //        printf("--command is ON\n");
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index-1]);
                    while(index < argc && !check_option(argv[index]))
                    {
                        strcat(verbose_strings, " ");
                        strcat(verbose_strings, argv[index]);
                        index++;
                    }
                    printf("%s\n", verbose_strings);
                    memset(verbose_strings, 0, argc * 10 * sizeof(char));
                }
                
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
                
                //  save optind and modify optind so getopt_long will jump to the next --options (long options)
                old_optind = optind;
                optind = cmd_index;
                cpids[cpid_index] = fork();
                if (cpids[cpid_index] == 0) {
                    //  create child process
                    //printf("Create child process number %i\n", cpid_index);
                    
                    //  prepare arg for execvp
                    argv[cmd_index] = '\0';
                    //  NOTE: seems like you can modify argv, which is super sweet, cuz the child get it's own copy
                    
                    //  set fds
                    cmd_index = old_optind - 1;   //  use this to access std i, o, e args

                    //  NOTE: strtol "converts the initial part of the string in nptr to a long integer"
                    dup2(fds[strtol(argv[cmd_index], NULL, 10)], 0);
                    dup2(fds[strtol(argv[cmd_index+1], NULL, 10)], 1);
                    dup2(fds[strtol(argv[cmd_index+2], NULL, 10)], 2);
                    
                    execvp(argv[cmd_index+3], &argv[cmd_index+3]);  //  cmd_index+3 is where the arg starts
                    
                    //exit(0);  //  for when we don't call execvp
                } 
                cpid_index++;
                break;
            case VERBOSE:
                Verbose_ON = TRUE;
                // index = optind;
                // while(index < argc)
                // {
                //     if(check_option(argv[index]))
                //         printf("\n%s ", argv[index]);
                //     else 
                //         printf("%s ", argv[index]);
                //     index++;
                // }
                //printf("--verbose is ON\n");
                break;
            case RDWR:
            case CATCH:
            case IGNORE:
            case DEFAULT:
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index-1]);
                    strcat(verbose_strings, " ");
                    strcat(verbose_strings, argv[index]);
                    printf("%s\n", verbose_strings);
                    memset(verbose_strings, 0, argc * 10 * sizeof(char));
                }
                break;
            case PIPE:
            case WAIT:
            case PROFILE:
            case ABORT:
            case PAUSE:
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    printf("%s\n", verbose_strings);
                    memset(verbose_strings, 0, argc * 10 * sizeof(char));
                }
                break;
            default:
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
               // printf("OOPS\n");
                break;
        }   
    }
    // if(verbose_strings[0] != '\0')
    //     printf("%s\n", verbose_strings);
    
    n = cpid_index - 1;
    while (n >= 0) {
        pid = waitpid(cpids[n], &status, 0);
        printf("Child number %i (PID %ld) exited with status 0x%x\n", n, (long)pid, status);
        --n;    //  // TODO(pts): Remove pid from the pids array.
    }
    
    //  close fds
    for (index = 0; index < fd_index; index++) {
        if (fds[index] == -1)   //  this didn't open successfully
            continue;
        if (close(fds[index]))  //  0 for success, -1 for error
            fprintf(stderr, "Something is wrong with close! %s\n", strerror(errno));
    }
    
    //  free memory
    free(fds);
    free(cpids);
    fds = cpids = NULL;
    
}

