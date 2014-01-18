#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <time.h>
#include <string.h>

#define NMAX 22 //define el tope de celdas de las matrices info y matriz
#define LMAX 10 //define el máximo de letras en las palabras
#define PMAX 9 //define el máximo de palabras admitidas
enum TipoOpcion {crear=1, cargar, salir};

//Imprime el menú del juego
enum TipoOpcion menu(){
    int opcion;

    do{
	system("clear");
	printf("\t\tMenu \n"
		"\t 1.Crear sopa\n"
		"\t 2.Cargar sopa\n"
		"\t 3.Salir \n"
		"\t\t Tú opción: "
	      );
	__fpurge(stdin);//evita colapso, si se inserta una letra en vez de un número.
	scanf(" %i", &opcion);
    }while(opcion<1 || opcion>3);

    return (enum TipoOpcion) opcion;
}//fin menu();

//tamaño de la sopa
int longitud_sopa(){
    int longitud;
    do{
	system("clear");
	printf("Seleccione el tamaño 10, 15 o 20: ");
	scanf(" %i", &longitud);
	__fpurge(stdin);
    }while(longitud !=10 && longitud !=15 && longitud !=20);
    return longitud + 2;//se suma 2 a longitud porque se insertara un marco.
}//fin longitud_sopa();


//inicializa la sopa de letras y la informativa.
void crear_sopa(int longitud, char matriz[NMAX][NMAX], char info[NMAX][NMAX]){
    for(int y=0; y<longitud; y++)
	for(int x=0; x<longitud; x++){
	    if(x==0 || x==longitud-1 || y==0 || y==longitud-1){
		matriz[y][x] = '*';
		info[y][x] = '*';
	    }
	    else{
		matriz[y][x] = '0';
		info [y][x] = '0';
	    }
	}

}//fin crear_sopa();

//calcula la cantidad de palabras que introducir en la sopa.
int cantidad_palabras(int longitud){
    int palabras;
    longitud -= 2;//se resta el más 2 a la longitud.
    /*Cantidad de palabras que se van a pedir*/
    if(longitud == 10)
	palabras = 5;
    else
	if(longitud == 15)
	    palabras = 7;
	else
	    palabras = 9;
    return palabras;
}//fin conatidad_palabras();

//guarda las palabras introducidas por el usuario.
void guardar_palabras(int longitud, int palabras, char letra[PMAX][LMAX], int cuenta[9]){

    for(int n=0; n<palabras; n++){
	cuenta [n] = 0;//se inicializa la variable cuenta.
	//pide las palabras para introducirlas en la sopa.
	do{
	    printf("Introduzca palabra nº%i: ", n+1);
	    scanf(" %s", letra[n]);
	    cuenta[n] = strlen(letra[n]);//se guarada en la variable cuenta la cantidad de caracteres introducidos en cada palabra.
	}while(cuenta[n] >LMAX);//Bucle que impide el desbordamiento de caracteres en las palabras.
    }

}//fin guardar_palabras();


//introducir las palabras en la sopa de forma aleatoria.
void introducir_palabras(int palabras, char letra[PMAX][LMAX], char matriz[NMAX][NMAX], char info[NMAX][NMAX], int cuenta[9], int size, int direccion[PMAX]){

    int punto_y, punto_x,
	longitud = size - 2;

    bool ya_hay_palabra = false;
    for(int palabra=0; palabra<palabras; palabra++){
	//generar una posición y dirección aleatoria para introducir las palabras en la sopa.
	do{
	    ya_hay_palabra = false;
	    punto_y = rand() % longitud;//generar posicion aleatoria en las filas
	    punto_x = rand() % longitud;//generar posicion aleatoria en las columnas
	    direccion[palabra] = rand() % 2;//generar dirección aleatoria, ahora se compone de 0 horizontal y 1 vertical.

	    //comprueba que no hay ya una palabra en la sopa en el lugar donde se insertara esta
	    //comprueba que no comience la palabra fuera de la sopa y que no se desborde de la misma
	    switch(direccion[palabra]){
		//comprobacion horizontal
		case 0:
		    for(int prueba=0; prueba<cuenta[palabra]; prueba++)
			if(info [punto_y][punto_x + prueba] == '1' || matriz[punto_y][punto_x + prueba] == '*')
			    ya_hay_palabra = true;
		    break;
		    //comprobacion vertical
		case 1:
		    for(int prueba=0; prueba<cuenta[palabra]; prueba++)
			if(info [punto_y + prueba][punto_x] == '1' || matriz[punto_y + prueba][punto_x] == '*')
			    ya_hay_palabra = true;
		    break;
	    }

	    //si se puede introducir la palabra se continua, si no otra vez se repetira todo el proceso
	}while(ya_hay_palabra == true);
	//introducir palabra en la sopa
	switch(direccion[palabra]){
	    //introduce de forma horizontal
	    case 0:
		for(int x=0; x<cuenta[palabra]; x++){
		    matriz[punto_y][punto_x + x] = letra[palabra][x];
		    info  [punto_y][punto_x + x] = '1';
		}
		break;
		//introduce de forma vertical
	    case 1:
		for(int x=0; x<cuenta[palabra]; x++){
		    matriz[punto_y + x][punto_x] = letra[palabra][x];
		    info  [punto_y + x][punto_x] = '1';
		}
		break;
	}//fin switch
    }//fin for que de las palabras
}//fin introducir_palabras();


