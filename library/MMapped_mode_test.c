// in main.h
  
  #include <string.h>


// in main.c

/* USER CODE BEGIN PV */
const char string1[30] = "GC RIGA1 di TEST";
//const char string2[30] __attribute__((section("ExtFlashSection"))) __attribute__((aligned(4))) = "GC RIGA2 di TEST";
const char string2[30] ON_EXT_FLASH = "GC RIGA2 di TEST";
/* USER CODE END PV */


 /* USER CODE BEGIN WHILE */
 if (QFlash_Init()!=HAL_OK) {
	  while (1) {}
  }
  static const char string3[30] ON_EXT_FLASH = "RIGA3 di TEST";

  char lvstring[30];

  strcpy(lvstring, string1);
  strcpy(lvstring, string2);
  strcpy(lvstring, string3);


  while (1)
  {

    /* USER CODE END WHILE */
...
