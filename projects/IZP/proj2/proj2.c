/*****************************************************************
** Autor: Dominik Křivohlávek (xkrivo02)						**
** Program byl vytvořen v rámci prvního projektu v předmětu IZP	**
** Listopad 2016												**
*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

// Vypocet logaritmu pomoci Taylorova polynomu
double taylor_log(double x, unsigned int n)
{
	if(x == 0)
		return -INFINITY;

	if(x < 0)
		return NAN;

	double result = 0.0;
	double vysledek = 0.0;
	
	if(x <= 1)
	{
		double z = (1-x);			// Uprava vstupniho 'x' pro vzorec
		double coeff;				// Koeficient pro vypocet nasledujiciho clenu z predchoziho
		double seg = -z;			// Zlomek ve vzorci (segment)
		result = -z;
		for(unsigned int i = 1; i <= n; i++)
		{			// Iterace dokud nedosahneme zadaneho poctu iteraci
			coeff = (i*z)/(i+1);
			seg = seg*coeff;
			result += seg;
		}
		return result;
	}
	else
	{
		double citatel = 1;
		double lomitko = 1.0/x;
		double zavorka = x-1;

		for(unsigned int i = 1; i <= n; i++)
		{
			citatel *= (lomitko*zavorka);
			vysledek += (double)((1.0/i)*citatel);
		}
	}
	return vysledek;
}

// Vypocet logaritmu pomoci zretezenych zlomku
double cfrac_log(double x, unsigned int n)		
{
	if(x == 0)
		return -INFINITY;

	if(x < 0)
		return NAN;

	double z = (x-1)/(1+x);			// Uprava vstupniho 'x' pro vzorec
	double result = 0;
	double cf = 1;
	unsigned int denom_const = 1;			// Konstanta pro jmenovatele retezoveho zlomku
	unsigned int numer_const = 1;			// Konstanta pro citatele retezoveho zlomku

	for(unsigned int i = 1; i < n; i++)
	{			// Spocitame konstanty pro citatele a jmenovatele retezeneho zlomku podle zadaneho rozvoje 
		denom_const += 2;
		numer_const += denom_const;
	}
	for(; n; n--)
	{			// Iterujeme dokud nedosahneme pozadovaneho rozvoje
	 	cf = denom_const - ((numer_const*z*z) / cf);
		numer_const -= denom_const;
		denom_const -= 2;
	}
	result = (2*z)/cf;
	return result;
}

// Vypocet exponencialni funkce pomoci Taylorova polynomu
double taylor_pow(double x, double y, unsigned int n)
{
	double result = 1;
	double ln_x = taylor_log(x,n);
	double seg = 1;							// Zlomek ve vzroci (segment), cleny prvniho segmentu jsou 'na prvou' a deleny 1 
	double coeff;							// Koeficient pro vypocet nasledujiciho segmentu ve vzorci
	for(unsigned int i = 0; i <= n; i++)
	{			// Iterace az po zadany exponent
		coeff = (y*ln_x)/(i+1);
		seg = seg*coeff;
		result += seg;	
	}
	return result;
}

// Vypocet exponencialni funkce pomoci zretezenych zlomku
double taylorcf_pow(double x, double y, unsigned int n)
{
	double result = 1;
	double ln_x = cfrac_log(x,n);
	double seg = 1;							// Zlomek ve vzroci (segment), cleny prvniho segmentu jsou 'na prvou' a deleny 1 
	double coeff;							// Koeficient pro vypocet nasledujiciho segmentu ve vzorci
	for(unsigned int i = 0; i <= n; i++)
	{			// Iterace az po zadany exponent
		coeff = (y*ln_x)/(i+1);
		seg = seg*coeff;
		result += seg;	
	}
	return result;
}

// Vypise vypoctene logaritmy cisla
void print_log(double x, unsigned int n)
{
	printf("       log(%g) = %.12g\n", x, log(x));
	printf("    cf_log(%g) = %.12g\n", x, cfrac_log(x, n));
	printf("taylor_log(%g) = %.12g\n", x, taylor_log(x, n));
}

// Vypise vypoctena umocnena cisla
void print_pow(double x, double y, unsigned int n)
{
	printf("         pow(%g, %g) = %.12g\n", x, y, pow(x, y));
	printf("  taylor_pow(%g, %g) = %.12g\n", x, y, taylor_pow(x, y, n));
	printf("taylorcf_pow(%g, %g) = %.12g\n", x, y, taylorcf_pow(x, y, n));
}

// Parsuje predane argumenty programu
bool arg_parse(int arg_c, char** arg_v)
{
	double number = 0;
	double exponent = 0;
	double iteration = 0;
	char *err;			// Ukazatel na char (zmeni se pouze pokud je chybny vstup u strtol)

	// Kontrola argumentu
	for(int i = 1; i < arg_c; i++)
	{
		if((!strcmp(arg_v[i], "--log")) && (i < (arg_c-1)))
		{
			number = strtod(arg_v[i+1], &err);
			if((arg_v[i+1] != err) && ((i + 1) < (arg_c-1)))
			{			// Kontrola na pritomnost a spravnost zakladniho cisla
					iteration = (unsigned int)strtol(arg_v[i+2], &err, 10);
					if((arg_v[i+3] != err) && ((i+2) == (arg_c-1)) && (iteration > 0))
					{			// Kontrola pritomnosti a spravnosti tretiho argumentu a celkoveho poctu argumentu
						print_log(number, iteration);
						return 1;
					}
					else 
					{
						fprintf(stderr, "Byly zadany neocekavane argumenty!\n");
						return 0;
					}
			}
			else
			{
				fprintf(stderr, "Spatne cislo nebo exponent!\n");
				return 0;
			}
		}
		else if((!strcmp(arg_v[i], "--pow")) && (i < (arg_c-1)))
		{
			number = strtod(arg_v[i+1], &err);
			if((arg_v[i+1] != err) && ((i + 1) < (arg_c-1)))
			{			// Kontrola na pritomnost a spravnost zakladniho cisla
			 	exponent = strtod(arg_v[i+2], &err);
				if((arg_v[i+2] != err) && ((i + 2) < (arg_c-1)))
				{			// Kontrola na pritomnost a spravnost druheho argumentu
				 	iteration = (unsigned int)strtol(arg_v[i+3], &err, 10);
					if((arg_v[i+3] != err) && ((i+3) == (arg_c-1)) && (iteration > 0))
					{			// Kontrola pritomnosti a spravnosti tretiho argumentu a celkoveho poctu argumentu
						print_pow(number, exponent, iteration);
						return 1;
					}
					else 
					{
						fprintf(stderr, "Byly zadany neocekavane argumenty!\n");
						return 0;
					}
				}
				else
				{
					fprintf(stderr, "Spatny exponent nebo rozvoj!\n");
					return 0;
				}
			}
			else
			{
				fprintf(stderr, "Spatne cislo nebo exponent!\n");
				return 0;
			}
		}
		else
		{
			fprintf(stderr, "Nebyl zadan vstup!\n");
			return 0;
		}
	}
	return 0;
}


int main (int argc, char* argv[])
{
	if(arg_parse(argc, argv))
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;	
}