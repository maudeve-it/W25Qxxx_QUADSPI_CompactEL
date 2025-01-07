_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**<br>Below the English text you'll find the Italian version</i>**_
<br>
<br>
<br>

# 2) Creating a <i>low-RAM-demanding</i> EXTERNAL LOADER for STM32CubeProgrammer and STM32CubeIDE
External Loader is a plug-in for STM32CubeProgrammer/CubeIDE allowing to read/write an external memory through an STM32 uC.<br>
Following the below instruction you can create an External Loader for a W25Q connected to a QuadSPI, adding it then to CubeProgrammer and CubeIDE.<br>

## "How to" create an External Loader for a specific project:
<ul>
After defining the CubeMX configuration details of your project with a flash memory via QuadSPI (e.g., having already set the QuadSPI mode and other details for managing the flash memory)<br><br>
<li><b>Create a new project in CubeIDE</b>.<br>
Give the project a name with this format:<br> 
"external flash used"_"uC or board name"_"oprional: other info, if any, - e.g. QSPI port, mode, bank, etc."<br>
(having character "_" dividing the three fields, must non be used elsewhere)<br>
Here some examples for a valid project name:
<ul>
W25Q80_STM32F411_QSPI-BANK1<br>
W25Q80_WEACT-H750 (third field is optional and here it is not used)<br>
etc.<br>
</ul>
<li><b>in CubeMX</b>:
<ul>
<li> setup the <b>QuadSPI port</b> for your flash memory, as per youe project.<br>
<i><b>Warning:</b><br>
I always recommend to setup the External Loader with a low QuadSPI port speed (e.g. lower than 2Mbit/s)</i><br>
<li> enable <b>CRC</b> (you just need to set "Activated" checkbox)<br>
<li> If on your board, you have available a led connected to a uC pin (GPIO_Output): you can use it in the External Loader. Useful for troubleshooting.<br>
give the uC led pin the name: <b>LED</b>.<br>
<li> in <i>"Project Manager"->"Code Generator"</i>, mark <b><i>"Generate peripheral initialization as a pair of '.c/.h' files per peripheral"</i></b> to have peripheral initializing files separated by <i>main.c</i>
<li> generate "c" code.
</ul>
<li><b>in CubeIDE</b>:
<ul>
<li>
in <i><b>include</b></i> folder, copy files:
<ul>
z_qflash_W25QXXX.h<br>
Dev_Inf.h<br>
</ul>
<li>
in <i><b>src</b></i> folder, copy files:
<ul>
z_qflash_W25QXXX.c<br>
Dev_Inf.c<br>
Loader_Src.c<br>
</ul>
<li>
in <i><b>root</b></i> folder, copy files:
<ul>
EL_linker.ld<br>
Post-Build command.txt<br>
</ul>
<li>
Setup <i>"<b>z_qflash_W25QXXX.h</b>"</i>
<ul>
registering flash chip characteristics (see above the setup of <i>"z_flash_W25QXXX.h"</i> Step 1 to 4)
</ul>
<li>
Setup <i><b>Loader_Src.c</b></i>:
<ul>
follow inside file: Step1, Step2, Step3
</ul>
<li>
Setup <i><b>Dev_Inf.c</b></i>: 
<ul>
update first struct field assigning a name to the External Loader: copy here the project name<br>
update third struct field indicating address where external flash is mapped<br>
leave unchanged other fields.
</ul>
<li>
open file <i>"<b>Post-Build command.txt</b>"</i>
<ul>
copy the text row and paste it into the field in<br>
<i>project->properties->C/C++ Build->settings->Build Steps->Post-Build Steps->Command</i><br>
click "Apply and Close"
</ul>
<li>
edit <i><b>EL_linker.ld</b></i> 
<ul>
<li>
set the memory origin and size as per uC available RAM:<br>
around line 30 you find text:<br>
	
```sh
(EL_linker.ld)
...
RAM_LOADER (xrw)      : ORIGIN = 0x20000004, LENGTH = 8M-4
...
```
<br>
change value "0x20000004" to "0x24000004" if using an H7 uC<br>
change value "8M" after "LENGTH =" indicating the uC RAM size (refer to the current active .ld file for correct information about the RAM configuration of your uC)<br>
<li>
save changes
<li>
in <i><b>project->properties->C/C++ Build->settings->Tool Setting->MCU GCC Linker->General->linker script</b></i>:
<ul>
1	change the name of the .ld file into "EL_linker.ld". Parameter should become: ${workspace_loc:/${ProjName}/EL_linker.ld}<br>
2	click "Apply and Close"<br>
</ul>
</ul>
</ul>
</ul>
<br>


