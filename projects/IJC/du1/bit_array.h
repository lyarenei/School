// bit_array.h
// Řešení IJC-DU1, příklad a), 10.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Makra a inline funkce pro práci s bitovým polem

#include <limits.h>             // CHAR_BIT
#include "error.h"              // chybové hlášky

#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

typedef unsigned long bit_array_t[];

#define ba_create(jmeno_pole,velikost)\
    unsigned long (jmeno_pole)[((velikost)%(CHAR_BIT*sizeof(unsigned long))) ? \
    (((velikost)/(CHAR_BIT*sizeof(unsigned long)))+2) : \
    (((velikost)/(CHAR_BIT*sizeof(unsigned long)))+1)] = {0}; (jmeno_pole)[0] = (velikost);

#ifndef USE_INLINE

// Získá hodnotu bitu v předaném poli na zadaném indexu
#define ba_get_bit(jmeno_pole,index) (\
((index) >= (jmeno_pole[0])) ? (error_msg("Index %lu mimo rozsah 0..%lu",index, (jmeno_pole[0]-1)),0) : \
\
(((jmeno_pole)[(((index)/(CHAR_BIT*sizeof(unsigned long)))+1)] >> \
((index)-(((index)/(CHAR_BIT*sizeof(unsigned long)))*(CHAR_BIT*sizeof(unsigned long))))) & 1))

// Zapíše zadanou hodnotu do pole na zadaný index
#define ba_set_bit(jmeno_pole,index,vyraz) (\
((index) >= jmeno_pole[0]) ? (error_msg("Index %lu mimo rozsah 0..%lu",index, (jmeno_pole[0]-1)),0) :  \
\
(vyraz == 0) ? \
((jmeno_pole)[(((index)/(CHAR_BIT*sizeof(unsigned long)))+1)] = ((jmeno_pole)[(((index)/(CHAR_BIT*sizeof(unsigned long)))+1)] & \
(~(1L << ((index)-(((index)/(CHAR_BIT*sizeof(unsigned long)))*(CHAR_BIT*sizeof(unsigned long)))))))) : \
\
((jmeno_pole)[(((index)/(CHAR_BIT*sizeof(unsigned long)))+1)] = ((jmeno_pole)[(((index)/(CHAR_BIT*sizeof(unsigned long)))+1)] | \
((1L << ((index)-(((index)/(CHAR_BIT*sizeof(unsigned long)))*(CHAR_BIT*sizeof(unsigned long)))))))))



// Vraci velikost pole v bitech
#define ba_size(jmeno_pole) ((jmeno_pole)[0])

#endif /* Not defined USE_INLINE */


#ifdef USE_INLINE

// Získá hodnotu bitu v předaném poli na zadaném indexu
static inline unsigned long ba_get_bit(bit_array_t jmeno_pole, unsigned long index)
{
    if((index < 0) || (index > (jmeno_pole[0]-1)))
        error_msg("Index %lu mimo rozsah 0..%lu",index, (jmeno_pole[0]-1));
    
    unsigned long i_pole = (index/(CHAR_BIT*sizeof(unsigned long)))+1;
    unsigned long bit_posun = (index-((index/(CHAR_BIT*sizeof(unsigned long)))*(CHAR_BIT*sizeof(unsigned long))));

    return ((jmeno_pole[i_pole] >> bit_posun) & 1);
}

// Zapíše zadanou hodnotu do pole na zadaný index
static inline void ba_set_bit(bit_array_t pole, unsigned long index, unsigned long vyraz)
{
    if((index < 0) || (index > (jmeno_pole[0]-1)))
        error_msg("Index %lu mimo rozsah 0..%lu",index, (jmeno_pole[0]-1));
    
    unsigned long i_pole = (index/(CHAR_BIT*sizeof(unsigned long)))+1;
    unsigned long bit_posun = (index-((index/(CHAR_BIT*sizeof(unsigned long)))*(CHAR_BIT*sizeof(unsigned long))));
    
    if(vyraz == 0)
        return (jmeno_pole[i_pole] & (~(1L << bit_posun));
    
    else
        return (jmeno_pole[i_pole] | (1L << bit_posun));
}

// Vraci velikost pole v bitech
static inline unsigned long ba_size(bit_array_t jmeno_pole)
{
    return jmeno_pole[0];
}

#endif /* Defined USE_INLINE */

#define BA_GET_BIT_(p,i)\
(((p)[(i)/(CHAR_BIT)] >> ((i)-(((i)/(CHAR_BIT))*(CHAR_BIT)))) & 1)


#define BA_SET_BIT_(p,i,b)\
((b == 0) ? \
((p)[(i)/(CHAR_BIT)] = ((p)[(i)/(CHAR_BIT)] & \
(~(1 << ((i)-(((i)/(CHAR_BIT))*(CHAR_BIT))))))) : \
\
((p)[(i)/(CHAR_BIT)] = ((p)[(i)/(CHAR_BIT)] | \
(1 << ((i)-(((i)/(CHAR_BIT))*(CHAR_BIT)))))))


#endif /* BIT_ARRAY_H */