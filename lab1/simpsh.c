#include <stdlib.h>
#include <stdio.h>
#include <getopt.h> //  parse
#include <fcntl.h>  //  open

int
main(int argc, char *argv[])
{
    //int digit_optind = 0;
    int const NUM_OPTIONS = 4;
    int option; //  TODO: don't know what this is for
    int option_index = 0;   //  TODO: might need to be reset to 0 for every iteration of the loop
    int fd_index = 0;
    
    int **flags = malloc(sizeof(int*) * NUM_OPTIONS);
    int *fds = malloc(sizeof(int) * argc);    //  TODO: will allocate too much memory
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
                    printf("%s is successfully opened with --rdonly option!\n", argv[optind-1]);
                }
                fd_index++; //  go to next place for save
                break;
            case WRONLY:
                //printf("--wronly is ON\n");
                if ((fds[fd_index] = open(argv[optind-1], O_WRONLY)) == -1) {
                    fprintf(stderr, "Cannot open %s.\n", argv[optind-1]);
                    //exit(1);
                } else {
                    printf("%s is successfully opened with --wronly option!\n", argv[optind-1]);
                }
                fd_index++; //  go to next place for save
                break;
                break;
            case COMMAND:
                printf("--command is ON\n");
                break;
            case VERBOSE:
                printf("--verbose is ON\n");
                break;
            default:
                printf("OOPS\n");
                break;
        }
        
    }
    
    
    
    
//    int flags, opt;
//    int nsecs, tfnd;
//    
//    nsecs = 0;
//    tfnd = 0;
//    flags = 0;
//    
//    while ((opt = getopt(argc, argv, "nt:")) != -1) {
//        switch (opt) {
//            case 'n':
//                flags = 1;
//                break;
//            case 't':
//                nsecs = atoi(optarg);
//                tfnd = 1;
//                break;
//            default: /* '?' */
//                fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
//                        argv[0]);
//                exit(EXIT_FAILURE);
//        }
//    }
//    
//    printf("flags=%d; tfnd=%d; optind=%d\n", flags, tfnd, optind);
//    
//    if (optind >= argc) {
//        fprintf(stderr, "Expected argument after options\n");
//        exit(EXIT_FAILURE);
//    }
//    
//    printf("name argument = %s\n", argv[optind]);
//    
//    /* Other code omitted */
//    
//    exit(EXIT_SUCCESS);
}
