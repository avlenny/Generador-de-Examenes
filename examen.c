#include "examen.h"

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Lista lista;
    lista.inicio = NULL;
    lista.fin = NULL;
    lista.total = 0;
    menu(&lista);
    liberarLista(&lista);
    return 0;
}

void menu(Lista *lista){
    int opc = 0;
    FILE *arch = NULL;
    while(opc != 5){
        system("cls");
        printf("╔══════════════════════════════════════════════╗\n");
        printf("║                 📚 EXÁMENES                  ║\n");
        printf("╠══════════════════════════════════════════════╣\n");
        printf("║  1. 📝 Generar examen                        ║\n");
        printf("║  2. ✏️ Modificar examen                      ║\n");
        printf("║  3. 📄 Mostrar archivo                       ║\n");
        printf("║  4. 🎓 Aplicar examen                        ║\n");
        printf("║  5. 🚪 Salir                                 ║\n");
        printf("╚══════════════════════════════════════════════╝\n");
        printf("\n► Elige una opción: ");
        scanf("%d", &opc);
        while(getchar() != '\n');
        switch(opc){
            case 1: generarExamen(arch, lista); break;
            case 2:
                mostrarCargarExam(lista);
                modificarExamen(lista);
                break;
            case 3:
                mostrarCargarExam(lista);
                mostrarExamen(lista);
                break;
            case 4:
                mostrarCargarExam(lista);
                aplicarExamen(arch,lista);
                break;
            default:
                printf("Opción no válida, intenta de nuevo.\n");
        }
    }
}

/* Lee la siguiente línea con contenido, saltando líneas vacías.
   Retorna 1 si leyó algo, 0 si llegó al EOF. */
static int leerLineaUtil(char *buf, int sz, FILE *f){
    while(fgets(buf, sz, f)){
        buf[strcspn(buf, "\r\n")] = '\0';
        if(strlen(buf) > 0) return 1;
    }
    return 0;
}

/* Lee una línea de texto carácter por carácter usando _getch(),
   evitando el buffer de stdin que queda sucio tras _getch() de las flechas.
   Muestra eco manual de cada carácter. Soporta Backspace. */
static void leerTexto(char *buf, int max){
    int i = 0;
    int c;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    memset(buf, 0, max);
    while(i < max - 1){
        c = _getch();
        if(c == 13){       
            printf("\n");
            break;
        } else if(c == 8){   
            if(i > 0){
                i--;
                buf[i] = '\0';
                printf("\b \b"); 
            }
        } else if(c == 0 || c == 224){
            _getch();
        } else if(c >= 32 && c < 127){ 
            buf[i] = (char)c;
            putchar(c);
            i++;
        }
    }
    buf[i] = '\0';
}

void generarExamen(FILE *arch, Lista *lista){
    system("cls");
    Examen exam;
    int cp = 0;
    char resp = 's';

    printf("Dame el nombre para el archivo: ");
    scanf("%s", exam.nombreArch);
    while(getchar() != '\n');

    strcat(exam.nombreArch, ".txt");
    arch = fopen(exam.nombreArch, "a");
    if(arch == NULL){
        printf("Error...el archivo no se pudo abrir\n");
        return;
    }

    while(resp != 'n' && resp != 'N'){
        cp++;
        printf("Pregunta %d:\n", cp);

        fgets(exam.pregunta, MAX_PREGUNTA, stdin);
        exam.pregunta[strcspn(exam.pregunta, "\r\n")] = '\0';

        printf("Opcion a) ");
        fgets(exam.opc1, MAX_OPCION, stdin);
        exam.opc1[strcspn(exam.opc1, "\r\n")] = '\0';

        printf("Opcion b) ");
        fgets(exam.opc2, MAX_OPCION, stdin);
        exam.opc2[strcspn(exam.opc2, "\r\n")] = '\0';

        printf("Opcion c) ");
        fgets(exam.opc3, MAX_OPCION, stdin);
        exam.opc3[strcspn(exam.opc3, "\r\n")] = '\0';

        printf("Opcion d) ");
        fgets(exam.opc4, MAX_OPCION, stdin);
        exam.opc4[strcspn(exam.opc4, "\r\n")] = '\0';

        printf("Respuesta correcta a/b/c/d: ");
        scanf(" %c", &exam.resp);

        printf("Puntos: ");
        scanf("%d", &exam.puntos);
        while(getchar() != '\n');

        system("cls");
        fprintf(arch, ":p;%s\n:op1;%s\n:op2;%s\n:op3;%s\n:op4;%s\n:r;%c\n%d\n",
                exam.pregunta,
                exam.opc1, exam.opc2, exam.opc3, exam.opc4,
                exam.resp, exam.puntos);

        insertarNodo(lista, exam);
        printf("Pregunta %d se guardo correctamente en %s\n", cp, exam.nombreArch);
        printf("Quieres añadir otra pregunta s/n? ");
        scanf(" %c", &resp);
        while(getchar() != '\n');
        system("cls");
    }

    fclose(arch);
    printf("¡¡Examen guardado!!\n");
}

