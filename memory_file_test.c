//
// Created by winxo on 2016-12-3.
//

#include "mfile.h"
int main()
{
    init_file_system();
    u8 file1[5]="hell";
    write_file("f1",file1,5);
    u8 buf[5];
    read_file("f1",buf);
    delete_file("f1");
    read_file("f1",buf);
    write_file("a22","goodday",8);
    return 0;
}