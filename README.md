# MicroMouse-FloodFill-Simulator
En este código hemos implementado el algoritmo de floodfill para nuestro robot de micromouse bajo la premisa de **Encontrar el camino mas rápido** en lugar de **El mas corto.**

![MicroMouse-FloodFill-Simulator](./images/Laberinto_ejemplo.jpg)
Aqui podemos ver un ejemplo sacado del video de Veritasium: https://www.youtube.com/watch?v=ZMQbHMgK2rw&t=600s&ab_channel=Veritasium Donde se ve un camino verde de 19.4m vs un camino azul mas corto de 13.9m. Sin embargo el camino verde tiene rectas mas largas y tan solo 45 giros, vs el camino azul que tiene 57 giros.


Para implementar este nuevo algoritmo nos hemos basado en este video:
https://www.youtube.com/watch?v=jIdBi7FrioE
Donde explica como implementar el floodfill mediante pesos calculados en funcion del tiempo teorico que tu robot de micromouse tardaria en llegar a la meta.

Para ello lo principal es calcular cuanto tarda tu robot en atravesar una casilla en funcion de su velocidad base, su aceleración, y su velocidad máxima. Y lo mismo para las casillas con diagonales. Asignandole una penalización por girar calculada teniendo en cuenta en cuanto tarda el robot en frenar a la velocidad base para realizar ese giro.

Teniendo todo esto en cuenta hemos generado este codigo al cual le puedes pasar un laberinto con este formato:
[14,28,4,4,4,20,20,20,20,20,20,20,20,20,20,6,8,6,10,26,26,12,20,20,20,20,20,4,6,14,14,10,10,10,24,20,6,8,20,6,12,20,6,26,10,24,16,2,10,24,6,12,18,26,14,24,18,14,24,6,8,20,6,10,10,12,18,24,6,12,0,6,12,0,6,10,26,12,18,10,10,24,20,6,24,18,26,24,18,26,24,16,22,24,6,10,10,12,6,24,4,22,12,20,20,4,6,12,20,20,18,10,10,10,24,6,24,6,10,12,6,26,24,16,20,4,22,10,10,24,6,10,12,18,10,8,18,12,6,12,20,0,22,10,10,12,18,24,16,22,24,16,20,18,24,18,28,0,22,10,10,10,12,6,12,6,14,12,6,14,12,6,28,16,6,10,10,24,18,24,18,24,16,18,24,16,18,24,6,12,18,10,8,20,6,12,6,12,20,20,6,12,20,20,18,24,6,10,8,20,2,10,24,18,12,6,24,18,12,6,12,20,18,10,8,6,10,24,20,20,18,24,20,20,18,24,18,14,12,18,26,24,16,20,20,20,20,20,20,20,20,20,20,16,16,22]

Este formato lo puedes conseguir montando tu laberinto en nuestro generador de laberintos:
https://oprobots.github.io/utils/maze-wall-placer.html
![MicroMouse-FloodFill-Simulator](./images/Generador_laberintos.png)

Una vez introducido el laberinto que quieras testear con el algoritmo este codigo te arrojaría lo siguiente:

![MicroMouse-FloodFill-Simulator](./images/Laberinto_ascii.jpg)
Este laberinto muestra los pesos de cada casilla basado en tiempo. Para analizar el recorrido mas corto deberas empezar desde la meta e ir avanzando eligiendo la casilla con menor valor hasta la meta.
Y como podras comprobar. El algoritmo prefiere ir por el camino mas largo, ya que en las rectas el robot alcanza altas velocidades lo que supone una notoria reduccion de tiempo total.