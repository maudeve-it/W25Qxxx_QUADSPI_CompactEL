/*********************************************
 * @file Dev_Inf.c
 * @brief modified by Mauro linking W25Qxxx library
 * @date: 02 april 2024
 * @version V.2.0.0
 * 
 *********************************************
 * Creating an new External loader you have to 
 * follow the below step 1 to 3 and set the
 * first field in the struct of Dev_Inf.c
 *********************************************/

#include "main.h"
#include "crc.h"
#include "quadspi.h"
#include "gpio.h"
#include "z_qflash_W25QXXX.h"

/* PLEASE NOTE: the below EXT_FLASH_ADDR_MASK is used to "convert" the "uC mapped address"
 * 				received to the "flash memory address" to read/write. That mask is used,
 * 				in below functions, in an "and" filter supposing the FLASH IS MAPPED ON 0x90000000.
 * 				In case you are using a different address you could need to change this filter
 * 				or even use another way to "extract" internal flash address from the uC memory address.
 */
#define EXT_FLASH_ADDR_MASK 0x0FFFFFFFUL
#define EXT_FLASH_ROUNDING_ADDR_MASK 0xFFFFFFFCUL		//address is rounded to values multiple of 4


#define LED_VERIFY		1
#define LED_CHKSUM		2
#define LED_SECTERAS	4
#define LED_FULLERAS	8
#define LED_WRITE		16
#define LED_READ		32
#define LED_INIT		64
#define LED_ANY			127




/* !!! EXTERNAL LOADER CONFIGURATION !!!!!!!! */

/* STEP 1 *************************************
 * remove comment in below "#define IS_LED" if you have
 * a led for the External Loader.
 * On CubeMX, led's GPIO pin label must be LED
 * ********************************************/
#define IS_LED
#define LED_LEVEL_SHOWN		((uint8_t) (LED_ANY))	//set here functions that have to blink e.g. ((uint8_t) (LED_READ+LED_WRITE))


/* STEP 2 *************************************
 * change the below #define assigning Pin name
 * and port you used for led,
 * specify also pin level to turn on led
 * ********************************************/
#ifdef IS_LED
#define LED_PIN_ON 	GPIO_PIN_SET    	//this is the GPIO level turning on led
#endif //IS_LED

/* STEP 3 *************************************
 * change the init function to call as per SPI
 * port used (hint: that's the function
 * available in spi.c, check it)
 * ********************************************/
void LOC_QSPI_Init(){
	MX_QUADSPI_Init();        // !!! this line needs to be aligned to the SPI port used!  Check this function: it is defined in spi.c !!!
}

/* END OF EXTERNAL LOADER CONFIGURATION !!!!! */


extern void SystemClock_Config(void);




void EL_Delay(uint32_t Delay) {
#define MULTIPLIER 100
uint32_t clock; 		// uC MHz

	clock=HAL_RCC_GetSysClockFreq()/1000000;
	for (uint32_t j=0; j<Delay; j++) {
		for (uint32_t k=0; (k<(clock*MULTIPLIER)); k++) {};
	}
}

void HAL_Delay(uint32_t Delay) {
	EL_Delay(Delay);
}

void LOC_LedOn(uint8_t level){
#ifdef IS_LED
	if (level & LED_LEVEL_SHOWN)
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, LED_PIN_ON);
#endif //IS_LED
}

void LOC_LedOff(uint8_t level){
#ifdef IS_LED
	if (level & LED_LEVEL_SHOWN)
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, !LED_PIN_ON);
#endif //IS_LED
}

void Qflash_Blink(uint32_t tempo1,uint32_t tempo2){
	while (1) {
	    LOC_LedOn(LED_INIT);
		EL_Delay(tempo1);
	    LOC_LedOff(LED_INIT);
		EL_Delay(tempo2);
	}
};



int Init (void){
uint8_t result=3;

//	*(uint32_t*)0xE000EDF0 = 0xA05F0000; //enable interrupts in debug

	SystemInit();

    /* ADAPTATION TO THE DEVICE
     *
     * change VTOR setting for H7 device
     * SCB->VTOR = 0x24000000 | 0x200;
     *
     * change VTOR setting for other devices
     * SCB->VTOR = 0x20000000 | 0x200;
     *
     */

//    SCB->VTOR = 0x20000000 | 0x200;
    SCB->VTOR = 0x24000000 | 0x200;

    HAL_DeInit();
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    LOC_QSPI_Init();
	MX_CRC_Init();

    __set_PRIMASK(0); //enable interrupts


    LOC_LedOn(LED_INIT);
    uint8_t k;
    for (k=0;((k<10) && (result!=HAL_OK));k++) { // retry 10 times init if returning error (result!=0)
        result=QFlash_Init();
        EL_Delay(10);
    }

	if (result!=HAL_OK) {
		Qflash_Blink(100,100);  //a blinking led indicating EL hangs
    }

	__HAL_CRC_DR_RESET(&hcrc);		//reset the CRC counter

    LOC_LedOff(LED_INIT);

    __set_PRIMASK(1); //disable interrupts

    return (!result);  //return 1 (OK) if no errors (result==0)
}



/**
  * Description :
  * Read data from the device
  * Inputs    :
  *      Address       : Write location
  *      Size          : Length in bytes
  *      buffer        : Address where to get the data to write
  * outputs   :
  *      R0             : "1" 			: Operation succeeded
  * 			 		  "0" 			: Operation failure
  * Note: Mandatory for all types except SRAM and PSRAM
  */
int Read (uint32_t Address, uint32_t Size, uint8_t* buffer){
    __set_PRIMASK(0); //enable interrupts

    Address = Address & EXT_FLASH_ADDR_MASK;
	LOC_LedOn(LED_READ);
	QFlash_Read(Address, buffer, Size);
	LOC_LedOff(LED_READ);

	__set_PRIMASK(1); //disable interrupts
	return 1;
}



