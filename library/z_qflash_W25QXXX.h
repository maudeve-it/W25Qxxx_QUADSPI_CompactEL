/*********************************************
 * @file Z_FLASH_W25QXXX.h
 * @author mauro
 * @date: 01 august 2023
 * @version V.1.0.0
 * 
 *********************************************
 * this version of library uses just polling 
 * mode transmission
 * this version of library uses standard SPI
 *********************************************
 * configure below STEP1 and STEP4.
 * Do not change STEP2 and STEP3 in this version
 *********************************************/

#ifndef INC_Z_QFLASH_W25QXXX_H_
#define INC_Z_QFLASH_W25QXXX_H_



/*||||||||||| USER/PROJECT PARAMETERS |||||||||||*/

/******************    STEP 1    ******************
 **************** PORT PARAMETERS *****************
 **** properly set the below define to address ****
 *******  the SPI port configured on CubeMX *******
 **************************************************/
#define FLASH_QSPI_PORT 	hqspi



/*****************     STEP 2      *****************
 ************* QSPI COMMUNICATION MODE *************
 ***** enable QSPI mode, uncommenting ONE row ******
 **** (Setup the same configuration on CubeMX) *****
 ***************************************************/
#define EXT_FLASH_QSPI_POLLING_MODE
//#define EXT_FLASH_QSPI_DMA_MODE



/*****************     STEP 3      *****************
 ***** READING FUNCTION BEHAVIOUR IN DMA MODE ******
 * Only for DMA mode, meaningless in polling mode **
 ** Only in indirect mode, meaningless in MM mode **
 ***************************************************
 *** If QFlash_Read() must return after the WHOLE **
 *** incoming data transfer completes: uncomment ***
 **** the below #define. If it must return just ****
 ***** after the command IS SENT to flash, let *****
 * the macro commented and use the related enquiry *
 *** library functions (QFlash_IsDataAvailable(), **
 ** QFlash_WaitForDataAvailable()) to detect the ***
 ************ availability of data read ************
 ***************************************************/
//#define FLASH_QSPI_WAIT_FOR_READING_COMPLETE


/******************    STEP 4    ******************
 **************** FLASH ACCESS MODE ***************
 **************************************************
 *** MEMORY MAPPED mode: remove comment to the ****
 *********************** below #define ************
 ******** INDIRECT mode: keep comment to #define **
 **************************************************/
//#define FLASH_QSPI_MEMORY_MAPPED



/******************    STEP 5    *******************
 **************** FLASH READING MODE ***************
 *** Library can communicate to W25Q flash memory **
 ***** using the below three instructions sets *****
 *** available. Depending on the PCB/wiring, set ***
 ******* CubeMX configuratione and the below *******
 ***************** instruction set. ****************
 ***************************************************
 **** FLASH_MODE_1	->	SPI mode
 **** FLASH_MODE_2	->	Dual mode
 **** FLASH_MODE_4	->	Quad mode
 **************************************************/
//#define FLASH_QMODE_1
//#define FLASH_QMODE_2
#define FLASH_QMODE_4



/*****************     STEP 6      *****************
 *********** set below information as per *************
 ********* chip memory used in the project *********
 ***************************************************
 ** WARNING: ***************************************
 ** Flash size must be indicated also in QUADSPI****
 ** CubeMx configuration following the formula: ****
 ** number of bytes in flash memory = 2^[FSIZE+1] **
 ** examples: **************************************
 **  1MB -> EXT_FLASH_SIZE 0100000 -> FSIZE = 19 ***
 **  8MB -> EXT_FLASH_SIZE 0800000 -> FSIZE = 22 ***
 ** 16MB -> EXT_FLASH_SIZE 1000000 -> FSIZE = 23 ***
 ***************************************************
 ** WARNING: ***************************************
 ** in case of DUAL_FLASH mode, FSIZE must *********
 ** the total flash amount. eg: ********************
 ** 2*1MB -> EXT_FLASH_SIZE 0200000 -> FSIZE = 20 **
 ***************************************************/

/* active information */
#define EXT_FLASH_PAGE_SIZE		0x0100		//256B 		page size (bytes)
#define EXT_FLASH_SECTOR_SIZE	0x1000		//4kB 		sector size (bytes)
#define EXT_FLASH_BLOCK_SIZE	0x00010000	//64kB 		block size (bytes)
#define EXT_FLASH_SIZE			0X00800000	//8MB-64MB	total size (bytes)
#define EXT_FLASH_PAGE_NUM		0x8000		//32768		pages
#define EXT_FLASH_SECTOR_NUM	0x0800		//2048 		sectors
#define EXT_FLASH_BLOCK_NUM		0x0080		//128		blocks


