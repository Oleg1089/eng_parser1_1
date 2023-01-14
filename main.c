#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "eng_globalconfig.h"
#include "eng_functions.h"


int main (int argc, char *argv[])
{
    FILE *fd_input, *fd_output;
    char *i_filebufptr, *o_filebufptr;/*Указатели на буфер входного и выходного файла*/
    char *pos_in_o_file_buf;
    int qnt_legs_in_file;
    char data_buffer[SIZE_OF_STR_IN_FILE];
    char bort_id_buf[8];/* Буфер для идентификатора борта */
    field_descr_t id_bort_pos={43, 7};
    // field_descr_t file_date_pos={64, 7};
    int num_first_datas_str=7;/* Номер первой строки данных */
    int current_str=7;
    int current_leg=0;
    id_data_t id_data={{0, 5}, {6, 5}, {12, 7}};

if (argc<2){
        printf("Отсутствует параметр имени файла для обработки\n");
        return 1;
    }
    fd_input=fopen(argv[1],"rb");
    if (fd_input==NULL){
        printf("%s - Файл не существует\n", argv[1]);
        return 2;
    }
   
    i_filebufptr=readfile_in_buf(fd_input);
    if (i_filebufptr==NULL){
        fclose(fd_input);
        return 3;
    }
    
    qnt_legs_in_file=
        (sizeof_file(fd_input)/SIZE_OF_STR_IN_FILE-num_first_datas_str)/
        QNT_OF_LEG_STR;
    get_bort_id(0, &id_bort_pos, i_filebufptr, bort_id_buf);
    o_filebufptr=pos_in_o_file_buf=malloc(SIZE_OF_OUTPUT_BUF);

    /* Основной цикл обработки */
    while (current_leg<qnt_legs_in_file)
    {
        strcpy(pos_in_o_file_buf,bort_id_buf);
        pos_in_o_file_buf+=sizeof(bort_id_buf)-1;
        *pos_in_o_file_buf=';';
        pos_in_o_file_buf++;
        strcpy(pos_in_o_file_buf,
            get_leg_data(&id_data, i_filebufptr,current_str));
        pos_in_o_file_buf+=SIZE_OF_LEGID_DATA;
        strcpy(pos_in_o_file_buf,"LEng;");
        pos_in_o_file_buf+=5;
        strcpy(pos_in_o_file_buf, get_datas(current_str, DATA_POSITION_1,
            i_filebufptr, data_buffer));
        pos_in_o_file_buf+=strlen(data_buffer);
        strcpy(pos_in_o_file_buf,bort_id_buf);
        pos_in_o_file_buf+=sizeof(bort_id_buf)-1;
        *pos_in_o_file_buf=';';
        pos_in_o_file_buf++;
        strcpy(pos_in_o_file_buf,
            get_leg_data(&id_data, i_filebufptr,current_str));
        pos_in_o_file_buf+=SIZE_OF_LEGID_DATA;
        strcpy(pos_in_o_file_buf,"REng;");
        pos_in_o_file_buf+=5;
        strcpy(pos_in_o_file_buf, get_datas(current_str, DATA_POSITION_2,
            i_filebufptr, data_buffer));
        pos_in_o_file_buf+=strlen(data_buffer);
        current_str+=QNT_OF_LEG_STR;
        current_leg++;
    }
        fd_output=fopen("test.csv", "wb");
        fwrite(o_filebufptr, 1, pos_in_o_file_buf-o_filebufptr, fd_output);
    printf("%2i:\tПолетов обработано.\n", current_leg);

/*Приборка мусора за собой*/
    free(i_filebufptr);
    free(o_filebufptr);
    fclose(fd_input);
    fclose(fd_output);
    return 0;
}