//
//  main.c
//  teelogger
//
//  Created by Arran Ubels on 25/01/13.
//  Copyright (c) 2013 Arran Ubels. All rights reserved.
//

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(const char *prog_name) {
    fprintf(stdout, "Usage: %s [OPTIONS]...\n", prog_name);
    fprintf(stdout, "Log rotation and tee built into one app.\n\n");
    fprintf(stdout, "Options:\n");
    fprintf(stdout, "  -h          Print this help message and exit.\n");
    fprintf(stdout, "  -b          Run in background mode (daemonize).\n");
    fprintf(stdout, "  -f <file>   Specify the output file to log to.\n\n");
    fprintf(stdout, "Report bugs to <arran.ubels@example.com>.\n");
}

void beginlogging() {
    printf("Starting logging...\n");
}

int main(int argc, char *const *argv) {
    int bflag = 0;
    int ch;
    int fd = -1;
    const char *prog_name = argv[0];

    // Parse command line arguments
    while ((ch = getopt(argc, argv, "hbf:")) != -1) {
        switch (ch) {
        case 'h':
            usage(prog_name);
            exit(EXIT_SUCCESS);
        case 'b':
            bflag = 1;
            printf("Background mode enabled.\n");
            break;
        case 'f':
            if ((fd = open(optarg, O_RDONLY, 0)) < 0) {
                fprintf(stderr, "%s: Error opening file '%s': %s\n", prog_name, optarg,
                        strerror(errno));
                exit(EXIT_FAILURE);
            }
            printf("Output file set to '%s' (fd: %d).\n", optarg, fd);
            break;
        case '?':
            fprintf(stderr, "Try '%s -h' for more information.\n", prog_name);
            exit(EXIT_FAILURE);
        default:
            usage(prog_name);
            exit(EXIT_FAILURE);
        }
    }

    argc -= optind;
    argv += optind;

    beginlogging();

    if (fd != -1) {
        close(fd);
    }

    return EXIT_SUCCESS;
}
