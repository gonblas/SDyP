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

**3.** Analizar los algoritmos productoVectorialRegistro.c y productoVectorialSinRegistro.c. Ambos programas parten de dos conjuntos de N vectores matemáticos y realizan el producto vectorial uno a uno de acuerdo al orden de cada vector en el conjunto. ¿Cuál de las dos soluciones es más rápida? ¿Por qué?:

Ejecución: ./ productoVectorialRegistro N
./ productoVectorialSinRegistro N
N : tamaño de los conjuntos de vectores

**4.** Optimización de instrucciones

**a.** El algoritmo instrucciones $1 . c$ compara el tiempo de ejecución de las operaciones básicas suma $(+)$, resta ( - ), multiplicación $\left(^{*}\right)$ y división (/), aplicadas sobre los elementos que se encuentran en la misma posición de dos vectores x e y. ¿Qué análisis se puede hacer de cada operación? ¿Qué ocurre si los valores de los vectores x e y son potencias de 2 ?

**Ejecución:** ./instrucciones 1 Nr
$N$ : tamaño de los vectores
$r$ : número de repeticiones
b. En función del ejercicio anterior analizar el algoritmo instrucciones2.c que aplica dos operaciones distintas a cada elemento de un vector x .

**Ejecución:** ./instrucciones 2 Nr
$N$ : tamaño de los vectores
$r$ : número de repeticiones
c. El algoritmo modulo.c compara el tiempo de ejecución de dos versiones para obtener el resto de un cociente $m$ ( m potencia de 2 ) de los elementos enteros de un vector de tamaño N . ¿Qué análisis se puede hacer de las dos versiones?

**Ejecución:** ./modulo Nm
$N$ : tamaño del vector
m: potencia de 2

**5.** Iteraciones

**a.** Analizar el algoritmo optForArray.c que inicializa un vector con sus valores en 1 de dos formas. ¿Cuál es más rápida?

Ejecución: ./optForArray NR
$N$ : tamaño de la matriz
$R$ : cantidad de repeticiones
**b.** Analizar el algoritmo GaussFor.c que calcula la suma de N números naturales consecutivos y lo compara con la suma de Gauss.
¿Por qué la suma para $\mathrm{N}=2147483647$ da resultado correcto y para $\mathrm{N}=2147483648$ el resultado es erróneo? ¿Cómo lo solucionaría?

Ejecución: ./GaussFor N
$N$ : número de elementos a sumar

**6.** El algoritmo overheadIF.c da tres soluciones al siguiente problema: dado un vector V y una posición P , el algoritmo cuenta la cantidad de números del vector V que son menores al elemento en la posición $P$.

Analizar los tiempos obtenidos de las tres soluciones y evaluar las fuentes de overhead en cada caso.

Ejecución: ./overheadIF N
$N$ : tamaño del vector
**7.** Compilar y ejecutar el archivo precision.c que calcula el número de fibonacci para los elementos de un vector de tamaño N . El algoritmo compara el resultado de aplicarlo a elementos de tipo de datos entero respecto a aplicarlo a elementos de coma flotante en simple y doble precisión.

Analizar los tiempos obtenidos para cada tipo de datos. ¿Qué conclusiones se pueden obtener del uso de uno u otro tipo de dato?

Compilar en simple precisión: gcc -O2 -lm -o singlep precision.c Compilar en doble precision: gcc -O2 -DDOUBLE -lm -o doblep precision.c

Ejecución simple precisión: ./singlep $N$ Ejecución doble precisión: ./doblep $N$
$N$ : tamaño del vector
**8.** El algoritmo nreinas.c resuelve el problemas de las N Reinas. Entender el problema que resuelve el algoritmo y analizar el comportamiento del tiempo de ejecución a medida que crece N. Probar para N, de uno en uno, desde 4 a 20 ¿Qué orden de ejecución tiene?

Ejecución doble precisión: ./nreinas $N$
$N$ : número de reinas
