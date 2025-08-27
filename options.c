//
// Created by josel on 24/08/2025.
//

#include "options.h"

char* createAgenda() {
    char nameArchive[100];
    int fd;

    printf("Ingresa el nombre del archivo (sin extension): ");
    scanf("%99s", nameArchive);

    // Reservamos memoria dinámica para la ruta
    char *route = malloc(strlen(nameArchive) + 5);
    if (route == NULL) {
        printf("Error de memoria.\n");
        exit(1);
    }

    sprintf(route, "%s.txt", nameArchive);

    // Usamos open en lugar de fopen
    fd = open(route, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error al crear el archivo");
        free(route);
        exit(1);
    }

    // Cerramos el archivo con close en lugar de fclose
    close(fd);

    return route;
}

char* openAgenda() {
    char nameArchive[100];
    int fd;

    printf("Ingresa el nombre del archivo (sin extension): ");
    scanf("%99s", nameArchive);

    // Reservamos memoria dinámica para la ruta
    char *route = malloc(strlen(nameArchive) + 5);
    if (route == NULL) {
        printf("Error de memoria.\n");
        return NULL;
    }

    sprintf(route, "%s.txt", nameArchive);

    // Intentamos abrir en modo solo lectura
    fd = open(route, O_RDONLY);
    if (fd == -1) {
        printf("Error: El archivo '%s' no existe.\n", route);
        free(route);
        return NULL;
    }

    // Cerramos porque solo era prueba de existencia
    close(fd);

    return route; // devolvemos la ruta
}

void addContact(const char *route) {
    Contact c;
    FILE *file;

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

    // Se abre archivo en modo append
    file = fopen(route, "a");
    if (file == NULL) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    // Se guarda en formato CSV-like
    fprintf(file, "%s;%s;%s\n", c.name, c.phone, c.email);

    fclose(file);
    printf("Contacto agregado correctamente\n");
}

void viewContacts(const char *route) {
    int fd = open(route, O_RDONLY);
    if (fd == -1) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    char line[200];
    int pos = 0;
    int count = 0;

    printf("\n=== Lista de contactos ===\n");

    // Leer el archivo en bloques
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n' || pos >= (int)sizeof(line) - 1) {
                line[pos] = '\0'; // cerramos la cadena
                pos = 0;

                if (strlen(line) == 0) continue; // saltar vacías

                Contact c;
                char *token = strtok(line, ";");
                if (token) strncpy(c.name, token, sizeof(c.name));

                token = strtok(NULL, ";");
                if (token) strncpy(c.phone, token, sizeof(c.phone));

                token = strtok(NULL, ";");
                if (token) strncpy(c.email, token, sizeof(c.email));

                printf("\nContacto %d\n", ++count);
                printf("Nombre   : %s\n", c.name);
                printf("Telefono : %s\n", c.phone);
                printf("Email    : %s\n", c.email);
            } else {
                line[pos++] = buffer[i];
            }
        }
    }

    if (count == 0) {
        printf("\n(No hay contactos en la agenda)\n");
    }

    close(fd);
}

void searchContact(const char *route) {
    int fd = open(route, O_RDONLY);
    if (fd == -1) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    char query[50];
    printf("\n=== Buscar contacto ===\n");
    printf("Ingrese nombre o telefono a buscar: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0'; // quitar salto de línea

    char buffer[1024];
    char line[200];
    int pos = 0;
    int found = 0;
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n' || pos >= (int)sizeof(line) - 1) {
                line[pos] = '\0'; // cerramos la cadena
                pos = 0;

                if (strlen(line) == 0) continue;

                Contact c;
                char *token = strtok(line, ";");
                if (token) strncpy(c.name, token, sizeof(c.name));

                token = strtok(NULL, ";");
                if (token) strncpy(c.phone, token, sizeof(c.phone));

                token = strtok(NULL, ";");
                if (token) strncpy(c.email, token, sizeof(c.email));

                if (strstr(c.name, query) || strstr(c.phone, query)) {
                    printf("\n=== Contacto encontrado ===\n");
                    printf("Nombre   : %s\n", c.name);
                    printf("Telefono : %s\n", c.phone);
                    printf("Email    : %s\n", c.email);
                    found = 1;
                }
            } else {
                line[pos++] = buffer[i];
            }
        }
    }

    if (!found) {
        printf("\nNo se encontraron contactos con \"%s\"\n", query);
    }

    close(fd);
}

