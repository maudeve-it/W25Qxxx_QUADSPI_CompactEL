[<img src="https://github.com/maudeve-it/W25Qxxx_QUADSPI_CompactEL/blob/main/COPERTINA%20QSPI.png" width="393" height="221">](https://youtu.be/_ds4ABvxQY8) [<img src="https://github.com/maudeve-it/W25Qxxx_QUADSPI_CompactEL/blob/main/COPERTINA%20QSPI2.png" width="393" height="221">](https://youtu.be/XVVtDWfh4Ks)



<br>
<br>

**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**<br>Below the English text you'll find the Italian version</i>**_
<br>
<br>
<br>
# A W25QXXX QuadSPI FLASH memory library for STM32: <br>
## including support functions for <i>12kB-RAM-needing</i> External Loaders<br>
## and for the flash memory integration on TouchGFX projects<br>
<br><br>
In this GitHub page, you'll find a library for STM32 (HAL) handling <b>Winbond Flash memories (W25Qxxx) via QuadSPI port</b>.<br>
Library includes functions you need to create External Loaders for STM32CubeProgrammer and STM32CubeIDE.<br>
External Loaders functions are basically as indicated by ST documentation but with a modified linker script allowing a reduced RAM needing (see https://youtu.be/zv0w_vhTTTo for details).<br>
<br>
Inside library you'll find also functions, that <b>TouchGFX</b> needs to handle an external flash memory, reading font and images via a direct transfer flash->display (without needing a cache in RAM).<br>
So, you can use this software for 4 distinct purposes:
<ul>
<li>handling an external W25Q flash directly reading and writing over it<br></li>
<li>create an External Loader to program flash memory via CubeProgrammer<br></li>
<li>add the created external loader to a CubeIde project in order to directly flash the external chip while creating projects (as same as the uC internal flash memory)<br></li>
<li>add an external flash to a TouchGFX project. TouchGFX provides tools making an external flash, handled via an external loader, absolutely transparent to the project.<br> </li>
</ul>
<br>

---

<br>

## GitHub Folders

This guide is divided in 4 chapters following the above list.<br><br>

- [handling an external W25Q flash with this library](./1_Flash_library)
- [creating and using an External Loader for the QuadSPI flash](./2_ExternalLoader)
- [using QuadSPI flash in memory mapped mode + External Loader](./3_CubeIDE_mapping)
- [using an external QuadSPI flash in TouchGFX projects](./4_TouchGFX_mapping)

<br>

---

<br>

Current version of library handles QuadSPI communication via:<br>
<ul>
<li><b>standard/dual/quad mode communication</b>: using the three communication mode available in QuadSPI<br></li>
<li><b>single/dual flash mode</b><br></li>
<li><b>polling mode</b>: no "interrupt mode" nor "DMA mode"<br></li>
</ul>
<br><br><br>

### Helping videos
The reference video for this library is: https://youtu.be/_ds4ABvxQY8 <br>
More information is available here:

|||
|---|---|
|Flash memories on SPI, "how to" develop an EL, "how to" install an EL into CubeProgrammer|https://youtu.be/KlG2doCkREM|
|"how to" create a low-RAM-demanding EL|https://youtu.be/zv0w_vhTTTo|
|"how to" use the SPI flash in TouchGFX projects, "how to" install an EL into CubeIDE|https://youtu.be/PO_2ZE1NQF0|
|"how to" map an external SPI flash memory in a CubeIDE project|https://youtu.be/K-7X8WKNu7c|
<br>
<br>

### Sources
The  sofware you find on this page, the page content and above videos, were developed upon this documentation:

|||
|---|---|
|Winbond W25Qxxx web page|https://www.winbond.com/hq/product/code-storage-flash-memory/serial-nor-flash|
|Jedec SFDP standard (requires registration)|https://www.jedec.org/document_search/field_doc_type/148?search_api_views_fulltext=JESD216|
|STM Mooc - External QSPI loader how to|https://www.st.com/content/st_com/en/support/learning/stm32-education/stm32-moocs/external_QSPI_loader.html|
|STM repository of external flash loaders|https://github.com/STMicroelectronics/stm32-external-loader|
|STM32CubeProgrammer documentation|https://www.st.com/resource/en/user_manual/um2237-stm32cubeprogrammer-software-description-stmicroelectronics.pdf|
|STM32 Graphics: external QSPI flash in TouchGFX|https://youtu.be/RMgVo_uCTbw|
|TouchGFX: Using Serial Flash for images and fonts|https://support.touchgfx.com/docs/development/scenarios/using-serial-flash|

<br>


## NOTES
> The software, schemes and PCB designs in this set of repositories are provided just for 
> illustrative purposes, sharing the personal experience on this project. 
> The software and design artifacts are provided as-is without any mantainence and without
> warranty of any kind. There is no guarantee of fitness for any particular application, 
> and no guarantee of completeness and correctness. 
> By downloading and using these materials the user accepts all risks associated with the
> run of the software and construction and use of the circuits and devices described.<br>

---

---

<br>
<br>
<br>

# Una libreria sulle memorie SPI Flash W25QXXXX per STM32: <br>
## incluse le funzioni di supporto per la creazione di External Loader con ridotto fabbisogno RAM<br>
## e per integrare la memoria flash in progetti TouchGFX<br>
<br><br>
In questa pagina GitHub trovi una libreria di funzioni per STM32 (HAL) per la gestione di <b>memorie Flash Winbond (W25Qxxxx) via QuadSPI</b>.<br>
La libreria include anche le funzioni richieste per generare <b>External Loader</b> per STM32CubeProgrammer o per STM32CubeIDE.<br>
Le funzioni per gli External Loaders sono sostanzialmente come indicato dalla documentazione ST ma con un linker script modificato che consente una ridotta esigenza di RAM (vedi https://youtu.be/zv0w_vhTTTo per dettagli).<br>
<br>
Nella libreria sono presenti anche le funzioni richeste per integrare una memoria Flash esterna in progetti <b>TouchGFX</b> per memorizzare font ed immagini con trasferimento diretto flash->display (non è richiesta cache in RAM).<br>
Puoi utilizzare quindi questo software per 4 scopi distinti:
<ul>
<li>gestire una memoria flash esterna W25Q, leggendo o scrivendo direttamente sopra<br></li>
<li>creare un External Loader per programmare la memoria flash via CubeProgrammer<br></li>
<li>aggiungere l'external loader creato ad un progetto CubeIde allo scopo di programmare direttamente il chip esterno durante la creazione del progetto (come fatto per la flash interna al uC)<br></li>
<li>aggiungere la memoria flash esterna ad un progetto TouchGFX. TouchGFX fornisce strumenti per rendere la memoria flash, gestita tramite un external loader, totalmente trasparente al progetto.<br> </li>
</ul>
<br>

---

<br>
## Folder GitHub

Questa guida è suddivisa in 4 capitoli seguendo lo schema sopra.<br><br>
- [gestire una memoria flash W25Q attraverso questa libreria](./1_Flash_library)
- [creare and usare an External Loader per le memorie flash via QuadSPI](./2_ExternalLoader)
- [usare una flash QuadSPI in memory mapped mode è External Loader](./3_CubeIDE_mapping)
- [usare una flash QuadSPI esterna in progetti TouchGFX](./4_TouchGFX_mapping)

<br>

---

<br>

La versione attuale della libreria gestisce la comunicazione QuadSPI via:<br>
<ul>
<li><b>standard/dual/quad mode </b>:usando i tre modi di comunicazione disponibili<br></li>
<li><b>single/dual flash mode</b><br></li>
<li><b>polling mode</b>: non viene usato "interrupt mode" nè "DMA mode"<br></li>
</ul>
<br><br><br>


### Supporto video
Il video di riferimento per questa libreria è:https://youtu.be/_ds4ABvxQY8 <br>
Altre informazioni sono disponibili qui: 

|||
|---|---|
| Memorie Flash via SPI, "how to" - come creare un EL, come installare un EL in Cubeprogrammer|https://youtu.be/KlG2doCkREM|
|"how to" - creare EL a basso consumo di RAM|https://youtu.be/zv0w_vhTTTo|
|"how to" - come usare la memoria FLash SPI in un progetto TouchGFX, come installare un EL in CubeIDE per programmare la memoria flash in un progetto TouchGFX|https://youtu.be/PO_2ZE1NQF0|
|"how to" - come mappare una memoria flash SPI in un progetto CubeIDE|https://youtu.be/K-7X8WKNu7c|

<br><br>

### Referimenti
Il software, questa pagina ed i video sopra sono stati sviluppati sulla base di questa documentazione:

|||
|---|---|
|Winbond W25Qxxx web page|https://www.winbond.com/hq/product/code-storage-flash-memory/serial-nor-flash|
|Jedec SFDP standard (requires registration)|https://www.jedec.org/document_search/field_doc_type/148?search_api_views_fulltext=JESD216|
|STM Mooc - External QSPI loader how to|https://www.st.com/content/st_com/en/support/learning/stm32-education/stm32-moocs/external_QSPI_loader.html|
|STM repository of external flash loaders|https://github.com/STMicroelectronics/stm32-external-loader|
|STM32CubeProgrammer documentation|https://www.st.com/resource/en/user_manual/um2237-stm32cubeprogrammer-software-description-stmicroelectronics.pdf|
|STM32 Graphics: external QSPI flash in TouchGFX|https://youtu.be/RMgVo_uCTbw|
|TouchGFX: Using Serial Flash for images and fonts|https://support.touchgfx.com/docs/development/scenarios/using-serial-flash|


<br>


---
> Il software e gli schemi di progetto come i layout PCB in questa serie di repository 
> sono messe a disposizione con puro intento illustrativo e di condivisione dell'esperienza fatta
> Il software and gli elementi del progetto sono messi a disposizione "allo stato in cui sono"
> senza impegno di manutenzione e senza garanzie di alcun tipo. Piu' esplicitamente, non c'e' garanzia di 
> adeguatezza a soddisfare specifiche esigenze, ne di completezza o correttezza di alcuna parte.
> Scaricando ed utilizzando questo materiale l'utilizzatore accetta il rischio associato all'esecuzione del programma e alla
> realizzazione ed all'utilizzo del circuito e dei componenti descritti in questo archivio.

---


