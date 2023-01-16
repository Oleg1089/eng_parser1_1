
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "eng_globalconfig.h"

static char *month_names[]={
    "JAN", "FEB", "MAR",
    "APR", "MAY", "JUN",
    "JUL", "AUG", "SEP",
    "OCT", "NOV", "DEC"
};

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

char *get_id_data(const int str_num, const field_descr_t *id_data,
    const char *file_buf, char *buffer)
{
    return read_word_from_file_buf(str_num, id_data->position, id_data->length,
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

char* substr(const char* s, size_t pos, size_t count)
{
   static char buf[BUFSIZ];
   if ( count >= BUFSIZ )
      return NULL;
   else
   buf[count] = '\0';
      return strncpy(buf, s + pos, count);
}

char *convertdate_to_yy_mm_dd(const char *ddMMMyy, char * yy_mm_dd)
{
    int i=0;
    strcpy(yy_mm_dd, substr(ddMMMyy, 5, 2));
    strcat(yy_mm_dd, "_");
    char *subbuf=substr(ddMMMyy, 2, 3);
    while (*subbuf != *month_names[i]) {i++;}
    i++;
    if (i<10) {
        i+='0';
        strcat(yy_mm_dd, "0");
        strcat(yy_mm_dd, (char *)&i);
    } else {
        i-=10; i+='0';
        strcat(yy_mm_dd, "1");
        strcat(yy_mm_dd, (char *)&i);
    }
    strcat(yy_mm_dd, "_");
    return strcat(yy_mm_dd,substr(ddMMMyy, 0, 2));
}

char *find_file_name(const char *full_path)
{
    static char outbuf[BUFSIZ];
    char *curcharptr, *charptr=NULL;
    strcpy(outbuf, full_path);
    curcharptr=strtok(outbuf, "/\\");
    while (curcharptr != NULL) {
        charptr=curcharptr;
        curcharptr=strtok(NULL, "\\");
    }
    return charptr;
}

char *full_file_name_form(const char *full_path, const char *ddMMMyy_date, const char *suffix)
{
    static char full_path_buf[BUFSIZ];
    size_t path_str_len, file_name_len;
    char *file_name=find_file_name(full_path);
    file_name_len=strlen(file_name);
    path_str_len=strlen(full_path)-file_name_len;
    strncpy(full_path_buf, full_path, path_str_len);
    convertdate_to_yy_mm_dd(ddMMMyy_date, &full_path_buf[path_str_len]);
    strcat(full_path_buf, "_");
    strcpy(file_name+file_name_len-3, suffix);
    return strcat(full_path_buf, file_name);
}
// void test(char *filebuf)
// {
//     printf("%s\n", get_leg_data(&id_data,filebuf, 7));
//     printf("%s\n", get_id_data(0, &id_bort_pos, filebuf, bort_id_buf));
//     printf("%s\n", get_datas(7, DATA_POSITION_1, filebuf, data_buffer));
//     printf("%s\n", get_datas(7, DATA_POSITION_2, filebuf, data_buffer));
// }