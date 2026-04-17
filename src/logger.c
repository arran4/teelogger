//
//  logger.c
//  teelogger
//
//  Created by Arran Ubels on 25/01/13.
//  Copyright (c) 2013 Arran Ubels. All rights reserved.
//

#include "logger.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void check_and_rotate_log(const char *filepath, int *fd, size_t max_log_size) {
    if (*fd == -1 || max_log_size == 0) {
        return;
    }

    struct stat st;
    if (fstat(*fd, &st) == -1) {
        fprintf(stderr, "Error stating file descriptor: %s\n", strerror(errno));
        return;
    }

    if (st.st_size >= (off_t)max_log_size) {
        close(*fd);

        char new_filepath[1024];
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        snprintf(new_filepath, sizeof(new_filepath), "%s.%04d%02d%02d-%02d%02d%02d", filepath,
                 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        if (rename(filepath, new_filepath) != 0) {
            fprintf(stderr, "Error renaming file '%s' to '%s': %s\n", filepath, new_filepath,
                    strerror(errno));
            // Try reopening original file anyway
            *fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
            return;
        }

        *fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (*fd < 0) {
            fprintf(stderr, "Error opening new log file '%s': %s\n", filepath, strerror(errno));
        }
    }
}
