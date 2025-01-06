/*
 * z_qflash_W25QXXX_test.c
 *
 *  Created on: Oct 10, 2024
 *      Author: mauro
 */
#include "main.h"


void BlinkIt(HAL_StatusTypeDef flashOK) {
	  // blinking indicating end of test: triple-blink if test==ok
	  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
	  HAL_Delay(100);
	  if (flashOK) {
		  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);
		  HAL_Delay(200);
		  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
		  HAL_Delay(100);
		  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);
		  HAL_Delay(200);
		  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
		  HAL_Delay(100);
	  }
	  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);
	  HAL_Delay(500);

}



void TriggerLogicAnalyzer(){
	  for (uint8_t k=0;k<3;k++) {
		  HAL_Delay(DELAY_TIME);
		  HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_SET);
		  HAL_Delay(DELAY_TIME);
		  HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_RESET);
	  }
}


#ifndef FLASH_QSPI_MEMORY_MAPPED


void SimpleReadingTest(){
	/*
	 * lettura di una stringa precedentemente registrata in dual flash mode
	 *
	*/
	  uint8_t mess[20];

	  strcpy((char*)mess,"           ");
	  QFlash_Read(0,mess,12);										//just read, no test

	  strcpy((char*)mess,"           ");
	  QFlash_Read(0X7ffa,mess,12);										//just read, no test

	  strcpy((char*)mess,"           ");
	  QFlash_Read(0Xfffa,mess,12);										//just read, no test

	  strcpy((char*)mess,"           ");
	  QFlash_Read(0X1fffa,mess,12);										//just read, no test

	  while (1) {};

}


HAL_StatusTypeDef TestSR1Setting (){
uint8_t sr1=4;
HAL_StatusTypeDef result=HAL_OK;

//test reading/ewriting SR1

	sr1=QFlash_ReadSR1();
	if (!(sr1==0))
		result=HAL_ERROR;
	QFlash_WriteEnable();
	HAL_Delay(1);
	sr1=QFlash_ReadSR1();
	if (!(sr1==2))
		result=HAL_ERROR;
	QFlash_WriteDisable();
	HAL_Delay(1);
	sr1=QFlash_ReadSR1();
	if (!(sr1==0))
		result=HAL_ERROR;

	return result;
}

void TestSR3Setting (){
uint8_t sr3=0;

sr3=QFlash_ReadSR3();
HAL_Delay(1);

sr3=0xFF;
sr3=QFlash_ReadSR3();
HAL_Delay(1);

QFlash_WriteEnable();
HAL_Delay(1);

sr3&=0x9F;
sr3=QFlash_WriteSR3(sr3);
HAL_Delay(1);

sr3=0;
sr3=QFlash_ReadSR3();
HAL_Delay(1);

sr3=0xFF;
sr3=QFlash_ReadSR3();
HAL_Delay(1);

}



void TestQESetting (){
uint8_t sr2;

	HAL_Delay(1);
	sr2=0xFF;
	sr2=QFlash_ReadSR2();
	HAL_Delay(1);
	QFlash_WriteEnable();
	HAL_Delay(1);
	sr2&=0xFD;
	sr2=QFlash_WriteSR2(sr2);
	HAL_Delay(1);
	sr2=0xFF;
	sr2=QFlash_ReadSR2();
	HAL_Delay(1);
	QFlash_WriteEnable();
	HAL_Delay(1);
	sr2|=0x02;
	sr2=QFlash_WriteSR2(sr2);
	HAL_Delay(1);
	sr2=0xFF;
	sr2=QFlash_ReadSR2();
	HAL_Delay(1);

	HAL_Delay(1);
	QFlash_VolatileWriteEnable();
	HAL_Delay(1);
	sr2&=0xFD;
	sr2=QFlash_WriteSR2(sr2);
	HAL_Delay(1);
	sr2=0xFF;
	sr2=QFlash_ReadSR2();
	HAL_Delay(1);
	QFlash_VolatileWriteEnable();
	HAL_Delay(1);
	sr2|=0x02;
	sr2=QFlash_WriteSR2(sr2);
	HAL_Delay(1);
	sr2=0xFF;
	sr2=QFlash_ReadSR2();
	HAL_Delay(1);

}



/****************************************
 * level = 0	verifies correct
 * 				velues in SR1, SR2,SR3
 * level <> 0	makes some tests
 ****************************************/
