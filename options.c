//
// Created by josel on 24/08/2025.
//

#include "options.h"

char* createAgenda() {
    char nameArchive[100];
    FILE *archive;

    printf("Ingresa el nombre del archivo (sin extension): ");
    scanf("%99s", nameArchive);

    // Reservamos memoria dinámica para la ruta
    char *route = malloc(strlen(nameArchive) + 5);
    if (route == NULL) {
        printf("Error de memoria.\n");
        exit(1);
    }

    sprintf(route, "%s.txt", nameArchive);

    archive = fopen(route, "w");
    if (archive == NULL) {
        printf("Error al crear el archivo.\n");
        free(route);
        exit(1);
    }

    fprintf(archive, "Archivo creado exitosamente.\n");
    fclose(archive);

    return route;
}

char* openAgenda() {
    char nameArchive[100];
    FILE *archive;

    printf("Ingresa el nombre del archivo (sin extension): ");
    scanf("%99s", nameArchive);

    // Reservamos memoria dinámica para la ruta
    char *route = malloc(strlen(nameArchive) + 5);
    if (route == NULL) {
        printf("Error de memoria.\n");
        return NULL;
    }

    sprintf(route, "%s.txt", nameArchive);

    archive = fopen(route, "r");
    if (archive == NULL) {
        printf("Error: El archivo '%s' no existe.\n", route);
        free(route);
        return NULL;
    }

    fclose(archive); // cerramos porque solo era prueba de existencia
    return route;     // devolvemos la ruta
}

void addContact() {
    printf("\nFunción addContact llamada\n");
}

void viewContacts() {
    printf("\nFunción viewContacts llamada\n");
}

void searchContact() {
    printf("\nFunción searchContact llamada\n");
}

void editContact() {
    printf("\nFunción editContact llamada\n");
}

void deleteContact() {
    printf("\nFunción deleteContact llamada\n");
}

void countContacts() {
    printf("\nFunción countContacts llamada\n");
}

void resetContacts() {
    printf("\nFunción resetContacts llamada\n");
}
