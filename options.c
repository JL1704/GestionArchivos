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

void viewContacts(const char *route) {
    FILE *file = fopen(route, "r");
    if (file == NULL) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    char line[200];
    int count = 0;

    printf("\n=== Lista de contactos ===\n");

    while (fgets(line, sizeof(line), file)) {
        // Elimina salto de línea al final
        line[strcspn(line, "\n")] = '\0';

        // Saltar líneas vacías o de cabecera
        if (strlen(line) == 0 || strstr(line, "Archivo creado exitosamente")) {
            continue;
        }

        Contact c;
        char *token = strtok(line, ";");
        if (token != NULL) strncpy(c.name, token, sizeof(c.name));

        token = strtok(NULL, ";");
        if (token != NULL) strncpy(c.phone, token, sizeof(c.phone));

        token = strtok(NULL, ";");
        if (token != NULL) strncpy(c.email, token, sizeof(c.email));

        printf("\nContacto %d\n", ++count);
        printf("Nombre : %s\n", c.name);
        printf("Telefono: %s\n", c.phone);
        printf("Email  : %s\n", c.email);
    }

    if (count == 0) {
        printf("\n(No hay contactos en la agenda)\n");
    }

    fclose(file);
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