//rellenar la sopa de letras aleatorias.
void rellenar_sopa(char matriz[NMAX][NMAX], int longitud){
    for(int y=0; y<longitud; y++)
	for(int x=0; x<longitud; x++)
	    if( matriz[y][x] == '0' )//comprueba que hay un cero y no una palabra.
		matriz[y][x] = (char) (rand() % ('z' - 'a') + 'a');//genera un numero aleatorio y lo cambia por una letra.
}//fin rellenar_sopa();

//cargar una sopa.
int cargar_sopa(char matriz[NMAX][NMAX], char info[NMAX][NMAX], int size, int palabras, char letra[PMAX][LMAX], 
	int palabras_acertadas, int cuenta[PMAX], int direccion[PMAX]){

    FILE *fichero;
    FILE *fichero2;
    FILE *fichero3;
    FILE *fichero4;
    FILE *fichero5;
    FILE *fichero6;
    //comprueba el tamaño de la sopa elegida y cargara los archivos del tamaño de dicha sopa.
    if(size == 12){
	fichero = fopen(".sopa10.txt", "r");
	fichero2 = fopen(".info10.txt", "r");
	fichero3 = fopen(".aciertos10.txt", "r");
	fichero4 = fopen(".palabras10.txt", "r");
	fichero5 = fopen(".longitud_palabras10.txt", "r");
	fichero6 = fopen(".direccion10.txt", "r");
    }
    else
	if(size == 17){
	    fichero = fopen(".sopa15.txt", "r");
	    fichero2 = fopen(".info15.txt", "r");
	    fichero3 = fopen(".aciertos15.txt", "r");
	    fichero4 = fopen(".palabras15.txt", "r");
	    fichero5 = fopen(".longitud_palabras15.txt", "r");
	    fichero6 = fopen(".direccion15.txt", "r");
	}
	else
	    if(size == 22){
		fichero = fopen (".sopa20.txt", "r");
		fichero2 = fopen(".info20.txt", "r");
		fichero3 = fopen(".aciertos20.txt", "r");
		fichero4 = fopen(".palabras20.txt", "r");
		fichero5 = fopen(".longitud_palabras20.txt", "r");
		fichero6 = fopen(".direccion20.txt", "r");
	    }
    //guarda la informacion de los ficheros en sus correspondientes variables.
    fscanf(fichero3, " %i", &palabras_acertadas);

    for(int y=0; y<size; y++)
	for(int x=0; x<size; x++){
	    fscanf(fichero, " %c", &matriz[y][x]);
	    fscanf(fichero2, " %c", &info[y][x]);
	}

    for(int y=0; y<palabras; y++)
	fscanf(fichero5, " %i", &cuenta[y]);

    for(int y=0; y<palabras; y++)
	fscanf(fichero4, " %s", letra[y]);

    for(int y=0; y<palabras; y++)
	fscanf(fichero6, " %i", &direccion[y]);

    //cierra los flujos
    fclose(fichero);
    fclose(fichero2);
    fclose(fichero3);
    fclose(fichero4);
    fclose(fichero5);
    fclose(fichero6);

    return palabras_acertadas;//devuelve un entero, que es la cantidad de palabras acertadas que llevaba el usuario.
}//cargar_sopa();

