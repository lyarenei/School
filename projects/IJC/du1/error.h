// error.h
// Řešení IJC-DU1, příklad b), 10.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Funkce pro výpis chybových hlášek

#ifndef ERROR_H
#define ERROR_H

// Vypíše chybový text na stderr
void warning_msg(const char *fmt, ...);

// Vypíše chybový text na stderr a ukončí program
void error_msg(const char *fmt, ...);

#endif /* ERROR_H */