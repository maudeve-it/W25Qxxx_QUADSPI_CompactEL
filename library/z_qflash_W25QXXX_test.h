/*
 * z_qflash_W25QXXX_test.h
 *
 *  Created on: Oct 10, 2024
 *      Author: mauro
 *
 *
 *
 * put below row in "USER CODE BEGIN PD" session of main.c
 *
  uint8_t flashOK =1; //test results
 *
 *
 * put below rows in "USER CODE BEGIN 2" session of main.c
 * compile and run the project
 *
  HAL_TIM_Base_Start(&htim2);
  TriggerLogicAnalyzer();

  //initialization triggering Logic Analyzer (and led)
  HAL_Delay(DELAY_TIME*2);
  HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
  if (QFlash_Init()!=HAL_OK) {
	  flashOK =0;
	  while (1) {}
  }
  HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_RESET);
  HAL_Delay(DELAY_TIME);
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);


  // testing Chip Erase
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_SET);
  // if (FlashOK)
  //     QFlash_ChipErase();
  HAL_GPIO_WritePin(TRIGGER_GPIO_Port,TRIGGER_Pin,GPIO_PIN_RESET);
  HAL_Delay(DELAY_TIME);
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

#ifndef  FLASH_QSPI_MEMORY_MAPPED

  if (flashOK)
	  flashOK=TimeTest();

#endif

 *
 *
 * Now put the below row into the main loop
 *
 *
 *
   BlinkIt(flashOK);  // a series of triple-blinking means test was good
 
 */

#ifndef INC_Z_QFLASH_W25QXXX_TEST_H_
#define INC_Z_QFLASH_W25QXXX_TEST_H_

#define DELAY_TIME 50


void BlinkIt(uint8_t flashOK);
void TriggerLogicAnalyzer();
void SimpleReadingTest();
HAL_StatusTypeDef TestStatusRegisters(uint8_t level);
uint8_t TimeTest();


#endif /* INC_Z_QFLASH_W25QXXX_TEST_H_ */
