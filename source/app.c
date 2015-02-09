//#include "communication.h"
#include "mmd.h"
#include "string.h"
#include "app.h"
//#include "uart.h"
#include "typedefs.h"
#include "eep.h"
#include "mb.h"

#define REG_INPUT_START 1
#define REG_INPUT_NREGS 17



// APP Structure
typedef struct _APP
{
	UINT8 model[MSG_LENGTH];
	UINT8 eepUpdate;


	UINT8	valueBuffer[40]; // to store current values of line   
}APP;

#pragma idata APP_DATA
APP app = {{0},0};
MMD_Config mmdConfig = {0};
UINT8 data[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456";//"01 IDEONICS IDEAS & ELECTRONICS S";
#pragma idata


void APP_init(void)
{
	UINT8 i;
	UINT16 sbaudrate , saddress;
	eMBErrorCode    eStatus;

	
	sbaudrate = 19200;
	saddress = 1;
	//modbus configuration
	eStatus = eMBInit( MB_RTU, ( UCHAR )saddress, 0, sbaudrate, MB_PAR_NONE);
	eStatus = eMBEnable(  );	/* Enable the Modbus Protocol Stack. */

	for(i = 0; i < 33; i++)
	{
		app.valueBuffer[i] = data[i];
	}
	MMD_clearSegment(0);
	mmdConfig.startAddress = 0;
	mmdConfig.length = 8;
	mmdConfig.symbolCount = 8;//strlen(app.valueBuffer);
	mmdConfig.symbolBuffer = app.valueBuffer;
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 0 , &mmdConfig);

	MMD_clearSegment(1);
	mmdConfig.startAddress = 8;
	mmdConfig.length = MMD_MAX_CHARS;
	mmdConfig.symbolCount = MMD_MAX_CHARS;//strlen(app.valueBuffer);
	mmdConfig.symbolBuffer = (app.valueBuffer+8);
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 1 , &mmdConfig);

	MMD_clearSegment(2);
	mmdConfig.startAddress = 16;
	mmdConfig.length = MMD_MAX_CHARS;
	mmdConfig.symbolCount = MMD_MAX_CHARS;//strlen(app.valueBuffer);
	mmdConfig.symbolBuffer = (app.valueBuffer+16);
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 2 , &mmdConfig);
/*

	MMD_clearSegment(3);
	mmdConfig.startAddress = 21;
	mmdConfig.length = 7;
	mmdConfig.symbolCount = 7;//strlen(app.valueBuffer);
	mmdConfig.symbolBuffer = (app.valueBuffer+21);
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 3 , &mmdConfig);

	MMD_clearSegment(4);
	mmdConfig.startAddress = 28;
	mmdConfig.length = 5;
	mmdConfig.symbolCount = 5;
	mmdConfig.symbolBuffer = (app.valueBuffer+28);
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 4 , &mmdConfig);
*/
}

void APP_task(void)
{


}

UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket)
{

	UINT8 i;

	UINT8 rxCode = rxPacket[0];
	UINT8 length = 0;

	switch( rxCode )
	{
		case CMD_SET_MODEL:
			app.eepUpdate = TRUE;
			strcpy(app.model,&rxPacket[1]);
			MMD_clearSegment(0);
			mmdConfig.startAddress = 0;
			mmdConfig.length = MMD_MAX_CHARS;
			mmdConfig.symbolCount = strlen(app.model);
			mmdConfig.symbolBuffer = app.model;
			mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
			MMD_configSegment( 0 , &mmdConfig);

			*txCode = CMD_SET_MODEL;
			break;

		default:
		break;

	}

	return length;

}


eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{

    eMBErrorCode    eStatus = MB_ENOERR;

    int             iRegIndex;
/*
    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0         {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
 
*/
    return eStatus;

}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{

	UINT8	starting_add = usAddress;
	UINT8	no_regs		 = usNRegs * 2;
	eMBErrorCode    eStatus = MB_ENOERR;
	UINT8 i = 0;

	switch(eMode)
	{
	case MB_REG_WRITE:

    
	while( no_regs > 0)
	{

		app.valueBuffer[i++] = * pucRegBuffer++;

		starting_add++;
		no_regs	--;
	}
//	app.valueBuffer[i++] = 0;
    break;

 	case MB_REG_READ: 

	while(no_regs > 0)
	{

			* pucRegBuffer++ =	'A';
			* pucRegBuffer++ =	'B';		
			
			* pucRegBuffer++ = 'C';
			* pucRegBuffer++ = 'D';

						



		starting_add++;
		no_regs	--;	
	}
   	 break;
	}

//	DigitDisplay_updateBufferPartial(app.valueBuffer, 0, NO_DIGITS);

	MMD_clearSegment(0);
	mmdConfig.startAddress = 0;
	mmdConfig.length = 3;
	mmdConfig.symbolCount = 3;//strlen(app.valueBuffer);
	mmdConfig.symbolBuffer = app.valueBuffer;
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 0 , &mmdConfig);
/*
	MMD_clearSegment(1);
	mmdConfig.startAddress = 3;
	mmdConfig.length = MMD_MAX_CHARS;
	mmdConfig.symbolCount = MMD_MAX_CHARS;//strlen(app.valueBuffer);
	mmdConfig.symbolBuffer = (app.valueBuffer+3);
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 1 , &mmdConfig);

	MMD_clearSegment(2);
	mmdConfig.startAddress = 13;
	mmdConfig.length = 8;
	mmdConfig.symbolCount = 8;//strlen(app.valueBuffer);
	mmdConfig.symbolBuffer = (app.valueBuffer+13);
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 2 , &mmdConfig);

	MMD_clearSegment(3);
	mmdConfig.startAddress = 21;
	mmdConfig.length = 7;
	mmdConfig.symbolCount = 7;//strlen(app.valueBuffer);
	mmdConfig.symbolBuffer = (app.valueBuffer+21);
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 3 , &mmdConfig);

	MMD_clearSegment(4);
	mmdConfig.startAddress = 28;
	mmdConfig.length = 5;
	mmdConfig.symbolCount = 5;
	mmdConfig.symbolBuffer = (app.valueBuffer+28);
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 4 , &mmdConfig);
*/

	return eStatus;
  }


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}

