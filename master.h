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

int is_reg_file(const char * path);
void create_queue(int number_files, key_t queue_key);
int slave_number_calc(int number_files);
void create_slaves(int number_files);

void run(int argc, const char ** argv);
int post_files(int number_files, int argc, const char ** argv, int parameters_offset, key_t queue_key);

#endif