//Guardar la sopa.
void guardar_sopa(char matriz[NMAX][NMAX], char info[NMAX][NMAX], int size, int palabras_acertadas, char letra[PMAX][LMAX], int cuenta[PMAX], int palabras, int  direccion[PMAX]){
    FILE *fichero;
    FILE *fichero2;
    FILE *fichero3;
    FILE *fichero4;
    FILE *fichero5;
    FILE *fichero6;

    //comprueba el tamaño de la sopa elegida y guarda la informacion en los archivos del tamaño de dicha sopa.
    if(size == 12){
	fichero = fopen(".sopa10.txt", "w");
	fichero2 = fopen(".info10.txt", "w");
	fichero3 = fopen(".aciertos10.txt", "w");
	fichero4 = fopen(".palabras10.txt", "w");
	fichero5 = fopen(".longitud_palabras10.txt", "w");
	fichero6 = fopen(".direccion10.txt", "w");
    }
    else
	if(size == 17){
	    fichero = fopen(".sopa15.txt", "w");
	    fichero2 = fopen(".info15.txt", "w");
	    fichero3 = fopen(".aciertos15.txt", "w");
	    fichero4 = fopen(".palabras15.txt", "w");
	    fichero5 = fopen(".longitud_palabras15.txt", "w");
	    fichero6 = fopen(".direccion15.txt", "w");
	}
	else
	    if(size == 22){
		fichero = fopen (".sopa20.txt", "w");
		fichero2 = fopen(".info20.txt", "w");
		fichero3 = fopen(".aciertos20.txt", "w");
		fichero4 = fopen(".palabras20.txt", "w");
		fichero5 = fopen(".longitud_palabras20.txt", "w");
		fichero6 = fopen(".direccion20.txt", "w");
	    }
    //se guardan en los archivos la informacion de las variables
    fprintf(fichero3, "%i", palabras_acertadas);
    //en los bucles se guarda con saltos de linea, si quieres comprobar abriendo los archivos que todo esta correcto,
    //de forma mas sencilla.
    for(int y=0; y<size; y++){
	for(int x=0; x<size; x++){
	    fprintf(fichero, "%c ", matriz[y][x]);
	    fprintf(fichero2, "%c", info[y][x]);
	}
	fprintf(fichero, "\n");
	fprintf(fichero2, "\n");
    }

    for(int y=0; y<palabras; y++){
	for(int x=0; x<cuenta[y]; x++)
	    fprintf(fichero4, "%c", letra[y][x]);
	fprintf(fichero4, "\n");
    }

    for(int y=0; y<palabras; y++){
	fprintf(fichero5, "%i", cuenta[y]);
	fprintf(fichero5, "\n");
    }

    for(int y=0; y<palabras; y++){
	fprintf(fichero6, "%i", direccion[y]);
	fprintf(fichero6, "\n");
    }
    //cierra los flujos
    fclose(fichero);
    fclose(fichero2);
    fclose(fichero3);
    fclose(fichero4);
    fclose(fichero5);
    fclose(fichero6);

}//guardar_sopa();

//imprime la sopa por pantalla.
void imprimir_sopa(int longitud, char matriz[NMAX][NMAX], char letra[PMAX][LMAX], int palabras, int palabras_acertadas){
    system("clear");//limpia la terminal
    //imprime la sopa
    for(int y=0; y<longitud; y++){
	for(int x=0; x<longitud; x++)
	    printf("%c ", matriz[y][x]);
	printf("\n");
    }
    //imprime las palabras escogidas por el usuario y la candidad de aciertos que lleva.
    for(int numero_palabra = 0; numero_palabra<palabras; numero_palabra++)
	printf("%s ", letra[numero_palabra]);
    printf("\n");
    printf("Llevas %i palabras acertadas de %i \n", palabras_acertadas, palabras);
}//fin impirmir_sopa();

