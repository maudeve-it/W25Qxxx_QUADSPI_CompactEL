**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**<br>Below the English text you'll find the Italian version</i>**_
<br>
<br>
<br>

# 1) Using the W25QXXXX Winbond Flash QuadSPI library
Here you'll find documentation about handling a flash memory chip connected to a uC QuadSPI port
<ul>
<li><b>(1)	in CubeMX</b>:<br>
creating your STM32 project, enable QuadSPI.<br>
QuadSPI must be set this way:<br><br>

|Mode|
|---|
|Enable QuadSPI mode and banks needed|

Library handles single/dual/quad communication mode and single/dual bank

|Parameter setting|value|
|---|---|
|Device type|Flash|
|Clock Mode|Low|

All the other parameter as per project requirement (see video https://youtu.be/_ds4ABvxQY8 for details)

</li>
<br>
<li>
<b>(2)	in CubeIDE</b>:<br>
after saving CubeMX and after C code generation:<br>
<ul>
copy into the "include" folder (core\inc) <i>z_qflash_W25QXXX.h</i> file<br>
copy into the "source" folder (core\src) <i>z_qflash_W25QXXX.c</i> file <br>
</ul>
</li>
<br>
<li>
<b>(3)	in <i>main.h</i></b>:<br>
open file <i>main.h</i>
and edit <i>/* USER CODE BEGIN Includes */</i> segment, this way:
<br>

```sh
(main.h)
...
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "z_qflash_W25QXXX.h"
/* USER CODE END Includes */
...
```
<br>
</li>
<li>
<b>(4)	in <i>z_flash_W25QXXX.h</i></b>:<br>
edit file <i>z_flash_W25QXXX.h</i> this way:<br>
<ul>
<li>
	into segment <b>STEP1</b>:<br>
		register QuadSPI used<br>
<li>
	into segment <b>STEP2</b>:<br>
        Specify if using QuadSPI in either Memory Mapped Mode or no<br>
<li>
    into segment <b>STEP3</b>:<br>
        no changes: currently available only "Polling Mode"<br>
<li>
    into segment <b>STEP4</b>:<br>
        update all parameters with information about memory chip to handle.<br>
        Currently file shows, inside comments, configuration parameters for:<br>
<ul>
            W25Q80DV/DL<br>
            W25Q64JV<br>
            W25Q128JV<br>
</ul>
        if you are using a chip of these, you just need to copy the related commented parameters to the uncommented area <b>STEP4</b><br>
		if you are using a different memory chip, just use this data as example of what is needed for a correct library setup.<br>
<br>
</ul>
</li>
</ul>
FLash library needs to be initialized: before entering in main loop you must call Flash_Init() function checking flash presence and initializing it:<br>

```sh
(main.c)
...
  /* USER CODE BEGIN 2 */
 	if (QFlash_Init()!=HAL_OK) {};   // if Flash_Init() returns 0 (any flash error) do not proceed
  /* USER CODE END 2 */
...
```

<br>
Having done what above shown, you can use all functions of the library handling flash.<br>
<i>Each function has its own description and help, inside <b>z_flash_W25QXXX.c</b> file</i><br>
<br>
<br><i><b>
<br>

[Back to the home page](../.)

</b></i><br>


<br>
<br>

---
<br>
<br>
<br>




# Usare la libreria QuadSPI per le Flash W25Q di Winbond 
Qui trovi le informazioni su come gestire una memoria flash connessa alla porta QuadSPI del uC
<ul>
<li><b>(1)	in CubeMX</b>:<br>
durante la creazione del progetto, attivare pa porta QuadSPI.<br>
la porta QuadSPI deve essere configurata così:<br><br>

|Mode|
|---|
|attiva il modo di comunicazione e i banchi richiesti|

La libreria gestisce la comunicazione in single/dual/quad mode e in single/dual bank

|Parameter setting|value|
|---|---|
|Device type|Flash|
|Clock Mode|Low|

Gli altri parametri secondo le esigenze del progetto (vedi https://youtu.be/_ds4ABvxQY8 per dettagli)

</li>
<br>
<li>
<b>(2)	in CubeIDE</b>:<br>
dopo aver salvato la configurazione CubeMX e generato il codice C:<br>
<ul>
copiare nella cartella "include" (core\inc) il file <i>z_qflash_W25QXXX.h</i><br>
copiare nella cartella "source" (core\src) il file <i>z_qflash_W25QXXX.c</i><br>
</ul>
</li>
<br>
<li>
<b>(3)	in <i>main.h</i></b>:<br>
aprire il file <i>main.h</i>
e modifica il segmento <i>/* USER CODE BEGIN Includes */</i> in questo modo:
<br>

```sh
(main.h)
...
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "z_qflash_W25QXXX.h"
/* USER CODE END Includes */
...
```
<br>
</li>
<li>

<b>(4)	in <i>z_flash_W25QXXX.h</i></b>:<br>
aprire il file <i>z_flash_W25QXXX.h</i> e modificare in questo modo:<br>
<ul>
<li>
	nel segmento <b>STEP1</b>:<br>
		indicare la porta SPI utilizzata<br>
<li>
	nel segmento <b>STEP2</b>:<br>
        specificare se si intende usare QuadSPI in Memory Mappd Mode o meno.<br>
<li>
    nel segmento <b>STEP3</b>:<br>
        non modificare: attualmente disponibile solo la modalità "Polling Mode"<br>
<li>
    nel segmento <b>STEP4</b>:<br>
        aggiornare tutti i parametri con le informazioni del chip di memoria utilizzato.<br>
        Attualmente il file contiene nei commenti le configurazioni relative a:<br>
<ul>
            W25Q80DV/DL<br>
            W25Q64JV<br>
            W25Q128JV<br>
</ul>
        se si utilizza uno di questi chip è sufficiente copiare i dati delle righe di configurazione commentate, nell'area di informazione <b>STEP4</b><br>
		se si usano altri chip, usare questi dati come esempio per la configurazione corretta.<br>
<br>
</ul>
</li>
</ul>
La libreria deve essere inizializzata: prima di entrare nel main loop devi chiamare la funzione Flash_Init() che verifica la presenza della memoria e la inizializza:<br>

```sh
(main.c)
...
  /* USER CODE BEGIN 2 */
 	if (QFlash_Init()!=HAL_OK) {};  // if Flash_Init() returns 0 (any flash error) do not proceed
  /* USER CODE END 2 */
...
```

<br>
Fatto quanto sopra, e' possibile utilizzare le varie funzioni della libreria per gestire la memoria Flash<br>
<i>Le funzioni sono autodescritte e commentate nel file <b>z_flash_W25QXXX.c</b></i><br>

<br>
<br><i><b>
<br>

[Torna alla home page](../.)

</b></i><br>

<br>
