# **Respuestas**


**1.** Álgebra de Matrices

  **a.**

  **i.** Estas funciones (getValor y setValor) no son necesarias, puesto que la distinción entre si la matriz esta ordenada por filas o por columnas puede realizarse modificando el calculo del desplazamiento de los vectores. El overhead introducido por el llamado a estas funciones es innecesario.

  |   N   | Con funciones [seg] | Sin funciones [seg] |
  | :---: | :-----------------: | :-----------------: |
  |  32   |      0.000393       |      0.000130       |
  |  64   |      0.003440       |      0.001021       |
  |  128  |      0.018032       |      0.004954       |
  |  256  |      0.109631       |      0.050508       |
  |  512  |      0.907196       |      0.552234       |
  | 1024  |      9.799982       |      5.039400       |
  | 2048  |     231.612737      |     108.763959      |
  
  **ii.** La solución que almacena todas las matrices por filas tiene un tiempo de 3.994845 seg, mientras que la que almacena a la matriz $\mathrm{B}$ por columnas tiene un tiempo de 2.282085 seg con N=1024. 
  

|   N   | B por filas [seg] | B por columnas [seg] |
| :---: | :---------------: | :------------------: |
|  32   |     0.000130      |       0.000134       |
|  64   |     0.001021      |       0.001155       |
|  128  |     0.004954      |       0.008396       |
|  256  |     0.050508      |       0.051392       |
|  512  |     0.552234      |       0.350135       |
| 1024  |     5.039400      |       2.851934       |
| 2048  |    108.763959     |      22.987597       |
  
  
  Esto se debe a como se comporta el algoritmo que multiplica matrices en relación con el principio de localidad: Para obtener una celda de la matriz resultado se requiere de una fila de la matriz $\mathrm{A}$ y una columna de la matriz $\mathrm{B}$, por el principio de localidad, al estar $\mathrm{B}$ almacenada en filas, al existir un miss en caché, se trae de la memoria RAM la posición de la matriz B requerida y las posiciones cercanas, las cuales son parte de la misma columna, lo cual provoca que las mismas ya se encuentren en caché para cuando se las requiera en el corto plazo. 


**b.** Para la primer operación la multiplicación en bucles separados es más rápida, esto se debe a la diferencia en los fallos de caché: Cuando se realiza todo en un mismo bucle, se debe traer los datos de todas las matrices, las cuales no entran todas en memoria para una gran cantidad de datos, lo que provoca muchos misses en caché. En cambio, cuando se realiza en 4 bucles esto sucede a menor escala. Lo mismo sucede en el segundo caso de estudio. NO así en el tercero, en el cual se trabaja con menos matrices, y se usan por lo menos dos bucles para realizar la multiplicación.?????

Para la primera operación los tiempos medidos son:

|   N   | Un bucle [seg] | Cuatro bucles [seg] |
| :---: | :------------: | :-----------------: |
|  32   |    0.000303    |      0.000887       |
|  64   |    0.003104    |      0.003104       |
|  128  |    0.018612    |      0.016752       |
|  256  |    0.129377    |      0.130763       |
|  512  |    0.974246    |      1.008708       |
| 1024  |    7.624256    |      7.361145       |
| 2048  |   67.865308    |      70.140416      |


Para la segunda operación los tiempos medidos son:

|   N   | Un bucle [seg] | Tres bucles [seg] |
| :---: | :------------: | :---------------: |
|  32   |    0.000248    |     0.000357      |
|  64   |    0.002405    |     0.003228      |
|  128  |    0.014067    |     0.013224      |
|  256  |    0.078209    |     0.077200      |
|  512  |    0.524877    |     0.711974      |
| 1024  |    5.065214    |     5.112862      |
| 2048  |   43.280783    |     46.067790     |



Para la tercera operación los tiempos medidos son:

|   N   | Dos bucles [seg] | Cuatro bucles [seg] |
| :---: | :--------------: | :-----------------: |
|  32   |     0.000340     |      0.000398       |
|  64   |     0.002261     |      0.001943       |
|  128  |     0.019749     |      0.017968       |
|  256  |     0.117193     |      0.109097       |
|  512  |     0.850511     |      0.892947       |
| 1024  |     7.257749     |      7.833700       |
| 2048  |    66.388276     |      65.091195      |


**c.** La segunda opción tiene mejor rendimiento. Este resultado se debe al principio de localidad. Para obtener una celda de la matriz resultado se requiere de una fila de la matriz $\mathrm{A}$ y una columna de la matriz $\mathrm{B}$, por el principio de localidad, al estar $\mathrm{B}$ almacenada en filas, al existir un miss en caché, se trae de la memoria RAM la posición de la matriz B requerida y las posiciones cercanas, las cuales son parte de la misma columna, lo cual provoca que las mismas ya se encuentren en caché para cuando se las requiera en el corto plazo. 

