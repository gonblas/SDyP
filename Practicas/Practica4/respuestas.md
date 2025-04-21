# **Respuestas**


**1.** Álgebra de Matrices

  **a)** Utilizando operaciones punto a punto se obtuvieron los siguientes resultados:

  Tabla Speedup:

  | comm_sz | 512      | 1024     | 2048     | 4096 |
  | ------- | -------- | -------- | -------- | ---- |
  | 2       | 1.959968 | 2.049082 | 1.798668 |      |
  | 4       | 3.340898 | 3.697907 | 4.083720 |      |
  | 8       | 5.066532 | 5.840257 | 6.470412 |      |


  Tabla Eficiencia:

  | comm_sz | 512      | 1024     | 2048     | 4096 |
  | ------- | -------- | -------- | -------- | ---- |
  | 2       | 0.979984 | 1.024541 | 0.899334 |      |
  | 4       | 0.835224 | 0.924477 | 1.020930 |      |
  | 8       | 0.633316 | 0.730032 | 0.808801 |      |

  Respecto a la escalabilidad, no es fuertemente escalable pues la eficiencia decrementa a medida que aumentan las unidades de procesamiento. 

  Respecto al overhead se puede ver en la tabla de eficiencia que ...

  **b)** Utilizando operaciones colectivas se obtuvieron los siguientes resultados:

  Tabla Speedup:

  | comm_sz | 512      | 1024     | 2048     | 4096     |
  | ------- | -------- | -------- | -------- | -------- |
  | 2       | 2.289750 | 2.165261 | 2.148498 | 1.871160 |
  | 4       | 3.524248 | 3.168820 | 3.885463 | 4.472184 |
  | 8       | 6.386248 | 7.170949 | 7.001241 |          |

  Tabla Eficiencia:

  | comm_sz | 512      | 1024     | 2048     | 4096     |
  | ------- | -------- | -------- | -------- | -------- |
  | 2       | 1.144875 | 1.082631 | 1.074249 | 0.935580 |
  | 4       | 0.881062 | 0.792205 | 0.971366 | 1.118046 |
  | 8       | 0.798281 | 0.896369 | 0.875155 |          |

  Respecto a la escalabilidad ...

  Respecto al overhead se puede ver en la tabla de eficiencia que ...


**2.** Se obtuvieron los siguientes resultados:

Tabla Speedup:

  | comm_sz | 512      | 1024     | 2048     | 4096     |
  | ------- | -------- | -------- | -------- | -------- |
  | 2       | 2.236615 | 1.322591 | 1.014462 | 1.136704 |
  | 4       | 1.257018 | 1.182883 | 1.076484 | 1.078774 |
  | 8       | 2.477985 | 2.616179 | 1.000869 | 0.867564 |

  Tabla Eficiencia:

  | comm_sz | 512      | 1024     | 2048     | 4096     |
  | ------- | -------- | -------- | -------- | -------- |
  | 2       | 1.118307 | 0.661295 | 0.507231 | 0.568352 |
  | 4       | 0.314254 | 0.295721 | 0.269121 | 0.269694 |
  | 8       | 0.309748 | 0.327022 | 0.125109 | 0.108445 |

  Respecto a la escalabilidad ...

  Respecto al overhead se puede ver en la tabla de eficiencia que ...


**3.** Se obtuvieron los siguientes resultados:

Tabla Speedup:

  | comm_sz | 512      | 1024     | 2048     | 4096     |
  | ------- | -------- | -------- | -------- | -------- |
  | 2       | 0.911392 | 1.628235 | 2.319242 | 3.825989 |
  | 4       | 0.782427 | 1.974080 | 2.021285 | 2.867554 |
  | 8       | 1.176155 | 2.266790 | 3.286544 | 3.835581 |

  Tabla Eficiencia:

  | comm_sz | 512      | 1024     | 2048     | 4096     |
  | ------- | -------- | -------- | -------- | -------- |
  | 2       | 0.455696 | 0.814118 | 1.159621 | 1.912995 |
  | 4       | 0.195607 | 0.493520 | 0.505321 | 0.716889 |
  | 8       | 0.147019 | 0.283349 | 0.410818 | 0.479448 |

  Respecto a la escalabilidad ...

  Respecto al overhead se puede ver en la tabla de eficiencia que ...


**4.** Se obtuvieron los siguientes resultados:

Tabla Speedup:

  | comm_sz | 512 | 1024 | 2048 | 4096 |
  | ------- | --- | ---- | ---- | ---- |
  | 2       |     |      |      |      |
  | 4       |     |      |      |      |
  | 8       |     |      |      |      |

  Tabla Eficiencia:

  | comm_sz | 512 | 1024 | 2048 | 4096 |
  | ------- | --- | ---- | ---- | ---- |
  | 2       |     |      |      |      |
  | 4       |     |      |      |      |
  | 8       |     |      |      |      |

  Respecto a la escalabilidad ...

  Respecto al overhead se puede ver en la tabla de eficiencia que ...