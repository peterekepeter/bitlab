/*
 * vigenere encryption
 *
 * usage:
 * encrypt using 4 threads: vigenere -ksecret -e -r -t4 infile1.bin infile2.bin infile3.bin ...
 * decrypt using 4 threads: vigenere -ksecret -d -r -t4 infile1.bin infile2.bin infile3.bin ...
 * 
 * -r flag: show running time, is optional
 * -t flag: thread count, is optional
 * -k flag: encryption key, program will ask for key if not provided
 * -e flag: encrypt
 * -d flag: decrypt
 * 
 * non flag arguments will be files which are to be encrypted or decrypted
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

void vigenere(unsigned char* data, unsigned data_length, unsigned char* key, unsigned key_length, unsigned key_offset = 0)
{
  auto j = key_offset % key_length;
  for (unsigned i=0; i<data_length; i++)
  {
    data[i] = data[i] + key[j++];
    if (j >= key_length) j = 0;
  }
}

void vigenere_parallel(unsigned char* data, unsigned data_length, unsigned char* key, unsigned key_length, unsigned key_offset = 0)
{
  auto thread_count = omp_get_num_threads();
  #pragma omp parallel 
  {
    auto i = omp_get_thread_num();
    int data_part = data_length/thread_count;
  
    auto data_start = i*data_part;
    auto data_count = data_part;
    if (i == thread_count - 1)
      data_count = data_length - data_start;
    auto key_start = data_start + key_offset;
    
    vigenere(data + data_start, data_count, key, key_length, key_start);
  }
}

void invert_key(unsigned char* key, unsigned key_length)
{
  for (unsigned i=0; i<key_length; i++)
  {
    key[i] = - key[i];
  }
}

int main(int argc, char** args)
{ 
  unsigned char* key = 0;
  unsigned key_length;
  bool encrypt = true;
  bool mode_provided = false;
  bool show_time = false;

  //try to find key in the argument list

  for (int i=1; i<argc; i++)
  {
    char* arg = args[i];
    if (arg[0] == '-')
    {
      if (arg[1] == 'k')
      {
        key_length = strlen(arg)-2;
        key = (unsigned char*) malloc(key_length+1);
        strcpy((char*)key,arg+2);
      }
      else if (arg[1] == 'd')
      {
        encrypt = false;
        mode_provided = true;
      }
      else if (arg[1] == 'e')
      {
        encrypt = true;
        mode_provided = true;
      }
      else if (arg[1] == 't')
      {
        omp_set_num_threads(atoi(arg+2));
      }
      else if (arg[1] == 'r')
      {
        show_time = true;
      }
    }
  }

  //request mode if not provided

  while (!mode_provided)
  {
    char c;
    printf("encrypt or decrypt? ");
    c = getc(stdin);
    if (c == 'e' || c == 'E')
    {
      encrypt = true;
      mode_provided = true;
    }
    else if (c == 'd' || c == 'D')
    {
      encrypt = false;
      mode_provided = true;
    }
    printf("\n");
  }
      
  //request key if not provided

  if (key == 0)
  {
    printf("key (256 chars max): ");
    key = (unsigned char*) malloc(512*sizeof(char));
    scanf("%s",key);
    key_length = strlen((char*)key);
  }

  //inver the key if decrypt

  if (!encrypt)
  {
    invert_key(key, key_length);
  }
             
  //process files

  for (int i=1; i<argc; i++)
  {
    if (args[i][0] == '-') continue; //skip flags

    auto file = fopen(args[i],"rb+");
    if (file == 0) continue;

    fseek(file,0,SEEK_END);
    auto file_length = ftell(file);
    
    auto chunk_count = (file_length >> 16) + 1;
    
    double start_time;
    
    if (show_time)
      start_time = omp_get_wtime();

    #pragma omp parallel for
    for (int chunk = 0; chunk<chunk_count; chunk++)
    {                           
      unsigned char* data = 0;
      
      int chunk_size = 65536;
      long long file_pos = (long long)chunk*chunk_size; 
      unsigned data_length = chunk_size;
      if (file_length - file_pos < chunk_size) {
        chunk_size = data_length = file_length - file_pos;
      }

      if (data_length <= 0) continue;

      data = (unsigned char*) malloc(chunk_size*sizeof(unsigned char));

      #pragma omp critical
      {
        fseek(file, file_pos, SEEK_SET);
        fread(data,1,chunk_size,file);
      }
      
      vigenere(data,chunk_size,key,key_length,file_pos);

      #pragma omp critical
      {
        fseek(file, file_pos, SEEK_SET);
        fwrite(data,1,data_length,file);
      }        
      free(data);
    }            
    fclose(file);

    if (show_time)
    {
      double delta = omp_get_wtime() - start_time;
      printf("%s took %f\n",args[i],delta);
    }
  }

}


