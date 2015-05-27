/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include <includes.h>
/* Definitions of physical drive number for each media */
extern MSD_CARDINFO SD0_CardInfo;


#define SECTOR_SIZE		512
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
		case FS_SD :		/* SD卡 */
			stat = MSD0_Init();
			if(stat==0) 
				return RES_OK; 
			else
				return STA_NOINIT; 

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

	int Status;
	
	if(!count)  
		return RES_PARERR;  /* count不能等于0，否则返回参数错误 */
	
	switch (pdrv)
	{
		case FS_SD :
			if(count == 1)
            {   
				Status =  MSD0_ReadSingleBlock( sector ,buff );
				if(Status == 0) 
					return RES_OK;
				else
					return RES_ERROR;    
			}                                                
			else	/* 多个sector的读操作 */     
			{  
				Status = MSD0_ReadMultiBlock( sector , buff ,count);
				if(Status == 0)
					return RES_OK;
				else
					return RES_ERROR; 
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
	return RES_PARERR;
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
	
	int Status;
	if( !count )
	{    
		return RES_PARERR;  /* count不能等于0，否则返回参数错误 */
	}
	
	switch (pdrv)
	{
		case FS_SD :
			if(count==1) /* 1个sector的写操作 */      
			{   
				Status = MSD0_WriteSingleBlock( sector , (uint8_t *)(&buff[0]) ); 
				if(Status == 0)
					return RES_OK;
				else
					return RES_ERROR;
			}                                                
			else /* 多个sector的写操作 */    
			{  
				Status = MSD0_WriteMultiBlock( sector , (uint8_t *)(&buff[0]) , count );
				if(Status == 0)
					return RES_OK;
				else
					return RES_ERROR;   
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
	return RES_PARERR;
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

	switch (pdrv) {
	case FS_SD :
		MSD0_GetCardInfo(&SD0_CardInfo);
		switch (cmd) 
		{
			case CTRL_SYNC : 
				return RES_OK;
			case GET_SECTOR_COUNT : 
				*(DWORD*)buff = SD0_CardInfo.Capacity/SD0_CardInfo.BlockSize;
				return RES_OK;
			case GET_BLOCK_SIZE :
				*(WORD*)buff = SD0_CardInfo.BlockSize;
				return RES_OK;	
			case CTRL_POWER :
				break;
			case CTRL_LOCK :
				break;
			case CTRL_EJECT :
				break;
		} 
		break;
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
