//
// Created by josel on 24/08/2025.
//

#include "menu.h"

// Limpia la pantalla (portátil entre Windows/Linux)
void clearScreen() {
    printf("\nPresione Enter para continuar...");
    getchar();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Función que muestra el menú
void showMenu() {
    printf("====== Agenda de Contactos ======\n");
    printf("1. Agregar contacto\n");
    printf("2. Mostrar todos los contactos\n");
    printf("3. Buscar contacto\n");
    printf("4. Editar contacto\n");
    printf("5. Eliminar contacto\n");
    printf("6. Contar contactos\n");
    printf("7. Reiniciar agenda\n");
    printf("8. Salir\n");
    printf("================================\n");
}

int getOption() {
    char input[100];
    int option = 0;
    int valido;

    do {
        valido = 1;
        showMenu();
        printf("Ingrese una opcion (1-8): ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error al leer entrada. Intente nuevamente.\n");
            valido = 0;
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        for (int i = 0; input[i] != '\0'; i++) {
            if (!isdigit((unsigned char)input[i])) {
                valido = 0;
                break;
            }
        }

        if (!valido || strlen(input) == 0) {
            printf("Entrada invalida. Debe ingresar un numero entero.\n\n");
            valido = 0;
            continue;
        }

        option = atoi(input);

        if (option < 1 || option > 8) {
            printf("Opcion fuera de rango. Ingrese un numero entre 1 y 8.\n\n");
            valido = 0;
        }

    } while (!valido);

    return option;
}