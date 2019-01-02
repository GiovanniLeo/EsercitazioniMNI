# Valutazione SpeedUp prodotto scalare

**Studente:** Giovanni Leo

**Matricola:** 0522500538

I test sono stati effettuati sul cluster multi-GPU messo a disposizione dal Dipartimento di Matematica 
collegandosi mediante ssh al nodo 2 della macchina.

##### Limitazioni Compute Capability 2.0

- 1536 max thread per ogni SM
- 1024 thread per blocco
- 8 blocchi per ogni SM
- 32 SM
- 32768  max registri per ogni SM

## Valutazione

#### Strategia 1 - Global Memory

Sono stati utilizzati 256 thread per blocco quindi il numero di blocchi che vengono attivati sarà:
$$
\frac{1536}{256} = 6
$$
Stiamo utilizzando meno blocchi di quanti ne abbiamo a disposizione ma tale scelta è stata fatta per problemi di memoria.

| Nx1000 | Tempi CPU | Tempi GPU | SpeedUp |
| ------ | --------- | --------- | ------- |
| 128    | 0,4996    | 0,8970    | 0,55    |
| 192    | 0,7135    | 1,1065    | 0,64    |
| 256    | 0,9576    | 1,3928    | 0,68    |
| 448    | 1,6649    | 2,2319    | 0,74    |
| 512    | 1,9004    | 2,4114    | 0,78    |

Inoltre il programma `Esercitazione6Punto1.cu`utilizza 10 registri quindi il numero di registri utilizzati sarà:$1536*10 = 15360 < 32768.$

<div style="page-break-after: always;"></div>
#### Strategia 2 - Shared Memory Divergente

Sono stati utilizzati 256 thread per blocco quindi il numero di blocchi che vengono attivati sarà:
$$
\frac{1536}{256} = 6
$$
Stiamo utilizzando meno blocchi di quanti ne abbiamo a disposizione ma tale scelta è stata fatta per problemi di memoria.

| Nx1000 | Tempi CPU | Tempi GPU | SpeedUp |
| ------ | --------- | --------- | ------- |
| 128    | 0,4996    | 0,0845    | 5,91    |
| 192    | 0,7135    | 0,1310    | 5,44    |
| 256    | 0,9576    | 0,1576    | 6,07    |
| 448    | 1,6649    | 0,2659    | 6,26    |
| 512    | 1,9004    | 0,3008    | 6,31    |

Inoltre il programma `Esercitazione6Punto2.cu`utilizza 10 registri quindi il numero di registri utilizzati sarà:$1536*10 = 15360 < 32768$.

#### Strategia 3 - Shared Memory

Sono stati utilizzati 256 thread per blocco quindi il numero di blocchi che vengono attivati sarà:
$$
\frac{1536}{256} = 6
$$
Stiamo utilizzando meno blocchi di quanti ne abbiamo a disposizione ma tale scelta è stata fatta per problemi di memoria.

| Nx1000 | Tempi CPU | Tempi GPU | SpeedUp |
| ------ | --------- | --------- | ------- |
| 128    | 0,4996    | 0,0443    | 11,27   |
| 192    | 0,7135    | 0,0612    | 11,65   |
| 256    | 0,9576    | 0,0788    | 12,15   |
| 448    | 1,6649    | 0,1255    | 13,26   |
| 512    | 1,9004    | 0,1426    | 13,32   |

Inoltre il programma `Esercitazione6Punto3.cu`utilizza 10 registri quindi il numero di registri utilizzati sarà:$1536*10 = 15360 < 32768$.

<div style="page-break-after: always;"></div>
#### Grafico

![](C:\Users\johnn\Desktop\Unisa\Magistrale\MNI\Esercitazioni\Esercitazione6\Img\Valutazione SpeedUp.png)