/**
  * Description :
  * Write data to the device
  * Inputs    :
  *      Address       : Write location
  *      Size          : Length in bytes  
  *      buffer        : Address where to get the data to write
  * outputs   :
  *      R0           : "1" 			: Operation succeeded
  *                     "0" 			: Operation failure
  * Note: Mandatory for all types except SRAM and PSRAM	
  */
int Write (uint32_t Address, uint32_t Size, uint8_t* buffer){
    __set_PRIMASK(0); //enable interrupts

    Address = Address & EXT_FLASH_ADDR_MASK;
	LOC_LedOn(LED_WRITE);
	QFlash_Write(Address, buffer, Size);
	LOC_LedOff(LED_WRITE);

	__set_PRIMASK(1); //disable interrupts
	return 1;
} 





/**
  * Description :
  * Erase the full chip
  * Inputs    :
  *     None
  * outputs   :
  *     R0             : "1" : Operation succeeded
  * 					 "0" : Operation failure
  * Note: Not Mandatory for SRAM PSRAM and NOR_FLASH
  */
int MassErase (void){
    __set_PRIMASK(0); //enable interrupts

    LOC_LedOn(LED_FULLERAS);
	QFlash_ChipErase();
	LOC_LedOff(LED_FULLERAS);

	__set_PRIMASK(1); //disable interrupts
	return 1;
}



/**
  * Description :
  * Erase a full sector in the device
  * Inputs    :
  *      SectrorAddress	: Start of sector
  *      Size          : Size (in WORD)  
  *      InitVal       : Initial CRC value
  * outputs   :
  *     R0             : "1" : Operation succeeded
  * 			 		 "0" : Operation failure
  * Note: Not Mandatory for SRAM PSRAM and NOR_FLASH
  */
int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress) {
	EraseStartAddress = EraseStartAddress & EXT_FLASH_ADDR_MASK;
	EraseEndAddress = EraseEndAddress & EXT_FLASH_ADDR_MASK;

	EraseStartAddress = (EraseStartAddress -  (EraseStartAddress % EXT_FLASH_SECTOR_SIZE));
	while (EraseEndAddress>=EraseStartAddress) {
	    __set_PRIMASK(0); //enable interrupts
		LOC_LedOn(LED_SECTERAS);
		QFlash_SErase4k(EraseStartAddress);
		LOC_LedOff(LED_SECTERAS);
		EraseStartAddress += EXT_FLASH_SECTOR_SIZE;
		__set_PRIMASK(1); //disable interrupts
	}
 	return 1;
}



/**
  * Description :
  * Calculates checksum value of the memory zone
  * Inputs    :
  *      StartAddress  : Flash start address
  *      Size          : Size (in WORD)  
  *      InitVal       : Initial CRC value
  * outputs   :
  *     R0             : Checksum value
  * Note - Optional for all types of device
  * NOTE - keeping original ST algorithm: not verified and optimized
  */

uint32_t CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal) {
uint32_t Val;

	StartAddress = StartAddress & EXT_FLASH_ADDR_MASK;
	uint32_t EndAddress = StartAddress+Size;
	StartAddress = StartAddress & EXT_FLASH_ROUNDING_ADDR_MASK;
	__set_PRIMASK(0); //enable interrupts
	LOC_LedOn(LED_CHKSUM);
	for( StartAddress=StartAddress ; StartAddress<EndAddress; StartAddress+=4) {
		QFlash_WaitForWritingComplete();
		QFlash_Read(StartAddress,(uint8_t *) &Val, 4);
		Val=__REV(Val);
		InitVal=HAL_CRC_Accumulate(&hcrc, &Val, 1);
	}
	LOC_LedOff(LED_CHKSUM);
	__set_PRIMASK(1); //disable interrupts

	return (InitVal);
}


/**
  * Description :
  * Verify flash memory with RAM buffer and calculates checksum value of
  * the programmed memory
  * Inputs    :
  *      FlashAddr     : Flash address
  *      RAMBufferAddr : RAM buffer address
  *      Size          : Size (in WORD)  
  *      InitVal       : Initial CRC value
  * outputs   :
  *     R0             : Operation failed (address of failure)
  *     R1             : Checksum value
  * Note: Optional for all types of device
  * NOTE - not verified!
  **/


/*
uint64_t Verify (uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement){
#define BUF_SIZE 16

  uint32_t InitVal = 0;
  uint32_t VerifiedData = 0;
  uint64_t checksum;
  Size*=4;
  uint8_t Buffer[BUF_SIZE];
  uint32_t LocAddr = MemoryAddr & EXT_FLASH_ADDR_MASK;
  uint32_t posBuf;
  checksum = CheckSum((uint32_t)LocAddr + (missalignement & 0xf), Size - ((missalignement >> 16) & 0xF), InitVal);

  while (Size>VerifiedData)
  {
    __set_PRIMASK(0); //enable interrupts
	LOC_LedOn(LED_VERIFY);

	Flash_Read(MemoryAddr+VerifiedData, Buffer, BUF_SIZE);

	LOC_LedOff(LED_VERIFY);
	__set_PRIMASK(1); //disable interrupts

	posBuf=0;
	while ((Size>VerifiedData) && (posBuf<BUF_SIZE)) {
	    if (Buffer[posBuf] != *((uint8_t*)RAMBufferAddr+VerifiedData))
		   return ((checksum<<32) + ((uint64_t)MemoryAddr+VerifiedData));
	    posBuf++;
	    VerifiedData++;
	}
  }
       
  return (checksum<<32);
}
*/
