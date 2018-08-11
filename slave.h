#ifndef _SLAVE_H
#define _SLAVE_H

#define ID 57526
#define MASTER_QUEUE_ID 1
#define SLAVE_QUEUE_ID 2
#define MD5_MAX_LENGHT 32

int calculate_MD5(const char * path, char * buffer);

#endif