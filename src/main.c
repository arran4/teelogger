//
//  main.c
//  teelogger
//
//  Created by Arran Ubels on 25/01/13.
//  Copyright (c) 2013 Arran Ubels. All rights reserved.
//

#include "logger.h"
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
    fprintf(stdout, "  -f <file>   Specify the output file to log to.\n");
    fprintf(stdout, "  -s <size>   Specify the maximum log size in bytes before rotating.\n\n");
    fprintf(stdout, "Report bugs to <arran.ubels@example.com>.\n");
}

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    // Change working directory to root so we don't hold on to mounted directories
    if (chdir("/") < 0) {
        // Not critical, continue
    }

    // Close standard file descriptors. Since we still need to process standard input,
    // we do not close STDIN. STDOUT should also not be closed if we want to act like tee,
    // but in background mode, standard input/output are usually redirected or closed.
    // However, if we're a tee logger, maybe background mode only outputs to the file?
    // Let's redirect standard output and standard error to /dev/null if daemonized.
    int devnull = open("/dev/null", O_RDWR);
    if (devnull >= 0) {
        dup2(devnull, STDOUT_FILENO);
        dup2(devnull, STDERR_FILENO);
        close(devnull);
    }
}

int main(int argc, char *const *argv) {
    int bflag = 0;
    int ch;
    int fd = -1;
    size_t max_log_size = 0;
    const char *filepath = NULL;
    const char *prog_name = (argc > 0 && argv[0] != NULL) ? argv[0] : "teelogger";

    // Parse command line arguments
    while ((ch = getopt(argc, argv, "hbf:s:")) != -1) {
        switch (ch) {
        case 'h':
            usage(prog_name);
            exit(EXIT_SUCCESS);
        case 'b':
            bflag = 1;
            break;
        case 'f':
            filepath = optarg;
            if ((fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0) {
                fprintf(stderr, "%s: Error opening file '%s': %s\n", prog_name, optarg,
                        strerror(errno));
                exit(EXIT_FAILURE);
            }
            break;
        case 's':
            max_log_size = (size_t)atoll(optarg);
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

    if (bflag) {
        daemonize();
    }

    char buffer[4096];
    ssize_t bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        // Write to stdout (if not daemonized and redirected, it goes to console)
        ssize_t written = 0;
        while (written < bytes_read) {
            ssize_t result = write(STDOUT_FILENO, buffer + written, bytes_read - written);
            if (result < 0) {
                if (errno == EINTR)
                    continue;
                break;
            }
            written += result;
        }

        // Write to log file
        if (fd != -1) {
            written = 0;
            while (written < bytes_read) {
                ssize_t result = write(fd, buffer + written, bytes_read - written);
                if (result < 0) {
                    if (errno == EINTR)
                        continue;
                    break;
                }
                written += result;
            }

            // Check if rotation is needed
            if (max_log_size > 0 && filepath != NULL) {
                check_and_rotate_log(filepath, &fd, max_log_size);
            }
        }
    }

    if (fd != -1) {
        close(fd);
    }

    return EXIT_SUCCESS;
}
