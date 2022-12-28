# CC5408-Tarea_01
Este repositorio es parte de una tarea realizada en el curso de Diseño y Análisis de Algoritmos (CC4102), Universidad de Chile.

Esta tarea demuestra la efectividad de distintas implementaciones del algoritmo que resuelve el problema de [calcular la distancia de edición de dos strings](https://es.wikipedia.org/wiki/Distancia_de_Levenshtein). El objetivo es comparar los distintos tiempos de ejecución cuando se hace más uso de la memoria RAM (heap) o de la memoria caché (principalmente stack):

 - La primera implementación usa mayormente memoria RAM al utilizar una matriz para ir guardando los valores intermedios del algoritmo
 - La segunda hace más eficiente el uso de la memoria al considerar sólo dos filas de valores (en lugar de una matriz completa)
 - La tercera además considera la eficiencia de considerar un tamaño de bloque de memoria (arbitrario) para dividir la matriz en submatrices
 
 El programa se corre ejecutando `main.cpp`, donde mediante una consola interactiva, se debe ingresar el número de tests, el largo de los strings (generados aleatoriamente) y el tamaño del bloque requerido (para la tercera implementación).
 
 Los resultados se guardan en un archivo csv llamado `time_results.csv`. Se puede graficar corriendo el programa `plot_usage.py`, sin argumentos. Requiere de un intérprete de Python3.x y de las librerías listadas en `requirements.txt`.
