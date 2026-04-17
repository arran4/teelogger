//
//  logger.h
//  teelogger
//
//  Created by Arran Ubels on 25/01/13.
//  Copyright (c) 2013 Arran Ubels. All rights reserved.
//

#ifndef teelogger_logger_h
#define teelogger_logger_h

#include <stddef.h>

void check_and_rotate_log(const char *filepath, int *fd, size_t max_log_size);

#endif
