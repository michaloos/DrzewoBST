#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <string.h>

using namespace std;

struct wezel
{
	string slowo;
	struct wezel* lewy_syn;
	struct wezel* prawy_syn;
};

struct wezel* insert(struct wezel* poczotek, string wyraz)
{
	//jak nie ma nic to stworz nowy wezel
	if (poczotek == NULL)
	{
		struct wezel* pomoc = new wezel;
		pomoc->slowo = wyraz;
		pomoc->lewy_syn = NULL;
		pomoc->prawy_syn = NULL;
		return pomoc;
	}
	//jeżeli wyraz jest mniejszy od korzenia to w lewo a jak nie to w prawo 
	if (wyraz < poczotek->slowo)
	{
		poczotek->lewy_syn = insert(poczotek->lewy_syn, wyraz);
	}
	else if (wyraz > poczotek->slowo)
	{
		poczotek->prawy_syn = insert(poczotek->prawy_syn, wyraz);
	}
	return poczotek;
}

struct wezel* minimum(struct wezel* poczotek)
{
	//idziemy najbardziej w lewo
	struct wezel* obecny = poczotek;
	while (obecny && obecny->lewy_syn != NULL)
	{
		obecny = obecny->lewy_syn;
	}
	return obecny;
}

void print(struct wezel* element, int spacja)
{
	int i = 0;
	int przestrzen = 15; 
	if (element == NULL)
	{
		return;
	}
	spacja += przestrzen;
	//zaczynamy od najbardziej prawego elementu w drzewie
	print(element->prawy_syn, spacja);
	cout << endl;
	for (int i = przestrzen; i < spacja; i++)
	{
		cout << " ";
	}
	cout << element->slowo << "\n";
	//wypisujemy lewe poddrzewo
	print(element->lewy_syn, spacja);
}

struct wezel* deletewezel(struct wezel* element, string wyraz)
{ 
	if (element == NULL) 
	{
		return element;
	}
	//jeżeli wyraz do usunięcia jest mniejszy od korzenia 
	// to wtedy leży w lewym poddrzewie
	if (wyraz < element->slowo)
	{
		element->lewy_syn = deletewezel(element->lewy_syn, wyraz);
	}
	//jeżeli wyraz do usunięcia jest większy od korzenia to 
	//leży on w prawym poddrzewie
	else if (wyraz > element->slowo)
	{
		element->prawy_syn = deletewezel(element->prawy_syn, wyraz);
	}
	//jeżeli wyraz jest taki jak korzen to usuwamy korzen
	else
	{
		// wezel z jednym  dzieckiem lub bez dzieci
		if (element->lewy_syn == NULL)
		{
			struct wezel* temp = element->prawy_syn;
			delete(element);
			return temp;
		}
		else if (element->prawy_syn == NULL)
		{
			struct wezel* temp = element->lewy_syn;
			delete(element);
			return temp;
		}
		//jeżeli ma dwoje dzieci to szukamy minimum w prawym poddrzewie
		// kopiujemy minimum i zastepujemy nim korzen
		// i usuwamy minimum 
		struct wezel* temp = minimum(element->prawy_syn);
		element->slowo = temp->slowo;
		element->prawy_syn = deletewezel(element->prawy_syn, temp->slowo);
	}
	return element;
}




int szukaj(struct wezel* element, string wyraz)
{
	if (element == NULL )
	{
		return 0;
	}
	if (element->slowo == wyraz)
	{
		return 1;
	}
	//szukamy dalej jak nie został znaleziony
	if (element->slowo < wyraz)
	{
		return szukaj(element->prawy_syn, wyraz);
	}
	else
	{
		return szukaj(element->lewy_syn, wyraz);
	}
}
int szukajprefiks(struct wezel* element, string prefiks)
{
	int dlugosc;
	dlugosc = size(prefiks);
	if (element == NULL)
	{
		return 0;
	}
	if (element->slowo.substr(0, dlugosc) == prefiks)
	{
		return 1 + szukajprefiks(element->prawy_syn, prefiks) + szukajprefiks(element->lewy_syn, prefiks);
	}
	if (element->slowo <= prefiks)
	{
		return szukajprefiks(element->prawy_syn, prefiks);
	}
	else
	{
		return szukajprefiks(element->lewy_syn, prefiks);
	}
}


int main()
{
	fstream plik;
	ofstream wyjscie;
	string nazwapliku = "induzy.txt";
	plik.open(nazwapliku);
	wyjscie.open("wyjscie.txt");
	string linia;
	int n;
	int linie = 0;
	plik >> n;
	if (plik.good())
	{
		while (!plik.eof())
		{
			getline(plik, linia);
			linie++;
		}
		plik.close();
	}
	else
	{
		cout << "blad" << endl;
		return 0;
	}
	plik.close();
	plik.open(nazwapliku);
	int liczba;
	plik >> liczba;
	string** tablica = new string* [linie - 1];
	for (int i = 0; i < linie - 1; i++)
	{
		tablica[i] = new string[2];
		for (int j = 0; j < 2; j++)
		{
			plik >> tablica[i][j];
		}
	}
	struct wezel* root;
	root = NULL;
	string zmienna;
	for (int i = 0; i < linie - 1; i++)
	{
		if (tablica[i][0] == "W" && i == 0)
		{
			root = insert(root, tablica[i][1]);
		}
		if (tablica[i][0] == "W")
		{
			insert(root, tablica[i][1]);
		}
		if (tablica[i][0] == "U")
		{
			deletewezel(root, tablica[i][1]);
		}
		if (tablica[i][0] == "S")
		{
			if (szukaj(root, tablica[i][1]) == 0)
			{
				wyjscie << "NIE" << endl;
			}
			else
			{
				wyjscie << "TAK" << endl;
			}
		}
		if(tablica[i][0] == "L")
		{
			zmienna = tablica[i][1];
			wyjscie << szukajprefiks(root,zmienna) << endl;
		}
	}
	char znak;
	string wyraz;
	for(;;)
	{
		cout << "============MENU============" << endl;
		cout << "1.Wstaw slowo do drzewa" << endl;
		cout << "2.Usun slowo z drzewa" << endl;
		cout << "3.Szukaj slowa w drzewie" << endl;
		cout << "4.Wypisz drzewo" << endl;
		cout << "0.KONIEC PROGRAMU" << endl;
		cout << "Jaka opcje wybierasz?" << endl;
		cout << "" << endl;

		cin >> znak;
		
		switch (znak)
		{
		case '1':
			cout << "Podaj jaki wyraz chcesz dodać: ";
			cin >> wyraz;
			insert(root, wyraz);
			break;
		case '2':
			cout << "Podaj jaki wyraz chcesz usunac: ";
			getline(cin, wyraz);
			cin >> wyraz;
			deletewezel(root, wyraz);
			break;
		case '3':
			cout << "Podaj jaki wyraz chcesz znalezc: ";
			getline(cin, wyraz);
			cin >> wyraz;
			if (szukaj(root, wyraz) == 0)
			{
				cout << "NIE" << endl;
			}
			else
			{
				cout << "TAK" << endl;
			}
			break;
		case '4':
			print(root, 0);
			break;
		case '0':
			exit(0);
		}
		getchar();
		getchar();
		system("cls");
	} 
	plik.close();
	wyjscie.close();
	delete[] tablica;
}