HAL_StatusTypeDef TestStatusRegisters(uint8_t level) {
	HAL_StatusTypeDef result = HAL_OK;
	uint8_t sr;

	sr=QFlash_ReadSR1();

	if (!((sr && 1)==0)) {  //why flash is busy
		result=HAL_ERROR;
	} else if (!((sr && 2)==0)) {  //if write_enabled, disable it
		QFlash_WriteDisable();
		HAL_Delay(1);
		sr=QFlash_ReadSR1();
		if (!((sr && 2)==0)){ //why flash is  still writeable?
			result=HAL_ERROR;
		}
	}

	if (level!=0) {
		QFlash_WriteEnable();
		HAL_Delay(1);
		sr=QFlash_ReadSR1();
		if (!(sr==2))
			result=HAL_ERROR;

		HAL_Delay(10);
		if (TestSR1Setting())
			result=HAL_ERROR;

		TestSR3Setting();

		HAL_Delay(10);
		TestQESetting();

		BlinkIt(!result);  //parameter is 1 if flash is OK
	}

	return result;
}




uint8_t TimeTest(){
	//#define DSIZE 65536
#define DSTEP_SIZE	2 	//data dimension increased on every step (2 = multiplied by 4)
#define DSTEP_NUMB	9	//number of step tests

uint8_t		flashOK=1;

uint32_t 	ttime;
uint32_t	ttimes[40]={0};
uint8_t 	pos_t=0;
uint8_t		bufferout[1<<(DSTEP_SIZE*(DSTEP_NUMB-1))]={0};
uint8_t		bufferin[(1<<DSTEP_SIZE*(DSTEP_NUMB-1))]={0};



	// start logick analyzer registering
	TriggerLogicAnalyzer();


	//flash initialization, Logic Analyzer (and led) announce
	HAL_Delay(DELAY_TIME*2);
	HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
	if (QFlash_Init()!=HAL_OK) {
		while (1) {}
	}
	//flash initialization and,  Logic Analyzer (and led) announce

	HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_RESET);
	HAL_Delay(DELAY_TIME);
	HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

	// full chip erase
/*
	HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_SET);
  	QFlash_ChipErase();
	HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_RESET);
	HAL_Delay(DELAY_TIME);
	HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);
*/

	TIM2->EGR=1;  //reset TIM7->CNT
	HAL_Delay(DELAY_TIME); //waiting timer reset

	// perform tests and register times

	//pos_t 0
	ttime=TIM2->CNT;
	HAL_Delay(1);
	ttime=TIM2->CNT-ttime;
	ttimes[pos_t++]=ttime;

	//pos_t 1
	HAL_Delay(DELAY_TIME);
	ttime=TIM2->CNT;
	HAL_Delay(1);
	ttime=TIM2->CNT-ttime;
	ttimes[pos_t++]=ttime;

	//pos_t 2
	HAL_Delay(DELAY_TIME);
	ttime=TIM2->CNT;
	HAL_Delay(2);
	ttime=TIM2->CNT-ttime;
	ttimes[pos_t++]=ttime;

	//test measuring Block Erase
	//pos_t 3
	HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_SET);
	ttime=TIM2->CNT;
	QFlash_BErase64k(0x00000);
	ttime=TIM2->CNT-ttime;
	ttimes[pos_t++]=ttime;
	HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_RESET);
	HAL_Delay(DELAY_TIME);


	for (uint32_t k=0; k<(1<<(DSTEP_SIZE*(DSTEP_NUMB-1))); k++)
		bufferout[k]=k;


	// loop testing various sizes of data
	uint32_t	dsize=1;
	for (uint8_t i=0;i<DSTEP_NUMB;i++){
		uint32_t addr = i*(0x10000); //base address writing test data: any loop uses a different 64k block)
		QFlash_BErase64k(addr);
		//test measuring writing
		HAL_Delay(DELAY_TIME);
		ttimes[pos_t++]=dsize;
		HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_SET);
		ttime=TIM2->CNT;
		QFlash_Write(addr, bufferout, dsize);
		ttime=TIM2->CNT-ttime;
		ttimes[pos_t++]=ttime;
		HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_RESET);

		//test measuring reading
		HAL_Delay(DELAY_TIME);
		HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_SET);
		ttime=TIM2->CNT;
		QFlash_Read(addr, bufferin, dsize);
		ttime=TIM2->CNT-ttime;
		ttimes[pos_t++]=ttime;
		HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_RESET);

		// checking correct reading
		for (uint32_t k=0; ((k<dsize) && flashOK); k++){
			if (bufferout[k]!=bufferin[k])
				flashOK=0;
		}

		HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
		HAL_Delay(DELAY_TIME);
		HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);
		dsize=(dsize<<DSTEP_SIZE);
	}

	if (!flashOK){
		HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
		__NOP();
	} else
		__NOP();

	HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

	return flashOK;
}

#endif //FLASH_QSPI_MEMORY_MAPPED

