#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "list.h"
#include "treemap.h"
//acceso de las librerias
typedef struct { // se define la estructura que se usaran en los videojuegos
    char nombre[30];
    char fecha[30];
    int valoracion;
    int precio;
} Videojuego;

//Funcion para leer el k-esimo elemento de un string (separado por comas)
char *get_csv_field(char *linea, int indice){
    char *campo = (char *) malloc(100 * sizeof(char *)); // Guarda el string a retornar
    int i = 0; // Recorre la linea
    int k = 0; // Cuenta las comas
    int n = 0; // Recorre el campo
    bool coma = false;
    while(linea[i] != '\0'){
        if(linea[i] == '\"'){
            coma = !coma;
        }
        if(k == indice){
            if(linea[i] != '\"'){
                campo[n] = linea[i];
                n++;
            }
        }
        i++;
        if(linea[i] == ',' && !coma){
            k++;
            i++;
        }
        if(k > indice || linea[i] == '\0' || linea[i] == '\n'){
            campo[n] = '\0';
            return campo;
        }
    }
    return NULL;
}

//son los prototipos de las funciones que se utilizaran
void menu();
int lower_than_int(void*,void*);
bool estaRepetido(HashMap *, Videojuego *);
void agregarVideojuego(TreeMap *, void *clave, Videojuego *);
void mostrar(TreeMap *mapa, Pair *pair, Pair *(avanzar)(TreeMap *mapa));
void mostrarJuegosAnio(TreeMap *mapaValoraciones, int anio, int max);
int obtenerAno(char *fecha);
Videojuego* introducirdatos();