/* here values for the W25Q80DV/DL chips
#define EXT_FLASH_PAGE_SIZE		0x0100		//256B 		page size (bytes)
#define EXT_FLASH_SECTOR_SIZE	0x1000		//4kB 		sector size (bytes)
#define EXT_FLASH_BLOCK_SIZE	0x00010000	//64kB 		block size (bytes)
#define EXT_FLASH_SIZE			0X00100000	//1MB-8Mb	total size (bytes)
#define EXT_FLASH_PAGE_NUM		0x1000		//4096 		pages
#define EXT_FLASH_SECTOR_NUM	0x0100		//256 		sectors
#define EXT_FLASH_BLOCK_NUM		0x0010		//16 		blocks
*/

/* here values for the W25Q64JV chips
#define EXT_FLASH_PAGE_SIZE		0x0100		//256B 		page size (bytes)
#define EXT_FLASH_SECTOR_SIZE	0x1000		//4kB 		sector size (bytes)
#define EXT_FLASH_BLOCK_SIZE	0x00010000	//64kB 		block size (bytes)
#define EXT_FLASH_SIZE			0X00800000	//8MB-64Mb	total size (bytes)
#define EXT_FLASH_PAGE_NUM		0x8000		//32768		pages
#define EXT_FLASH_SECTOR_NUM	0x0800		//2048 		sectors
#define EXT_FLASH_BLOCK_NUM		0x0080		//128		blocks
*/

/* here values for the W25Q128JV chips
#define EXT_FLASH_PAGE_SIZE		0x0100		//256b 		page size (bits)
#define EXT_FLASH_SECTOR_SIZE	0x1000		//4kB 		sector size (bytes)
#define EXT_FLASH_BLOCK_SIZE	0x00010000	//64kB 		block size (bytes)
#define EXT_FLASH_SIZE			0X01000000	//16MB-128Mb	total size (bytes)
#define EXT_FLASH_PAGE_NUM		0x00010000	//32768		pages
#define EXT_FLASH_SECTOR_NUM	0x1000		//4096 		sectors
#define EXT_FLASH_BLOCK_NUM		0x0100		//256		blocks
*/

/*|||||||| END OF USER/PROJECT PARAMETERS ||||||||*/



/*|||||||||||||| LIBRARY PARAMETERS ||||||||||||||*/

/***************************************************
 ********* below information set behaviour *********
 ****** of QSPI in the various configurations  *****
 ******* (eg. read command to use in single, *******
 ************* dual, quad mode, etc.) **************
 ***************************************************/

//default configuration
#define QFLASH_INSTRUCTION_MODE		QSPI_INSTRUCTION_1_LINE
#define QFLASH_ADDRESS_SIZE			QSPI_ADDRESS_24_BITS
#define QFLASH_ALTERNATE_BYTE_MODE	QSPI_ALTERNATE_BYTES_NONE
#define QFLASH_DDR_MODE				QSPI_DDR_MODE_DISABLE
#define QFLASH_DDR_HOLD_HALF_CYCLE	QSPI_DDR_HHC_ANALOG_DELAY
#define QFLASH_SIOO_MODE			QSPI_SIOO_INST_EVERY_CMD
#define QFLASH_DUMMY_CYCLES			0

#define QFLASH_DEF_TIMEOUT			HAL_QSPI_TIMEOUT_DEFAULT_VALUE
#define QFLASH_BSY_TIMEOUT			100000 //a chip erase command requires up to 100s. (see "AC Electrical Characteristics" on Winbond datasheets)

#ifdef FLASH_QMODE_1
//quad mode parameters
#define QFLASH_ADDRESS_MODE			QSPI_ADDRESS_1_LINE
#define QFLASH_DATA_MODE			QSPI_DATA_1_LINE
//quad mode read parameters
#define QFLASH_READ_COMMAND			W25_FREAD
#define QFLASH_READ_DUMMYC			8
#define QFLASH_ALT_BYTES			0xFF
#define QFLASH_ALT_BYTES_S			QSPI_ALTERNATE_BYTES_8_BITS
#define QFLASH_ALT_BYTES_M			QSPI_ALTERNATE_BYTES_NONE
//quad mode write parameters
#define QFLASH_WRITE_COMMAND		W25_PAGE_P
#define QFLASH_WDATA_MODE			QSPI_DATA_1_LINE

