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

| Nx1000 | Tempi CPU | Tempi GPU | SpeedUp     |
| ------ | --------- | --------- | ----------- |
| 128    | 0,4996    | 0,3448    | 1,44 |
| 192    | 0,7135    | 0,5186    | 1,37        |
| 256    | 0,9576    | 0,8412    | 1,13        |
| 448    | 1,6649    | 1,3006    | 1,28        |
| 512    | 1,9004    | 1,46      | 1,30        |

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
| 128    | 0,4996    | 0,0989    | 5,05    |
| 192    | 0,7135    | 0,1258    | 5,67    |
| 256    | 0,9576    | 0,1617    | 5,92    |
| 448    | 1,6649    | 0,2832    | 5,87    |
| 512    | 1,9004    | 0,3199    | 5,94    |

Inoltre il programma `Esercitazione6Punto2.cu`utilizza 10 registri quindi il numero di registri utilizzati sarà:$1536*10 = 15360 < 32768$.

#### Strategia 3 - Shared Memory

Sono stati utilizzati 256 thread per blocco quindi il numero di blocchi che vengono attivati sarà:
$$
\frac{1536}{256} = 6
$$
Stiamo utilizzando meno blocchi di quanti ne abbiamo a disposizione ma tale scelta è stata fatta per problemi di memoria.

| Nx1000 | Tempi CPU | Tempi GPU | SpeedUp |
| ------ | --------- | --------- | ------- |
| 128    | 0,4996    | 0,0480    | 10,40   |
| 192    | 0,7135    | 0,0660    | 10,81   |
| 256    | 0,9576    | 0,0826    | 11,59   |
| 448    | 1,6649    | 0,1320    | 12,61   |
| 512    | 1,9004    | 0,1594    | 11,92   |

Inoltre il programma `Esercitazione6Punto3.cu`utilizza 10 registri quindi il numero di registri utilizzati sarà:$1536*10 = 15360 < 32768$.

<div style="page-break-after: always;"></div>
#### Grafico

![](C:\Users\johnn\Desktop\Unisa\Magistrale\MNI\Esercitazioni\Esercitazione6\Img\Valutazione SpeedUp.png)