int main(){ // nuestra funcion principal (main)
    int num;
    bool flag = true;
    char nombre_archivo[30];
    Videojuego* videogame = (Videojuego*)malloc(sizeof(Videojuego));
    TreeMap* precio = createTreeMap(lower_than_int);
    TreeMap* valoracion = createTreeMap(lower_than_int);
    HashMap* mapaNombres = createMap(55);

    menu();
    scanf("%d",&num);
    
    // INICIO DE los comandos
    while(num<0||num>7){
        printf("Introdusca un numero valido: ");
        scanf("%d", &num);
    }
    while (flag){
        switch (num){
            
            case 0:{ //al ingresar un numero cero se da por finalizado el programa
                printf("Programa finalizado");
                flag=false;
                break;
            }
	        
            case 1:{ //opcion 1 importar archivos
                printf("Ingrese el nombre del arhivo: ");
                fflush(stdin);
                scanf("%29[^\n]s", nombre_archivo);
                int k=0;
                FILE *fp = fopen (nombre_archivo,"r");
                if (fp==NULL){
                    printf("Error al cargar el archivo");
                    return 0;
                }
                // Cadena para guardar la linea completa del archivo csv
                char linea[1024];
                fgets (linea, 1023, fp);
                while (fgets (linea, 1023, fp) != NULL){ // Se lee la linea 

                    strcpy(videogame->nombre, get_csv_field(linea, 0));
                    strcpy(videogame->fecha, get_csv_field(linea, 1)); 
                    videogame->valoracion = atoi(get_csv_field(linea, 2));
                    videogame->precio = atoi(get_csv_field(linea, 3));
                    
                    if (!estaRepetido(mapaNombres, videogame)) { // aca nos aseguramos que NO este repetido
                        insertMap(mapaNombres, videogame->nombre, videogame);
                        agregarVideojuego(precio, &(videogame->precio), videogame);
                        agregarVideojuego(valoracion, &(videogame->valoracion), videogame);
                    }
                    
                    videogame = (Videojuego*)malloc(sizeof(Videojuego));

                    k++; 
                    if(k==51) break;
                }
                break;
            }
            case 2:{ // opcion 2 agregar un juego
                Videojuego* videogame = (Videojuego*)malloc(sizeof(Videojuego));

                videogame = introducirdatos();

                if (!estaRepetido(mapaNombres, videogame)){ //para que NO este repetido
                    insertMap(mapaNombres, videogame->nombre, videogame);
                    agregarVideojuego(precio, &(videogame->precio), videogame);
                    agregarVideojuego(valoracion, &(videogame->valoracion), videogame);
                }

                break;
            }
            case 3:{ // opcion 3 mostrar juegos por precio
                int opcion;
                printf("1.Mayor a Menor\n");
                printf("2.Menor a Mayor\n");
                printf("Introduzca opcion: ");
                scanf("%d",&opcion);
                  while(opcion<1||opcion>2){ 
                    printf("Introduzca una opcion valida: ");
                    scanf("%d", &opcion);
                }
                printf("\n");
                if(opcion == 1) { // Menor a Mayor
                    mostrar(precio, lastTreeMap(precio), prevTreeMap);
                } // se empieza por el ultimo del arbol el menor y con prev se subira hasta llegar al first
                else { //opcion 2 Mayor a Menor
                    mostrar(precio, firstTreeMap(precio), nextTreeMap);
                }// se empieza por el first que es el mayor y se usara next para llegar al ultimo osea el menor
                break;
            }
            case 4:{ // opcion 4 filtrar juegos por valoracion
                int valor;
                printf("Introduzca valoracion: ");
                scanf("%d",&valor);
                mostrar(valoracion, upperBound(valoracion, &valor), nextTreeMap);
                break;
            }
            case 5:{ // opcion 5 mostrar juegos del anio
                int anio;
                printf("Introduzca anio: ");
                scanf("%d",&anio);
                mostrarJuegosAnio(valoracion, anio, 5); // ese 5 es porque se mostrara los cinco mejores juegos
                break;
            }
            case 6:{ // opcion 6 Buscar juego
                //definir variables
                char ing_nombre[30];
                int opcion;
                Pair_map* pair = (Pair_map*)malloc(sizeof(Pair_map));
                Pair* pair_treev = (Pair*)malloc(sizeof(Pair));
                Pair* pair_treep = (Pair*)malloc(sizeof(Pair));
                Videojuego* aux = (Videojuego*)malloc(sizeof(Videojuego));
                Videojuego* aux2 = (Videojuego*)malloc(sizeof(Videojuego));

                printf("Ingrese nombre del juego: ");
                fflush(stdin);
                scanf("%[^\n]", ing_nombre);
                pair = searchMap(mapaNombres, ing_nombre);
                if (pair == NULL){
                    printf("Juego no encontrado");
                    break;
                }
                else{
                    Videojuego* original = pair->value;
                    printf("%s / %s / %d / %d\n", original->nombre, original->fecha, original->valoracion, original->precio);
                    printf("Ingrese 1) para odificar o 2) para eliminar: ");
                    fflush(stdin);
                    scanf("%d", &opcion);
                    while(opcion<1||opcion>2){
                        printf("Introduzca una opcion valida: ");
                        scanf("%d", &opcion);
                    }
                    if (opcion == 1){  // si se escoge la primera opcion para modificar el videojuego
                        Videojuego* actualizado = (Videojuego*)malloc(sizeof(Videojuego));

                        actualizado = introducirdatos();

                        eraseMap(mapaNombres, ing_nombre);

                        pair_treev = searchTreeMap(valoracion, &original->valoracion);
                        aux = firstList(pair_treev->value);
                        while(aux != NULL){
                             if(strcmp(aux->nombre, original->nombre) == 0){
                                popCurrent(pair_treev->value);
                             }
                             aux = nextList(pair_treev->value);
                        }

                        pair_treep = searchTreeMap(precio, &original->precio);
                        aux2 = firstList(pair_treep->value);
                        while(aux2 != NULL){
                             if(strcmp(aux2->nombre, original->nombre) == 0){
                                popCurrent(pair_treep->value);
                             }
                             aux2 = nextList(pair_treep->value);
                        }
                        
                        if (!estaRepetido(mapaNombres, actualizado)){
                            insertMap(mapaNombres, actualizado->nombre, actualizado);
                            agregarVideojuego(precio, &(actualizado->precio), actualizado);
                            agregarVideojuego(valoracion, &(actualizado->valoracion), actualizado);
                        }
                        break;
                    }
                    else{ //  es la opcion 2 donde se eliminara el videojuego
                        eraseMap(mapaNombres, ing_nombre);

                        pair_treev = searchTreeMap(valoracion, &original->valoracion);
                        aux = firstList(pair_treev->value);
                        while(aux != NULL){
                             if(strcmp(aux->nombre, original->nombre) == 0){
                                popCurrent(pair_treev->value);
                             }
                             aux = nextList(pair_treev->value);
                        }

                        pair_treep = searchTreeMap(precio, &original->precio);
                        aux2 = firstList(pair_treep->value);
                        while(aux2 != NULL){
                             if(strcmp(aux2->nombre, original->nombre) == 0){
                                popCurrent(pair_treep->value);
                             }
                             aux2 = nextList(pair_treep->value);
                        }
                        break;
                    }
                }
            }
            case 7:{ // opcion 7 exportar datos
                char nombre_archivo2[30];
                strcpy(nombre_archivo2, "vj2.csv");
                FILE *fp = fopen (nombre_archivo2, "w");
                fprintf(fp,"Nombre,a??o de salida,valoracion,precio\n");
                Pair *pair = firstTreeMap(precio);
                while(pair) {
                    List *lista = pair->value;
                    Videojuego *videojuego = firstList(lista);
                    while(videojuego) {
                        fprintf(fp,"%s,%s,%d,%d\n",videojuego->nombre, videojuego->fecha, videojuego->valoracion, videojuego->precio);
                        videojuego = nextList(lista);
                    }
                    pair = nextTreeMap(precio);
                
                }
                fclose(fp);
                break;
            }
        }
        if (num==0){
            break;
        }
        printf("\n");
        printf("Ingrese otro valor: ");
        scanf("%d", &num);
        while(num<0||num>7){
            printf("Introdusca un numero valido: ");
            scanf("%d", &num);
        }
    }
return 0;
}

