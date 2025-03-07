# **Respuestas**


**1.** Álgebra de Matrices

  **a.**

  **i.** El tiempo de ejecución con las funciones getValor y setValor es 0.091989 seg con N=256. Si no se utilizan tales funciones el tiempo es de 0.046572 seg con N=256. Estas funciones no son necesarias, puesto que la distinción entre si la matriz esta ordenada por filas o por columnas puede realizarse modificando el calculo del desplazamiento de los vectores. El overhead introducido por el llamado a estas funciones es innecesario.
  
  **ii.** La solución que almacena todas las matrices por filas tiene un tiempo de 3.994845 seg, mientras que la que almacena a la matriz $\mathrm{B}$ por columnas tiene un tiempo de 2.282085 seg con N=1024. Esto se debe a como se comporta el algoritmo que multiplica matrices en relación con el principio de localidad: Para obtener una celda de la matriz resultado se requiere de una fila de la matriz $\mathrm{A}$ y una columna de la matriz $\mathrm{B}$, por el principio de localidad, al estar $\mathrm{B}$ almacenada en filas, al existir un miss en caché, se trae de la memoria RAM la posición de la matriz B requerida y las posiciones cercanas, las cuales son parte de la misma columna, lo cual provoca que las mismas ya se encuentren en caché para cuando se las requiera en el corto plazo. 


**b.** Para la primer operación la multiplicación en bucles separados es más rápida, esto se debe a la diferencia en los fallos de caché: Cuando se realiza todo en un mismo bucle, se debe traer los datos de todas las matrices, las cuales no entran todas en memoria para una gran cantidad de datos, lo que provoca muchos misses en caché. En cambio, cuando se realiza en 4 bucles esto sucede a menor escala. Lo mismo sucede en el segundo caso de estudio. NO así en el tercero, en el cual se trabaja con menos matrices, y se usan por lo menos dos bucles para realizar la multiplicación.

**c.** La segunda opción tiene mejor rendimiento.

**d.** El algoritmo multBloques.c realiza la multiplicación de matrices por bloques más pequeños de la siguiente forma: Toma submatrices cuadradas de $\mathrm{BS} \cdot \mathrm{BS}$ de las matrices originales, y realiza la multiplicación con estas submatrices. El algoritmo optimizado de el ejercicio 1a tarda 2.229850 seg para el con N = 1024, mientras que el algoritmo multBloques tarda:

| BS |  Tiempo [seg] |
| :--------: | :----: |
| 16          | 2.422246  |
| 64          | 2.385468  |
| 256         | 2.339609  |
| 512         | 2.219465  |
| 1024         | 2.170937  |

Esto se debe a que, al estar ambas matrices ordenadas de forma tal que los siguientes valores que se requeriran estan consecutivos (por estar ordenadas por filas y columnas, respectivamente) no conviene segmentar mucho la cantidad de datos a utilizar, puesto que se traeran mayor cantidad de datos que no se requeriran en un tiempo proximo. SI conviene, cuando la segmentación permite traer a caché solo los datos que se requeriran para la multiplicación aprovechando el principiode localidad. El valor de bloque óptimo depende de la caché del sistema que se este utilizando.


**e.** La alternativa sin ceros es más rápida puesto que no almacena en caché los ceros innecesarios.


**2.** Al ser el algoritmo iterativo un algoritmo de orden $N$ y el recursivo de orden $2^N$ es lógico que al aumentar el valor de N, la diferencia aumente. Además de todas las llamadas recursivas.

| N |  Tiempo Iter [seg] | Tiempo Rec [seg] |
| :--------: | :----: | :----: |
| 10          | 0.0000000000  | 0.0000001907  |
| 20          | 0.0000000000  | 0.0000272036   |
| 30         | 0.0000000954  | 0.0040948153  |
| 40         | 0.0000000954  | 0.3819421053  |
| 50         | 0.0000000954  | 54.1052495956  |

**3.** La opción con registro tarda menos, eso se debe a que los vectores que son parte de la operación tienen todos los datos agrupados, por lo que se trae a caché datos que pueden ser utilizados. No así en el caso de sin registro, aunque por resultado del azar tambien puede suceder.

