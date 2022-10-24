#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "list.h"
#include "treemap.h"

typedef struct {
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
void menu();
int lower_than_int(void*,void*);
bool estaRepetido(HashMap *, Videojuego *);
void agregarVideojuego(TreeMap *, void *clave, Videojuego *);
void mostrar(TreeMap *mapa, Pair *pair, Pair *(avanzar)(TreeMap *mapa));
void mostrarJuegosAno(TreeMap *mapaValoraciones, int ano, int max);
int obtenerAno(char *fecha);

int main(){
    int num;
    bool flag = true;
    char nombre_archivo[30];
    Videojuego* videogame = (Videojuego*)malloc(sizeof(Videojuego));
    TreeMap* precio = createTreeMap(lower_than_int);
    TreeMap* valoracion = createTreeMap(lower_than_int);
    HashMap* mapaNombres = createMap(50);

    menu();
    scanf("%d",&num);
    while(num<0||num>7){
        printf("Introdusca un numero valido: ");
        scanf("%d", &num);
    }
    while (flag){
        switch (num){
            case 0:{
                printf("Programa finalizado");
                flag=false;
                break;
            }
	        case 1:{
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
                    
                    if (!estaRepetido(mapaNombres, videogame)) {
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
            case 2:{
                break;
            }
            case 3:{
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
                if(opcion == 1) {
                    mostrar(precio, lastTreeMap(precio), prevTreeMap);
                }
                else {
                    mostrar(precio, firstTreeMap(precio), nextTreeMap);
                }
                break;
            }
            case 4:{
                int valor;
                printf("Introduzca valoracion: ");
                scanf("%d",&valor);
                mostrar(valoracion, upperBound(valoracion, &valor), nextTreeMap);
                break;
            }
            case 5:{
                int ano;
                printf("Introduzca año: ");
                scanf("%d",&ano);
                mostrarJuegosAno(valoracion, ano, 5);
                break;
            }
            case 6:{
                break;
            }
            case 7:{
                char nombre_archivo2[30];
                strcpy(nombre_archivo2, "vj2.csv");
                FILE *fp = fopen (nombre_archivo2, "w");
                fprintf(fp,"Nombre,Fecha,Valoracion,Precio\n");
                Pair* aux = firstTreeMap(precio);
                Videojuego* aux2 = aux->value;
                while(aux!=NULL){
                    aux2 = aux->value;
                    fprintf(fp,"%s,%s,%d,%d\n",aux2->nombre, aux2->fecha, aux2->valoracion, aux2->precio);
                    aux = nextTreeMap(precio);
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

void menu(){
    printf("1. Importar archivo CSV\n");
    printf("2. Agregar Juego\n");
    printf("3. Mostrar Juegos por precio\n");
    printf("4. Filtrar Juegos por valoración\n");
    printf("5. Mostrar Juegos del año\n");
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

bool estaRepetido(HashMap *mapaNombres, Videojuego *videojuego) {
    Pair_map *pair = searchMap(mapaNombres, videojuego->nombre);
    if(pair) return true;
    return false;
}

void agregarVideojuego(TreeMap *mapa, void *clave, Videojuego *videojuego) {
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

int obtenerAno(char* fecha) {
    return atoi(fecha + 6);
}

void mostrarJuegosAno(TreeMap *mapaValoraciones, int ano, int max) {
    Pair *pair = lastTreeMap(mapaValoraciones);
    int cont = 0;
    while(pair) {
        List *listaValoracion = pair->value;
        Videojuego *videojuego = firstList(listaValoracion);
        while(videojuego) {
            if(obtenerAno(videojuego->fecha) == ano) {
                printf("%s / %s / %d / %d\n", videojuego->nombre, videojuego->fecha, videojuego->valoracion, videojuego->precio);
                cont++;
            }
            if(cont == max) return;
            videojuego = nextList(listaValoracion);
        }
        pair = prevTreeMap(mapaValoraciones);
    }
}