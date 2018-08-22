Instrucciones de compilación y ejecución

Para compilación se debe navegar al directorio que contiene el código fuente y ejecutar el siguiente comando por consola:

$ make

Para ejecución se debe inicializar los procesos maestro y, de manera opcional, vista, aportando como parámetro los paths con los que se desea trabajar, de la siguiente manera:

$ ./Binaries/hash “path”
$ ./Binaries/view

O también se puede correr utilizando un pipe:

$ ./Binaries/hash “path” | ./Binaries/view

Para ejecutar los tests del programa, en donde se verifica la creación correcta de pipes, el agregado de archivos a los mismos y el cálculo de los hashes md5, se utiliza un flag y ningún parámetro:

$ ./Binaries/hash -t
