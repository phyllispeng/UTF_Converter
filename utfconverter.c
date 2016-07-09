#include "utfconverter.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef CSE320
	#define debug(fmt,...) do{printf("CSE320: " fmt, ##__VA_ARGS__);}while(0) 
#else
	#define debug(fmt,...)
#endif


int main(int argc,char **argv)
{
    int opt;
    char *input_path = NULL;
    char *output_path = NULL;
    int input_fd = -1, output_fd = -1;
    bool success = false;
    char*OUTPUT_ENCODING=NULL;
    int vlevel=0;


    

    /* Parse short options */
    while((opt = getopt(argc, argv, "he:v")) != -1) {
        switch(opt) {
            case 'h':
                /* The help menu was selected */
            
                USAGE(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 'e':
                /* The help menu was selected */
                 OUTPUT_ENCODING=optarg;
      
                break;
            case 'v':
                 vlevel++;
            /*visible ascii ; number of bype ; code point*/
            break;
            case '?':
            
                /* Let this case fall down to default;
                 * handled during bad option.
                 */
            default:
            
                /* A bad option was provided. */
                USAGE(argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }
    if(OUTPUT_ENCODING==NULL){
    	USAGE(argv[0]);
                exit(EXIT_FAILURE);
    }

    
    /*(strcmp(check,input) != 0)*/
    /*check if OUTPUT_ENCODING is UTF-8, UTF16LE, OR UTF16BE*/
    /*if UTF-8 */
    if(strcmp(OUTPUT_ENCODING,"UTF-8")==0){

    printf(" UTF-8 \n");

   
    /*if UTF-16LE*/
    } else if(strcmp(OUTPUT_ENCODING,"UTF-16LE")==0){
    	printf("UTF16LE, 56\n");
    	// little_e=true;
    }
    /*if UTR-16BE*/
    else if(strcmp(OUTPUT_ENCODING,"UTF-16BE")==0) {
    	printf("UTF16BE, 59\n");
    	// little_e=false;
    }

    /*else invalid*/
    else{
    	return EXIT_FAILURE;
    }



    /* Get position arguments */
    if(optind < argc && (argc - optind) == 2) {
        input_path = argv[optind++];
        output_path = argv[optind++];
    } 




else {
        if((argc - optind) <= 0) {
            fprintf(stderr, "Missing INPUT_FILE and OUTPUT_FILE.\n");
        } else if((argc - optind) == 1) {
            fprintf(stderr, "Missing OUTPUT_FILE.\n");
        } else {
            fprintf(stderr, "Too many arguments provided.\n");
        }
        USAGE(argv[0]);
        exit(EXIT_FAILURE);
    }

#ifdef CSE320
    char buffer[256];
     gethostname(buffer,256);
    debug("Host: %s\n", buffer);
    struct stat *in = malloc(sizeof(struct stat));
    
            
    memset(in, 0, sizeof(struct stat));
    stat(input_path, in);

    debug("Input: %s, %d, %d,%d byte(s)\n",input_path, (int)in->st_ino,(int)in->st_dev,(int)in->st_size);
    debug("Output:%s\n",output_path);
    debug("Input Encoding: lol \n");
    debug("Output Encoding:%s\n",OUTPUT_ENCODING);
    debug("The file %s was successfully created.\n",output_path);
#endif








    /* Make sure all the arguments were provided */
    if(input_path != NULL && output_path != NULL) {
        
        switch(validate_args(input_path, output_path)) {
                case VALID_ARGS:
                    /* Attempt to open the input file */
                    if((input_fd = open(input_path, O_RDONLY)) < 0) {
                        fprintf(stderr, "Failed to open the file.\n");
                        perror(NULL);
                        goto conversion_done;
                    }
                    /* Delete the output file if it exists; Don't care about return code. */
                    unlink(output_path);
                    /* Attempt to create the file */
                    if((output_fd = open(output_path, O_CREAT | O_WRONLY,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
                        /* Tell the user that the file failed to be created */
                        fprintf(stderr, "Failed to open the file %s\n", input_path);
                        perror(NULL);
                        goto conversion_done;
                    }
                     write_BOM(output_fd,OUTPUT_ENCODING);
                    /* Start the conversion */
                    success = convert(input_fd, output_fd, vlevel, OUTPUT_ENCODING);
                    break;
                case SAME_FILE:
                    fprintf(stderr, "The output file %s was not created. Same as input file.\n", output_path);
                    break;
                case FILE_DNE:
                    fprintf(stderr,"The input file %s does not exist.\n", input_path);
                    break;
                default:
                    fprintf(stderr, "An unknown error occurred\n");
                    break;
        }
    } else {
        /* Alert the user what was not set before quitting. */
        if(input_path == NULL) {
            fprintf(stderr, "INPUT_FILE was not set.\n");
        }
        if(output_path == NULL) {
            fprintf(stderr, "OUTPUT_FILE was not set.\n");
        }
        // Print out the program usage
        USAGE(argv[0]);
    }
    conversion_done:
                    if(success) {
                        /* We got here so it must of worked right? */
                        
                        /*return_code =*/
                         EXIT_SUCCESS;
                    } else {
                        /* Conversion failed; clean up */
                        if(output_fd < 0 && input_fd >= 0) {
                            close(input_fd);
                        }
                        if(output_fd >= 0) {
                            unlink(output_path);
                        }
                        /* Just being pedantic... */
                        
                        /*return_code =*/
                         return EXIT_FAILURE;
                    }
    return EXIT_SUCCESS;
}

/*main ends here*/

int validate_args(const char *input_path, const char *output_path)

{
    int return_code = FAILED;
    /* Make sure both strings are not NULL */
    if(input_path != NULL && output_path != NULL) {
        /* Check to see if the the input and output are two different files. */

      
            /* Check to see if the input file exists */
            struct stat *sb = malloc(sizeof(struct stat));
            struct stat *sb1= malloc(sizeof(struct stat));
            
            memset(sb, 0, sizeof(struct stat));
            memset(sb1, 0, sizeof(struct stat));
           
            /* now check to see if the file exists */
            if(stat(input_path, sb) == -1) {
                /* something went wrong */
                if(errno == ENOENT) {
                    /* File does not exist. */
                    return_code = FILE_DNE;
                } else {
                    /* No idea what the error is. */
                    perror(NULL);
                }
            } else {
            	return_code = VALID_ARGS;
            	if(stat(output_path, sb1)== 0){
                  if(sb->st_dev==sb1->st_dev){
                  	if(sb->st_ino==sb1->st_ino){
                  		return_code = SAME_FILE;
                  	}
                  }
            	}
            }
        
    }
    return return_code;
}
/*valid args ends*/
bool convert(const int input_fd, const int output_fd, int vlevel, char *output_encoding)
{
    bool success = false;
    if(input_fd >= 0 && output_fd >= 0) {
        /* UTF-8 encoded text can be @ most 4-bytes */
        unsigned char bytes[4];
        unsigned char read_value;
        unsigned char check_bom;
        size_t count = 0;
        ssize_t bytes_read;
        bool encode = false;

        

        if((bytes_read = read(input_fd, &check_bom, 1)) == 1){
        	if(check_bom== 0xEF ){
              
              read(input_fd, &check_bom,1);
              if(check_bom==0xBB){
               
              read(input_fd, &check_bom,1);
              if(check_bom==0xBF){
               
              }else{
              	return success;
              }

              }else{
              	return success;
              }

        	}else if(check_bom==0xFF){
        		 read(input_fd, &check_bom,1);
              if(check_bom==0xFE){
               
              }else{
              	return success;
              }

        	}else if(check_bom==0xFE){

        		 read(input_fd, &check_bom,1);
              if(check_bom==0xFF){
               
              }else{
              	return success;
              }

        	}else{
        		return success;
        	}
        }
    

    if(vlevel==1){/*one v*/
    	printf("+---------+------------------+------------------+\n" );
    	printf("|  ASCII  |  #  of   bytes   |    codeppoint    |\n" );
    	printf("+---------+------------------+------------------+\n" );



    }else if(vlevel==2){/*two vs*/
    	printf("+---------+------------------+------------------+------------------+\n" );
    	printf("|  ASCII  |  #  of   bytes   |    codeppoint    |       input      |\n" );
    	printf("+---------+------------------+------------------+------------------+\n" );


    }else if(vlevel>=3){/*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
        printf("+---------+------------------+------------------+------------------+------------------+\n" );
    	printf("|  ASCII  |  #  of   bytes   |    codeppoint    |       input      |      output      |\n" );
    	printf("+---------+------------------+------------------+------------------+------------------+\n" );
    }









        /*never used so parse_error = true;*/
        /* Read in UTF-8 Bytes */
        while((bytes_read = read(input_fd, &read_value, 1)) == 1) {
            /* Mask the most significate bit of the byte */
            unsigned char masked_value = read_value & 0x80;
            if(masked_value == 0x80) {
                if((read_value & UTF8_4_BYTE) == UTF8_4_BYTE ||
                   (read_value & UTF8_3_BYTE) == UTF8_3_BYTE ||
                   (read_value & UTF8_2_BYTE) == UTF8_2_BYTE) {
                    // Check to see which byte we have encountered
                    if(count == 0) {
                        bytes[count++] = read_value;
                    } else {
                        /* Set the file position back 1 byte */
                        if(lseek(input_fd, -1, SEEK_CUR) < 0) {
                            /* failed to move the file pointer back */
                            /*never used so parse_error = true;*/
                            perror(NULL);
                            goto conversion_done;

                        }
                        /* Enocde the current values into UTF-16LE */
                        encode = true;
                    }
                } else if((read_value & UTF8_CONT) == UTF8_CONT) {
                    /* continuation byte */
                    bytes[count++] = read_value;
                }
            } else {
                if(count == 0) {
                    /* US-ASCII */
                    bytes[count++] = read_value;
                    encode = true;
                } else {
                    /* Found an ASCII character but theres other characters
                     * in the buffer already.
                     * Set the file position back 1 byte.
                     */
                    if(lseek(input_fd, -1, SEEK_CUR) < 0) {
                        /* failed to move the file pointer back */
                        /*never used so parse_error = true;*/
                        perror(NULL);
                        goto conversion_done;
                    }
                    /* Enocde the current values into UTF-16LE */
                    encode = true;
                }
            }


           

            /* If its time to encode do it here */
            if(encode) {
                int i, value = 0;
                bool  isAscii = false;
                /*bool isAscii = false;*/
                for(i=0; i < count; i++) {
                	
                 /*1*/   if(i == 0) {
                        if((bytes[i] & UTF8_4_BYTE) == UTF8_4_BYTE) {
                            value = bytes[i] & 0x7;
                        } else if((bytes[i] & UTF8_3_BYTE) == UTF8_3_BYTE) {
                            value =  bytes[i] & 0xF;
                        } else if((bytes[i] & UTF8_2_BYTE) == UTF8_2_BYTE) {
                            value =  bytes[i] & 0x1F;
                        } else if((bytes[i] & 0x80) == 0) {
                            /* Value is an ASCII character */
                            value = bytes[i];
                            isAscii = true;
                        } else {
                            /* Marker byte is incorrect */
                            /*never used so parse_error = true;*/
                            goto conversion_done;
                        }
                    } 
                 /*1*/  else {
                        if(!isAscii) {
                            value = (value << 6) | (bytes[i] & 0x3F);
                        } else {
                            /* How is there more bytes if we have an ascii char? */
                            /*never used so parse_error = true;*/
                           
                        }
                    }
                }
                /* Handle the value if its a surrogate pair*/

            if(vlevel==1 ){
                    
                printf("|    %c   |        %d        |        U+%04x    |\n",read_value,i,value);
                printf("+---------+------------------+------------------+\n");
            }else if(vlevel==2){
            	printf("|    %c   |        %d        |        U+%04x    |      0x%x        |\n",read_value,i,value, read_value);
                printf("+---------+------------------+------------------+------------------+\n");

            }else if(vlevel>=3){
            	printf("|    %c   |        %d        |        U+%04x    |      0x%x        |        %04x      |\n",read_value,i,value, read_value, value);
                printf("+---------+------------------+------------------+------------------+------------------+\n");

            }
            int isLE = Endianess();
    
                if(value >= SURROGATE_PAIR) {
                    int vprime=value-0x10000; /* v` = v - 0x10000= value - SURROGATE_PAIR;*/
                     /* subtract the constant from value */
                    int w1 = (vprime >> 10) + 0xD800;
                    int w2 =  (vprime & 0x3FF) + 0xDC00;
                    /* write the surrogate pair w1 to file*/
                    bool isBE = !Endianess();
                    if(isBE==true){
                    	printf("this is the isBE\n");
                       int bw1 = w1>>8;
                       int bw11 = w1<<8;
                       w1=bw1 | bw11;

                       int bw2 =w2>>8;
                       int bw22=w2<<8;
                       w2= bw2 | bw22;


                    }
                    if(!safe_write(output_fd, &w1, 2, isLE)) {
                       /*never used so parse_error = true;*/

                        goto conversion_done;
                    }
                    /* write the surrogate pair w2 to file */
                    if(!safe_write(output_fd, &w2, 2, isLE)) {
                        /*never used so parse_error = true;*/
                        goto conversion_done;
                    }
                } 
                else {

                	if(!strcmp(output_encoding, "UTF-16BE")) {
                		int bw1 = value >> 8;
                       	int bw11 = value << 8;
                       	value = bw1 | bw11;
                	}

                    /* write the codeunit to file */
                    if(!safe_write(output_fd, &value, 2,isLE)) {
                       /*never used so parse_error = true;*/
                        goto conversion_done;
                    }
                }
                /* Done encoding the value to UTF-16LE */
                encode = false;
                count = 0;
            }/*if encode*/
        }/*while loop*/
        /* If we got here the operation was a success! */
        success = true;
    }
conversion_done:
    return success;
}
/*convert ends*/

bool safe_write( const int output_fd, void *value, size_t size,bool isLE)

{
    bool success = true;
    ssize_t bytes_written;
    unsigned int a = *(int *)value;

    if(isLE==false){
    unsigned int  buffer1=a<<24;

    unsigned int buff2= a>>8;
    buff2=buff2<<24;
    buff2=buff2>>8;
    unsigned int buff3 = a>>16;
        buff3 = buff3<<8;
    unsigned int b = buff2|buff3;
    a= buffer1 | b;
    }
    /*int i;*/
    if((bytes_written = write(output_fd, &a, size)) != size) {
        /* The write operation failed */
        fprintf(stderr, "Write to file failed. Expected %zu bytes but got %zd\n", size, bytes_written);
        success = false;
    }

    return success;
}


bool Endianess (){
/*if is little Endian return ture else false*/
	unsigned int i =1;
	char *c = (char*)&i;

	if(*c){
         return  true;
	}
	else{
		return false;
	}
	
}

void write_BOM(int output_fd, char* OUTPUT_ENCODING){
	int bom = 0;
	bool isLE = Endianess();


if(strcmp(OUTPUT_ENCODING,"UTF-8")==0){
	bom = 0xbfbbef;

    safe_write( output_fd,  &bom, 3, isLE);

/* 00 00 FF FE*/
}else if(strcmp(OUTPUT_ENCODING,"UTF-16LE")==0){
      bom = 0xfeff;
  safe_write( output_fd, &bom, 2 , isLE);

/* 00 00 FE FF*/
}else if(strcmp(OUTPUT_ENCODING,"UTF-16BE")==0){
       bom=0xfffe;
  safe_write(  output_fd, &bom, 2 , isLE);
}else {
	return;
}

}