|   N   | Mismo orden [seg] | Distinto orden [seg] |
| :---: | :---------------: | :------------------: |
|  256  |     0.000001      |       0.000000       |
|  512  |     0.000001      |       0.000000       |
| 1024  |     0.000001      |       0.000000       |
| 2048  |     0.000002      |       0.000000       |
| 4096  |     0.000001      |       0.000000       |
| 8192  |     0.000001      |       0.000000       |



**d.** El algoritmo multBloques.c realiza la multiplicación de matrices por bloques más pequeños de la siguiente forma: Toma submatrices cuadradas de $\mathrm{BS} \cdot \mathrm{BS}$ de las matrices originales, y realiza la multiplicación con estas submatrices. La comparación de tiempos es:

|   N   |  BS   | Bloques [seg] | Sin bloques [seg] |
| :---: | :---: | :-----------: | :---------------: |
|  128  |  32   |   0.005353    |     0.008396      |
|  128  |  64   |   0.009886    |     0.008396      |
|  256  |  64   |   0.045431    |     0.051392      |
|  256  |  128  |   0.041318    |     0.051392      |
|  512  |  128  |   0.287134    |     0.350135      |
|  512  |  256  |   0.300667    |     0.350135      |
| 1024  |  256  |   2.701628    |     2.851934      |
| 1024  |  512  |   2.570317    |     2.851934      |
| 2048  |  512  |   17.863765   |     22.987597     |
| 2048  | 1024  |   23.113593   |     22.987597     |


Esto se debe a que, al estar ambas matrices ordenadas de forma tal que los siguientes valores que se requeriran estan consecutivos (por estar ordenadas por filas y columnas, respectivamente) no conviene segmentar mucho la cantidad de datos a utilizar, puesto que se traeran mayor cantidad de datos que no se requeriran en un tiempo proximo. SI conviene, cuando la segmentación permite traer a caché solo los datos que se requeriran para la multiplicación aprovechando el principiode localidad. El valor de bloque óptimo depende de la caché del sistema que se este utilizando.


**e.** La alternativa sin ceros es más rápida puesto que no almacena en caché los ceros innecesarios (los cuales no se utilizan para ninguno de los algoritmos). Esto diferencia se refleja a medida que N crece, como se puede ver a continuación:

|   N   | Con ceros [seg] | Sin ceros [seg] |
| :---: | :-------------: | :-------------: |
|  32   |    0.000240     |    0.000248     |
|  64   |    0.002122     |    0.001548     |
|  128  |    0.020260     |    0.014048     |
|  256  |    0.105727     |    0.091918     |
|  512  |    1.068523     |    1.154903     |
| 1024  |    13.804393    |    16.563723    |
| 2048  |   211.723917    |   196.267308    |


**2.** Al ser el algoritmo iterativo un algoritmo de orden $N$ y el recursivo de orden $2^N$ es lógico que al aumentar el valor de N, la diferencia aumente. Además de todas las llamadas recursivas.

|   N   | Tiempo Iter [seg] | Tiempo Rec [seg] |
| :---: | :---------------: | :--------------: |
|  10   |   0.0000000000    |   0.0000001907   |
|  20   |   0.0000000000    |   0.0000272036   |
|  30   |   0.0000000954    |   0.0040948153   |
|  40   |   0.0000000954    |   0.3819421053   |
|  50   |   0.0000000954    |  54.1052495956   |



**3.** La opción sin registro tarda menos????

|     N     | Con Reg. [seg] | Sin Reg. [seg] |
| :-------: | :------------: | :------------: |
|  1000000  |  0.0125489235  |  0.0120441914  |
| 10000000  |  0.0933120251  |  0.0747880936  |
| 100000000 |  0.9534091949  |  0.7237589359  |


**4.** Optimización de instrucciones

**a.** Las operaciones de división son las que más tiempo tardan, seguidas de las de producto, y luego suma y resta con tiempos parecidos. Esto se debe a que las operaciones de división y multiplicación requieren más ciclos de reloj que la suma y la resta.

Los promedios sin potencias de 2:

|   N   |   R    |  Suma [seg]   |  Resta [seg]  |  Mult [seg]  |  Div [seg]   |
| :---: | :----: | :-----------: | :-----------: | :----------: | :----------: |
| 2048  | 100000 | 12.0915006846 | 12.0244105347 | 0.0000193948 | 0.0000270898 |
| 4096  | 100000 | 0.0000182132  | 0.0000218803  | 0.0000381377 | 0.0000346161 |

