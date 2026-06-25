#ifndef EXAMEN_H
#define EXAMEN_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#define MAX_PREGUNTA 512
#define MAX_OPCION 256
#define MAX_NOMBRE 128
#define MAX_ARCH 12

typedef struct Examen {
    char nombreArch[MAX_ARCH];
    char pregunta[MAX_PREGUNTA];
    char opc1[MAX_OPCION];
    char opc2[MAX_OPCION];
    char opc3[MAX_OPCION];
    char opc4[MAX_OPCION];
    char resp; 
    int  puntos;
} Examen;

typedef struct Nodo {
    struct Examen exam;
    struct Nodo *anter;
    struct Nodo *sig;
}Nodo;

typedef struct Lista {
    struct Nodo *inicio;
    struct Nodo *fin;
    int total;
}Lista;

struct Nodo *crearNodo(Examen reactivo);
void insertarNodo(Lista *lista,Examen reactivo);
struct Nodo *eliminarNodo(Lista *lista,Nodo *nodo); 
void liberarLista(Lista *lista);

// NavegaciÃ³n (circular)
struct Nodo *siguienteNodo(Lista *lista, Nodo *actual);
struct Nodo *anteriorNodo(Lista *lista, Nodo *actual);

int  cargarExamen(FILE *archivo, Lista *lista); 
void guardarExamen(FILE *archivo, Lista *lista);
int  listarArchivos(char *rutaSalida); 

void limpiarPantalla();
void mostrarReactivo(Nodo *nodo, int numero, int total);
void editarReactivo(Nodo *nodo);
int leerTecla();

void menu();
void mostrarExamen(Lista *lista);
void mostrarCargarExam(Lista *lista);
void generarExamen(FILE *arch,Lista *lista);
void modificarExamen(Lista *lista);
void aplicarExamen(FILE *arch,Lista *lista);

#endif