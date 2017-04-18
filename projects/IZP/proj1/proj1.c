/*****************************************************************
** Autor: Dominik Křivohlávek (xkrivo02)						**
** Program byl vytvořen v rámci prvního projektu v předmětu IZP	**
** Říjen - Listopad 2016													**
*****************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

// Vypise napovedu
void print_help()
{
	fprintf(stderr,"Program je mozne spustit s nasledujicimi parametry:\n"
		"	-s [N]	- Preskakuje N znaku ke zpracovani, N musi byt nezaporne cislo.\n"
		"	-n [N]  - Urcuje maximalni pocet znaku ktisku, N musi byt nezaporne cislo.\n"
		"	-S [N]	- Program bude tisknout pouze takove posloupnosti v binarnim vstupu, ktere vypadaji jako textovy retezec. Kazdy retezec, jehoz delka je vetsi nebo rovna N znaku, je vytisten na jeden radek. N musi byt v rozsahu 1 - 199 vcetne.\n"
		"	-r 	- Obraci funkci programu - na vstupu ocekava hexadecimalni reprezentaci a vypisuje jejich tisknutelnou podobu.\n"
		"	-x	- Prevadi vstupni retezec na hexadecimalni hodnoty znaku\n");
}

// Cte vstup, vraci nacteny znak, v pripade EOF vraci EOF
int input()
{
	int c = getchar();
	if(c != EOF)
		return c;
	else
		return EOF;
}

// Spocita a vraci delku pole
int arr_length(char *array)
{
	int i = 0;
	for(; array[i] != '\0'; i++);
	return i;
}

// Zkontroluje, zda je pole ciselne, vraci 0 pokud pole obsahuje cisla
bool is_array_digit(char *array)
{
	for(int i = 0; array[i] != '\0'; i++)
	{
		if(!isdigit(array[i]))
			return 1;			// Pole obsahuje alepon jeden neciselny znak
	}
	return 0;			// Pole obsahuje ciselne znaky
}

// Prevadi znaky cisel v poli na jejich int hodnotu (pouze kladna cisla), vraci prevedene cislo, nebo -1 pokud vstup obsahoval neplatne znaky
int my_atoi(char *array)
{
	if(is_array_digit(array))
	{			// Kontrola vstupu na nespravne znaky
		fprintf(stderr, "Byly zadany neplatne znkay!\n");
		return -1;
	}
	int result = 0;
				// Samotny vypocet z char hodnoty na int hodnotu
    for (int i = 0; array[i] != '\0'; i++)
        result = result * 10 + array[i] - '0';			// Odecteme hodnotu znaku '0' pro ziskani "skutecne hodnoty"
  
    return result;
}

// Porovna stringy (obdoba strcmp), vraci 0, pokud se stringy shoduji, jinak 1
bool my_strcmp(char *array1, char *array2)
{
	int arr_length1 = arr_length(array1);
	int arr_length2 = arr_length(array2);

	if(arr_length2 == arr_length1)
	{
		for(int i = 0; i < arr_length1; i++)
		{
			if(array1[i] != array2[i])
			{
				return 1;			// Stringy se rovnaji v delce, ale neshoduji se ve znacich
			}
		}
		return 0;			// Stringy se shoduji
	}
	else
		return 1;			// Stringy se nerovnaji v delce
}

// Vypis hexa hodnot znaku ze vstupu
void print_hexa_chars()
{
	int c = 0;
	while((c = input()) != EOF)
	{
		if(c < 16)			// Doplneni nuly na dvojici znaku, pokud ma znak hodnotu mene nez 16
			printf("0%x", c);
		else
			printf("%x", c);
	}
	printf("\n");
}

// Vypise index prvniho znaku na radce, nasledne 16 znaku v hex a nasledne v jejich tisknutelne podobe
int standardni_vypis(int jump, int max)
{
	int index = 0;
	int c = 0;
	int arr[16];			// Vypisujeme 16 znaku na radek

	for(index = 0; (index < jump); index++)
	{			// Odstraneni znaku ze stdin (skip)
		if((c = getchar()) == EOF)
			return 1;
	}

	while((c = input()) != EOF)
	{
		printf("%08x  ", index);			// Vypis relativni adresy (index pole)
		arr[0] = c;
		index++;

		for(int i = 1; i < 16; i++)
		{			// Plneni pole pro vypis hex a tisk
			if((c = getchar()) == EOF)
			{
				arr[i] = EOF;
				break;
			}
			else if(index == (max + jump))
			{			// Pokud dosahneme zadaneho maxima znaku pro tisk, pridame na konec pole znak \0
				arr[i] = EOF;
				break;
			}
			else
			{
				arr[i] = c;
				index++;
			}
		}

		for(int i = 0; i < 16; i++)
			{				// Vypis hexa hodnot znaku
				if(arr[i] == EOF)
				{			// Pokud dojdeme na konec vstupu, zarovaname pripadne vystup na 16 znaku
					while(i < 16)
					{
						printf("   ");
						if(i == 7)			// Mezi osmicemi bajtu je mezera navic
							printf(" ");
						i++;
					}
					break;	
				}
				else if(arr[i] == 0)
				{
					printf("00 ");
					if(i == 7)			// Mezi osmicemi bajtu je mezera navic
						printf(" ");
				}
				else if((arr[i] < 16) && (arr[i] > 0))
				{			// Doplneni nuly na dvojici znaku, pokud ma znak hodnotu mene nez 16
					printf("0%x ", arr[i]);
					if(i == 7)			// Mezi osmicemi bajtu je mezera navic
						printf(" ");
				}
				else if(arr[i] < 0)
				{			// Vypis znaku s diakritikou (jejich hodnota je zaporna)
					printf("%x ", (char)arr[i]);
					if(i == 7)			// Mezi osmicemi bajtu je mezera navic
						printf(" ");
				}	
				else
				{
					printf("%x ", arr[i]);
					if(i == 7)			// Mezi osmicemi bajtu je mezera navic
						printf(" ");
				}
			}
		printf(" |");

		for(int i = 0; i < 16; i++)
			{			// Vypis tisknutelnych znaku
				if(arr[i] == EOF)
				{			// Pokud dojdeme na konec vstupu, zarovaname pripadne vystup na 16 znaku
					while(i < 16)
					{
						printf(" ");
						i++;
					}
					break;	
				}

				else if(isprint(arr[i]))			// Kontrola "tisknutelnosti" znaku
					printf("%c", arr[i]);

				else
					printf(".");			// Pokud neni znak tisknutelny, vypiseme tecku
			}
			printf("|\n");
			if(index == (max + jump))			// Kontrola, zda jsme jiz vypsali stanoveny maximalni pocet znaku
				break;
	}
	return 0;
}

// Prevede vstupni znaky v hex formatu na dec a vytiskne jejich podobu
bool convert_hex_to_dec()
{
	char hex_char[2];
	int c = 0;

	while((c = input()) != EOF)
	{
		if(c == ' ')			// Mezeru preskocime
			continue;

		if((isxdigit(c) == 0) && (c != 10))
		{			// Pokud je na vstupu neplatny znak z hlediska hexa hodnoty a je ruzny od 10 (mezera je ignorovana)
			fprintf(stderr, "Byl zadan neplatny vstup!\n");
			return 1;
		}
		else
		{			// Doplnime 0 na dvojici znaku
			hex_char[0] = c;
			hex_char[1] = 0;
		}

		while((c = input()) != EOF)
		{			
			if(c == ' ')			// Mezeru preskocime
				continue;

			if((isxdigit(c) == 0) && (c != 10))
			{			// Pokud je na vstupu neplatny znak z hlediska hexa hodnoty a je ruzny od 10 (mezera je ignorovana)
				fprintf(stderr, "Byl zadan neplatny vstup!\n");
				return 1;
			}
			else
			{			// Doplnime na dvojici hexa znaku
				hex_char[1] = c;
				break;
			}
		}
		if(hex_char[0] == '\n')
			continue;
		else
		{
		printf("%c", (int)strtol(hex_char, NULL, 16));			// Prevedeni hexa hodnoty na int a jeji tisk
		fflush(stdout);
		}
	}
	return 0;
}

// Vyhleda ve vstupu stringy delsi nez zadane minimum a vytiskne je
void print_strings(int min)
{
	int c = 0;
	int i = 0;
	int j = 0;
	int arr[200];			// -S je omezeno na 199 znaku (+1 pro '\0')
	              			
	while((c = input()) != EOF)
	{
		if((c != '\n') && (c != '\0') && ((isprint(c)) || isblank(c)))
		{			// Pokud neni nacteny znak mezerou a koncem stringu
			arr[0] = c;
			i = 1;

			for(; i < 200; i++)
			{			// Pokud je "podretezec" znaku mensi nez 200, nacitame jej do pole
				c = getchar();
				if(isprint(c) || isblank(c))			// Pokud je znak tisknutelny, nebo je bilym znakem, tak jej ulozime
					arr[i] = c;
				else
					break;
			}
			if((i >= min) && (i < 200))
			{			// Pokud bylo dosazeno platneho "podretezce", tedy mensiho nez zadane minimum, vypiseme jej z alokovaneho pole
				for(j = 0; j < i; j++)
				{	
					printf("%c", arr[j]);
				}
				if(arr[j] != '\n')
					printf("\n");
			}
			else if((i > min) && i >= 200)			// Pokud je "podretezec" vetsi nebo roven 200 znaku, vypiseme co mame ulozeno a pak muzeme vypisovat dokud nebude \n, nebo \0
			{
				for(j = 0; j < i; j++)
				{	
					printf("%c", arr[j]);
				}
				while(isprint(c = getchar()) || isblank(c))
				{			// Vypisujeme dokud je znak tisknutelny, nebo je bilym znakem
						printf("%c", c);
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int jump, max, min;			// Deklarace promennych
	jump = max = min = 0;			// Inicializace promennych
	
	if(argc == 1)
	{			// Pokud nenni zadan zadny argument, zpracujeme vstup
		standardni_vypis(jump, max);
		return EXIT_SUCCESS;
	}
	else
	{
		int skip_flag = 0;			// Flag pro argument -s
		int NOC_flag = 0;			// Flag pro argument -n

		for(int i = 1; i < argc; i++)
		{			//kontrola zadanych argumentu
			if((my_strcmp(argv[i], "-s")) == 0)
			{			// Kontrola zadani argumentu -s
				if((i+1 == argc) || (is_array_digit(argv[i+1]) != 0))
				{			// Pokud neni za parametrem cislo, vypiseme napovedu
					print_help();
					return EXIT_FAILURE;
				}
				else if(i+2 == argc)
				{			// Pokud bylo zadano cislo, zpracujeme vstup
					int z = my_atoi(argv[i+1]);
					if(z == -1)
						return EXIT_FAILURE;
					standardni_vypis(z, max);
					return EXIT_SUCCESS;
				}
				else
				{			// Pokud bylo zadano cislo a vice argumentu, tak zmenime hodnotu flagu
					int z = my_atoi(argv[i+1]);
					if(z == -1)
						return EXIT_FAILURE;	
					jump =z;
					skip_flag = 1;
					i++;
					continue;
				}
			}
			else if((my_strcmp(argv[i], "-n")) == 0)
			{			// Kontrola zadani argumentu -n
				if((i+1 == argc) || (is_array_digit(argv[i+1]) != 0))
				{			// Pokud neni za parametrem cislo, vypiseme napovedu
					print_help();
					return EXIT_FAILURE;
				}
				else if(i+2 == argc)
				{
					int z = my_atoi(argv[i+1]);
					if(z == -1)
						return EXIT_FAILURE;
					standardni_vypis(jump, z);
					return EXIT_SUCCESS;
				}
				else
				{			// Pokud je zadano cislo pro max znaku, ulozime jej
					int z = my_atoi(argv[i+1]);
					if(z == -1)
						return EXIT_FAILURE;
					max = z;
					NOC_flag = 1;
					i++;
					continue;
				}
			}
			else if((my_strcmp(argv[i], "-S")) == 0)
			{			// Kontrola zadani argumentu -S			
				if((i+1 == argc) || (is_array_digit(argv[i+1]) != 0) || i+2 < argc)
				{			// Pokud neni za parametrem cislo, vypiseme napovedu
					print_help();
					return EXIT_FAILURE;
				}
				else
				{			// Pokud bylo zadano cislo, overime rozsah
					min = my_atoi(argv[i+1]);
					if(min < 1 || min > 199)
					{		// Pokud bylo zadano cislo mimo povoleny rozsah
						print_help();
						return EXIT_FAILURE;
					}
					print_strings(min);
					return EXIT_SUCCESS;
				}
			}
			else if((my_strcmp(argv[i], "-x")) == 0)
			{			// Kontrola zadani argumentu -x
				if((i+1) == argc)
				{			// Pokud je zadan skutecne samotny argument, zpracujeme vstup
					print_hexa_chars();
					return EXIT_SUCCESS;
				}
				else
				{			// Pokud je zadano vice argumentu, vypiseme napovedu
					print_help();
					return EXIT_FAILURE;
				}
			}
			else if((my_strcmp(argv[i], "-r")) == 0)
			{			// Kontrola zadani argumentu -r
				if((i + 1) == argc)
				{			// Pokud je zadan skutecne samotny argument, zpracujeme vstup
					if(convert_hex_to_dec())
						return EXIT_FAILURE;
					else
						return EXIT_SUCCESS;
				}
				else
				{			// Pokud je zadano vice argumentu, vypiseme napovedu
					print_help();
					return EXIT_FAILURE;
				}
			}
			else
			{			// Pokud neni zadan znamy argument, vypiseme napovedu
				print_help();
				return EXIT_FAILURE;
			}
		}
		if(skip_flag == 1 && NOC_flag == 1)
		{			// Pokud byly spravne zadany argumenty -s a -n, zpracujeme vstup
			standardni_vypis(jump,max);
			return EXIT_SUCCESS;
		}
	}
	return EXIT_SUCCESS;
}