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

    archive = fopen(route, "w");
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

        if (strlen(line) == 0) {
            continue; // saltar líneas vacías
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

void searchContact(const char *route) {
    FILE *file = fopen(route, "r");
    if (file == NULL) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    char query[50];
    printf("\n=== Buscar contacto ===\n");
    printf("Ingrese nombre o telefono a buscar: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0'; // quitar salto de línea

    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            continue; // saltar líneas vacías
        }

        Contact c;
        char *token = strtok(line, ";");
        if (token != NULL) strncpy(c.name, token, sizeof(c.name));

        token = strtok(NULL, ";");
        if (token != NULL) strncpy(c.phone, token, sizeof(c.phone));

        token = strtok(NULL, ";");
        if (token != NULL) strncpy(c.email, token, sizeof(c.email));

        // Coincidencia por nombre o teléfono
        if (strstr(c.name, query) || strstr(c.phone, query)) {
            printf("\n=== Contacto encontrado ===\n");
            printf("Nombre : %s\n", c.name);
            printf("Telefono: %s\n", c.phone);
            printf("Email  : %s\n", c.email);
            found = 1;
        }
    }

    if (!found) {
        printf("\nNo se encontraron contactos con \"%s\"\n", query);
    }

    fclose(file);
}

void editContact(const char *route) {
    FILE *file = fopen(route, "r");
    if (!file) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    // Crear archivo temporal
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error: no se pudo crear archivo temporal\n");
        fclose(file);
        return;
    }

    char query[50];
    printf("\n=== Editar contacto ===\n");
    printf("Ingrese nombre o telefono del contacto a editar: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        Contact c = {0};
        char *token = strtok(line, ";");
        if (token) strncpy(c.name, token, sizeof(c.name)-1);
        token = strtok(NULL, ";");
        if (token) strncpy(c.phone, token, sizeof(c.phone)-1);
        token = strtok(NULL, ";");
        if (token) strncpy(c.email, token, sizeof(c.email)-1);

        if (!found && (strstr(c.name, query) || strstr(c.phone, query))) {
            found = 1;
            printf("\nContacto encontrado: %s, %s, %s\n", c.name, c.phone, c.email);

            printf("Nuevo nombre (dejar vac%co para no cambiar): ", 161);
            char newName[50];
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';
            if (strlen(newName) > 0) strncpy(c.name, newName, sizeof(c.name)-1);

            printf("Nuevo telefono (dejar vac%co para no cambiar): ", 161);
            char newPhone[20];
            fgets(newPhone, sizeof(newPhone), stdin);
            newPhone[strcspn(newPhone, "\n")] = '\0';
            if (strlen(newPhone) > 0) strncpy(c.phone, newPhone, sizeof(c.phone)-1);

            printf("Nuevo email (dejar vac%co para no cambiar): ", 161);
            char newEmail[50];
            fgets(newEmail, sizeof(newEmail), stdin);
            newEmail[strcspn(newEmail, "\n")] = '\0';
            if (strlen(newEmail) > 0) strncpy(c.email, newEmail, sizeof(c.email)-1);
        }

        // Guardar contacto (editado o no)
        fprintf(temp, "%s;%s;%s\n", c.name, c.phone, c.email);
    }

    fclose(file);
    fclose(temp);

    // Reemplazar archivo original
    remove(route);
    rename("temp.txt", route);

    if (found) {
        printf("\nContacto editado correctamente.\n");
    } else {
        printf("\nNo se encontr%c contacto con \"%s\".\n", 162, query);
    }
}

void deleteContact(const char *route) {
    FILE *file = fopen(route, "r");
    if (!file) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error: no se pudo crear archivo temporal\n");
        fclose(file);
        return;
    }

    char query[50];
    printf("\n=== Eliminar contacto ===\n");
    printf("Ingrese nombre o tel%cfono del contacto a eliminar: ", 130);
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        Contact c = {0};
        char *token = strtok(line, ";");
        if (token) strncpy(c.name, token, sizeof(c.name)-1);
        token = strtok(NULL, ";");
        if (token) strncpy(c.phone, token, sizeof(c.phone)-1);
        token = strtok(NULL, ";");
        if (token) strncpy(c.email, token, sizeof(c.email)-1);

        if (!found && (strstr(c.name, query) || strstr(c.phone, query))) {
            found = 1;
            printf("\nContacto eliminado: %s, %s, %s\n", c.name, c.phone, c.email);
            continue; // no escribir al archivo temporal
        }

        fprintf(temp, "%s;%s;%s\n", c.name, c.phone, c.email);
    }

    fclose(file);
    fclose(temp);

    remove(route);
    rename("temp.txt", route);

    if (!found) {
        printf("\nNo se encontr%c contacto con \"%s\".\n", 162, query);
    }
}

void countContacts(const char *route) {
    FILE *file = fopen(route, "r");
    if (!file) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    // Usando fseek y ftell para obtener tamaño del archivo
    fseek(file, 0, SEEK_END);      // mover al final
    long fileSize = ftell(file);   // obtener posición (tamaño en bytes)
    fseek(file, 0, SEEK_SET);      // volver al inicio

    // Contar número de contactos leyendo línea por línea
    char line[200];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) > 0) {
            count++;
        }
    }

    fclose(file);

    printf("\nTotal de contactos en la agenda: %d\n", count);
    printf("Tamano del archivo: %ld bytes\n", fileSize);
}


void resetContacts(const char *route) {
    FILE *file = fopen(route, "w"); // abre en modo escritura y borra todo el contenido
    if (file == NULL) {
        printf("Error: no se pudo reiniciar la agenda.\n");
        return;
    }
    fclose(file);

    printf("\nAgenda reiniciada correctamente. Cerranndo programa.\n");
    exit(0); // termina la ejecución del programa
}

