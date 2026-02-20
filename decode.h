#include<stdio.h>
#ifndef DECODE_H
#define DECODE_H

#include "types.h"


typedef struct _DecodeInfo
{
    /* data */
    //stego image info
    char *stego_image_fname;
    FILE *fptr_stego_image;
    
    //output file info
    char *optr;
    FILE *out_ptr;
    int size_of_extn;
    int size_of_file;

} DecodeInfo;

//Function prototypes

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status open_img_file(DecodeInfo *decInfo);

Status skip_bmp_header(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

char decode_byte_from_lsb(char*image_buffer);

int decode_int_from_lsb(char *image_buffer);

Status compare_files(const char *file1, const char *file2);


#endif