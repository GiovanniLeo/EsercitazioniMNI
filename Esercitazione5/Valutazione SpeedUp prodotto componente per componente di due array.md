# Valutazione SpeedUp prodotto componente per componente di due array

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

Il programma `Esercitazione5MNIPunto1.cu` utilizza 10 registri, consideriamo tre configurazioni per kernel.

#### Configurazione 1

Sapendo che il massimo numero di thread per SM è 1536 e supponendo di voler attivare solo 2 blocchi si ha: 
$$
\frac{1536}{2} = 768
$$
ovvero vengono attivati 768 thread per blocco. 

Considerando che vengono attivati 1536 thread ed il numero di registri utilizzati dal programma è 10 allora il numero di registri totale utilizzati sarà: $1536*10 = 15360 < 32768$. 

| Nx1000 | Tempi CPU | Tempi GPU | SpeedUp |
| ------ | --------- | --------- | ------- |
| 128    | 0,5098    | 0,0207    | 24,62   |
| 192    | 0,7444    | 0,0275    | 27,06   |
| 256    | 0,9941    | 0,0345    | 28,81   |
| 448    | 1,72      | 0,0589    | 29,20   |
| 512    | 1,97      | 0,0635    | 31,02   |

<div style="page-break-after: always;"></div>

#### Configurazione 2

Sapendo che il massimo numero di thread per SM è 1536 e supponendo di voler attivare solo 4 blocchi si ha: 
$$
\frac{1536}{4} = 384
$$
ovvero vengono attivati 384 thread per blocco. 

Considerando che vengono attivati 1536 thread ed il numero di registri utilizzati dal programma è 10 allora il numero di registri totale utilizzati sarà: $1536*10 = 15360 < 32768$. 

| Nx1000 | Tempi CPU | Tempi GPU | SpeedUp |
| ------ | --------- | --------- | ------- |
| 128    | 0,5098    | 0,0348    | 14,64   |
| 192    | 0,7444    | 0,0480    | 15,50   |
| 256    | 0,9941    | 0,0586    | 16,96   |
| 448    | 1,72      | 0,0578    | 29,75   |
| 512    | 1,97      | 0,0648    | 31,40   |

#### Configurazione 3

Sapendo che il massimo numero di thread per SM è 1536 e supponendo di voler attivare solo 8 blocchi si ha: 
$$
\frac{1536}{8} = 192
$$
ovvero vengono attivati 192 thread per blocco. 

Considerando che vengono attivati 1536 thread ed il numero di registri utilizzati dal programma è 10 allora il numero di registri totale utilizzati sarà: $1536*10 = 15360 < 32768$. 

| Nx1000 | Tempi CPU | Tempi GPU | SpeedUp |
| ------ | --------- | --------- | ------- |
| 128    | 0,5098    | 0,0352    | 14,48   |
| 192    | 0,7444    | 0,0483    | 15,41   |
| 256    | 0,9941    | 0,0568    | 17,50   |
| 448    | 1,72      | 0,0596    | 28,85   |
| 512    | 1,97      | 0,0663    | 29,71   |

<div style="page-break-after: always;"></div>

#### Grafico

![](C:\Users\johnn\Desktop\Unisa\Magistrale\MNI\Esercitazioni\Esercitazione5\Img\Valutazione SpeedUp punto1.png)

