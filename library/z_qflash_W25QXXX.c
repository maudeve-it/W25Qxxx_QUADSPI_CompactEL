/*********************************************
 * @file Z_QFLASH_W25QXXX.c
 * @author mauro
 * @date: 01 august 2024
 * @version V.1.0.0
 * 
 *********************************************
 * this version of library uses QSPI port
 * mode transmission
 * this version of library uses QUADSPI flash mode
 *********************************************
 * it previously neets to setup
 * configuration in Z_QFLASH_W25QXXX.h
 *********************************************/


#include "main.h"
#include "z_qflash_W25QXXX.h"

extern QSPI_HandleTypeDef FLASH_QSPI_PORT;



/**************************
 * @BRIEF	Desault configuration
 * 			of the "sCommand" parameters set
 * 			for "HAL_QSPI_Command()"
 * @PARAM	SCommand	parameters set to setup
 **************************/
void QFlash_DefaultCmd(QSPI_CommandTypeDef *sCommand) {

    sCommand->InstructionMode = QFLASH_INSTRUCTION_MODE;
    sCommand->AddressMode = QFLASH_ADDRESS_MODE;
    sCommand->AddressSize = QFLASH_ADDRESS_SIZE;
    sCommand->DataMode = QFLASH_DATA_MODE;
    sCommand->AlternateByteMode = QFLASH_ALTERNATE_BYTE_MODE;
    sCommand->DdrMode = QFLASH_DDR_MODE;
    sCommand->DdrHoldHalfCycle = QFLASH_DDR_HOLD_HALF_CYCLE;
    sCommand->SIOOMode = QFLASH_SIOO_MODE;
    sCommand->DummyCycles = QFLASH_DUMMY_CYCLES;
    return;
}






/******************************************************************
 * "result[] string will contain:
 * 		result[0]		Jedec Manufacturer ID (0xEF means Winbond)
 * 		result[1]		Memory Type
 * 		result[2]		Capacity
 ******************************************************************
 * Memory Capacity code:
 * 		10H ->	 5Mb		11H ->  10Mb		12H ->  20Mb
 * 		13H ->  40Mb		14H ->  80Mb		15H ->  16Mb
 * 		16H ->  32Mb		17H ->  64Mb		18H -> 128Mb
 * 		19H -> 256Mb		20H -> 512Mb		21H ->   1Gb
 ******************************************************************/
HAL_StatusTypeDef QFlash_ReadJedecID(uint8_t *data) {
QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction	= W25_JEDEC_ID;
	sCommand.DataMode 		= QSPI_DATA_1_LINE;
	sCommand.AddressMode	= QSPI_ADDRESS_NONE;
	sCommand.Address		= 0;
    sCommand.NbData			= 3;
	sCommand.DummyCycles 	= 0;

	if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
		return HAL_ERROR;
	}
    if (HAL_QSPI_Receive(&FLASH_QSPI_PORT, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) { // Receive data
    	return HAL_ERROR;
    }
	return HAL_OK;
}




/*********************************
 * @RETURN	256byte SFDP register content:
 *********************************/
HAL_StatusTypeDef QFlash_ReadSFDP(uint8_t* data) {
QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction	= W25_R_SFPD_REG;
    sCommand.DataMode 		= QSPI_DATA_1_LINE;
    sCommand.AddressMode	= QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize 	= QSPI_ADDRESS_24_BITS;
    sCommand.Address		= 0;
    sCommand.NbData			= 256;
    sCommand.DummyCycles 	= 8;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return HAL_ERROR;
    }
   	if (HAL_QSPI_Receive(&FLASH_QSPI_PORT, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) { // Receive data
   	    return HAL_ERROR;
   	}
	return HAL_OK;
}




/******************************************************************
 * @BRIEF	reset memory chip, then
 * 			read manufacutrer and device ID
 * 			checking if the connected device is a Winbond Flash
 ******************************************************************/
