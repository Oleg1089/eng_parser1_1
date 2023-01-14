#ifndef ENGDATAPARSER_REFACTSTRPARS_ENGFUNCTIONS_H_
#define ENGDATAPARSER_REFACTSTRPARS_ENGFUNCTIONS_H_

#include <stdio.h>
#include "eng_globalconfig.h"

void *readfile_in_buf(FILE *fd_);
char *get_bort_id(const int str_num, const field_descr_t *id_bort,
    const char *file_buf, char *buffer);
off_t sizeof_file(FILE *fd_);
char *get_leg_data(const id_data_t *id_data, const char *file_buf, 
    const int str_num);
char *get_datas(const int beginingstr_num, const int data_pos, const char *filebuf,
    char *buffer);

#endif