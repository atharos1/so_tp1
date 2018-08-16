#ifndef _MASTER_H
#define _MASTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>

#define ID 57526
#define MASTER_QUEUE_ID 1
#define SLAVE_QUEUE_ID 2
#define READ 0
#define WRITE 1

int is_reg_file(const char * path);
int slave_number_calc(int number_files);
int create_slaves(int number_files);

void run(int argc, const char ** argv);
int post_files(int number_files, int argc, const char ** argv, int parameters_offset);

#endif
