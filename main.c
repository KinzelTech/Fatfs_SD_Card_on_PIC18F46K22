/******************************************************************************/
/* Fatfs on PIC18F46K22                                                       */
/*                                                                            */
/* This program has a PIC18F46K22 write to an SD card using the Fatfs library.*/
/* The configuration was done largely through the Microchip Code Configurator */
/* (MCC), with step-by-step instructions being found at:                      */ 
/* https://microchip.my.site.com/s/article/dsPIC33Cx--How-to-create-an-SD-card-Example-using-MCC-Classic */
/*                                                                            */ 
/* After following the above steps and generating a project, replace your     */
/* main.c file with this one. The below code should be agnostic to any PIC,   */
/* but modifications may have to be made.                                     */
/*                                                                            */
/* The below code provides an example of how you have write to four files in  */
/* a single program. A custom wrapper function for writing is provided to     */
/* ease the file-writing process. Custom string-processing functions are also */
/* included, due to XC8 compiler errors with the string.h library's versions. */
/* Better error-handling will likely be needed in any serious application of  */
/* this program. Enjoy!                                                       */
/*                                                                            */
/* My Pinout:                                                                 */
/*              MOSI = RC5                                                    */
/*              MISO = RC4                                                    */
/*              SCK  = RC3                                                    */
/*              CS   = RC2                                                    */
/*                                                                            */
/******************************************************************************/

#include "mcc_generated_files/mcc.h"
#include <string.h>

/******************************************************************************/
/*                                 CONSTANTS                                  */
/******************************************************************************/
#define DATA_LENGTH 20  //Max number of characters to write to the file.

/******************************************************************************/
/*                             Global Variables                               */
/******************************************************************************/
FATFS FatFs;	//Acts as a mount point for the SD card.
FIL Fil;		//Points to the file that is being processed.

char temp_data[DATA_LENGTH]; //Temperature data string
char alt_data[DATA_LENGTH];  //Altitude data string
char hum_data[DATA_LENGTH];  //Humidity data string
char vel_data[DATA_LENGTH];  //Velocity data string

/******************************************************************************/
/*                           Function Declarations                            */
/******************************************************************************/
void write_to_file(char* file_name, char* data);
    //Write a string of data to a file.
char* get_temp_data(char* temp_string);
    //Returns the current temperature data
char* get_alt_data (char* alt_string);
    //Returns the current altitude data.
char* get_hum_data (char* hum_string);
    //Returns the current humidity data.
char* get_vel_data (char* vel_string);
    //Returns the current velocity data.
char *string_copy(char* dest, char* source);
    //Copies the source string to the destination string.
void append(char* result, char* dest, char* source);
    //Produces a string that is the combined result of two other strings.

/******************************************************************************/
/*                               Main Function                                */
/******************************************************************************/
void main(void)
{
    
    // Initialize the device
    SYSTEM_Initialize();
    
    //Assign the names of the files to write/open to variables
    //Note that file names cannot be longer that 13 characters
    char* temp_file = "temp.txt";
    char* alt_file  = "altitude.txt";
    char* hum_file  = "humidity.txt";
    char* vel_file  = "velocity.txt";

    //Write a line to each file.
    write_to_file(alt_file,  get_alt_data (alt_data));
    write_to_file(hum_file,  get_hum_data (hum_data));
    write_to_file(vel_file,  get_vel_data (vel_data));
    write_to_file(temp_file, get_temp_data(temp_data));
    
    //Infinite loop stops program from restarting
    while(1);
    
	return;
}

/******************************************************************************/
/*                    Write a line of text to a file                          */
/******************************************************************************/
void write_to_file(char* file_name, char* data)
{
    UINT bw;              //Returns the number of characters actually written.
    char* formatted_data; //The string that will be written to the file.
    UINT data_size;       //The length of the string to be written.

    //Allocate memory to and format the string to include CR-LF
    formatted_data = (char*) malloc(strlen(data) + 1);    
    append(formatted_data, data, "\r\n");
    data_size  = (UINT) strlen(formatted_data);
    
    //Mount the SD card if not already mounted
    if (f_mount(&FatFs, "", 1) == FR_OK) 
    {
        //Open or create the file, with Fil pointing to its mount point
		if (f_open(&Fil, file_name, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)
        {	
            //Jump to end of file if it already has text, to prevent overwriting
            if (f_size(&Fil) != 0)
                f_lseek(&Fil, f_size(&Fil));
            
            //Write the data string to the file
			f_write(&Fil, formatted_data, data_size-1 , &bw);

            //Close the file, which saves the changes to the card.
			f_close(&Fil);
		}
	}
    return;
}

/******************************************************************************/
/*             Get and return the temperature data from a sensor              */
/******************************************************************************/
char* get_temp_data(char* temp_string)
{
    string_copy(temp_string, "77 degrees");
    temp_string = "77 degrees";
    return temp_string;
}

/******************************************************************************/
/*              Get and return the altitude data from a sensor                */
/******************************************************************************/
char* get_alt_data (char* alt_string)
{
    alt_string = "200m";
    return alt_string;
}

/******************************************************************************/
/*              Get and return the humidity data from a sensor                */
/******************************************************************************/
char* get_hum_data (char* hum_string)
{
    hum_string = "95%";
    return hum_string;
}

/******************************************************************************/
/*              Get and return the velocity data from a sensor                */
/******************************************************************************/
char* get_vel_data (char* vel_string)
{
    vel_string = "10m/s";
    return vel_string;
}

//NOTE: The XC8 compiler evidently does not like string.h's strcpy and strcat
//      Below are my custom versions of each.
/******************************************************************************/
/*      Produces a string that is the combined result of two other strings    */
/******************************************************************************/
void append(char* result, char* dest, char* source)
{
    int dest_size   = (int) strlen(dest);
    int source_size = (int) strlen(source);
    int counter;
    
    for(counter = 0; counter < dest_size; counter++)
        result[counter] = dest[counter];
    
    for(counter = 0; counter < source_size; counter++)
        result[counter+dest_size] = source[counter]; 
    
    result[dest_size + source_size] = '\0';
    return;
}

/******************************************************************************/
/*                       Copies one string to another                         */
/******************************************************************************/
char* string_copy(char* dest, char* source)
{
    int counter = 0;
	do
    {
        dest[counter] = source[counter];
        counter++;
    }
    while(source[counter] != '\0');
    return dest;
}