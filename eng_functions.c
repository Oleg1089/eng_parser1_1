
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "eng_globalconfig.h"


off_t sizeof_file(FILE *fd_)
{
    struct stat st_f;
    if (fstat(fileno(fd_), &st_f)==-1)
        return -1;
    return st_f.st_size;
}


void *readfile_in_buf(FILE *fd_)
{
    void *fileptr;
    off_t filesize;
    filesize=sizeof_file(fd_);
    if ((fileptr=malloc(filesize))==NULL){
        printf("Ошибка выделения памяти для буфера файла");
        return NULL;
    }
    rewind(fd_);
    if (!(fread(fileptr, filesize,1, fd_))){
        printf("Ошибка чтения файла в буфер\n");
        return NULL;
    }
    return fileptr;
}

char *read_string_in_buf(const char *file_buf, const int str_num)
{
    static char str_buf[SIZE_OF_STR_IN_FILE];
    int str_ptr=str_num*SIZE_OF_STR_IN_FILE;
    for (int i=0; i<SIZE_OF_STR_IN_FILE; i++) {
        str_buf[i]=file_buf[str_ptr];
        str_ptr++;
    }
    return str_buf;
}

char *read_word_from_string(
    const char *string_buf, 
    const int begin_position,
    const size_t len_of_word)
{
    static char word_ptr[SIZE_OF_WORD_BUF+1];
    strncpy(word_ptr, &string_buf[begin_position], len_of_word);
    word_ptr[len_of_word]='\000';
    return word_ptr;
}

char *read_word_from_file_buf(const int str_num, const int word_position, 
        const size_t word_length, const char *file_buf, char *word_buf)
{
    char *begin_ptr=(char *)(SIZE_OF_STR_IN_FILE*str_num+file_buf);
    strncpy(word_buf, begin_ptr+word_position, word_length);
    word_buf[word_length]='\000';
    return word_buf;
}

char *get_leg_data(const id_data_t *id_data, const char *file_buf, 
    const int str_num)
{
    static char leg_data[SIZE_OF_LEGID_DATA+1];
    int pos;
    size_t length;
    field_descr_t *datas=(field_descr_t*)id_data;
    char *current_ptr=leg_data;
    for (size_t i=0; i<sizeof(id_data_t); i+=sizeof(field_descr_t)){
    pos=datas->position;
    length=datas->length;
    read_word_from_file_buf(str_num, pos, length, file_buf, current_ptr);
    current_ptr+=length;
    *current_ptr=';';
    current_ptr++;
    datas++;
    }
    *current_ptr='\000';
    return leg_data;
}

char *get_bort_id(const int str_num, const field_descr_t *id_bort,
    const char *file_buf, char *buffer)
{
    return read_word_from_file_buf(str_num, id_bort->position, id_bort->length,
        file_buf, buffer);
}

void remove_spaces(char * str)
{
    char *current_ptr=str;
    do{
        while(*current_ptr==' '){
            ++current_ptr;
        }
    } while (*str++=*current_ptr++);
}

char *get_datas(const int beginingstr_num, const int data_pos, const char *filebuf,
    char *buffer)
{
    char *current_ptr=buffer;
    size_t i = beginingstr_num;
    for (i=i; i < beginingstr_num+QNT_OF_LEG_STR-3; i++)
    {
        read_word_from_file_buf(i, data_pos,  LEG_DATA_FIELD_SIZE, filebuf, current_ptr);
        current_ptr+=LEG_DATA_FIELD_SIZE;
        *current_ptr=';';
        current_ptr++;
    }
    for (i=i; i<beginingstr_num+QNT_OF_LEG_STR; i++)
    {
        read_word_from_file_buf(i, DATA_POSITION_ENVIR, ENVIR_DATA_FIELD_SIZE,
            filebuf, current_ptr);
                current_ptr+=ENVIR_DATA_FIELD_SIZE;
        *current_ptr=';';
        current_ptr++;
    }
    *--current_ptr='\r';
    *++current_ptr='\n';
    *++current_ptr='\000';
    remove_spaces(buffer);
    return buffer;
}

char *file_name_forming(const char *filebuf,field_descr_t *file_date_pos,
    char *o_file_name)
{
    char file_date[file_date_pos->length+1];
    read_word_from_file_buf(0, file_date_pos->position, file_date_pos->length,
        filebuf, file_date);
    
}
// void test(char *filebuf)
// {
//     printf("%s\n", get_leg_data(&id_data,filebuf, 7));
//     printf("%s\n", get_bort_id(0, &id_bort_pos, filebuf, bort_id_buf));
//     printf("%s\n", get_datas(7, DATA_POSITION_1, filebuf, data_buffer));
//     printf("%s\n", get_datas(7, DATA_POSITION_2, filebuf, data_buffer));
// }