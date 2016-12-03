//
// Created by winxo on 2016-12-3.
//

#ifndef MEMORY_FILE_SIMULATION_MFILE_H
#define MEMORY_FILE_SIMULATION_MFILE_H

#include <stdlib.h>


typedef unsigned char u8;
typedef unsigned short u16;

enum RETURN_CODE {
    SUCCESS, FILE_NOT_EXIST, FILE_ALREADY_EXIST
};

#define MAX_NAME_LEN 8

typedef struct {
    char name[MAX_NAME_LEN];
    u8 start_address;
    u8 size;
    u8 pre_file_table_loc; //pre created file's address in table
    u8 xor_sum; //whole struct
} FILE_INFO, *pFILE_INFO; //12 byte

typedef struct {
    u8 fs_type; //file system type
    char logo[8];
    u8 fs_version; //for update migrate
    u8 sector_size; //default size is 1, for future use.
    u8 fs_file_table_address; //can added copy to table2 for safety
    u16 fs_file_start_address;
    u8 future;
} FS_HEAD; //15 byte

int init_file_system();

int read_file(char name[], u8 buf[]);

int write_file(char name[], u8 buf[]);

int delete_file(char name[], u8 buf[]);

#endif //MEMORY_FILE_SIMULATION_MFILE_H