#elif defined FLASH_QMODE_2
//dual mode parameters
#define QFLASH_ADDRESS_MODE			QSPI_ADDRESS_2_LINES
#define QFLASH_DATA_MODE			QSPI_DATA_2_LINES
//dual mode read parameters
#define QFLASH_READ_COMMAND			W25_FREAD_DUAL
#define QFLASH_READ_DUMMYC			0
#define QFLASH_ALT_BYTES			0xFF
#define QFLASH_ALT_BYTES_S			QSPI_ALTERNATE_BYTES_8_BITS
#define QFLASH_ALT_BYTES_M			QSPI_ALTERNATE_BYTES_2_LINES
//dual mode write parameters
#define QFLASH_WRITE_COMMAND		W25_PAGE_P
#define QFLASH_WDATA_MODE			QSPI_DATA_1_LINE

#elif defined FLASH_QMODE_4
//quad mode parameters
#define QFLASH_ADDRESS_MODE			QSPI_ADDRESS_4_LINES
#define QFLASH_DATA_MODE			QSPI_DATA_4_LINES
//quad mode read parameters
#define QFLASH_READ_COMMAND			W25_FREAD_QUAD
#define QFLASH_READ_DUMMYC			4
#define QFLASH_ALT_BYTES			0xFF
#define QFLASH_ALT_BYTES_S			QSPI_ALTERNATE_BYTES_8_BITS
#define QFLASH_ALT_BYTES_M			QSPI_ALTERNATE_BYTES_4_LINES
//quad mode write parameters
#define QFLASH_WRITE_COMMAND		W25_FPAGE_P_QUAD
#define QFLASH_WDATA_MODE			QSPI_DATA_4_LINES
#endif

/*|||||||||||| END OF LIBRARY PARAMETERS |||!!||||||||*/




/*||||||||||||||| DEVICE PARAMETERS ||||||||||||||||||*/
// W25QXX EEPROM family commands

#define W25_RESET_EN		0x66	//sequence is 0x66 + 0x99 + 30us delay
#define W25_RESET			0x99 	//sequence is 0x66 + 0x99 + 30us delay
#define W25_W_ENABLE		0x06
#define W25_READ 			0x03
#define W25_FREAD 			0x0B
#define W25_FREAD_DUAL		0xBB
#define W25_FREAD_QUAD		0xEB
#define W25_PAGE_P 			0x02
#define W25_FPAGE_P_QUAD 	0x32
#define W25_S_ERASE4K 		0x20
#define W25_B_ERASE32K		0x52
#define W25_B_ERASE64K		0xD8
#define W25_CH_ERASE		0xC7
#define W25_POWERDOWN		0xB9
#define W25_POWERUP_ID		0xAB
#define W25_JEDEC_ID		0x9F
#define W25_R_SR1			0x05
#define W25_R_SFPD_REG		0x5A
#define W25_W_DISABLE		0x04
#define W25_MAN_DEVICE_ID	0x90
#define W25_R_SR2			0x35
#define W25_W_SR2			0x31
#define W25_R_SR3			0x15
#define W25_W_SR3			0x11
#define W25_SR_W_ENABLE		0x50




/* unused commands
#define W25_MAN_DEVICE_ID	0x90
#define W25_UNIQUE_ID		0x4B
#define W25_EP_SUS	 		0x75
#define W25_EP_RES	 		0x7A
#define W25_W_SR1			0x01
#define W25_R_SFPD_REG		0x5A
#define W25_E_SEC_REG		0x44
#define W25_P_SEC_REG		0x42
#define W25_R_SEC_REG		0x48
#define W25_G_BL_LOCK		0x7E
#define W25_G_BL_UNLK		0x98
#define W25_R_BL_LOCK		0x3D
#define W25_I_BL_LOCK		0x36
#define W25_I_BL_UNLK		0x39
#define W25_EP_SUSPEND		0x75
#define W25_EP_RESUME		0x75
 end of unused commands */
// W25QXX EEPROM family commands

#define W25_DUMMY			0x00	//dummy MUST be 0x00, in "read manufacturer"

// bit masks of W25QXX SR1, SR2, SR3 registers
#define SR1_BIT_BSY			(01U)  //status only: 1 means busy device
#define SR1_BIT_WEL			(02U)  //status only: 1 means write enabled. set by W25_W_ENABLE command