| N |  Con Reg. [seg] | Sin Reg. [seg] |
| :--------: | :----: | :----: |
| 2048          | 0.0000338554  | 0.0000629425  |
| 4096          | 0.0000610352  | 0.0001530647   |
| 8192         | 0.0001080036  | 0.0001268387  |


**4.** Optimización de instrucciones

**a.** Las operaciones de división son las que más tiempo tardan, seguidas de las de producto, y luego suma y resta con tiempos parecidos. Esto se debe a que las operaciones de división y multiplicación requieren más ciclos de reloj que la suma y la resta.

Los promedios sin potencias de 2:

| N |  R | Suma [seg] | Resta [seg] | Mult [seg] | Div [seg] |
| :--------: | :----: | :----: |:----: |:----: |:----: |
| 2048          | 100000 | 12.0915006846   | 12.0244105347  | 0.0000193948  | 0.0000270898  |
| 4096          | 100000 | 0.0000182132  |  0.0000218803   | 0.0000381377   | 0.0000346161 |

Con potencias de 2:

| N |  R | Suma [seg] | Resta [seg] | Mult [seg] | Div [seg] |
| :--------: | :----: | :----: |:----: | :----: |:----: |
| 2048          | 100000 | 0.0000140630    |  0.0000141677   | 0.0000112806  | 0.0000168453  |
| 4096          | 100000 | 0.0000296335  |  0.0000309083  | 0.0000223634 | 0.0000259315 |

Puesto que las operaciones de multiplicación y division de números de potencias de 2 puede realizarse como desplazamientos a izquierda y derecha respectivamente, las operaciones toman menos tiempo.


**b.** La operación que se realiza es una division por 5 y una multiplicación por 0.2, lo cual es equivalente en términos matemáticos. 

| N |  R |  Div [seg] | Mult [seg] |
| :--------: | :----: | :----: |:----: |
| 2048          | 100000 | 0.0001300927     |  0.0001057065   |
| 4096          | 100000 | 0.0001041755  | 0.0001199758   |
| 8192          | 100000 | 0.0000958917  | 0.0001435796   |

Se puede observar que la operación de producto es menos eficiente, esto se debe a que el producto por un número flotante es más costoso que la división por un número entero. ?????

**c.** Por como se implementa la operación de módulo, se requiere dividir por el cociente m. Al ser m una potencia de 2, la división es más eficiente por implementarse como desplazamientos.

| N |  r |  Usando % [seg] | Equivalencia [seg] |
| :--------: | :----: | :----: |:----: |
| 2048          | 128 | 0.000015    |  0.000012   |
| 4096          | 128 | 0.000044  | 0.000022   |
| 8192          | 128 | 0.000037  | 0.000028   |

**5.** 

**a.** La forma más rápida, a medida que la cantidad de datos crece, de inicializar los valores del array en 1 es con a[i]. Por que??

**b.** Al ser la variable i un ```int```, no puede representar el número XXXXX es por ello que se lo debe cambiar por el tipo de dato ```unsigned long```.

**6.** La tabla que muestra los tiempos para distintos valores de N se muestra a continuación:

| N | Sol 1 [seg] | sol2 [seg] | sol3 [seg] |
| :--------: |  :----: |:----: |:----: |
| 2048          | 0.000007    |  0.000006   | 0.000006 |
| 4096          | 0.000024  | 0.000021   |  0.000020 |
| 1000000       | 0.003804  | 0.003659   | 0.004096 |
| 1000000000    | 5.166677  | 5.255893   | 5.623722 |

Esto muestra el overhead que produce un if, es por ello que, de ser posible, hay que evitarlo.

**7.** 



**8.** El algoritmo nreinas.c resuelve el problemas de las N Reinas. Entender el problema que resuelve el algoritmo y analizar el comportamiento del tiempo de ejecución a medida que crece N. Probar para N, de uno en uno, desde 4 a 20 ¿Qué orden de ejecución tiene?

Ejecución doble precisión: ./nreinas $N$
$N$ : número de reinas
