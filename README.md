# Fatfs_SD_Card_on_PIC18F46K22

Fatfs on PIC18F46K22                                                       
                                                                            
This program has a PIC18F46K22 write to an SD card using the Fatfs library. The configuration was done largely through the Microchip Code Configurator (MCC), with step-by-step instructions being found at:                       

https://microchip.my.site.com/s/article/dsPIC33Cx--How-to-create-an-SD-card-Example-using-MCC-Classic 
                                                                             
After following the above steps and generating a project, replace your main.c file with the one in this repo. The code should be agnostic to any PIC, but modifications may have to be made.                                     
                                                                            
Main.c provides an example of how you have write to four files in a single program. A custom wrapper function for writing is provided to ease the file-writing process. Custom string-processing functions are also included, due to XC8 compiler errors with the string.h library's versions. Better error-handling will likely be needed in any serious application of this program. Enjoy!                                                       
                                                                            
My Pinout:                                                                 
              MOSI = RC5                                                    
              MISO = RC4                                                    
              SCK  = RC3                                                    
              CS   = RC2                                                 
                                                                           

