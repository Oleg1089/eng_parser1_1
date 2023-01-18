#ifndef ENGDATAPARSER_REFACTSTRPARS_ENGFUNCTIONS_H_
#define ENGDATAPARSER_REFACTSTRPARS_ENGFUNCTIONS_H_

#include <stdio.h>
#include "eng_globalconfig.h"

void *readfile_in_buf(FILE *fd_);
char *get_id_data(const int str_num, const field_descr_t *id_data,
    const char *file_buf, char *buffer);
off_t sizeof_file(FILE *fd_);
char *get_leg_data(const id_data_t *id_data, const char *file_buf, 
    const int str_num);
char *get_datas(const int beginingstr_num, const int data_pos, const char *filebuf,
    char *buffer);
char *full_file_name_form(const char *full_path, const char *ddMMMyy_date,
    const char *leg_id, const char *suffix);
char *get_last_leg(const int str_num, const char *cmp_word,
    const field_descr_t *cmp_word_pos, const char *filebuf, char *last_leg_buf);
#endif