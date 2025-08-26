// main.c
#include "menu.h"
#include "options.h"

int main(void) {
    int option = getInitialOption();
    char *route = NULL;

    switch (option) {
        case 1: route = createAgenda();
            break;
        case 2: route = openAgenda();
            break;
        case 3: printf("Saliendo...\n");
            exit(0);
        default: ;
    }

    if (route == NULL) {
        fprintf(stderr, "\nError: La ruta no se pudo obtener.\n");
        exit(1);
    }

    clearScreen();

    do {
        option = getOption();

        switch (option) {
            case 1: addContact(route); break;
            case 2: viewContacts(route); break;
            case 3: searchContact(); break;
            case 4: editContact(); break;
            case 5: deleteContact(); break;
            case 6: countContacts(); break;
            case 7: resetContacts(); break;
            case 8: printf("Saliendo...\n"); break;
            default: ;
        }
        clearScreen();
    } while (option != 8);

    return 0;
}