> <em><b>PLEASE NOTE:</b><br> This is a "compact" version of EL: you don't need to unmark flag <i><b>Discard Unused Sections</b></i> in Project->Properties as indicated in my youtube video "[Part One](https://youtu.be/KlG2doCkREM)".<br>
> Characteristics and needs of this library are described in "[Part Two](https://youtu.be/zv0w_vhTTTo)".<br></em>

<br>

Now "Compile" project: If everithing goes fine <b>you'll find the file <i>project_name</i>.stldr into the project root folder</b><br><br> 
THIS IS THE EXTERNAL LOADER<br><br>

## "How to" add External Loader to STM32CubeProgrammer
<ul>
<li>
Go to the STM32CubeProgrammer program folder<br> 
(right-click the program icon and choose "open file location")
<li>
once in the STM32CubeProgrammer program folder go into "ExternalLoader" subfolder
<li>
copy here the External Loader created (.stldr file)<br><br>
Opening STM32CubeProgrammer program you can select the new External Loader to access the external flash memory.
</ul>
<br>

## "How to" add External Loader to STM32CubeIDE
<ul>
<li>
Go to the STM32Cubeide program folder<br> 
(right-click the program icon and choose "open file location")
<li>
once in the STM32CubeProgrammer program folder go subfolder:
<i>./plugins/xxxxxx.externaltools.cubeprogrammer.xxxxx/tools/bin/ExternalLoader</i><br>

<li>
copy here the External Loader created (.stldr file)<br><br>
Now the External Loader is available to CubeIDE and can be selected in "Run/Debug Settings" in the project "Properties"<br>
</ul>
<br>
<br>
<br>
<br><i><b>
<br>

[Back to the home page](../.)

</b></i><br>

<br>

---

---

<br>
<br>

# creare un EXTERNAL LOADER <i>a basso utilizzo di RAM</i> per STM32CubeProgrammer e STM32CubeIDE
External Loader e' un plug-in per STM32CubeProgrammer per leggere e/o programmare un chip di memoria gestito attraverso da un uC STM32.<br>
Attraverso la libreria di funzioni indicata sopra e' possibile creare un external loader per un progetto STM32 che ha una memoria QuadSPI Flash Winbond esterna .<br>
Lo stesso plugin puo' essere utilizzato in STM32CubeIDE per programmare la memoria Flash direttamente durante la creazione di un progetto: CubeIDE usa i moduli CubeProgrammer per programmare la memoria flash interna od esterna del uC.

