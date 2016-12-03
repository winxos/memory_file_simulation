//
// Created by winxo on 2016-12-3.
//

#include "mfile.h"
#include <memory.h>
#include <stdio.h>

FS_HEAD fs_head = {
        .fs_type=0x01,
        .logo="AISTLFS",
        .fs_version=0x01,
        .sector_size=0x01,
        .fs_file_table_address=0x10,
        .fs_file_start_address=0x0100,
};
#define MAX_FILE_ADDRESS 10000
#define MAX_FILE_COUNT 20
u8 vfbuf[MAX_FILE_ADDRESS];
int new_file_address;
const u8 file_xor_sum_offset = 0xE5;

int is_valid_file(pFILE_INFO f) {
    if (f->start_address < fs_head.fs_file_start_address)
        return 0;
    u8 xor_sum = file_xor_sum_offset;
    u8 *p = (u8 *) f;
    for (int i = 0; i < sizeof(FILE) - 1; i++) {
        xor_sum ^= *(p + i);
    }
    return xor_sum == f->xor_sum;
}

pFILE_INFO to_pFILE_INFO(int address) {
    return &vfbuf[address];
}

pFILE_INFO find_file(char name[]) {
    pFILE_INFO f = to_pFILE_INFO(fs_head.fs_file_start_address);
    if (is_valid_file(f)) {
        return f;
    }
    return NULL;
}

int init_file_system() {
    memcpy(vfbuf, &fs_head, sizeof(fs_head));
    memset(to_pFILE_INFO(fs_head.fs_file_table_address), 0, sizeof(FILE));
    new_file_address = fs_head.fs_file_start_address;
    return SUCCESS;
}

int read_file(char name[], u8 buf[]) {
    pFILE_INFO f = find_file(name);
    if (f != NULL) {
        memcpy(buf, to_pFILE_INFO(f->start_address), f->size);
    } else {
        return FILE_NOT_EXIST;
    }
    return SUCCESS;
}

int write_file(char name[], u8 buf[]) {
    pFILE_INFO f = find_file(name);
    if (f != NULL) {
        return FILE_ALREADY_EXIST;
    }

}

int delete_file(char name[], u8 buf[]) {
    pFILE_INFO f = find_file(name);
    if (f != NULL) {
        f->start_address = 0;
        pFILE_INFO pre_f = to_pFILE_INFO(f->pre_file_table_loc);
    } else {
        return FILE_NOT_EXIST;
    }
    return SUCCESS;
}