HAL_StatusTypeDef QFlash_Init(){
uint8_t data[256];
QSPI_CommandTypeDef sCommand = {0};

	HAL_Delay(6);	// supposing init is called on system startup: 5 ms (tPUW) required after power-up to be fully available

// send a "reset enable" command
	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction = W25_RESET_EN;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.DataMode = QSPI_DATA_NONE;
	if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {		//Send command
	   return HAL_ERROR;
	}

	HAL_Delay(1);									// 30us needed by resetting

// send a "reset" command
	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction = W25_RESET;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;
	sCommand.DataMode = QSPI_DATA_NONE;
	if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {		//Send command
	   return HAL_ERROR;
	}

	HAL_Delay(1);									// 30us needed by resetting

//	if (CheckSR1()== HAL_ERROR){
//		return HAL_ERROR;
//	}

// testing if an SFPD device is connected and working
	for (uint8_t k=0;k!=3;k++)
		  data[k]=0xFF;
	if (QFlash_ReadSFDP(data))
		return HAL_ERROR;
	if (!((data[0]=='S') && (data[1]=='F') && (data[2]=='D') && (data[3]=='P')))
		if (!((data[0]=='S') && (data[2]=='F') && (data[5]=='D') && (data[7]=='P')))  //this is in case of dual flash configuration
			return HAL_ERROR;

//testing if it is a Winbond memory
	if (QFlash_ReadJedecID(data)) //select the memSize byte
		return HAL_ERROR;;
	if (data[0] != 0xEF)  // if ManufacturerID is not Winbond (0xEF)
		return HAL_ERROR;

#ifdef FLASH_QSPI_MEMORY_MAPPED
	if (QSPI_EnableMemoryMappedMode()==HAL_ERROR)
		return HAL_ERROR;
#endif //FLASH_QSPI_MEMORY_MAPPED


	return HAL_OK;  //return memSize as per table in Flash_ReadJedecID() definition
}




#ifdef FLASH_QSPI_MEMORY_MAPPED


HAL_StatusTypeDef QSPI_EnableMemoryMappedMode(void) {
	QSPI_CommandTypeDef sCommand = {0};
	QSPI_MemoryMappedTypeDef sMemMappedCfg = {0};

	// Setup reading command
	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction		= QFLASH_READ_COMMAND;
	sCommand.DummyCycles 		= QFLASH_READ_DUMMYC;
    sCommand.AlternateBytes     = QFLASH_ALT_BYTES;
    sCommand.AlternateBytesSize = QFLASH_ALT_BYTES_S;
    sCommand.AlternateByteMode  = QFLASH_ALT_BYTES_M;

	// Set memory-mapped mode
	sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

	// go on
	if (HAL_QSPI_MemoryMapped(&hqspi, &sCommand, &sMemMappedCfg) != HAL_OK) {
		return HAL_ERROR;
	}

//	HAL_Delay(100);

	return HAL_OK;
}

#else


/**************************
 * @BRIEF	reads from Flash Eeprom
 * 			using "communication mode" selected by
 * 			command doesn't check for the BUSY flag in SR1
 * 			that must be done before calling this function
 * 			current version of library doesn't need it
 * @PARAM	addr		EEPROM address to start reading
 *  		data		buffer to fill with read data
 * 			dataSize	number of bytes to read
 **************************/
HAL_StatusTypeDef QFlash_Read(uint32_t address,  uint8_t* buffer, uint32_t dataSize) {
QSPI_CommandTypeDef sCommand = {0};

	if (dataSize==0)
		return HAL_OK;

	QFlash_DefaultCmd(&sCommand);
    sCommand.Instruction		= QFLASH_READ_COMMAND;
    sCommand.Address			= address;
    sCommand.NbData				= dataSize;
    sCommand.DummyCycles 		= QFLASH_READ_DUMMYC;
    sCommand.AlternateBytes     = QFLASH_ALT_BYTES;
    sCommand.AlternateBytesSize = QFLASH_ALT_BYTES_S;
    sCommand.AlternateByteMode  = QFLASH_ALT_BYTES_M;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return HAL_ERROR;
    }
    if (HAL_QSPI_Receive(&FLASH_QSPI_PORT, buffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) { // Receive data
    	return HAL_ERROR;
    }
	return HAL_OK;
}



HAL_StatusTypeDef QFlash_WriteEnable(){
QSPI_CommandTypeDef sCommand = {0};


//Send a "Write Enable" command
	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction	= W25_W_ENABLE;
	sCommand.DataMode 		= QSPI_DATA_NONE;
	sCommand.AddressMode	= QSPI_ADDRESS_NONE;

	if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
		return HAL_ERROR;
	}

