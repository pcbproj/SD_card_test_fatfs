#include "diskio.h"		/* FatFs lower layer API */
#include "sdcard.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	// already initialized in init() procedure
    return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{	
	
	SD_Error errorstatus = SD_OK;
    if(count == 1) {
		errorstatus = SD_ReadBlockBytes( ( sector << 9) , buff, SD_BLOCK_SIZE_BYTES);
    	if(errorstatus != SD_OK) {
    	    return RES_ERROR;
    	}
	}
	else if (count > 1) {
		errorstatus = SD_ReadMultiBlocksBytes( ( sector << 9) , buff, SD_BLOCK_SIZE_BYTES, count);
    	if(errorstatus != SD_OK) {
    	    return RES_ERROR;
    	}
	}

    if(errorstatus != SD_OK) {
        return RES_ERROR;
    }

    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */ // 
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	SD_Error errorstatus = SD_OK;
	
	if(count == 1) {
		errorstatus = SD_WriteBlockBytes( ( sector << 9 ) , buff, SD_BLOCK_SIZE_BYTES);
        if(errorstatus != SD_OK) {
            return RES_ERROR;
        }
	}
	else if(count > 1){
        errorstatus = SD_WriteMultiBlockBytes( ( sector << 9 ) , buff, SD_BLOCK_SIZE_BYTES, count);
        if(errorstatus != SD_OK) {
            return RES_ERROR;
        }
    }

    if(errorstatus != SD_OK) {
        return RES_ERROR;
    }

    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if(cmd == CTRL_SYNC) {
		return RES_OK;
	} else {
		// should never be called
		return RES_ERROR;
	}
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */

/*
DWORD get_fattime(void)
{
  return 0;
}
*/
