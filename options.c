//
// Created by josel on 24/08/2025.
//

#include "options.h"

char* createAgenda() {
    char nameArchive[100];
    FILE *archive;

    printf("Ingresa el nombre del archivo (sin extension): ");
    scanf("%99s", nameArchive);

    // Reservamos memoria dinamica para la ruta
    char *route = malloc(strlen(nameArchive) + 5);
    if (route == NULL) {
        printf("Error de memoria.\n");
        exit(1);
    }

    sprintf(route, "%s.txt", nameArchive);

    archive = fopen(route, "r");
    if (archive == NULL) {
        printf("Error al crear el archivo.\n");
        free(route);
        exit(1);
    }
    fclose(archive);

    return route;
}

char* openAgenda() {
    char nameArchive[100];
    FILE *archive;

    printf("Ingresa el nombre del archivo (sin extension): ");
    scanf("%99s", nameArchive);

    // Reservamos memoria dinamica para la ruta
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

void addContact(const char *route) {
    Contact c;
    FILE *file;

    // Pedir datos
    printf("\n=== Agregar nuevo contacto ===\n");
    printf("Nombre: ");
    fgets(c.name, sizeof(c.name), stdin);
    c.name[strcspn(c.name, "\n")] = '\0';

    printf("Telefono: ");
    fgets(c.phone, sizeof(c.phone), stdin);
    c.phone[strcspn(c.phone, "\n")] = '\0';

    printf("Email: ");
    fgets(c.email, sizeof(c.email), stdin);
    c.email[strcspn(c.email, "\n")] = '\0';

    // Abrir archivo en modo append
    file = fopen(route, "a");
    if (file == NULL) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    // Guardar en formato CSV-like
    fprintf(file, "%s;%s;%s\n", c.name, c.phone, c.email);

    fclose(file);
    printf("Contacto agregado correctamente\n");
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