void editContact(const char *route) {
    int fd = open(route, O_RDONLY);
    if (fd == -1) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    // Crear archivo temporal
    int tempFd = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (tempFd == -1) {
        printf("Error: no se pudo crear archivo temporal\n");
        close(fd);
        return;
    }

    char query[50];
    printf("\n=== Editar contacto ===\n");
    printf("Ingrese nombre o telefono del contacto a editar: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    char buffer[1024];
    char line[200];
    int pos = 0;
    ssize_t bytesRead;
    int found = 0;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n' || pos >= (int)sizeof(line)-1) {
                line[pos] = '\0';
                pos = 0;

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

                // Guardar contacto (editado o no) en temp.txt usando write
                char writeLine[200];
                int n = snprintf(writeLine, sizeof(writeLine), "%s;%s;%s\n", c.name, c.phone, c.email);
                if (write(tempFd, writeLine, n) != n) {
                    perror("Error escribiendo en archivo temporal");
                }

            } else {
                line[pos++] = buffer[i];
            }
        }
    }

    close(fd);
    close(tempFd);

    // Reemplazar archivo original
    if (remove(route) != 0) {
        perror("Error eliminando archivo original");
    }
    if (rename("temp.txt", route) != 0) {
        perror("Error renombrando archivo temporal");
    }

    if (found) {
        printf("\nContacto editado correctamente.\n");
    } else {
        printf("\nNo se encontr%c contacto con \"%s\".\n", 162, query);
    }
}

void deleteContact(const char *route) {
    int fd = open(route, O_RDONLY);
    if (fd == -1) {
        printf("Error: no se pudo abrir el archivo %s\n", route);
        return;
    }

    int tempFd = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (tempFd == -1) {
        printf("Error: no se pudo crear archivo temporal\n");
        close(fd);
        return;
    }

    char query[50];
    printf("\n=== Eliminar contacto ===\n");
    printf("Ingrese nombre o tel%cfono del contacto a eliminar: ", 130);
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    char buffer[1024];
    char line[200];
    int pos = 0;
    ssize_t bytesRead;
    int found = 0;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n' || pos >= (int)sizeof(line)-1) {
                line[pos] = '\0';
                pos = 0;

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
                    continue; // no escribir en temp.txt
                }

                // Guardar contacto en temp.txt
                char writeLine[200];
                int n = snprintf(writeLine, sizeof(writeLine), "%s;%s;%s\n", c.name, c.phone, c.email);
                if (write(tempFd, writeLine, n) != n) {
                    perror("Error escribiendo en archivo temporal");
                }

            } else {
                line[pos++] = buffer[i];
            }
        }
    }

    close(fd);
    close(tempFd);

    // Reemplazar archivo original
    if (remove(route) != 0) perror("Error eliminando archivo original");
    if (rename("temp.txt", route) != 0) perror("Error renombrando archivo temporal");

    if (!found) {
        printf("\nNo se encontr%c contacto con \"%s\".\n", 162, query);
    }
}

void countContacts(const char *route) {
    int fd = open(route, O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        return;
    }

    char buffer[1024];
    int count = 0;
    ssize_t bytesRead;
    long fileSize = 0;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        fileSize += bytesRead;

        // Contar saltos de línea
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n') {
                count++;
            }
        }
    }

    if (bytesRead == -1) {
        perror("Error al leer el archivo");
    }

    close(fd);

    printf("\nTotal de contactos en la agenda: %d\n", count);
    printf("Tamano del archivo: %ld bytes\n", fileSize);
}

void resetContacts(const char *route) {
    // Abrir archivo en modo escritura y truncar su contenido
    int fd = open(route, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("Error: no se pudo reiniciar la agenda");
        return;
    }

    close(fd);

    printf("\nAgenda reiniciada correctamente. Cerrando programa.\n");
    exit(0);
}