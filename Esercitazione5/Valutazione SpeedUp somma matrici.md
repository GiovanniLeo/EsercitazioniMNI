# Valutazione SpeedUp somma matrici

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

Il programma `Esercitazione5MNIPunto2.cu` utilizza 12 registri, consideriamo tre configurazioni per kernel.

#### Configurazione 1

Sapendo che il massimo numero di thread per SM è 1536 e supponendo di voler  utilizzare blocchi di thread di dimensione 8x8 quindi i thread per blocco attivati saranno 64.

Quindi il numero di blocchi che verranno attivati per ogni SM sarà:
$$
\frac{1536}{64} = 24
$$
questo dovrebbe causare un peggioramento delle performance poiché in parallelo ne possono essere attivati solo 8 per ogni SM.

| N    | Tempi CPU | Tempi GPU | SpeedUp |
| ---- | --------- | --------- | ------- |
| 1024 | 4,8235    | 0,3058    | 15,77   |
| 2048 | 18,7159   | 1,1915    | 15,70   |
| 4096 | 73,5342   | 5,1810    | 14,19   |
| 6144 | 162,5213  | 17,7274   | 9,16    |
| 7168 | 223,7443  | 17,0711   | 13,10   |

<div style="page-break-after: always;"></div>

#### Configurazione 2

Sapendo che il massimo numero di thread per SM è 1536 e supponendo di voler  utilizzare blocchi di thread di dimensione 16x16 quindi i thread per blocco attivati saranno 256.

Quindi il numero di blocchi che verranno attivati per ogni SM sarà:
$$
\frac{1536}{256} = 6
$$
In questo caso stiamo utilizzando meno blocchi di quanto ne abbiamo a disposizione.

| N    | Tempi CPU | Tempi GPU | SpeedUp |
| ---- | --------- | --------- | ------- |
| 1024 | 4,8235    | 0,3083    | 15,64   |
| 2048 | 18,7159   | 1,1676    | 16,02   |
| 4096 | 73,5342   | 5,3809    | 13,66   |
| 6144 | 162,5213  | 12,8731   | 12,62   |
| 7168 | 223,7443  | 14,5891   | 15,33   |

#### Configurazione 3

Sapendo che il massimo numero di thread per SM è 1536 e supponendo di voler  utilizzare blocchi di thread di dimensione 32x32 quindi i thread per blocco attivati saranno 1024.

Quindi il numero di blocchi che verranno attivati per ogni SM sarà:
$$
\frac{1536}{1024} = 1,5
$$
In questo caso stiamo utilizzando meno blocchi di quanto ne abbiamo a disposizione, poiché stiamo andando ad utilizzare 2 blocchi.

| N    | Tempi CPU | Tempi GPU | SpeedUp |
| ---- | --------- | --------- | ------- |
| 1024 | 4,8235    | 0,6643    | 7,26    |
| 2048 | 18,7159   | 2,4753    | 7,56    |
| 4096 | 73,5342   | 10,5287   | 6,98    |
| 6144 | 162,5213  | 22,5318   | 7,21    |
| 7168 | 223,7443  | 30,2957   | 7,38    |
<div style="page-break-after: always;"></div>

#### Grafico

![](C:\Users\johnn\Desktop\Unisa\Magistrale\MNI\Esercitazioni\Esercitazione5\Img\Valutazione SpeedUp punto2.png)

