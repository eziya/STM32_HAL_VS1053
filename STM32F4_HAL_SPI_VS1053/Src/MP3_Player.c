#include "MP3_Player.h"
#include "fatfs.h"

#define BUFFER_SIZE 	32

uint8_t mp3Buffer[BUFFER_SIZE];
uint32_t mp3FileSize;
uint32_t readBytes;
uint16_t cnt = 0;

bool isPlaying = false;
bool isFileOpen = false;

FATFS fs;
FIL mp3File;

/* Initialize VS1053 & Open a file */
bool MP3_Init()
{
	/* Initialize VS1053 */
    if(!VS1053_Init()) return false;

    /* Mount SD Card */
    if(f_mount(&fs, "", 0) != FR_OK) return false;

    return true;
}

bool MP3_Play(const char *filename)
{
	if(!VS1053_SetMode(0x4800)) return false;	/* SM LINE1 | SM SDINEW */
	if(!VS1053_AutoResync()) return false;		/* AutoResync */
	if(!VS1053_SetDecodeTime(0)) return false;	/* Set decode time */
	if(!VS1053_SetVolume( 0x3F, 0x3F )) return false;	/* Small number is louder */

	/* Open file to read */
	if(f_open(&mp3File, filename, FA_READ) != FR_OK) return false;

	/* Get the file size */
	mp3FileSize = f_size(&mp3File);

	/* Set flags */
	isFileOpen = true;
	isPlaying = true;

    return true;
}

void MP3_Stop(void)
{
	/* Refer to page 49 of VS1053 datasheet */

	uint16_t mode;
	VS1053_SendEndFill(2052);	/* send endfill bytes */
	VS1053_SetMode(0x4808);		/* SM LINE1 | SM SDINEW | SM CANCEL */
	VS1053_SendEndFill(32);		/* send endfill bytes */
	HAL_Delay(100);
	VS1053_GetMode(&mode);		/* get mode value */
	if((mode & 0x08) != 0x0)	/* if SM CANCEL is not clear, soft reset */
	{
		VS1053_SoftReset();
	}

	f_close(&mp3File);
	isPlaying = false;			/* Stop flag */
	isFileOpen = false;			/* Close flag */
}

void MP3_Pause(void)
{
	if(isPlaying) isPlaying = false;
}

void MP3_Resume(void)
{
	if(!isPlaying) isPlaying = true;
}

/* Send mp3 buffer to VS1053 */
void MP3_Feeder(void)
{
	if(!isPlaying || !isFileOpen) return;

	/* Toggle Green LED */
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

	if(mp3FileSize > BUFFER_SIZE)
	{
		/* Fill the buffer */
		f_read(&mp3File, mp3Buffer, BUFFER_SIZE, (void*)&readBytes);

		/* Tx buffer */
		VS1053_SdiWrite32( mp3Buffer );

		/* bytes to send */
		mp3FileSize -= BUFFER_SIZE;
	}
	else
	{
		/* Read left bytes */
		f_read(&mp3File, mp3Buffer, mp3FileSize, (void*)&readBytes);

		/* Tx buffer */
		for (cnt = 0; cnt < mp3FileSize; cnt++)
		{
			while(!VS1053_SdiWrite(*(mp3Buffer + cnt)));
		}

		/* Stop when played the whole file */
		MP3_Stop();
	}
}