void mostrarReactivo(struct Nodo *nodo, int num, int total){
    if(nodo == NULL){
        printf("Aún no hay datos en la lista...\n");
        return;
    }
    Examen *exam = &nodo->exam;
    printf("══════════════════════════════════════════════════════\n");
    printf("             REACTIVO %d / %d\n", num, total);
    printf("══════════════════════════════════════════════════════\n");
    printf("❖ Pregunta:\n%s\n\n", exam->pregunta);
    printf("   a) %s\n", exam->opc1);
    printf("   b) %s\n", exam->opc2);
    printf("   c) %s\n", exam->opc3);
    printf("   d) %s\n", exam->opc4);
    printf("\n──────────────────────────────────────────────────────\n");
    printf("✔ Respuesta correcta: %c\n", exam->resp);
    printf("★ Puntos: %d\n", exam->puntos);
    printf("══════════════════════════════════════════════════════\n");
}

void editarReactivo(Nodo *nodo){
    system("cls");
    int opc;
    char tmpNum[16];
    printf("¿Qué quieres editar del reactivo?\n");
    printf("  1. Pregunta\n");
    printf("  2. Opción a)\n");
    printf("  3. Opción b)\n");
    printf("  4. Opción c)\n");
    printf("  5. Opción d)\n");
    printf("  6. Respuesta correcta\n");
    printf("  7. Puntos\n");
    printf("► ");
    leerTexto(tmpNum, sizeof(tmpNum));
    opc = atoi(tmpNum);
    switch(opc){
        case 1:
            printf("Nueva pregunta:\n");
            leerTexto(nodo->exam.pregunta, MAX_PREGUNTA);
            break;
        case 2:
            printf("Nueva a) ");
            leerTexto(nodo->exam.opc1, MAX_OPCION);
            break;
        case 3:
            printf("Nueva b) ");
            leerTexto(nodo->exam.opc2, MAX_OPCION);
            break;
        case 4:
            printf("Nueva c) ");
            leerTexto(nodo->exam.opc3, MAX_OPCION);
            break;
        case 5:
            printf("Nueva d) ");
            leerTexto(nodo->exam.opc4, MAX_OPCION);
            break;
        case 6:
            printf("Nueva respuesta (a/b/c/d): ");
            leerTexto(tmpNum, sizeof(tmpNum));
            if(tmpNum[0] >= 'a' && tmpNum[0] <= 'd')
                nodo->exam.resp = tmpNum[0];
            else
                printf("Respuesta no válida, no se cambió.\n");
            break;
        case 7:
            printf("Puntos nuevos: ");
            leerTexto(tmpNum, sizeof(tmpNum));
            nodo->exam.puntos = atoi(tmpNum);
            break;
        default:
            printf("Opción no válida.\n");
    }
    printf("\n✔ Se realizo el cambio. Presiona cualquier tecla...\n");
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
}

void mostrarCargarExam(Lista *lista){
    Examen exam;
    FILE *arch;
    char nomArch[MAX_ARCH];
    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile("*.txt", &fd);
    if(hFind == INVALID_HANDLE_VALUE){
        printf("No se encontraron archivos .txt\n");
        return;
    }
    printf("Archivos disponibles:\n");
    printf("------------------------\n");
    int i = 1;
    do{
        printf("%d. %s\n", i++, fd.cFileName);
    }while(FindNextFile(hFind, &fd));
    FindClose(hFind);
    printf("\nEscribe el nombre del archivo (sin .txt): ");
    scanf("%s", nomArch);
    while(getchar() != '\n');
    strcat(nomArch, ".txt");
    arch = fopen(nomArch, "r");
    if(arch == NULL){
        printf("No se pudo abrir el archivo...\n");
        return;
    }
    liberarLista(lista);
    char linea[MAX_PREGUNTA];
    while(leerLineaUtil(linea, sizeof(linea), arch)){
        if(strncmp(linea, ":p;", 3) != 0) continue;  /* saltar líneas que no sean :p; */
        memset(&exam, 0, sizeof(Examen));
        strcpy(exam.nombreArch, nomArch);
        strcpy(exam.pregunta, linea + 3);
        if(!leerLineaUtil(linea, sizeof(linea), arch)) break;
        if(strncmp(linea, ":op1;", 5) == 0) strcpy(exam.opc1, linea + 5);
        if(!leerLineaUtil(linea, sizeof(linea), arch)) break;
        if(strncmp(linea, ":op2;", 5) == 0) strcpy(exam.opc2, linea + 5);
        if(!leerLineaUtil(linea, sizeof(linea), arch)) break;
        if(strncmp(linea, ":op3;", 5) == 0) strcpy(exam.opc3, linea + 5);
        if(!leerLineaUtil(linea, sizeof(linea), arch)) break;
        if(strncmp(linea, ":op4;", 5) == 0) strcpy(exam.opc4, linea + 5);
        if(!leerLineaUtil(linea, sizeof(linea), arch)) break;
        if(strncmp(linea, ":r;", 3) == 0) exam.resp = linea[3];
        if(!leerLineaUtil(linea, sizeof(linea), arch)) break;
        exam.puntos = atoi(linea);  
        insertarNodo(lista, exam);
    }
    fclose(arch);
    printf("Se cargaron %d reactivos de '%s'\n", lista->total, nomArch);
}

