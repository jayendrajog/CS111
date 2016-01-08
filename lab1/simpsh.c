#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

int
main(int argc, char *argv[])
{
    //int digit_optind = 0;
    int const NUM_OPTIONS = 4;
    int option;
    int option_index = 0;   //  TODO: might need to be reset to 0 for every iteration of the loop
    
    int **flags = malloc(sizeof(int*) * NUM_OPTIONS);
    enum FLAGS {
        RDONLY,
        WRONLY,
        COMMAND,
        VERBOSE
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
                printf("--rdonly is ON\n");
                break;
            case WRONLY:
                printf("--wronly is ON\n");
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