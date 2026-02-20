#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/*read and validate decode arguements*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char *ret = strstr(argv[2],".bmp");
    if(ret == NULL)
    {
        printf(" ERROR: Invalid stego image extension! file name must end with .bmp extn.\n");
        return e_failure;
    }
    else if(strcmp(ret,".bmp")!=0)
    {
        printf(" ERROR: Invalid stego image extension! file name must end with .bmp extn.\n");
        return e_failure;
    }
    else
    {
        
        printf(" INFO: Stego image has valid .bmp extension.\n");
        decInfo->stego_image_fname = argv[2];
    }

    if(argv[3] == NULL)
    {
        printf(" INFO: No output filename provided. Using default: output\n");
        decInfo->optr = malloc(50);   // allocate memory
        strcpy(decInfo->optr, "output");
    }
    else
    {
        decInfo->optr = malloc(strlen(argv[3]) + 10);
        strcpy(decInfo->optr, argv[3]);
    }
    printf(" Arguments validated successfully\n\n");
    return e_success;
}

/*do encoding*/
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_img_file(decInfo) == e_success)
    {
        printf(" INFO: files opened.\n");
        printf("\n----------Decoding Procedure Started ----------\n\n");
        if(skip_bmp_header(decInfo) == e_success)
        {
            printf(" INFO: Skipped bmp header\n");
            if(decode_magic_string(decInfo) == e_success)
            {
                printf(" INFO: Magic string decoded.\n");
                if(decode_file_extn_size(decInfo) == e_success)
                {
                    printf(" INFO: File extension size decoded. \n");
                    if(decode_secret_file_extn(decInfo) == e_success)
                    {
                            printf(" INFO: Extension decoded.\n");
                            if(decode_secret_file_size(decInfo) == e_success)
                            {
                                printf(" INFO: secret file size decoded.\n");
                                if(decode_secret_file_data(decInfo) == e_success)
                                {
                                    printf("\n INFO: secret file data decoded.\n");

                                    /* FILE COMPARE */
                                    compare_files("secret.txt", decInfo->optr);
                                    return e_success;
                                }
                                else
                                {
                                    printf("ERROR:  Decode secret file data failed.\n");
                                    return e_failure;
                                }

                            }
                            else
                            {
                                printf("ERROR:  Decode secret file size failed.\n");
                                return e_failure;
                            }

                    }
                    else
                    {
                        printf("ERROR:  Encode secret file extension failed.\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR:  Encode file extension size failed.\n");
                    return e_failure;

                }
                
            }
            else
            {
                printf("ERROR:  Magic string encoding failed.\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR:  skipping bmp header failed\n");
            return e_failure;
        }

    }
    else
    {
        printf("ERROR:  opening files failed\n");
        return e_failure;
    }
}

/*open image file*/
Status open_img_file(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image =  fopen(decInfo->stego_image_fname ,"rb");
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }
    printf(" INFO : Opened stego.bmp\n");
    return e_success;
}

/*skip bmp header*/
Status skip_bmp_header(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    return e_success;
}

/*decode magic string*/
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string[3];
    char image_buffer[9];
    int i;
    for( i = 0;i<strlen(MAGIC_STRING);i++)
    {
        fread(image_buffer,8,1,decInfo->fptr_stego_image);
        magic_string[i] = decode_byte_from_lsb(image_buffer);
    }
    magic_string[i] = '\0';
    if(strcmp(magic_string,MAGIC_STRING) ==0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*decode byte from lsb*/
char decode_byte_from_lsb(char*image_buffer)
{
    char ch = 0;

    for (int i = 0; i < 8; i++)
    {
        unsigned int bit = image_buffer[i] & 1;  // extract LSB
        ch = ch | (bit << (7-i));                    // place it from MSB position
    }
    return ch;
}

/*decode size from lsb*/
int decode_int_from_lsb(char *image_buffer)
{
    int ch = 0;

    for (int i = 0; i < 32; i++)
    {
        unsigned int bit = image_buffer[i] & 1;  // extract LSB
        ch = ch | (bit << (31-i));                    // place it from MSB position
    }
    return ch;
}

/*decode secret file extn size*/
Status decode_file_extn_size(DecodeInfo * decInfo)
{
    char image_buffer[32];
    fread(image_buffer,32,1,decInfo->fptr_stego_image);
    decInfo->size_of_extn = decode_int_from_lsb(image_buffer);
    //printf("%d\n",decInfo->size_of_extn);
    return e_success;
}

/*decode secret file extn*/
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char Extn[decInfo->size_of_extn+1];
    char image_buffer[8];
    int i;
    for(i = 0;i<decInfo->size_of_extn;i++)
    {
        fread(image_buffer,8,1,decInfo->fptr_stego_image);
        Extn[i] = decode_byte_from_lsb(image_buffer);
    }
    Extn[i] = '\0';
    strcat(decInfo->optr,Extn);
    //printf("%s",decInfo->optr);
    decInfo->out_ptr = fopen(decInfo->optr,"w");
    if(decInfo->out_ptr == NULL)
    {
        printf("fopen failed\n");
        return e_failure;
    }

    return e_success;
}

/*decode secret file size*/
Status decode_secret_file_size(DecodeInfo * decInfo)
{
    char image_buffer[32];
    fread(image_buffer,32,1,decInfo->fptr_stego_image);
    decInfo->size_of_file = decode_int_from_lsb(image_buffer);
    //printf("%d",decInfo->size_of_file);
    return e_success;
}

/*decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    int i;
    char image_buffer[8];
    for( i = 0;i<decInfo->size_of_file;i++)
    {
    fread(image_buffer,8,1,decInfo->fptr_stego_image);
    ch = decode_byte_from_lsb(image_buffer);
    fwrite(&ch,1,1,decInfo->out_ptr);
    printf("%c",ch);
    }

    return e_success;
}

/*compare both secret.txt and output.txt*/
Status compare_files(const char *file1, const char *file2)
{
    FILE *fp1 = fopen(file1, "rb");
    FILE *fp2 = fopen(file2, "rb");

    if (!fp1 || !fp2)
    {
        printf("ERROR: Unable to open files for comparison\n");
        return e_failure;
    }

    int ch1, ch2;
    int difference_found = 0;

    while ((ch1 = fgetc(fp1)) != EOF && (ch2 = fgetc(fp2)) != EOF)
    {
        if (ch1 != ch2)
        {
            difference_found = 1;
            break;
        }
    }

    fclose(fp1);
    fclose(fp2);

    if (difference_found == 0)
    {
        printf("RESULT: Files are SAME. Decoding SUCCESS.\n");
        return e_success;
    }
    else
    {
        printf("RESULT: Files are NOT SAME. Decoding FAILED.\n");
        return e_failure;
    }
}
