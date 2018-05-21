//
//  Created by jstefanop on 1/25/18.
//
//  extended for remote tuning script by psycodad 05/10/18
//
//  Change to support Antminer A3 and Antminer D3 by FNT May/20/2018 - forexneurotrader.com

#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define PIC_COMMAND_1                       0x55
#define PIC_COMMAND_2                       0xaa
#define GET_VOLTAGE                         0x18
#define SET_VOLTAGE                         0x10
#define JUMP_FROM_LOADER_TO_APP             0x06
#define RESET_PIC                           0x07
#define READ_PIC_SOFTWARE_VERSION           0x17
static unsigned char Pic_command_1[1] = {PIC_COMMAND_1};
static unsigned char Pic_command_2[1] = {PIC_COMMAND_2};
static unsigned char Pic_set_voltage[1] = {SET_VOLTAGE};
static unsigned char Pic_get_voltage[1] = {GET_VOLTAGE};
static unsigned char Pic_read_pic_software_version[1] = {READ_PIC_SOFTWARE_VERSION};
static unsigned char Pic_jump_from_loader_to_app[1] = {JUMP_FROM_LOADER_TO_APP};
static unsigned char Pic_reset[1] = {RESET_PIC};
pthread_mutex_t iic_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t i2c_mutex = PTHREAD_MUTEX_INITIALIZER;

int const i2c_slave_addr[4] = {0xa0,0xa2,0xa4,0xa6};

void get_software_version(unsigned char *version, int fd)
{
    unsigned char length = 0x04, crc_data[2] = {0xff}, read_back_data[5] = {0xff};
    unsigned short crc = 0;
    unsigned char i,send_data[6] = {0};

    crc = length + READ_PIC_SOFTWARE_VERSION;
    crc_data[0] = (unsigned char)((crc >> 8) & 0x00ff);
    crc_data[1] = (unsigned char)((crc >> 0) & 0x00ff);
  

    send_data[0] = PIC_COMMAND_1;
    send_data[1] = PIC_COMMAND_2;
    send_data[2] = length;
    send_data[3] = READ_PIC_SOFTWARE_VERSION;
    send_data[4] = crc_data[0];
    send_data[5] = crc_data[1];

    pthread_mutex_lock(&i2c_mutex);
    for(i=0; i<6; i++)
    {
        write(fd, send_data+i, 1);
    }
    usleep(400*1000);
    for(i=0; i<5; i++)
    {
        read(fd, read_back_data+i, 1);
    }
    pthread_mutex_unlock(&i2c_mutex);

    usleep(200*1000);

   
    if((read_back_data[1] != READ_PIC_SOFTWARE_VERSION) || (read_back_data[0] != 5))
    {
       
		printf("\n version = read ERROR 1 ");

    }
    else
    {
        crc = read_back_data[0] + read_back_data[1] + read_back_data[2];
        if(((unsigned char)((crc >> 8) & 0x00ff) != read_back_data[3]) || ((unsigned char)((crc >> 0) & 0x00ff) != read_back_data[4]))
        {
          
		printf("\n version  = ERRORR2 ");

        }
        *version = read_back_data[2];

    }
}


void get_voltage(unsigned char *voltage, int fd)
{
    unsigned char length = 0x04, crc_data[2] = {0xff}, read_back_data[5] = {0xff};
    unsigned short crc = 0;
    unsigned char i,send_data[6] = {0};

    crc = length + GET_VOLTAGE;
    crc_data[0] = (unsigned char)((crc >> 8) & 0x00ff);
    crc_data[1] = (unsigned char)((crc >> 0) & 0x00ff);
  
    send_data[0] = PIC_COMMAND_1;
    send_data[1] = PIC_COMMAND_2;
    send_data[2] = length;
    send_data[3] = GET_VOLTAGE;
    send_data[4] = crc_data[0];
    send_data[5] = crc_data[1];

    pthread_mutex_lock(&i2c_mutex);
    for(i=0; i<6; i++)
    {
        write(fd, send_data+i, 1);
    }
    usleep(200*1000);
    for(i=0; i<5; i++)
    {
        read(fd, read_back_data+i, 1);
    }
    pthread_mutex_unlock(&i2c_mutex);

 
    if((read_back_data[1] != GET_VOLTAGE) || (read_back_data[0] != 5))
    {
        
	printf("\n Error1 read voltage!");
     
    }
    else
    {
        crc = read_back_data[0] + read_back_data[1] + read_back_data[2];
        if(((unsigned char)((crc >> 8) & 0x00ff) != read_back_data[3]) || ((unsigned char)((crc >> 0) & 0x00ff) != read_back_data[4]))
        {
          
	    printf("\n Error2 read voltage!");
       
        }
        *voltage = read_back_data[2];
      
    }
}

