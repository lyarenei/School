// error.c
// Řešení IJC-DU1, příklad b), 10.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Funkce pro výpis chybových hlášek

#include <stdio.h>
#include <stdlib.h>             // exit()
#include <stdarg.h>             // vfprintf(), va_*

// Vypíše chybový text na stderr
void warning_msg(const char *fmt, ...)
{
    va_list argumenty;
    va_start (argumenty, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, argumenty);
    va_end (argumenty);
}

// Vypíše chybový text na stderr a ukončí program
void error_msg(const char *fmt, ...)
{
    va_list argumenty;
    va_start (argumenty, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, argumenty);
    va_end (argumenty);
    exit(1);
}