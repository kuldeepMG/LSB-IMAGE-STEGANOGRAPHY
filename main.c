#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

int main(int argc,char*argv[])
{
    EncodeInfo encInfo;
    uint img_size;
    DecodeInfo decInfo;
    if(argc<2)
    {
        printf("Encoding:./lsb_steg -e <.bmp file> <.txt file> [output file] \n");
        printf("Decoding:./lsb_steg -d <.bmp file> [output file] \n");
        return 0;
    }

    if((check_operation_type(argv)) == e_encode)
    {
        if(argc<4 || argc>5)
        {
            printf("Encoding:./lsb_steg -e <.bmp file> <.txt file> [output file] \n");
            printf("Decoding:./lsb_steg -d <.bmp file> [output file] \n");
            return e_failure;
        }
        
    printf("\n============================================================================\n\n");
    printf("\n************************SELECTED ENCODING OPERATION ************************\n\n");
    printf("\n============================================================================\n\n");

        

        if(read_and_validate_encode_args(argv,&encInfo) == e_success)
        {
            
            if(do_encoding(&encInfo) == e_success)
            {
                printf("========== GREAT JOB KULDEEP! ENCODING COMPLETED SUCCESSFULLY ==========\n");

                
            }
            else
            {
                printf("ERROR : Encoding failed");
            }
        }        
    }


    else if((check_operation_type(argv)) == e_decode)
    {
        if(argc<3 || argc>4)
        {            
            printf("Encoding:./lsb_steg -e <.bmp file> <.txt file> [output file] \n");
            printf("Decoding:./lsb_steg -d <.bmp file> [output file] \n");
            return e_failure;            
        }
    printf("\n============================================================================\n\n");
    printf("\n************************SELECTED DECODING OPERATION ************************\n\n");
    printf("\n============================================================================\n\n");


       if(read_and_validate_decode_args(argv,&decInfo) == e_success)
        {
            
            
            if(do_decoding(&decInfo) == e_success)
            {
                printf("========== GREAT JOB KULDEEP! DECODING COMPLETED SUCCESSFULLY ==========\n");

            }
            else
            {
                printf("ERROR : Decoding failed\n");
            }
        }  
    }
    // Test get_image_size_for_bmp
    //img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    //printf("INFO: Image size = %u\n", img_size);

    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
        return e_encode;
    else if(strcmp(argv[1],"-d")==0)
        return e_decode;
    else
        return e_unsupported;  

}


