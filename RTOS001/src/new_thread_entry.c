#include "new_thread.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Buffer Size
#define OUTPUT_BUFFER_SIZE 1024

// Buffers
char outputBuffer[OUTPUT_BUFFER_SIZE];

// Flags
volatile bool transmitComplete;

int _write(int file, char *buffer, int count);
int _write(int file, char *buffer, int count)
{
    // As far as I know, there isn't a way to retrieve how many
    // bytes were send on using the uart->write function if it does not return
    // SSP_SUCCESS (unless we want to use the tx interrupt function and a global counter
    // so, we will send each character one by one instead.
    int bytesTransmitted = 0;

    for (int i = 0; i < count; i++)
    {
        // Start Transmission
        transmitComplete = false;
        g_uart.p_api->write (g_uart.p_ctrl, (uint8_t const *) (buffer + i), 1);
        while (!transmitComplete)
        {
        }

        bytesTransmitted++;
    }

    return bytesTransmitted;
}

// Callback Function for UART interrupts
void user_uart_callback(uart_callback_args_t * p_args)
{
    // Get Event Type
    switch (p_args->event)
    {
        // Transmission Complete
        case UART_EVENT_TX_COMPLETE:
            transmitComplete = true;
        break;
        default:
        break;
    }
}

void new_thread_entry(void);

FX_FILE g_my_file;

/* New Thread entry function */
void new_thread_entry(void)
{
    CHAR writeBuffer[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer nec odio. Praesent libero. Sed cursus ante dapibus diam. Sed nisi. Nulla quis sem at nibh elementu.";
    CHAR filename[FX_MAX_LONG_NAME_LEN];
    CHAR destFilename[] = "my_test_file.txt";
    UCHAR readBuffer[1024];
    ULONG bytesRead;
    UINT error;
    UINT fileAttributes;
    ULONG fileSize;
    UINT fileCount;
    ULONG totalSize;
    ULONG mediaSpaceAvailable;
    bool isFirstRead = true;

    fileCount = 0;
    totalSize = 0;

    // Open UART
    g_uart.p_api->open (g_uart.p_ctrl, g_uart.p_cfg);

    // Disable Output Buffering
    setvbuf ( stdout, NULL, _IONBF, OUTPUT_BUFFER_SIZE);

    // Use TTY100 commands to clear screen and reset screen pointer
    printf ("\033[2J"); // Clear Screen
    printf ("\033[H"); // Return Home
    printf ("\033[3J"); // Clear Back Buffer

    // Print Header
    printf ("RTOS 001: SD-Card\r\n\r\n");

    // Media is opened during the auto-generated startup code (which sucks, it would be nice to have control over this).

    // Get the number of available bytes
    error = fx_media_space_available (&g_fx_media, &mediaSpaceAvailable);
    if (FX_SUCCESS != error)
    {
        printf ("An error occurred while accessing the media free space. Error Code: 0x%02x.", error);
        while (1)
            ;
    }

    printf ("Media Information\r\n");
    printf ("\tFree Space: %lu bytes\r\n", mediaSpaceAvailable);

    // Try to open our file for reading. If successful, read up to 1024 bytes and print them out.
    error = fx_file_open(&g_fx_media, &g_my_file, (CHAR *)destFilename, FX_OPEN_FOR_READ);
    if (FX_SUCCESS == error)
    {
        error = fx_file_read(&g_my_file, readBuffer, sizeof(readBuffer), &bytesRead);
        if (FX_SUCCESS == error)
        {
            readBuffer[bytesRead - 1] = '\0';
            printf("Read %lu bytes from %s. Contents: %s\r\n", bytesRead, destFilename, readBuffer);
            fx_file_close (&g_my_file);
        }
    }

    // There is no way to check if a file exists by filename (we can loop through each directory entry to see if the file exists)
    // So instead, we will try to create the file. If the method returns FX_ALREADY_CREATED, we know the file exists, so we
    // should delete it.
    //
    // Another approach would be to truncate the file then open it instead of delete and create
    printf ("\r\nCreating file %s...", (CHAR *)destFilename);

    error = fx_file_create (&g_fx_media, (CHAR *) destFilename);
    if (FX_ALREADY_CREATED == error)
    {
        printf ("ERROR\r\n\tFile exists. Deleting...");
        error = fx_file_delete (&g_fx_media, (CHAR *) destFilename);
        if (FX_SUCCESS != error)
        {
            printf ("ERROR!");
            while (1)
                ;
        }
        else
        {
            printf ("OK!\r\n");

            // We've deleted the file, so lets try to create it again
            printf ("Creating File...");
            error = fx_file_create (&g_fx_media, (CHAR *) destFilename);

        }
    }

    // Check if the file was created
    if (FX_SUCCESS != error)
    {
        printf ("ERROR!");
        while (1)
            ;
    }
    else
    {
        printf ("OK!\r\n");
    }

    // Open the file for writing
    printf("Opening file %s for writing...", (CHAR * )destFilename);
    error = fx_file_open(&g_fx_media, &g_my_file, (CHAR * )destFilename, FX_OPEN_FOR_WRITE);
    if (FX_SUCCESS != error)
    {
        printf ("ERROR!");
        while (1)
            ;
    }
    else
    {
        printf ("OK!\r\n");
    }

    // Write the buffer to the file
    printf("Writing data to file...");
    error = fx_file_write (&g_my_file, &writeBuffer, strlen (writeBuffer));
    if (FX_SUCCESS != error)
    {
        printf ("ERROR!");
        while (1)
            ;
    }
    else
    {
        printf ("OK!\r\n");
    }

    // Close the file
    printf("Closing file...");
    error = fx_file_close (&g_my_file);
    if (FX_SUCCESS != error)
    {
        printf ("ERROR!");
        while (1)
            ;
    }
    else
    {
        printf ("OK!\r\n");
    }

    // Flush the media
    printf("Flushing Media...");
    error = fx_media_flush (&g_fx_media);
    if (FX_SUCCESS != error)
    {
        printf ("ERROR!");
        while (1)
            ;
    }
    else
    {
        printf ("OK!\r\n");
    }

    // Close Media
    printf("Closing Media...");
    error = fx_media_close (&g_fx_media);
    if (FX_SUCCESS != error)
    {
        printf ("ERROR!");
        while (1)
            ;
    }
    else
    {
        printf ("OK!\r\n");
    }

    // Sleep
    while (1)
    {
        tx_thread_sleep (1);
    }
}