/*MEJORAR TIENE BUGS*/
//jugar a la sopa
int jugar(char matriz[NMAX][NMAX], char info[NMAX][NMAX], int palabras, char letra[PMAX][LMAX], int cuenta[PMAX], int size, int aciertos_acumulados, int direccion[PMAX]){
    int  coordenada1 = 0, coordenada2 = 0, letras_ok = 0, palabras_acertadas = 0;
    char respuesta[LMAX];
    bool palabras_iguales = false;
    char salir_guardar[10];

    //pedir las coordenadas de la palabra, primero fila y luego columna.
    do{
	printf("Coordenada de filas: ");
	scanf(" %c", salir_guardar);
	//se establece unas respuestas para poder guardar la sopa y poder salir del juego
	//guarda la sopa actual
	if(salir_guardar[0] == 'G')
	    guardar_sopa(matriz, info, size, aciertos_acumulados, letra, cuenta, palabras, direccion);
    }while(salir_guardar[0] == 'G');//si se guarda la sopa se vuelve a imprimir la peticion de la primera coordenada
    //sale del programa
    if(salir_guardar[0] == 'S')
	exit(EXIT_SUCCESS);
    //
    coordenada1 = atoi(salir_guardar);
    __fpurge(stdin);

    printf("Coordenada de columnas: ");
    scanf(" %i", &coordenada2);
    __fpurge(stdin);

    //comprobar si las coordenadas son correctas
    //Compara si hay un 1 en la sopa info en las coordenadas introducidas.

    if(info[coordenada1][coordenada2] == '1'){
	palabras_iguales = false;
	//Si estan bien pide la palabra completa.
	printf("Palabra: ");
	scanf(" %s", respuesta);
	//va comprobando todas las letras de todas las palabras a ver si coincide con la respuesta.
	for(int y=0; y<palabras; y++){
	    letras_ok = 0;
	    for(int x=0; x<cuenta[y]; x++)
		if( respuesta[x] == letra[y][x] ){
		    //se suma letras_ok cada vez que una letra es correcta, se reinicia a 0 por cada palabra incorrecta.
		    letras_ok += 1;
		    //comprueba que la cantidad de letras correctas sea igual a la cantidad de la palabra.
		    if(letras_ok == cuenta[y]){

			//especifica que se ha encontrado una palabra
			palabras_iguales = true;
			palabras_acertadas += 1;
			//cambia los 1 por 2 para que no se pueda repetir la direccion./*BUG*/
			switch(direccion[y]){
			    case 0:
				for(int x=0; x<cuenta[y]; x++)
				    info [coordenada1][coordenada2 + x] = '2';
				break;
			    case 1:
				for(int x=0; x<cuenta[y]; x++)
				    info [coordenada1 + x][coordenada2] = '2';
				break;

			}//fin switch info
			//finaliza el bucle de palabras
			y = palabras;
		    }//fin if comparar letras_ok
		}//fin if comprobar palabras
	}//fin for y
    }//fin if comprobar coordenadas

    return palabras_acertadas;//devuelve un entero entre 1 y 0, 1 si se ha acertado la palabra y 0 si fallo
}//fin jugar();

int main(int argc, char *argv[]){
    int size, palabras, cuenta[PMAX], palabras_acertadas = 0, direccion[PMAX];
    char sopa[NMAX][NMAX], info[NMAX][NMAX], opcion, letra[PMAX][LMAX];
    //pone la semilla
    srand(time(NULL));

    do{
	switch(menu()){
	    case crear:
		size = longitud_sopa();
		crear_sopa(size, sopa, info);
		palabras = cantidad_palabras(size);
		guardar_palabras(size, palabras, letra, cuenta);	
		introducir_palabras(palabras, letra, sopa, info, cuenta, size, direccion);
		rellenar_sopa(sopa, size);
		do{
		    imprimir_sopa(size, sopa, letra, palabras, palabras_acertadas);
		    palabras_acertadas += jugar(sopa, info, palabras, letra, cuenta, size, palabras_acertadas, direccion);
		}while( palabras_acertadas < palabras);
		system("clear");
		printf("Ganaste!!!\n");
		break;
	    case cargar:
		size = longitud_sopa();
		palabras = cantidad_palabras(size);
		palabras_acertadas = cargar_sopa(sopa, info, size, palabras, letra, palabras_acertadas, cuenta, direccion);
		do{
		    imprimir_sopa(size, sopa, letra, palabras, palabras_acertadas);
		    palabras_acertadas += jugar(sopa, info, palabras, letra, cuenta, size, palabras_acertadas, direccion);
		}while( palabras_acertadas < palabras);
		system("clear");
		printf("Ganaste!!!\n");
		break;
	    case salir:
		return EXIT_SUCCESS;
		break;
	}//switch
	printf("¿Volver al menu? s/n ");
	scanf(" %c", &opcion);
    }while(opcion != 'n');
    return EXIT_SUCCESS;
}//fin main();