/*
//Wait for Write Enable bit set

	QSPI_AutoPollingTypeDef sConfig;

	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction = W25_R_SR1;
	sCommand.DataMode = QSPI_DATA_1_LINE;
	sCommand.AddressMode = QSPI_ADDRESS_NONE;

	sConfig.Mask = SR1_BIT_WEL;
	sConfig.Match = 2;
	sConfig.MatchMode = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval = 0x10;
	sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_QSPI_AutoPolling(&FLASH_QSPI_PORT, &sCommand, &sConfig, QFLASH_BSY_TIMEOUT) != HAL_OK) {
		return HAL_ERROR;
	}
*/

	return HAL_OK;

}





HAL_StatusTypeDef QFlash_WriteDisable(){
	QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction	= W25_W_DISABLE;
    sCommand.DataMode 		= QSPI_DATA_NONE;
    sCommand.AddressMode	= QSPI_ADDRESS_NONE;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return HAL_ERROR;
    }

    return HAL_OK;
}





/**********************************
 * @BRIEF	read STATUS REGISTER 1
 * @RETURN
 *********************************/
uint8_t QFlash_ReadSR1(){
uint8_t data[2];
QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction	= W25_R_SR1;
    sCommand.DataMode 		= QSPI_DATA_1_LINE;
	sCommand.AddressMode	= QSPI_ADDRESS_NONE;
	sCommand.AddressSize	= QSPI_ADDRESS_NONE;
	sCommand.Address		= 0x0;
    sCommand.NbData			= 2;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return 0;
    }
    if (HAL_QSPI_Receive(&FLASH_QSPI_PORT, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) { // Receive data
    	return 0;
    }
    if (data[0]!=data[1])
		__NOP();
	return data[0];
}





/**********************************
 * @BRIEF	read STATUS REGISTER 2
 * @RETURN
 *********************************/
uint8_t QFlash_ReadSR2(){
uint8_t data[10];
QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction	= W25_R_SR2;
    sCommand.DataMode 		= QSPI_DATA_1_LINE;
	sCommand.AddressMode	= QSPI_ADDRESS_NONE;
	sCommand.AddressSize	= QSPI_ADDRESS_NONE;
	sCommand.Address		= 0x0;
    sCommand.NbData			= 10;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return 0;
    }
    if (HAL_QSPI_Receive(&FLASH_QSPI_PORT, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) { // Receive data
    	return 0;
    }
	return data[0];
}





/**********************************
 * @BRIEF	read STATUS REGISTER 3
 * @RETURN
 *********************************/
uint8_t QFlash_ReadSR3(){
uint8_t data[10];
QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction	= W25_R_SR3;
    sCommand.DataMode 		= QSPI_DATA_1_LINE;
	sCommand.AddressMode	= QSPI_ADDRESS_NONE;
	sCommand.AddressSize	= QSPI_ADDRESS_NONE;
	sCommand.Address		= 0x0;
    sCommand.NbData			= 10;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return 0;
    }
    if (HAL_QSPI_Receive(&FLASH_QSPI_PORT, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) { // Receive data
    	return 0;
    }
	return data[0];
}






HAL_StatusTypeDef  QFlash_WriteSR2(uint8_t data){
	QSPI_CommandTypeDef sCommand = {0};

		if (QFlash_WriteEnable())
			return HAL_ERROR;

		QFlash_DefaultCmd(&sCommand);
		sCommand.Instruction	= W25_W_SR2;
	    sCommand.DataMode 		= QSPI_DATA_1_LINE;
		sCommand.AddressMode	= QSPI_ADDRESS_NONE;
		sCommand.AddressSize	= QSPI_ADDRESS_NONE;
		sCommand.Address		= 0x0;
		sCommand.NbData			= 1;

		if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {		//Send command
	        return HAL_ERROR;
	    }
	    if (HAL_QSPI_Transmit(&FLASH_QSPI_PORT, &data, QFLASH_DEF_TIMEOUT) != HAL_OK) { 	//Send data
	        return HAL_ERROR;
	    }

		QFlash_WaitForWritingComplete();

		return HAL_OK;
}





