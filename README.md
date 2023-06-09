# Compilador para Grafos usando Flex/Bison

Un compilador donde un usuario puede especificar un tipo de grafo, aplicarle operaciones/algoritmos, para luego poder visualizarlo en una imagen generada como salida.

## Requerimientos

Para construir el compilador, se requieren las siguientes dependencias:

- [Bison v3.8.2](https://www.gnu.org/software/bison/)
- [CMake v3.24.1](https://cmake.org/)
- [Flex v2.6.4](https://github.com/westes/flex)
- [GCC v11.1.0](https://gcc.gnu.org/)
- [Make v4.3](https://www.gnu.org/software/make/)
- [Python v3.11](https://www.python.org/downloads/release/python-3114/)
- [Pipenv v2023.6.18](https://pypi.org/project/pipenv/)
- [NetworkX v3.1](https://pypi.org/project/networkx/)
- [Matplotlib v3.7](https://matplotlib.org/3.7.1/index.html)

Se debe correr

```bash
user@machine:path/ $ pipenv shell
user@machine:path/ $ pipenv install
```

en un terminal de la carpeta para instalar las dependencias vinculadas a Python, después de haber instalado pipenv y python en la correcta versión.

Si en lugar de trabajar con un entorno _Linux_, se está construyendo el proyecto sobre un entorno _Microsoft Windows_, se debe instalar _Microsoft Visual Studio 2022_ con las extensiones para desarrollar aplicaciones en _C/C++_, así como también las herramientas requeridas, con excepción del compilador _GCC_ y la herramienta _Make_.

## Sintaxis del lenguaje

Un programa consta de varios bloques de operaciones o declaraciones de grafos, donde un programa debe **SIEMPRE** empezar por una declaración de un Grafo.

### Declaración de variables

Primero vemos los tipos de grafos existentes, donde un usuario puede declarar un grafo vacío, o alguno de un tipo predefinido (como un Kconexo o _Complete_). Estos tipos pueden ser:

```c
Graph simple        /* Grafo simple, sin contenido inicial */

Cycle B:            /* Grafo ciclo, con sus nodos iniciales*/
    nodes a, b, c, d, e

Wheel Cgraph:       /* Grafo rueda, con su centro y nodos externos */
    center a
    nodes bb, cc, dd

Star anotherGraph:  /* Grafo estrella, con su centro y nodos externos */
    center aNode
    nodes bNode, cNode, dNode, eNode

Complete kConnected:  /* Grafo completo, con sus nodos */
    nodes alfredo, beto, cecilia, duran, eduardo

BipartiteComplete Knm:  /* Grafo Kn,m, con sus nodos separados por grupo */
    group auto, colectivo, moto, micro
    group avión, zeppelin
```

Por otro lado, como se puede ver, los nombres de nodos y de grafos son strings y pueden representar lo que el usuario desee, mientras que las aristas tienen sus pesos enteros como se verá luego.

### Palabras reservadas

Estos nombres tienen el limitante de que, actualmente, **SÓLO** pueden ser compuestos por letras y números, sin guiones o demás caracteres especiales, además que dichos nombres no deben coincidir con las _palabras reservadas_ del lenguaje destinadas a otros comandos. Algunas son:

```c
colors
to
mst
```

Luego hay otras como _nodes_ o _Graph_ que podrían ser usadas como nombres de variables, siempre y cuando no sean seguidas por un espacio, donde el compilador puede detectar dicha acción como el inicio de una lista de nodos o declaración correspondiente.

### Bloques add y remove

Luego, para operar con los grafos creados tenemos los antes mencionados bloques, que en la versión actual del lenguaje son 3: **add _(add to graph)_**, **remove _(remove from graph)_** y **apply _(apply to graph)_**.

Primero nos concentramos en **add** y **remove**, que tienen un comportamiento similar, donde en cada uno podemos agregar o quitar nodos o aristas de un grafo, que son pasados como listas de nodos o aristas, listados de la siguiente forma:

```c
Graph test              /* Código de ejemplo */

add to test:
    nodes a, b, c, d
    edges a-b, a-3-a

remove from test:
    nodes b
```

Como se puede ver, es posible declarar aristas con o sin su peso, donde en este segundo caso se defaultearía a 1.

Luego, el borrado de nodos tendría en cuenta sus aristas, por lo que la acción realizada en el código de ejemplo también eliminaría las demás aristas del grafo.

### Bloque apply

Luego tenemos el bloque **apply**, donde se pueden aplicarle distintos algoritmos al grafo generado y visualizarlos en un archivo de salida de nombre especificado. Dicha redirección se indica con el caracter '>', donde el nombre de archivo actualmente tiene la misma restricción de las variables, al sólo poder ser compuesto por letras.

Un ejemplo de este bloque, con todas las operaciones posibles, es:

```c
...

apply to A:
    bfs from c to d > hola          /* BFS */
    dfs from d to c > archivoA      /* DFS */
    colors:
        #ff0000 e, d		        /* Es lo mismo que red e, d */
        #00ff00 c, f		        /* Es lo mismo que green c, f */
    find cut nodes                  /* Mostrar nodos de corte */
    delete cut nodes > adios        /* Borrar nodos de corte. No necesariamente se debe haber ejecutado find fut nodes antes */
    mst                             /* Árbol recubridor mínimo*/

...
```

Como se puede ver no es obligatorio que el resultado de cada algoritmo derive en un archivo de salida.

Por último, cabe destacar que, a pesar de que el primer 'bloque' siempre debe ser una declaración, los siguientes bloques pueden ir en cualquier orden, teniendo en cuenta que funcionalmente puede que el programa sea incorrecto, mas no aún sintácticamente.

### Bloque output

Por último, tenemos un bloque con una única operación, la de _output_ o redirección, que hace que se genere una imagen con el estado actual del grafo teniendo en cuenta las operaciones previamente aplicadas. El nombre del archivo es un parámetro opcional, donde si no se lo indica, el archivo tendrá el nombre del grafo.

```c
Cycle BB:
    nodes a, b, c, d, e

BB >                /* El archivo de salida será el ciclo, y se llamará BB.png */

apply to BB:
    colors:
        #ff0000 a, b, c

BB > coloredBB      /* El archivo de salida tendrá a, b y c coloreados, y se llamará BB.png */

```

Esto puede ser muy útil cuando simplemente se desea ver un grafo luego de quitarle algún nodo o arista, o hasta luego de colorearlo. De todas maneras, se debe tener cuidado, ya que las imágenes se sobreescriben si existía alguna anterior, siendo la restante la última generada con dicho nombre durante la ejecución el programa.

## Construcción

Para construir el proyecto por completo, ejecute en la raíz del repositorio los siguientes comandos (en _Linux_):

```bash
user@machine:path/ $ chmod u+x --recursive script
user@machine:path/ $ script/build.sh
```

En un entorno _Microsoft Windows_, en cambio, se debe ejecutar:

```bash
user@machine:path/ $ script\build.bat
```

Luego se deberá abrir la solución generada `bin\Compiler.sln` con el IDE _Microsoft Visual Studio 2022_. Los ejecutables que este sistema construye se depositan dentro del directorio `bin\Debug` y `bin\Release`, según corresponda.

Adicionalmente, puede que sea necesario borrar archivos de caché como `bin\CMakeCache.txt` en caso de querer usar el mismo código en otra carpeta.

## Ejecución

Para compilar un programa, primero cree un archivo vacío denominado `program` (o el nombre que desee), con el siguiente contenido:

```
Graph A

add to A:
    nodes a, b, c, d, e, f, g, h
    edges a-b, a-d, b-c, b-d, b-f, c-e, c-f, d-e, e-g

A > outputAfterAdd

remove from A:
    nodes h
    edges c-f

A > outputAfterRemove

apply to A:
    bfs from a to g > bfsAtoG
    dfs from b to e > dfsBtoE
    colors:
        #ff0000 e, d
        #00ff00 c, f
    find cut nodes > cutNodesA
    delete cut nodes > deletedCutNodesA
    mst > mstA

A > outputAfterEverything
```

Luego, ejecute el compilador desde el directorio raíz del proyecto, o desde cualquier otro lugar indicando el path hacia el script `start.sh` y pasando por parámetro el path hacia el programa a compilar:

En Linux

```bash
user@machine:path/ $ script/start.sh program
```

En Windows:

```bash
user@machine:path/ $ script\start.bat program
```

Este programa de ejemplo crea un ejecutable llamado `output.py`, el cual posee las operaciones pedidas, y debe ser ejecutado para generar las imágenes finales especificadas. Para ejecutarlo, simplemente correr:

```bash
user@machine:path/ $ py output.py
```

o

```bash
user@machine:path/ $ python output.py
```

Finalmente, las imágenes pedidas serán guardadas en una carpeta generada llamada `output`, que se encontrará en el directorio raíz del proyecto.

En el caso del programa de ejemplo, algunas de las imágenes serán:

#### Grafo luego de las operaciones add y remove
![Alt text](/example_images/outputAfterRemove.png "outputAfterRemove")

#### BFS desde A hacia G
![Alt text](/example_images/bfsAtoG.png "bfsAtoG")

#### Nodos de corte en el grafo
![Alt text](/example_images/cutNodesA.png "cutNodesA")

## Testing

En Linux:

```bash
user@machine:path/ $ script/test.sh
```

En Windows:

```bash
user@machine:path/ $ script\test.bat
```

Si desea agregar nuevos casos de uso, deberá crear un archivo por cada uno, que contenga el programa a testear dentro de las carpetas `test/accept` o `test/reject` según corresponda (es decir, si el mismo debe ser aceptado o rechazado por el compilador).