void set_voltage_new(unsigned char voltage, int fd)
{	
    unsigned char length = 0x05, crc_data[2] = {0xff}, read_back_data[2] = {0xff};
    unsigned short crc = 0;
    unsigned char i,send_data[7] = {0};

    crc = length + SET_VOLTAGE + voltage;
    crc_data[0] = (unsigned char)((crc >> 8) & 0x00ff);
    crc_data[1] = (unsigned char)((crc >> 0) & 0x00ff);
    
    send_data[0] = PIC_COMMAND_1;
    send_data[1] = PIC_COMMAND_2;
    send_data[2] = length;
    send_data[3] = SET_VOLTAGE;
    send_data[4] = voltage;
    send_data[5] = crc_data[0];
    send_data[6] = crc_data[1];

    pthread_mutex_lock(&i2c_mutex);
    for(i=0; i<7; i++)
    {
        write(fd, send_data+i, 1);
    }
    usleep(200*1000);
    for(i=0; i<2; i++)
    {
        read(fd, read_back_data+i, 1);
    }
    pthread_mutex_unlock(&i2c_mutex);

    usleep(200*1000);

    if((read_back_data[0] != SET_VOLTAGE) || (read_back_data[1] != 1))
    {
  
	printf("\n Error write voltage!\n");        

    }
    else
    {
    
	printf("\n Voltage OK!\n");        
   
    }
}


int init_i2c(int chain) {
    char filename[40];
    unsigned char version = 0;
    int fd;
    sprintf(filename,"/dev/i2c-0");
    
    if ((fd = open(filename,O_RDWR)) < 0) {
        printf("Failed to open the bus\n");
        exit(1);
    }

    pthread_mutex_lock(&iic_mutex);
    if (ioctl(fd,I2C_SLAVE,i2c_slave_addr[chain] >> 1 )) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }
    
    get_software_version(&version, fd);
   
    
    if(version != 0x81){
        printf("Wrong PIC version\n");
        exit(1);
    }
    return fd;
}


void read_voltage(int chain) {
    unsigned char voltage = 0;
    int fd;
    fd = init_i2c(chain);
    get_voltage(&voltage, fd);   
    printf("chain %i: voltage = 0x%02x", chain+1, voltage);
    pthread_mutex_unlock(&iic_mutex);
    close(fd);
}

void read_voltage_all() {
    int chain;
    for (chain = 0; chain < 3; chain++) {
      //printf("\nchain %i: ", chain+1, "");
      printf("\n");
      read_voltage(chain);
    }
}


void write_voltage(int chain, unsigned char set_voltage) {
    unsigned char voltage = 0;
    int fd;
    fd = init_i2c(chain);
    printf("Reading voltage");
    get_voltage(&voltage, fd);
    printf("\nchain %i: voltage = 0x%02x\n", chain+1, voltage);
    printf("Setting voltage on chain %i\n", chain+1);
    set_voltage_new(set_voltage, fd);
    
    printf("Reading voltage");
    get_voltage(&voltage, fd);
    printf("\nchain %i: voltage = 0x%02x\n", chain+1, voltage);
    pthread_mutex_unlock(&iic_mutex);
    
    if(voltage != set_voltage) {
        printf("ERROR: Voltage was not successfully set\n");
        exit(1);
    }
    else
        printf("Success: Voltage updated!\n");
        printf("If you save money, check it and maybe then donate something.\n");
    close(fd);
}

void print_help() {
    printf("Usage:\n");
    printf("./set_voltage [chain# 1-3] [voltage in hex]\n");
    printf("If no voltage is given, voltage is read out from chain.\n");
    printf("If no chain is given, all chains will be read out.\n");
}

void main (int argc, char *argv[]){
    int chain;
    if (argc == 1) {
      printf("Reading all voltages");   
      read_voltage_all();
      printf("\n\n"); 
    }
    if (argc > 1) {
      if (strcmp(argv[1], "help") == 0) {
        print_help();
        exit(1);
      }
      chain = atoi(argv[1]);
      if(chain > 3 || chain == 0){
          printf("Invalid chain #, valid range 1-3\n");
          print_help();
          exit(1);
      }
      chain--;
      if (argc == 2) { 
        printf("Reading voltage chain %i:\n", chain+1);
        read_voltage(chain);
        printf("\n"); 
      }
    }
    if (argc == 3) {
      if(strtol(argv[2], NULL, 16) > 0xfe){
        printf("Invalid hex voltage, valid range 0x00-0xfe\n");
        print_help();
        exit(1);
      }    
      unsigned char set_voltage = strtol(argv[2], NULL, 16);
      write_voltage(chain, set_voltage);
    }
    if (argc > 3) {
      printf("Incorrect arguments\n");
      print_help();
      exit(1);
    }            
    
}