HAL_StatusTypeDef  QFlash_WriteSR3(uint8_t data){
	QSPI_CommandTypeDef sCommand = {0};

		if (QFlash_WriteEnable())
			return HAL_ERROR;

		QFlash_DefaultCmd(&sCommand);
		sCommand.Instruction	= W25_W_SR3;
	    sCommand.DataMode 		= QSPI_DATA_1_LINE;
		sCommand.AddressMode	= QSPI_ADDRESS_NONE;
		sCommand.AddressSize	= QSPI_ADDRESS_NONE;
		sCommand.Address		= 0x0;
		sCommand.NbData			= 1;

		if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {		//Send command
	        return HAL_ERROR;
	    }
	    if (HAL_QSPI_Transmit(&FLASH_QSPI_PORT, &data, QFLASH_DEF_TIMEOUT) != HAL_OK) { 	//Send data
	        return HAL_ERROR;
	    }

		QFlash_WaitForWritingComplete();

		return HAL_OK;
}






/*********************************
 * just a funcion testing Status
 * Register 1 reading
 * verifies flash is not busy and
 * not write-enabled
 *********************************/
HAL_StatusTypeDef CheckSR1() {
	HAL_StatusTypeDef result = HAL_OK;
	uint8_t sr;

	sr=QFlash_ReadSR1();

//bitwise test of the SR1 content
	if (!((sr & 1)==0)) {  //why flash is busy? It shouldn't
		result=HAL_ERROR;
	} else if (!((sr & 2)==0)) {  //if write_enabled, disable it
		QFlash_WriteDisable();
		HAL_Delay(1);
		sr=QFlash_ReadSR1();
		if (!((sr & 2)==0)){ //why flash is still writeable? It shouldn't
			result=HAL_ERROR;
		}
	}
	return result;
}













/**************************
 * @BRIEF	keeps looping inside this function until "BUSY" bit in SR1 register
 * 			becomes 0, meaning that the runnin data operation (writing or erasing)
 * 			on the chip, ended
 **************************/
HAL_StatusTypeDef QFlash_WaitForWritingComplete(){
	QSPI_CommandTypeDef sCommand = {0};


/* using HAL_QSPI_AutoPolling()*/

QSPI_AutoPollingTypeDef sConfig;

QFlash_DefaultCmd(&sCommand);
    sCommand.Instruction = W25_R_SR1;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.DataMode = QSPI_DATA_1_LINE;

    sConfig.Mask = SR1_BIT_BSY;
    sConfig.Match = 0;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0x10;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&FLASH_QSPI_PORT, &sCommand, &sConfig, QFLASH_BSY_TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

/* alternative implementation "SPI style"
 * looping, directly  reading Status Register 1
 * until Busy flag resets
 * (k just count loops)
 */
/*
	uint8_t sr;
    uint32_t k=0;
	do {
		sr=QFlash_ReadSR1();
		sr=(sr & 1);
		k+=1;
	}	while (sr);

*/

    return HAL_OK;

}







/***********************************************************************
 * @BRIEF	it writes into a single FLASH page
 * 			function doesn't check for the BUSY flag in SR1
 * 			function doesn't check for the EEPROM writing enabled
 * 			function doesn't wait for the writing complete
 * 			function doesn't check for the EEPROM page boundary override
 * @PARAM	addr		EEPROM address to start writing
 *  		data		buffer containing data to write into EEPROM
 * 			dataSize	number of bytes to write
 ***********************************************************************/
HAL_StatusTypeDef QFlash_WriteASinglePage(uint32_t addr, uint8_t* data, uint16_t dataSize){
	QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	if (!dataSize)
		return HAL_OK;

    sCommand.Instruction	= QFLASH_WRITE_COMMAND;
	sCommand.AddressMode	= QSPI_ADDRESS_1_LINE;
    sCommand.DataMode 		= QFLASH_WDATA_MODE;
	sCommand.NbData			= dataSize;
	sCommand.Address		= addr;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {		//Send command
        return HAL_ERROR;
    }
    if (HAL_QSPI_Transmit(&FLASH_QSPI_PORT, data, QFLASH_DEF_TIMEOUT) != HAL_OK) { 	//Send data
        return HAL_ERROR;
    }
	return HAL_OK;
}




/**********************************************************************
 * @BRIEF	function writing into EEPROM
 * 			Handling "write enable" commands
 * 			It splits (if needed) received data into the single pages,
 * 			lounching writing sessions for each page
 * 			and waiting the writing complete each time
 * @PARAM	addr		EEPROM address to start writing
 *  		data		buffer containing data to write into EEPROM
 * 			dataSize	number of bytes to write
 ***********************************************************************/
