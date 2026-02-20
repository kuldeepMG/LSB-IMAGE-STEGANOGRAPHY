#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

/*read and validate encode arguements*/
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    
    char *ret = strstr(argv[2],".bmp");
    if(ret == NULL)
    {
        printf(" ERROR: Wrong extension! source image file name must end with .bmp extension.\n");
        return e_failure;
    }
    else if(strcmp(ret,".bmp")!=0)
    {
        printf(" ERROR: Wrong extension! source image file name must end with .bmp extension.\n");
        return e_failure;
    }
    else
    {
        
        printf(" INFO: Source image has valid .bmp extension.\n");
        encInfo->src_image_fname = argv[2];
    }
    char * res = strstr(argv[3],".txt");
    if(res == NULL)
    {
        printf(" ERROR: Wrong extension! text file name must eend with .txt extn.\n");
        return e_failure;
    }
    else if(strcmp(res,".txt")!= 0)
    {
        printf(" ERROR: Wrong extension! text file name must eend with .txt extn.\n");
        return e_failure;
    }
    else
    {
        printf(" INFO: Secret file has valid .txt extension.\n");
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, res);   
    }
    if(argv[4] == NULL)
    {
        printf(" INFO: No output file provided. use default file name : stego.bmp\n");
        encInfo->stego_image_fname = "stego.bmp";
    }
    else
    {
        if(strstr(argv[4],".bmp")==NULL)
        {
            printf(" ERROR: Output file must have .bmp extension.\n");
            return e_failure;
        }
        else
        {
            printf(" INFO: Output file has valid .bmp extension.\n");
            encInfo->stego_image_fname = argv[4];
        }
    }
    printf(" Arguments validated successfully\n\n");
    return e_success;
}

/*open all files*/
Status open_files(EncodeInfo *encInfo)
{
    //open  Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
  
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    printf(" INFO : Opened beautiful.bmp\n");


    // open Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
 
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    printf(" INFO : Opened secret.txt\n");


    // open stego_Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
   
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    printf(" INFO : Opened stego.bmp\n");
   
    return e_success;
}

/*do encoding*/
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf(" INFO: Files opened successfully.\n");
        printf("\n----------Encoding Procedure Started ----------\n\n");
        if(check_capacity(encInfo) == e_success)
        {
            printf("\n INFO: Capacity check passed.\n");
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
           {
                printf(" INFO: Header copied.\n");
                if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
                {
                    printf(" INFO: Magic string encoded.\n");
                    if(encode_file_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_success)
                    {
                        printf(" INFO: Encoded file extension size.\n");
                        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
                        {
                            printf(" INFO: Encoded file extension.\n");
                            if(encode_secret_file_size(encInfo->secret_file_size,encInfo) == e_success)
                            {
                                printf(" INFO: Encoded secret file size.\n");
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf(" INFO: Encoded secret file data.\n");
                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
                                    {
                                        printf(" INFO: Remaining image data copied.\n");
                                        fclose(encInfo->fptr_src_image);
                                        fclose(encInfo->fptr_stego_image);
                                        //FILE COMPARISON
                                        compare_bmp_files(encInfo->src_image_fname, encInfo->stego_image_fname);

                                        return e_success;
                                    }
                                    else
                                    {
                                        printf("ERROR: Copy remaining data failed.\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("ERROR: Encode secret file data failed.\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("ERROR: Encode secret file size failed.\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR: Encode secret file extension failed.\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR: Encode file extension size failed.\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR: Magic string encoding failed.\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR: Copy BMP header failed.\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: Capacity check failed.\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Opening files failed.\n");
        return e_failure;
    }
}

/*check image capacity*/
Status check_capacity(EncodeInfo *encInfo)
{
    //FILE*fp = fopen("secret.txt" ,"r");
    int image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    int magic_string = strlen(MAGIC_STRING);
    int extension = strlen(encInfo->extn_secret_file);
    encInfo->secret_file_size =get_file_size(encInfo->fptr_secret);
    int Encoding_thing = (magic_string + sizeof(int) + extension + sizeof(int) + encInfo->secret_file_size) *8;
    if(Encoding_thing<image_capacity)
        return e_success;
    else
        return e_failure;

}

/*get image size*/
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Skip to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height 
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/*get secret file size*/
uint get_file_size(FILE*fptr)
{
    if(fptr == NULL)
    {
        printf("error opening the file!");
        return 0;
    }
    fseek(fptr,0,SEEK_END);
    uint file_size = ftell(fptr);
    return file_size;
}

/*copy bmp header*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char header[100];
    fseek(fptr_src_image,0,SEEK_SET);
    fread(header,54,1,fptr_src_image);
    fwrite(header,54,1,fptr_dest_image);
    return e_success;
}

/*encode magic string*/
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*common function for encoding (byte to lsb)*/
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{

    char str[8];
    for(int i = 0;i<size;i++)
    {
        fread(str,8,1,fptr_src_image);
        encode_byte_to_lsb(data[i],str);
        fwrite(str,8,1,fptr_stego_image);
    }
    return e_success;
}

/*encode secret_file extn size*/
Status encode_file_extn_size(int size,EncodeInfo*encInfo)
{
    char str[33];
    fread(str,32,1,encInfo->fptr_src_image);
    encode_int_to_lsb(size,str);
    fwrite(str,32,1,encInfo->fptr_stego_image);
    return e_success;
}

/*encode size to lsb*/
Status encode_int_to_lsb(int size,char*image_buffer)
{
     for (int i = 0; i < 32; i++)
    {
        unsigned int bit = (size >> (31-i)) & 1;                    // extract ith bit
        image_buffer[i] = (image_buffer[i] & ~1);
        image_buffer[i] = image_buffer[i] | bit;        // put that bit into LSB
    }
        return e_success;
}

/*encode byte to lsb*/
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8 ; i++)
    {
        unsigned int bit = (data >> (7-i)) & 1;                    // extract ith bit
        image_buffer[i] = (image_buffer[i] & ~1) ;
        image_buffer[i] = image_buffer[i]| bit;        // put that bit into LSB
    }
        return e_success;
    
}

/*encode secret file extn*/
Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo)
{
    if(encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*encode secret file size (already defined in get_image())*/
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char str[33];
    fread(str,32,1,encInfo->fptr_src_image);
    encode_int_to_lsb(file_size,str);
    fwrite(str,32,1,encInfo->fptr_stego_image);
    return e_success;
}

/*encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int i = 0;
    char ch;
    char secret_file_data[encInfo->secret_file_size];
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    while ((ch = fgetc(encInfo->fptr_secret)) != EOF && i < encInfo->secret_file_size)
    {
        secret_file_data[i] = ch;
        i++;
    }
    if(encode_data_to_image(secret_file_data,encInfo->secret_file_size,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*copy remaining image_data*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int ch;
    while ((ch = fgetc(fptr_src)) != EOF)
    {
        fputc(ch, fptr_dest);
    }
    return e_success;
    
}

/*compare both beautiful.bmp and stego.bmp*/
Status compare_bmp_files(const char *src, const char *stego)
{
    FILE *fp1 = fopen(src, "rb");
    FILE *fp2 = fopen(stego, "rb");

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
        printf("RESULT: No differences found. Encoding NOT done.\n");
        return e_failure;
    }
    else
    {
        printf("RESULT: Files are different. Encoding SUCCESSFUL.\n");
        return e_success;
    }
}
