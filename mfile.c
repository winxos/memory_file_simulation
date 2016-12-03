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

const u8 file_xor_sum_offset = 0xE5;
pFILE_INFO file_root, file_tail;

u8 calc_xor_sum(pFILE_INFO f) {
    u8 xor_sum = file_xor_sum_offset;
    u8 *p = (u8 *) f;
    for (int i = 0; (p + i) < &f->xor_sum; i++) {
        xor_sum ^= *(p + i);
    }
    return xor_sum;
}

int is_valid_file(pFILE_INFO f) {
    if (f->start_address < fs_head.fs_file_start_address)
        return 0;
    return calc_xor_sum(f) == f->xor_sum;
}

pFILE_INFO to_pFILE_INFO(int address) {
    return (pFILE_INFO) &vfbuf[address];
}


pFILE_INFO find_file(char name[]) {
    pFILE_INFO f = file_root->next;
    while (is_valid_file(f)) {
        if (0 == strcmp(f->name, name)) {
            return f;
        }
        f = f->next;
    }
    return NULL;
}

pFILE_INFO get_empty_file() {
    if (file_root->next == NULL)
        return file_root + 1;
    if (!is_valid_file(file_tail)) {
        return file_tail;
    }
    if (!is_valid_file(file_tail)) {
        return file_tail + 1;
    }
    pFILE_INFO f = file_root->next;
    while (is_valid_file(f)) {
        f++;
    }
    return f;
}

int init_file_system() {
    memcpy(vfbuf, &fs_head, sizeof(fs_head));
    memset(to_pFILE_INFO(fs_head.fs_file_table_address), 0, sizeof(FILE_INFO));
    file_root = to_pFILE_INFO(fs_head.fs_file_table_address);
    file_tail = get_empty_file();
    file_tail->start_address = fs_head.fs_file_start_address;
    file_root->next = file_tail;
    return SUCCESS;
}

int read_file(char name[], u8 buf[]) {
    pFILE_INFO f = find_file(name);
    if (f == NULL)
        return FILE_NOT_EXIST;
    memcpy(buf, to_pFILE_INFO(f->start_address), f->size);
    return SUCCESS;
}

int write_file(char name[], u8 buf[], u8 len) {
    pFILE_INFO f = find_file(name);
    if (f != NULL) {
        return FILE_ALREADY_EXIST;
    }
    f = file_tail;
    f->size = len;
    f->xor_sum = calc_xor_sum(f);
    file_tail = get_empty_file();
    f->next = file_tail;
    memcpy(f->name, name, strlen(name));
    f->xor_sum = calc_xor_sum(f);
    file_tail->start_address = f->start_address + f->size;
    memcpy(to_pFILE_INFO(f->start_address), buf, f->size);
}

int delete_file(char name[]) {
    pFILE_INFO f = file_root;
    while (1) {
        if (0 == strcmp(f->next->name, name)) {
            f->next->start_address = 0;//invalid f-next
            f->next = f->next->next;
            break;
        }
        f = f->next;
        if (!is_valid_file(f)) {
            return FILE_NOT_EXIST;
        }
    }
    return SUCCESS;
}