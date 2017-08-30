// tail2.cc
// Řešení IJC-DU2, příklad 1b), 30.3.2017
// Autor: Dominik Křivohlávek, FIT
// Přeloženo: gcc 6.3.1
// Program pro vypis poslednich n radku ze souboru / stdin

#include <iostream>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>

// Hlavni funkce, zpracovani cin a po zpracovani tisk na cout
void print_tail(std::istream& input, unsigned lines_cnt)
{
    // Buffer pro lines_cnt radku
    std::queue<std::string> buffer;
    std::string line; 
    
    // Nacitani ze cin
    while(std::getline(input, line))
    {
        buffer.push(line);
        if(buffer.size() == (lines_cnt+1))
            buffer.pop();
    }

    // Vypis bufferu (poslednich n radku)
    while(!buffer.empty())
    {
        std::cout << buffer.front() << std::endl;
        buffer.pop();
    }
}

// Prevadi string na int, pokud prevadeny string neni cislo, vraci false
bool convert_str_int(int &i, const char *string)
{
    // Pomocna promenna pro get
	char c;

	// Prevod stringu na int
	std::stringstream tested_string(string);
	tested_string >> i;

	// Pokud se v tested_string nachazi dalsi znaky, neni to cislo
	if(tested_string.fail() || tested_string.get(c))
		return false;

	return true;
}

int main(int argc, char **argv)
{
    // Ze zadani...
    std::ios::sync_with_stdio(false);

    // Vychozi pocet radku k tisku
    int lines_cnt = 10;
    
    // Kontrola argumentu
    if(argc > 1) 
    {
        for(int i = 1; i < argc; i++)
        {
            // Prevod na c++ string pro pouziti compare
            std::string argv_line = argv[i];
            if(argv_line.compare("-n") == 0)
            {
				if(i + 1 == argc)
				{
					std::cerr << "Nebyl zadan pocet radku k vypisu!" << std::endl;
					return 1;
				}

                if(!convert_str_int(lines_cnt, argv[i+1]))
                {
					std::cerr << "Byl zadan spatny pocet radku k vypisu!" << std::endl;
					return 1;
                }
                
				// Osetreni zaporneho poctu radku
                if(lines_cnt < 0)
                {
                    std::cerr << "Byl zadan spatny pocet radku k vypisu!" << std::endl;
					return 1;
                }
				
                // Posun v argv
                i++;
            }
            else // Pravdepodobne byl predan soubor
            {
                std::fstream data;
                data.open (argv[i], std::fstream::in);

				// Kontrola otevreni souboru
                if (!data.is_open()) 
                {
					std::cerr << "Nezdarilo se otevrit soubor " << argv[i] << std::endl;
                    return 1;
                }
                print_tail(data, lines_cnt);
                data.close();
                return 0;
            }
        }
    }
    // Nebyl zadan soubor, zpracovani cin
    print_tail(std::cin, lines_cnt);
    return 0;
}