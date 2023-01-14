#ifndef ENG_GLOBALCONFIG_H
#define ENG_GLOBALCONFIG_H
#define SIZE_OF_OUTPUT_BUF 10000
#define SIZE_OF_WORD_BUF 15 /*Максимальная длина слова (Размер буфера для слова)*/
#define SIZE_OF_STR_IN_FILE 82 /*Размер строки в байтах *.ENG файла*/
#define QNT_OF_LEG_STR 14 /*Кол-во строк данных в одном LEG*/
#define SIZE_OF_LEGID_DATA 20
#define DATA_POSITION_1 50
#define DATA_POSITION_2 58
#define DATA_POSITION_ENVIR 65
#define LEG_DATA_FIELD_SIZE 4
#define ENVIR_DATA_FIELD_SIZE 5

#include <stdio.h>

typedef struct field_descr {
    int position;
    size_t length;
} field_descr_t;
extern field_descr_t id_bort_pos;
extern field_descr_t file_date_pos;

typedef struct id_data
{
     field_descr_t legnum_fld;
     field_descr_t time_fld;
     field_descr_t date_fld;
} id_data_t;
extern id_data_t id_data;
#endif