void modificarExamen(Lista *lista){
    FILE *archTemp;
    int pos, opc, cambio = 0;
    if(lista->inicio == NULL){
        printf("No hay nada guardado aún...\n");
        return;
    }
    Nodo *act = lista->inicio;
    pos = 1;
    do{
        system("cls");
        mostrarReactivo(act, pos, lista->total);
        printf("\n[<--] Anterior  [-->] Siguiente  [E] Editar  [S] Salir\n");
        opc = leerTecla();
        switch(opc){
            case 'I':
                if(act->anter != NULL){ act = act->anter; pos--; }
                break;
            case 'D':
                if(act->sig != NULL){ act = act->sig; pos++; }
                break;
            case 'e': case 'E':
                editarReactivo(act);
                cambio++;
                break;
        }
    }while(opc != 's' && opc != 'S');
    if(cambio > 0 && lista->inicio != NULL){
        char nombreArch[MAX_ARCH];
        strcpy(nombreArch, lista->inicio->exam.nombreArch);
        archTemp = fopen("temporal.txt", "w");
        if(archTemp == NULL){
            printf("Error al abrir el archivo...\n");
            return;
        }
        act = lista->inicio;
        while(act != NULL){
            Examen exam = act->exam;
            fprintf(archTemp, ":p;%s\n:op1;%s\n:op2;%s\n:op3;%s\n:op4;%s\n:r;%c\n%d\n",
                    exam.pregunta,
                    exam.opc1, exam.opc2, exam.opc3, exam.opc4,
                    exam.resp, exam.puntos);
            act = act->sig;
        }
        fclose(archTemp);
        remove(nombreArch);
        rename("temporal.txt", nombreArch);
        printf("Cambios guardados correctamente.\n");
    }
}

int leerTecla(){
    int c = _getch();
    if(c == 224){
        c = _getch();
        switch(c){
            case 75: return 'I';
            case 77: return 'D';
        }
    }
    return c;
}

void mostrarExamen(Lista *lista){
    int pos, opc;
    if(lista->inicio == NULL){
        printf("No hay nada guardado aún...\n");
        return;
    }
    Nodo *act = lista->inicio;
    pos = 1;
    do{
        system("cls");
        mostrarReactivo(act, pos, lista->total);
        printf("\n[<--] Anterior  [-->] Siguiente  [S] Salir\n");
        opc = leerTecla();
        switch(opc){
            case 'I':
                if(act->anter != NULL){ act = act->anter; pos--; }
                break;
            case 'D':
                if(act->sig != NULL){ act = act->sig; pos++; }
                break;
        }
    }while(opc != 's' && opc != 'S');
}

