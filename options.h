// Options.h
#ifndef GESTIONARCHIVOS_OPTIONS_H
#define GESTIONARCHIVOS_OPTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Declaraciones de funciones
typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

char* createAgenda();
char* openAgenda();
void addContact(const char *route);
void viewContacts(const char *route);
void searchContact(const char *route);
void editContact(const char *route);
void deleteContact(const char *route);
void countContacts();
void resetContacts();

#endif // GESTIONARCHIVOS_OPTIONS_H