Con potencias de 2:

|   N   |   R    |  Suma [seg]  | Resta [seg]  |  Mult [seg]  |  Div [seg]   |
| :---: | :----: | :----------: | :----------: | :----------: | :----------: |
| 2048  | 100000 | 0.0000140630 | 0.0000141677 | 0.0000112806 | 0.0000168453 |
| 4096  | 100000 | 0.0000296335 | 0.0000309083 | 0.0000223634 | 0.0000259315 |

Puesto que las operaciones de multiplicación y division de números de potencias de 2 puede realizarse como desplazamientos a izquierda y derecha respectivamente, las operaciones toman menos tiempo.


**b.** La operación que se realiza es una division por 5 y una multiplicación por 0.2, lo cual es equivalente en términos matemáticos. 

|   N   |   R    |  Div [seg]   |  Mult [seg]  |
| :---: | :----: | :----------: | :----------: |
| 2048  | 100000 | 0.0001300927 | 0.0001057065 |
| 4096  | 100000 | 0.0001041755 | 0.0001199758 |
| 8192  | 100000 | 0.0000958917 | 0.0001435796 |

Se puede observar que la operación de producto es menos eficiente, esto se debe a que el producto por un número flotante es más costoso que la división por un número entero. ?????

**c.** Por como se implementa la operación de módulo, se requiere dividir por el cociente m. Al ser m una potencia de 2, la división es más eficiente por implementarse como desplazamientos.

|   N   |   r   | Usando % [seg] | Equivalencia [seg] |
| :---: | :---: | :------------: | :----------------: |
| 2048  |  128  |    0.000015    |      0.000012      |
| 4096  |  128  |    0.000044    |      0.000022      |
| 8192  |  128  |    0.000037    |      0.000028      |

**5.** 

**a.** La forma más rápida, a medida que la cantidad de datos crece, de inicializar los valores del array en 1 es con a[i]. Por que??

**b.** Al ser la variable i un ```int```, no puede representar el número XXXXX es por ello que se lo debe cambiar por el tipo de dato ```unsigned long```.

**6.** La tabla que muestra los tiempos para distintos valores de N se muestra a continuación:

|     N      | Sol 1 [seg] | sol2 [seg] | sol3 [seg] |
| :--------: | :---------: | :--------: | :--------: |
|    2048    |  0.000007   |  0.000006  |  0.000006  |
|    4096    |  0.000024   |  0.000021  |  0.000020  |
|  1000000   |  0.003804   |  0.003659  |  0.004096  |
| 1000000000 |  5.166677   |  5.255893  |  5.623722  |

Esto muestra el overhead que produce un if, es por ello que, de ser posible, hay que evitarlo.

**7.** Para el tipo de dato de simple precision:

|     N     | FLOAT [seg] | INT [seg] | Precision (Error medio) |
| :-------: | :---------: | :-------: | :---------------------: |
|  1000000  |    0.01     |   0.02    |        17.496799        |
| 10000000  |    0.10     |   0.16    |        17.461400        |
| 100000000 |    1.05     |   1.69    |        17.425294        |

Para el tipo de dato de doble precision:

|     N     | FLOAT [seg] | INT [seg] | Precision (Error medio) |
| :-------: | :---------: | :-------: | :---------------------: |
|  1000000  |    0.02     |   0.02    |        0.000000         |
| 10000000  |    0.18     |   0.17    |        0.000000         |
| 100000000 |    2.22     |   1.73    |        0.000000         |

Si bien se gana precision con el tipo de datos de doble precision, se pierde eficiencia. Por lo tanto, se debe usar doble precision solo cuando es necesario.


**8.** La cantidad de soluciones para n entre 4 y 20 se muestra a continuación:

|    n |           Q(n) |
| ---: | -------------: |
|    4 |              2 |
|    5 |             10 |
|    6 |              4 |
|    7 |             40 |
|    8 |             92 |
|    9 |            352 |
|   10 |            724 |
|   11 |          2,680 |
|   12 |         14,200 |
|   13 |         73,712 |
|   14 |        365,596 |
|   15 |      2,279,184 |
|   16 |     14,772,512 |
|   17 |     95,815,104 |
|   18 |    666,090,624 |
|   19 |  4,968,057,848 |
|   20 | 39,029,188,884 |


Por lo tanto, el orden del algoritmo es de orden exponencial.

[Información acerca del problema de la n-reinas](https://www.cs.buap.mx/~zacarias/FZF/nreinas3.pdf)