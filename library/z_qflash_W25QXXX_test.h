/*
 * z_qflash_W25QXXX_test.h
 *
 *  Created on: Oct 10, 2024
 *      Author: mauro
 *
 * A series of functions testing the various features of W25Q flash memory
 * in a QuadSPI communication
 *
 * TimeTest() preforms the Indirect Mode time detection analyzed on the 2 fideos about QSPI library
 *
 * MMTest() preforms the MemoryMapped Mode time detection analyzed on the 2 fideos about QSPI library
 *
 * PLEASE NOTE:
 * before running the MMTest() function, set library in Indirect Mode and run one time the pre-MMTest() function
 * than you can run MMTest() any times you like
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
uint8_t pre_MMTest();
uint8_t TimeTest();
uint8_t MMTest();

#endif /* INC_Z_QFLASH_W25QXXX_TEST_H_ */