HAL_StatusTypeDef  QFlash_Write(uint32_t addr, uint8_t* data, uint32_t dataSize){
uint32_t quota;
uint32_t inpage_addr;

	if (dataSize==0)
		return HAL_OK;

	// quota is the data size transferred until now
	quota=0;

	// define the starting write position inside the first Flash page to write...
	inpage_addr=addr & (EXT_FLASH_PAGE_SIZE-1);

	// ... so I can detect if more than 1 Flash page has still to be written
	while ((dataSize-quota+inpage_addr)>EXT_FLASH_PAGE_SIZE){
	//loop here, writing separate flash pages...

		if (QFlash_WriteEnable())
			return HAL_ERROR;
//		if (QFlash_WriteASinglePage(addr+quota,data+quota,dataSize-quota))
		if (QFlash_WriteASinglePage(addr+quota,data+quota,EXT_FLASH_PAGE_SIZE-inpage_addr))
			return HAL_ERROR;

		quota+=(EXT_FLASH_PAGE_SIZE-inpage_addr);
		// having aligned data to page border on the first writing
		// next writings start from 0 position inside a page
		inpage_addr=0;
		QFlash_WaitForWritingComplete();
	}
	// now just the final Flash page...
	if (dataSize-quota) {

		if (QFlash_WriteEnable())
			return HAL_ERROR;
		if (QFlash_WriteASinglePage(addr+quota,data+quota,dataSize-quota))
			return HAL_ERROR;
		QFlash_WaitForWritingComplete();
	}
	return HAL_OK;
}




/**********************************
 * @BRIEF	Erase to 0XFF all bytes in a 4k block
 * 			4k block bounary is 0x1000, that means:
 * 			0x1000, 0x2000, 0x3000, ...
 * 			waiting the writing complete in each page
 * @PARAM	addr	starting erase address
 * 					(it must be a 4k sector boundary)
 *********************************/
HAL_StatusTypeDef  QFlash_SErase4k(uint32_t addr){
	QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;
	if (QFlash_WriteEnable())
		return HAL_ERROR;
	sCommand.Instruction	= W25_S_ERASE4K;
    sCommand.DataMode 		= QSPI_DATA_NONE;
	sCommand.AddressMode 	= QSPI_ADDRESS_1_LINE;
	sCommand.NbData 		= 0;
	sCommand.Address 		= addr;


	if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {		//Send command
		return HAL_ERROR;
	}
	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;
	return HAL_OK;
}




/**********************************
 * @BRIEF	Erase to 0XFF all bytes in a 32k block
 * 			32k block bounary is 0x08000, that means:
 * 			0x008000, 0x010000, 0x018000, ...
 * 			waiting the writing complete in each page
 * @PARAM	addr	starting erase address
 * 					(it must be a 32k block boundary)
 *********************************/
HAL_StatusTypeDef  QFlash_BErase32k(uint32_t addr){
	QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;
	if (QFlash_WriteEnable())
		return HAL_ERROR;
	sCommand.Instruction	= W25_B_ERASE32K;
    sCommand.DataMode 		= QSPI_DATA_NONE;
	sCommand.AddressMode 	= QSPI_ADDRESS_1_LINE;
	sCommand.NbData 		= 0;
	sCommand.Address 		= addr;


	if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {		//Send command
		return HAL_ERROR;
	}
	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;
	return HAL_OK;
}





/**********************************
 * @BRIEF	Erase to 0XFF all bytes in a 64k block
 * 			64k block bounary is 0x08000, that means:
 * 			0x010000, 0x020000, 0x030000, ...
 * 			waiting the writing complete in each page
 * @PARAM	addr	starting erase address
 * 					(it must be a 64k block boundary)
 *********************************/
HAL_StatusTypeDef QFlash_BErase64k(uint32_t addr){
	QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;
	if (QFlash_WriteEnable())
		return HAL_ERROR;
	sCommand.Instruction	= W25_B_ERASE64K;
    sCommand.DataMode 		= QSPI_DATA_NONE;
	sCommand.AddressMode 	= QSPI_ADDRESS_1_LINE;
	sCommand.NbData 		= 0;
	sCommand.Address 		= addr;


	if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {		//Send command
		return HAL_ERROR;
	}
	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;
	return HAL_OK;
}




/**********************************
 * @BRIEF	Full chip erase to 0XFF
 * 			Chip Erase may need up to 100s
 * 			(typ. 20s)
 * 			waiting the writing complete in each page
 *********************************/