void aplicarExamen(FILE *arch, Lista *lista){
    (void)arch;
    if(lista->inicio == NULL){
        printf("No hay reactivos cargados.\n");
        _getch();
        return;
    }
    // Arreglo paralelo de respuestas del alumno (0 = sin responder)
    char *respuestas = (char *)calloc(lista->total, sizeof(char));
    if(respuestas == NULL){
        printf("Error al reservar memoria.\n");
        return;
    }
    Nodo *act = lista->inicio;
    int   pos = 1;
    int   opc;
    do{
        system("cls");

        // Mostrar reactivo SIN respuesta correcta ni puntos
        Examen *exam = &act->exam;
        printf("══════════════════════════════════════════════════════\n");
        printf("             REACTIVO %d / %d\n", pos, lista->total);
        printf("══════════════════════════════════════════════════════\n");
        printf("❖ Pregunta:\n%s\n\n", exam->pregunta);
        printf("   a) %s\n", exam->opc1);
        printf("   b) %s\n", exam->opc2);
        printf("   c) %s\n", exam->opc3);
        printf("   d) %s\n", exam->opc4);
        printf("──────────────────────────────────────────────────────\n");
        if(respuestas[pos - 1] != 0)
            printf("✏️  Tu respuesta: %c\n", respuestas[pos - 1]);
        else
            printf("✏️  Sin responder\n");
        printf("══════════════════════════════════════════════════════\n");
        printf("[<--] Anterior  [-->] Siguiente  [a/b/c/d] Responder  [F] Finalizar\n");
        opc = leerTecla();
        switch(opc){
            case 'I':   // flecha izquierda
                if(act->anter != NULL){ act = act->anter; pos--; }
                break;
            case 'D':   // flecha derecha
                if(act->sig != NULL){ act = act->sig; pos++; }
                break;
            case 'a': case 'A': respuestas[pos - 1] = 'a'; break;
            case 'b': case 'B': respuestas[pos - 1] = 'b'; break;
            case 'c': case 'C': respuestas[pos - 1] = 'c'; break;
            case 'd': respuestas[pos - 1] = 'd'; break;
        }
    } while(opc != 'f' && opc != 'F');
    system("cls");
    int puntosObtenidos = 0;
    int puntosTotal= 0;
    int respondidas     = 0;

    Nodo *temp = lista->inicio;
    for(int i = 0; i < lista->total; i++){
        puntosTotal += temp->exam.puntos;
        if(respuestas[i] != 0){
            respondidas++;
            if(respuestas[i] == temp->exam.resp)
                puntosObtenidos += temp->exam.puntos;
        }
        temp = temp->sig;
    }

    printf("╔══════════════════════════════════════════════╗\n");
    printf("║           RESULTADOS DEL EXAMEN              ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  Preguntas respondidas : %2d / %-2d             ║\n", respondidas, lista->total);
    printf("║  Puntos obtenidos      : %2d / %-2d             ║\n", puntosObtenidos, puntosTotal);
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\nPresiona cualquier tecla para volver al menú...\n");
    _getch();

    free(respuestas);
}

Nodo *crearNodo(Examen exam){
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->exam = exam;
    nuevo->anter = NULL;
    nuevo->sig = NULL;
    return nuevo;
}

void insertarNodo(Lista *lista, Examen exam){
    Nodo *nuevo = crearNodo(exam);
    if(lista->inicio == NULL){
        lista->inicio = nuevo;
        lista->fin = nuevo;
    }
    else{
        nuevo->anter = lista->fin;
        lista->fin->sig = nuevo;
        lista->fin = nuevo;
    }
    lista->total++;
}

void liberarLista(Lista *lista){
    Nodo *act = lista->inicio, *temp;
    while(act != NULL){
        temp = act->sig;
        free(act);
        act = temp;
    }
    lista->inicio = NULL;
    lista->fin = NULL;
    lista->total = 0;
}

Nodo *siguienteNodo(Lista *lista, Nodo *actual){
    if(lista->inicio == NULL || actual == NULL) return NULL;
    if(actual->sig == NULL) return lista->inicio;
    return actual->sig;
}

Nodo *anteriorNodo(Lista *lista, Nodo *actual){
    if(lista->inicio == NULL || actual == NULL) return NULL;
    if(actual->anter == NULL) return lista->fin;
    return actual->anter;
}

Nodo *navegarReactivos(Lista *lista, char *teclaFin){
    if(lista->inicio == NULL){
        printf("No hay reactivos cargados.\n");
        *teclaFin = 's';
        return NULL;
    }
    Nodo *actual = lista->inicio;
    int   pos    = 1;
    int   tecla;
    system("cls");
    mostrarReactivo(actual, pos, lista->total);
    printf("\n[<--] Anterior   [-->] Siguiente   [E] Editar   [S] Salir\n");
    while(1){
        if(!kbhit()) continue;
        tecla = getch();
        if(tecla == 0 || tecla == 224){
            tecla = getch();
            if(tecla == 77){
                actual = siguienteNodo(lista, actual);
                pos = (pos < lista->total) ? pos + 1 : 1;
            } else if(tecla == 75){
                actual = anteriorNodo(lista, actual);
                pos = (pos > 1) ? pos - 1 : lista->total;
            } else {
                continue;
            }
            system("cls");
            mostrarReactivo(actual, pos, lista->total);
            printf("\n[<--] Anterior   [-->] Siguiente   [E] Editar   [S] Salir\n");
        } else {
            char c = (char)tecla;
            if(c == 'S' || c == 's'){ *teclaFin = 's'; return actual; }
            if(c == 'E' || c == 'e'){ *teclaFin = 'e'; return actual; }
        }
    }
}