/* unused bitmasks
#define SR1_BIT_BP0			(04U)  //writable: block protect bit 0
#define SR1_BIT_BP1			(08U)  //writable: block protect bit 1
#define SR1_BIT_BP2			(10U)  //writable: block protect bit 2
#define SR1_BIT_TB			(20U)  //writable: top(=1)/bottom(=0) starting, block protection bit
#define SR1_BIT_SEC			(40U)  //writable: sector(4kb)/block(64kb) block protection (1=sector)
#define SR1_BIT_SRP			(80U)  //writable: set SR registers protection (together with SRL)
#define SR2_BIT_SRL			(01U)  //writable: set SR registers protection (together with SRL)
#define SR2_BIT_QE			(02U)  //writable: enable (=1) QUAD SPI mode. if =0 SPI is Standard/Dual
#define SR2_BIT_LB1			(08U)  //OTP: 1 means Security Register 1 is permanently set readonly
#define SR2_BIT_LB2			(10U)  //OTP: 1 means Security Register 2 is permanently set readonly
#define SR2_BIT_LB3			(20U)  //OTP: 1 means Security Register 3 is permanently set readonly
#define SR2_BIT_CMP			(40U)  //writable: complement protect: reverse protection of BP0-1-2,TB,SEC
#define SR2_BIT_SUS			(80U)  //writable: suspend status: 1 indicates erase/program suspended
#define SR3_BIT_WPS			(04U)  //writable: Write protect scheme: 1=using individual block flag, 0=using BPx, etc, flags
#define SR3_BIT_DRV0		(20U)  //writable: sets output driver strength
#define SR3_BIT_DRV1		(40U)  //writable: sets output driver strength
 end of W25QXX SR1, SR2, SR3 registers bitmasks */

/*||||||||||| END OF DEVICE PARAMETERS ||||||||||||*/




/*||||||||||| DEFINITIONS FOR E.L. GENERATION ||||||||||||*/

#ifndef HAL_MAX_DELAY		// if there is no the uc HAL definition (building an E.L.

/**
  * @brief  HAL Status structures definition
  */
typedef enum
{
  HAL_OK       = 0x00,
  HAL_ERROR    = 0x01,
  HAL_BUSY     = 0x02,
  HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;

#endif

/*||||||||||| END OF DEFINITIONS FOR E.L. GENERATION ||||||||||||*/






HAL_StatusTypeDef 	QFlash_Reset();
HAL_StatusTypeDef	QFlash_Init();
uint8_t 			QFlash_IsQSPIAvailable();
HAL_StatusTypeDef 	QFlash_WaitForQSPIAvailable(uint32_t timeout);
uint8_t 			QFlash_IsDataAvailable();
HAL_StatusTypeDef 	QFlash_WaitForDataAvailable(uint32_t timeout);

#ifdef FLASH_QSPI_MEMORY_MAPPED

#define ON_EXT_FLASH __attribute__((section("ExtFlashSection"))) __attribute__((aligned(4)))
HAL_StatusTypeDef 	QFlash_EnableMemoryMappedMode(void);

#else

HAL_StatusTypeDef 	QFlash_ReadDevID(uint8_t *dataptr);
HAL_StatusTypeDef 	QFlash_ReadManufactutrerAndDevID(uint16_t *dataptr);
HAL_StatusTypeDef	QFlash_ReadJedecID(uint8_t *data);
HAL_StatusTypeDef	QFlash_ReadSFDP(uint8_t* data);

HAL_StatusTypeDef	QFlash_ChipErase();
HAL_StatusTypeDef	QFlash_SErase4k(uint32_t addr);
HAL_StatusTypeDef	QFlash_BErase32k(uint32_t addr);
HAL_StatusTypeDef	QFlash_BErase64k(uint32_t addr);

HAL_StatusTypeDef	QFlash_Read(uint32_t address,  uint8_t* buffer, uint32_t dataSize);
HAL_StatusTypeDef 	QFlash_WaitForDataAvailable(uint32_t timeout);
HAL_StatusTypeDef	QFlash_Write(uint32_t addr, uint8_t* data, uint32_t dataSize);
HAL_StatusTypeDef	QFlash_WaitForWritingComplete();
HAL_StatusTypeDef	QFlash_WriteEnable();
HAL_StatusTypeDef 	QFlash_VolatileWriteEnable();
HAL_StatusTypeDef	QFlash_WriteDisable();
HAL_StatusTypeDef	QFlash_PowerUp();
HAL_StatusTypeDef	QFlash_PowerDown();
HAL_StatusTypeDef 	QFlash_ReadSR1(uint8_t *dataptr);
HAL_StatusTypeDef 	QFlash_ReadSR2(uint8_t *dataptr);
HAL_StatusTypeDef 	QFlash_ReadSR3(uint8_t *dataptr);
HAL_StatusTypeDef 	QFlash_CheckSR1();
HAL_StatusTypeDef  	QFlash_WriteSR2(uint8_t data);
HAL_StatusTypeDef  	QFlash_WriteSR3(uint8_t data);

#endif //FLASH_QSPI_MEMORY_MAPPED


void 	 			DataReader_WaitForReceiveDone();
void 	 			DataReader_ReadData(uint32_t address24, uint8_t* buffer, uint32_t length);
void 	 			DataReader_StartDMAReadData(uint32_t address24, uint8_t* buffer, uint32_t length);


#endif /* INC_Z_QFLASH_W25QXXX_H_ */
