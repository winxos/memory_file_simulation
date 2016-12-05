//
// Created by winxo on 2016-12-3.
//

#ifndef MEMORY_FILE_SIMULATION_MFILE_H
#define MEMORY_FILE_SIMULATION_MFILE_H

#include <stdlib.h>


typedef unsigned char u8;
typedef unsigned short u16;

enum RETURN_CODE {
    SUCCESS, FILE_NOT_EXIST, FILE_ALREADY_EXIST, DISK_NOT_ENOUGH, DISK_FULL,
};
enum FILE_STATE {
    NORMAL = 0x01, READ_ONLY = 0x02, HIDE = 0x04, OPENING = 0x08, DELETED = 0x10
};
#define MAX_NAME_LEN 8

struct FILE_INFO {
    char name[MAX_NAME_LEN];
    struct FILE_INFO *next; //pre created file's address in table
    u16 start_address;
    u16 size;
    u8 state; //normal, read only, hide, opening delete?
    u8 xor_sum; //
};
typedef struct FILE_INFO FILE_INFO;
typedef FILE_INFO *pFILE_INFO;

typedef struct {
    u8 fs_type; //file system type
    u8 fs_version; //for update migrate
    u8 sector_size; //default size is 1, for future use.
    u8 fs_file_table_address; //can added copy to table2 for safety
    char logo[8];
    u16 fs_file_start_address;
    u16 future;
} FS_HEAD; //16 byte

int init_file_system();

int read_file(char name[], u8 buf[]);

int write_file(char name[], u8 buf[], u8 len);

int delete_file(char name[]);

#endif //MEMORY_FILE_SIMULATION_MFILE_H
