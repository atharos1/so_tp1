#ifndef _MASTER_H
#define _MASTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include "./Tests/test_master.h"

#define READ 0
#define WRITE 1
#define TEST 1
#define NORMAL 0
#define FILES_SLAVES_RATIO 10
#define SLAVE_LIMIT 15

int is_valid(char * path);
int slave_number_calc(int number_files);
int create_slaves(int number_files);
void kill_slaves(int n);

void run(int argc, char ** argv, int mode);
void run_test_mode();
int post_files(int number_files, int argc, char ** argv, int parameters_offset);

#endif