## "How to" come creare an External Loader relativo ad un progetto:
<ul>
Dopo aver definito i dettagli di configurazione CubeMX del tuo progetto con una memoria flash via QuadSPI (ad esempio avendo già definito  il QuadSPI mode, e gli altri dettagli necessari per la gestione della memoria Flash<br><br>
<li><b>Crea un nuovo progetto in CubeIDE</b>.<br>
Assegna al progetto un nome con questa forma:<br> 
"external flash in uso"_"nome uC o board in uso"_"eventuali altre informazioni - es. Porta QuadSPI, mode, bank, ecc."<br>
(il carattere "_" separa i tre campi e non deve essere utilizzato nel nome se non per questa separazione)<br>
Esempi di nomi di progetto protrebbero essere:
<ul>
W25Q80_STM32H750_QSPI-BANK1<br>
W25Q80_WEACT-H750 (il terzo campo è opzionale e qui non c'e')<br>
ecc.<br>
</ul>
<li><b>in CubeMX</b>:
<ul>
<li> configura una porta <b>QuadSPI</b> per la memoria flash, seguendo il tuo progetto<br>
<i><b>Attenzione:</b><br>
E' sempre consigliabile configurare una velocià ridotta in un External Loader (es. inferiore a 2Mbit/s)</i><br>
<li> abilita <b>CRC</b> (devi solo spuntare "Activated")<br>
<li> Se hai un led a disposizione sulla scheda, puoi usarlo nell'External Loader (utile nel troubleshooting)<br>
dai al pin del led il nome: <b>LED</b> 
<li> in <i>"Project Manager"->"Code Generator"</i>, spunta <b><i>"Generate peripheral initialization as a pair of '.c/.h' files per peripheral"</i></b> per avere i file di inizializzazione separati da main.c
<li> genera il codice.
</ul>
<li><b>in CubeIDE</b>:
<ul>
<li>
copiare in <i><b>include</b></i> folder
<ul>
z_qflash_W25QXXX.h<br>
Dev_Inf.h<br>
</ul>
<li>
copiare in <i><b>src</b></i> folder
<ul>
z_qflash_W25QXXX.c<br>
Dev_Inf.c<br>
Loader_Src.c<br>
</ul>
<li>
copiare in <i><b>root</b></i> folder
<ul>
EL_linker.ld<br>
Post-Build command.txt<br>
</ul>
<li>
definire i parametri in <i>"<b>z_qflash_W25QXXX.h</b>"</i>
<ul>
definire le caratteristiche della memoria (vedi piu' sopra la configurazione di <i>"z_qflash_W25QXXX.h"</i> da Step 1 a 4)
</ul>
<li>
configurare <i><b>Loader_Src.c</b></i>:
<ul>
seguire gli step di configurazione: Step1, Step2, Step3
</ul>
<li>
configurare <i><b>Dev_Inf.c</b></i>: 
<ul>
aggiorna il primo campo della struct assegnando il nome all'External Loader: deve essere "rif alla flash" _ "rif alla board" _ "altre info (es versione)" (copiare il nome del progetto)<br>
aggiorna il terzo campo della struct indicando l'indirizzo su cui e' mappata la flash<br>
non modificare gli altri campi
</ul>
<li>
aprire il file <i>"<b>Post-Build command.txt</b>"</i>
<ul>
copiare il contenuto e incollarlo nel campo in<br>
<i>project->properties->C/C++ Build->settings->Build Steps->Post-Build Steps->Command</i><br>
scegliere "Apply and Close"
</ul>
<li>
modificare <i><b>EL_linker.ld</b></i> 
<ul>
<li>
modificare origine e dimensione della memoria indicando la RAM disponibile sul uC:<br>
attorno alla riga 30 trovi il testo:<br>
	
```sh
(EL_linker.ld)
...
RAM_LOADER (xrw)      : ORIGIN = 0x20000004, LENGTH = 8M-4
...
```
<br>
cambia il valore "0x20000004" in "0x24000004" se il uC in uso è un H7<br>
modifica il valore "8M" dopo "LENGTH =" specificando la dimensione della RAM (fai riferimento al file .ld attualmente attivo per la corretta informazione sulla configurazione RAM del uC in uso)<br>
<li>
salvare il tutto
<li>
in <i><b>project->properties->C/C++ Build->settings->Tool Setting->MCU GCC Linker->General->linker script</b></i>:
<ul>
1	cambiare il nome del file .ld in "EL_linker.ld": dovrebbe essere: ${workspace_loc:/${ProjName}/EL_linker.ld}<br>
2	cliccare "Apply and Close"<br>
</ul>
</ul>
</ul>
</ul>
<br>


> <em><b>NOTA BENE:</b><br> Non hai bisogno di deselezionare il flag <i><b>Discard Unused Sections</b></i> in Project->Properties, come indicato nel mio video "[Prima Parte](https://youtu.be/KlG2doCkREM)".<br>
> Le caratteristiche e le necessità di questa libreria sono descritte in "[Seconda Parte](https://youtu.be/zv0w_vhTTTo)"</em>.<br>

<br>

"Compila" il progetto: se tutto e' andato bene <b>trovi il file "nomeprogetto".stldr nella cartella di root</b><br><br> 
Questo e' l'external loader<br><br>

## "How to": come aggiungere un  External Loader a STM32CubeProgrammer
<ul>
<li>
Andare nel folder del programma STM32CubeProgrammer<br> 
(ad esempio click-destro del mouse sull'icona del programma e scegliere "Apri percorso file")
<li>
raggiunta la cartella di STM32CubeProgrammer, scegliere la cartella "ExternalLoader"
<li>
copiare qui il file .stldr creato<br><br>
Aprendo STM32CubeProgrammer si puo' selezionare il nuovo External Loader per accedere alla memoria Flash esterna
</ul>


## "How to" add External Loader to STM32CubeIDE
<ul>
<li>
Andare nel folder del programma STM32CubeIDE<br> 
(ad esempio click-destro del mouse sull'icona del programma e scegliere "Apri percorso file")
<li>
Raggiunta la cartella di STM32CubeIDE, andare nella sottocartella:
<i>./plugins/xxxxxx.externaltools.cubeprogrammer.xxxxx/tools/bin/ExternalLoader</i><br>
<li>
Copiare qui il file .stldr creato<br><br>
Ora l'External Loader è disponibile in CubeIDE e puo' essere selezionato in "Run/Debug Settings" in Project->Properties<br>
</ul>

<br>

[Torna alla home page](../.)

</b></i><br>

<br>