void menu(){ //funcion para poder mostrar el menu por pantalla
    printf("1. Importar archivo CSV\n");
    printf("2. Agregar Juego\n");
    printf("3. Mostrar Juegos por precio\n");
    printf("4. Filtrar Juegos por valoraci??n\n");
    printf("5. Mostrar Juegos del a??o\n");
    printf("6. Buscar Juego\n");
    printf("7. Exportar archivo CSV\n");
    printf("0. Salir\n");
    printf("Introdusca un numero: ");
}

int lower_than_int(void* key1, void* key2){
    int k1 = *((int*) (key1));
    int k2 = *((int*) (key2));
    return k1<k2;
}

bool estaRepetido(HashMap *mapaNombres, Videojuego *videojuego) { // funcion para comprobar nombres repetidos
    Pair_map *pair = searchMap(mapaNombres, videojuego->nombre);
    if(pair) return true;
    return false;
}

void agregarVideojuego(TreeMap *mapa, void *clave, Videojuego *videojuego) { //funcion para agregar videojuegos
    Pair *pair = searchTreeMap(mapa, clave);
    List *listaVideojuegos = NULL;

    // No existe la clave en el mapa
    if (pair == NULL) {
        listaVideojuegos = createList();
        insertTreeMap(mapa, clave, listaVideojuegos);
    }
    // Si existe 
    else {
        listaVideojuegos = pair->value;
    }
    pushBack(listaVideojuegos, videojuego);
}

void mostrar(TreeMap *mapa, Pair *primero, Pair* (avanzar)(TreeMap *mapa)) {
    Pair *pair = primero;
    while(pair) {
        List *lista = pair->value;
        Videojuego *videojuego = firstList(lista);
        while(videojuego) {
            printf("%s / %s / %d / %d\n", videojuego->nombre, videojuego->fecha, videojuego->valoracion, videojuego->precio);
            videojuego = nextList(lista);
        }
        pair = avanzar(mapa);
    }
}

int obtenerAnio(char* fecha) { // funcion para poder sacar el anio en el formato dd/mm/aaaa
    return atoi(fecha + 6);
}

void mostrarJuegosAnio(TreeMap *mapaValoraciones, int anio, int max) { // funcion capaz de imprimir los 5 mejores videojuegos
    Pair *pair = lastTreeMap(mapaValoraciones);
    int cont = 0;
    while(pair) {
        List *listaValoracion = pair->value;
        Videojuego *videojuego = firstList(listaValoracion);
        while(videojuego) {
            if(obtenerAnio(videojuego->fecha) == anio) {
                printf("%s / %s / %d / %d\n", videojuego->nombre, videojuego->fecha, videojuego->valoracion, videojuego->precio);
                cont++;
            }
            if(cont == max) return;
            videojuego = nextList(listaValoracion);
        }
        pair = prevTreeMap(mapaValoraciones);
    }
}

Videojuego* introducirdatos(){ //funcion para introducir/definir la estructura de datos e ingresar sus datos correspondientes
    char ing_nombre[30];
    char ing_fecha[30];
    int ing_valoracion;
    int ing_precio;

    Videojuego* videogame = (Videojuego*)malloc(sizeof(Videojuego));
                
    printf("Ingrese nombre: ");
    fflush(stdin);
    scanf("%[^\n]", ing_nombre);

    printf("Ingrese fecha: ");
    fflush(stdin);
    scanf("%[^\n]", ing_fecha);

    printf("Ingrese valoracion: ");
    fflush(stdin);
    scanf("%d", &ing_valoracion);

    printf("Ingrese precio: ");
    fflush(stdin);
    scanf("%d", &ing_precio);

    strcpy(videogame->nombre, ing_nombre);
    strcpy(videogame->fecha, ing_fecha); 
    videogame->valoracion = ing_valoracion;
    videogame->precio = ing_precio;

    return videogame;
}