HAL_StatusTypeDef QFlash_ChipErase(){
	QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;

	if (QFlash_WriteEnable())
		return HAL_ERROR;
	sCommand.Instruction	= W25_CH_ERASE;
    sCommand.DataMode 		= QSPI_DATA_NONE;
    sCommand.AddressMode	= QSPI_ADDRESS_NONE;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return HAL_ERROR;
    }

	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;
	return HAL_OK;
}





/**********************************
 * @BRIEF	Initiates a powerdown
 * 			after a powerDown only accepted a porweUp command
 * 			opwerDown operation is 3us long
 *********************************/
HAL_StatusTypeDef QFlash_PowerDown(){
	QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;
	sCommand.Instruction	= W25_POWERDOWN;
    sCommand.DataMode 		= QSPI_DATA_NONE;
    sCommand.AddressMode	= QSPI_ADDRESS_NONE;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return HAL_ERROR;
    }
    return HAL_OK;
}







/**********************************
 * @BRIEF	Release from powerdown (3 us to restart) or read device ID
 *********************************/
HAL_StatusTypeDef QFlash_PowerUp(){
	QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	if (QFlash_WaitForWritingComplete())
		return HAL_ERROR;
	sCommand.Instruction	= W25_POWERUP_ID;
    sCommand.DataMode 		= QSPI_DATA_NONE;
    sCommand.AddressMode	= QSPI_ADDRESS_NONE;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return HAL_ERROR;
    }
    return HAL_OK;
}




/**********************************
 * @BRIEF	read device id from chip
 * @RETURN	device id
 *********************************/
uint8_t QFlash_ReadDevID(){
uint8_t data;
QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction	= W25_POWERUP_ID;
    sCommand.DataMode 		= QSPI_DATA_1_LINE;
    sCommand.AddressMode	= QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize 	= QSPI_ADDRESS_24_BITS;  // I need to send 3 dummy bytes avter command
    sCommand.Address		= 0;
    sCommand.NbData			= 1; //after 3 dummy bytes I can read the Device ID
    sCommand.DummyCycles 	= 0;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return 0;
    }
    if (HAL_QSPI_Receive(&FLASH_QSPI_PORT, &data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) { // Receive data
    	return 0;
    }
	return data;
}





uint16_t QFlash_ReadManufactutrerAndDevID() {
uint16_t data=0;
QSPI_CommandTypeDef sCommand = {0};

	QFlash_DefaultCmd(&sCommand);
	sCommand.Instruction	= W25_MAN_DEVICE_ID;
    sCommand.DataMode 		= QSPI_DATA_1_LINE;
    sCommand.AddressMode	= QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize 	= QSPI_ADDRESS_24_BITS;  // I need to send 3 dummy bytes avter command
    sCommand.Address		= 0;
    sCommand.NbData			= 2; //after 3 dummy bytes I can read the Device ID
    sCommand.DummyCycles 	= 0;

    if (HAL_QSPI_Command(&FLASH_QSPI_PORT, &sCommand, QFLASH_DEF_TIMEOUT) != HAL_OK) {  //Send command
        return HAL_ERROR;
    }
    if (HAL_QSPI_Receive(&FLASH_QSPI_PORT,(uint8_t *) &data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) { // Receive data
    	return HAL_ERROR;
    }
	return data;
}


#endif //FLASH_QSPI_MEMORY_MAPPED



/**************************************
 * functions required for TouchGFX
 * integration
 **************************************/


void DataReader_WaitForReceiveDone(){
// nothing to do, being reading always in polling mode
	return;
}

void DataReader_ReadData(uint32_t address24, uint8_t* buffer, uint32_t length){
#ifdef FLASH_QSPI_MEMORY_MAPPED
	memcpy(buffer, (void *)0x90000000+address24, length);
#else
	QFlash_Read(address24, buffer, length);
#endif //FLASH_QSPI_MEMORY_MAPPED
}


void DataReader_StartDMAReadData(uint32_t address24, uint8_t* buffer, uint32_t length){
//currently using polling mode
#ifdef FLASH_QSPI_MEMORY_MAPPED
	memcpy(buffer, (void *)0x90000000+address24, length);
#else
	QFlash_Read(address24, buffer, length);
#endif //FLASH_QSPI_MEMORY_MAPPED
}





