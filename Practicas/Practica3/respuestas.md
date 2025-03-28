# **Respuestas**

**1.** El programa tiene un error en ejecución debido a que la variable ``i`` se declara como *private*, esto provoca que el mismo tenga un valor indefinido. Para que esto no suceda, se puede poner como *first private*o *shared*: de esta forma, la inicialización del indice ``i`` del for externo se podrá utilizar en el for interno. De todas formas, esta no es la mejor opción para distribuir las iteraciones, para ello se debe llevar la directiva de forma que afecte a los dos bucles, de esta forma, la distribución de las iteraciones se realizará de mejor forma. Se añade ``collapse(2)`` para que la directiva afecta a ambos bucles y se elimina ``private(i, j)`` puesto que los indices de los bucles afectados por la directiva ya son privados por defecto. 

```c
#pragma omp parallel for collapse(2) shared(A)
for (i = 0; i < N; i++)
{
  for (j = 0; j < N; j++)
  {
    A[i * N + j] = i * j;
  }
}
```

**2.** El programa suma N veces en una variable de la siguiente forma:

$$
  x = x + \sqrt{i\cdot 2.78} + 2\cdot x
$$

Siendo $x$ la variable en cuestión, e $i$ el índice del for.

Al ejecutar el programa múltiples veces nos damos cuenta que el valor final de la variable $x$ cambia, esto se debe a que la distribución de las iteraciones del bucle hace que no se ejecuten en orden. Como la variable $x$ es una variable compartida, esto provoca que todos modifiquen la misma variable y que el resultado sea incierto puesto que no realizan la acción atómicamente. Para ello se debe utilizar un ``reduction`` de la siguiente forma:

```c
#pragma omp parallel for reduction(+:x)
for (i = 1; i <= N; i++)
{
  x = x + sqrt(i * scale) + 2 * x;
}
```

De esta forma, para una cantidad de threads dada el resultado es prácticamente el mismo.

**3.** Los resultados de la comparación se muestran en la siguiente tabla:

|   N   |   T   | Primer for [seg] | Segundo for [seg] |
| :---: | :---: | :--------------: | :---------------: |
| 1024  |   2   |     1.245569     |     1.215448      |
| 1024  |   4   |     0.814045     |     0.843131      |
| 1024  |   8   |     0.620820     |     0.761618      |
| 2048  |   2   |    10.152034     |     14.416451     |
| 2048  |   4   |     9.824431     |     11.112171     |
| 2048  |   8   |     8.889582     |     10.282979     |
| 4096  |   2   |    114.481271    |    112.395292     |
| 4096  |   4   |    73.149457     |     79.917950     |
| 4096  |   8   |    63.968045     |     71.877105     |


**4.** En comparación con el algoritmo secuencial se obtiene:

|   N   |   T   | Secuencial [seg] | Paralelo [seg] |
| :---: | :---: | :--------------: | :------------: |
| 1024  |   2   |     0.006520     |    0.004727    |
| 1024  |   4   |     0.008068     |    0.002738    |
| 1024  |   8   |     0.006166     |    0.003333    |
| 2048  |   2   |     0.038066     |    0.031163    |
| 2048  |   4   |     0.034593     |    0.022443    |
| 2048  |   8   |     0.032797     |    0.018238    |
| 4096  |   2   |     0.168881     |    0.155290    |
| 4096  |   4   |     0.171870     |    0.133420    |
| 4096  |   8   |     0.184954     |    0.147137    |

Dado que la diferencia de tiempo de ejecución entre los threads puede ser de más del doble, se puede concluir que la distribución de las iteraciones del for no es la más indicada. Una mejora posible es añadir a la directiva de openMP el ``collapse(2)`` el cuál distribuirá todas las iteraciones de los bucles involucrados. Una opción que se puede agregar es la política de distribución de iteraciones, las cuales pueden ser:
- **Estática (por defecto):** se distribuyen proporcionalmente entre los hilos.
- **Dinámica:** se distribuyen por demanda y de a cierta cantidad (chunk).
- **Guiada:** distribución de iteraciones variable.
- **Runtime:** distribución que se indica desde el Sistema Operativo por medio de la variable de ambiente ``OMP_SCHEDULE``.

Dado que la política estática es la utilizada por defecto, podemos probar con la dinámica o la guiada. De todas formas, como los cálculos que se deben realizar en cada iteración no difieren mucho, no es necesario utilizarlo en este caso.


|   N   |   T   | Paralelo [seg] | Paralelo con collapse [seg] |
| :---: | :---: | :------------: | :-------------------------: |
| 4096  |   2   |    0.158035    |          0.130609           |
| 4096  |   4   |    0.137322    |          0.117270           |
| 4096  |   8   |    0.145122    |          0.143993           |
| 8192  |   2   |    0.645738    |          0.505722           |
| 8192  |   4   |    0.517071    |          0.576582           |
| 8192  |   8   |    0.457160    |          0.544751           |
| 16384 |   2   |    2.172798    |          2.098539           |
| 16384 |   4   |    1.948136    |          1.841305           |
| 16384 |   8   |    1.867530    |          1.674319           |



**5.** Al paralelizar utilizando sections obtenemos la siguiente comparación de resultados:


|   N   |   T   | Secuencial [seg] | Paralelo con collapse [seg] | Speedup  |
| :---: | :---: | :--------------: | :-------------------------: | :------: |
|  512  |   2   |     0.600879     |          0.814313           | 0.737185 |
|  512  |   4   |     0.600879     |          0.978478           | 0.613352 |
| 1024  |   2   |     5.109079     |          7.760253           | 0.659064 |
| 1024  |   4   |     5.109079     |          8.718323           | 0.586399 |
| 2048  |   2   |    44.731859     |          65.152399          | 0.686741 |
| 2048  |   4   |    44.731859     |          65.417605          | 0.683971 |


A medida que los threads aumentan el speedup es menor. Esto se debe al overhead que introduce la creación de lo hilos, puesto que se utilizan dos sections no tiene sentido tener más de dos threads.


**6.** 
