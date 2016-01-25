#include <stdlib.h>
#include <stdio.h>
#include <getopt.h> //  parse
#include <fcntl.h>  //  open
#include <unistd.h> //  fork and pipe
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
    char *pipeFds = malloc(sizeof(char) * argc);    //  keep track of pipe fds
    pid_t *cpids = malloc(sizeof(pid_t) * argc);    //  TODO: will allocate too much memory
    int cpid_index = 0;
    int pipefd[2];  //  to store pipe fds
    
    int status;
    pid_t pid;
    int n;
    int index;
    int cmd_count;
    int cmd_index;
    int old_optind;
    int index_i, index_o, index_e;

    int oflag_val = 0;
    
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
                if ((fds[fd_index] = open(argv[optind-1], O_RDONLY | oflag_val)) == -1) {
                    fprintf(stderr, "Cannot open %s.\n", argv[optind-1]);
                    //exit(1);
                }
                oflag_val = 0;
                pipeFds[fd_index] = 'n';    //  not pipe
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
                if ((fds[fd_index] = open(argv[optind-1], O_WRONLY | oflag_val)) == -1) {
                    fprintf(stderr, "Cannot open %s.\n", argv[optind-1]);
                } 
                oflag_val = 0;
                pipeFds[fd_index] = 'n';    //  not pipe
                fd_index++; //  go to next place for save
                break;
            case RDWR:
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index-1]);
                    strcat(verbose_strings, " ");
                    strcat(verbose_strings, argv[index]);
                    printf("%s\n", verbose_strings);
                    memset(verbose_strings, 0, argc * 10 * sizeof(char));
                }
                if ((fds[fd_index] = open(argv[optind-1], O_RDWR | oflag_val)) == -1) {
                    fprintf(stderr, "Cannot open %s.\n", argv[optind-1]);
                }
                oflag_val = 0;
                fd_index++; //  go to next place for save
                break;
            case COMMAND:
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
                 oflag_val = 0;
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
                    fprintf(stderr, "Missing operands for --command\n");
                    exit(1);
                }
                
                //  save optind and modify optind so getopt_long will jump to the next --options (long options)
                old_optind = optind;
                optind = cmd_index;
                
                //  set fds
                cmd_index = old_optind - 1;   //  use this to access std i, o, e args
                
                //  TODO: check strtol, return 0 on error
                //  NOTE: strtol "converts the initial part of the string in nptr to a long integer"
                index_i = strtol(argv[cmd_index], NULL, 10);
                index_o = strtol(argv[cmd_index+1], NULL, 10);
                index_e = strtol(argv[cmd_index+2], NULL, 10);
                if (index_i >= fd_index || index_i < 0) {
                    fprintf(stderr, "Bad file descriptor\n");
                    exit(1);
                }
                if (index_o >= fd_index || index_o < 0) {
                    fprintf(stderr, "Bad file descriptor\n");
                    exit(1);
                }
                if (index_e >= fd_index || index_e < 0) {
                    fprintf(stderr, "Bad file descriptor\n");
                    exit(1);
                }
                
                cpids[cpid_index] = fork();
                if (cpids[cpid_index] == 0) {
                    //  In child
                    
                    //  prepare arg for execvp
                    argv[optind] = '\0';
                    //  NOTE: seems like you can modify argv, which is super sweet, cuz the child get it's own copy
                    
                    //  Note: child need to close unused end of pipe
                    if (pipeFds[index_i] == 'r') {
                        close(fds[index_i+1]);  //  close unused write end of pipe
                    }
                    if (pipeFds[index_o] == 'w') {
                        close(fds[index_o-1]);  //  close unused read end of pipe
                    }
                    
                    //  TODO: this could be buggy? maybe? if the first one fails, none of the rest will be executed
                    if (dup2(fds[index_i], 0) == -1)
                        fprintf(stderr, "dup2 on input %d failed\n", index_i);
                    else if (dup2(fds[index_o], 1) == -1)
                        fprintf(stderr, "dup2 on ouput %d failed\n", index_o);
                    else if (dup2(fds[index_e], 2) == -1)
                        fprintf(stderr, "dup2 on err %d failed\n", index_e);
                    
                    if (execvp(argv[cmd_index+3], &argv[cmd_index+3]) == -1) {  //  cmd_index+3 is where the arg starts
                        fprintf(stderr, "Execvp error %s\n", strerror(errno));
                        exit(1);
                    }
                } else {
                    //  parent
                    //  Note: In parent, we need to close the end of the pipe that's already used by child
                    if (pipeFds[index_i] == 'r') {
                        close(fds[index_i]);
                        fds[index_i] = -1;  //  set this to -1 so we won't close this again
                    }
                    if (pipeFds[index_o] == 'w') {
                        close(fds[index_o]);
                        fds[index_o] = -1;  //  set this to -1 so we won't close this again
                    }
                }
                cpid_index++;
                break;
            case VERBOSE:
                Verbose_ON = TRUE;
                 oflag_val = 0;
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
                 oflag_val = 0;
                break;
            case PIPE:
                if (pipe(pipefd) == -1) {
                    fprintf(stderr, "Failed to open pipe\n");
                } else {
                    //  success
                    fds[fd_index] = pipefd[0];    //  read
                    pipeFds[fd_index++] = 'r';    //  this is the read end
                    fds[fd_index] = pipefd[1];    //  write
                    pipeFds[fd_index++] = 'w';    //  this is the write end
                }
                break;
            case WAIT:
            case PROFILE:
            case ABORT:
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    printf("%s\n", verbose_strings);
                    memset(verbose_strings, 0, argc * 10 * sizeof(char));
                }
                int *a = NULL;
                int t = *a;
            case PAUSE:
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    printf("%s\n", verbose_strings);
                    memset(verbose_strings, 0, argc * 10 * sizeof(char));
                }
                oflag_val = 0;
                break;
            case APPEND:
                oflag_val |= O_APPEND;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case CLOEXEC:
                oflag_val |= O_CLOEXEC;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case CREAT:
                oflag_val |= O_CREAT;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case DIRECTORY:
                oflag_val  |= O_DIRECTORY;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case DSYNC:
                oflag_val  |= O_DSYNC;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case EXCL:
                oflag_val  |= O_EXCL;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case NOFOLLOW:
                oflag_val  |= O_NOFOLLOW;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case NONBLOCK:
                oflag_val  |= O_NONBLOCK;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case RSYNC:
                oflag_val |= O_RSYNC;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case SYNC:
                oflag_val |= O_SYNC;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
                }
                break;
            case TRUNC:
                oflag_val |= O_TRUNC;
                if(Verbose_ON)
                {
                    index = optind - 1;
                    strcat(verbose_strings, argv[index]);
                    strcat(verbose_strings, " ");
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
        //  TODO: we don't need to print this do we??
        //printf("Child number %i (PID %ld) exited with status 0x%x\n", n, (long)pid, status);
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
    free(verbose_strings);
    fds = cpids = NULL;
    verbose_strings = NULL;
    
}

