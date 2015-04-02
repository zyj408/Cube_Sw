/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "bsp_sdio_sd.h"

/* Definitions of physical drive number for each media */
#define ATA		0
#define MMC		1
#define USB		2


#define SECTOR_SIZE		512
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = STA_NOINIT;
	SD_CardInfo SDCardInfo;

	switch (pdrv)
	{
		case FS_SD :		/* SD卡 */
			if (SD_Init() == SD_OK)
			{
				stat = RES_OK;
			}
			else
			{
				stat = STA_NODISK;
			}
			break;

		case FS_NAND :		/* NAND Flash */
		
			break;

		case FS_USB :		/* STM32 USB Host 口外接U盘 */
		
			break;

		case FS_SPI :		/* SPI Flash */
		
			break;

		case FS_NOR :		/* NOR Flash */
		
			break;

		default :
			break;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
		case FS_SD :
			stat = 0;
			break;

		case FS_NAND :
			stat = 0;
			break;

		case FS_USB :
			stat = 0;
			break;

		case FS_SPI :		/* SPI Flash */
			stat = 0;
			break;

		case FS_NOR :		/* NOR Flash */
			stat = 0;
			break;

		default:
			stat = 0;
			break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;

	switch (pdrv)
	{
		case FS_SD :
		{
			SD_Error Status = SD_OK;

			if (count == 1)
			{
				Status = SD_ReadBlock(buff, sector << 9 , SECTOR_SIZE);
			}
			else
			{
				Status = SD_ReadMultiBlocks(buff, sector << 9 , SECTOR_SIZE, count);
			}
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}

		#ifdef SD_DMA_MODE
			/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
			Status = SD_WaitReadOperation();
			if (Status != SD_OK)
			{
				res = RES_ERROR;
				break;
			}

			while(SD_GetStatus() != SD_TRANSFER_OK);
		#endif

			res = RES_OK;
			break;
		}

		case FS_NAND :
		
			break;

		case FS_USB :
		
			break;

		case FS_SPI :		/* SPI Flash */
		
			break;

		case FS_NOR :		/* NOR Flash */
		
			break;

		default:
	
			break;
	}
	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
 DRESULT res;

	switch (pdrv)
	{
		case FS_SD :
			{
				SD_Error Status = SD_OK;
	
				if (count == 1) 
				{
					Status = SD_WriteBlock((uint8_t *)buff, sector << 9 ,SECTOR_SIZE);
				} 
				else 
				{
					Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count);	
				}
				if (Status != SD_OK) 
				{
					res =  RES_ERROR;
				}
			
			#ifdef SD_DMA_MODE	
				/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
				Status = SD_WaitReadOperation();
				if (Status != SD_OK) 
				{
					res =  RES_ERROR;
				}
				
				while(SD_GetStatus() != SD_TRANSFER_OK);
			#endif	
	
				while(SD_GetStatus() != SD_TRANSFER_OK);
	
				res =  RES_OK;
				break;
			}

		case FS_NAND :
		
			break;

		case FS_USB :

			break;

		case FS_SPI :		/* SPI Flash */
		
			break;

		case FS_NOR :		/* NOR Flash */
		
			break;

		default:
			res = RES_PARERR;
			break;
	}
	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) {
	case FS_SD :
		/* SD卡磁盘容量： SDCardInfo.CardCapacity */
		res = RES_OK;
		return res;

	case FS_NAND :
	    break;

	case FS_USB :
	
        break;

	case FS_SPI :		/* SPI Flash */
	
		break;

	case FS_NOR :		/* NOR Flash */
	
		break;
	}

	return RES_PARERR;
}
#endif

/*
*********************************************************************************************************
*	函 数 名: get_fattime
*	功能说明: 获得系统时间，用于改写文件的创建和修改时间。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* 如果有全局时钟，可按下面的格式进行时钟转换. 这个例子是2013-01-01 00:00:00 */

	return	  ((DWORD)(2013 - 1980) << 25)	/* Year = 2013 */
			| ((DWORD)1 << 21)				/* Month = 1 */
			| ((DWORD)1 << 16)				/* Day_m = 1*/
			| ((DWORD)0 << 11)				/* Hour = 0 */
			| ((DWORD)0 << 5)				/* Min = 0 */
			| ((DWORD)0 >> 1);				/* Sec = 0 */
}
