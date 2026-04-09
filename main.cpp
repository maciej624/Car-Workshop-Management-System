// Maciej Drajewski 2025/2026 Warsztat Mechaniczny projekt 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <map>
#include <chrono>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include <cstdlib>
#include "podstawy.h"

//kolory i tekst
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string CYAN = "\033[36m";
const string MAGENTA = "\033[35m";

using namespace std;
using namespace chrono;


//baza usterek
vector<Usterka> pobierzKatalogUsterek() {
    vector<Usterka> katalog;
    katalog.push_back(Usterka("Wymiana oleju i filtrow", 250, 10));
    katalog.push_back(Usterka("Wymiana klockow hamulcowych", 400, 15));
    katalog.push_back(Usterka("Wymiana tarcz hamulcowych", 600, 20));
    katalog.push_back(Usterka("Naprawa klimatyzacji", 500, 18));
    katalog.push_back(Usterka("Wymiana akumulatora", 350, 8));
    katalog.push_back(Usterka("Wymiana opon (komplet)", 800, 12));
    katalog.push_back(Usterka("Naprawa zawieszenia", 700, 25));
    katalog.push_back(Usterka("Wymiana sprzegla", 1200, 30));
    katalog.push_back(Usterka("Naprawa alternatora", 550, 20));
    katalog.push_back(Usterka("Wymiana rozrzadu", 1500, 35));
    katalog.push_back(Usterka("Naprawa silnika", 2500, 40));
    katalog.push_back(Usterka("Naprawa skrzyni biegow", 2000, 38));
    katalog.push_back(Usterka("Wymiana swiec zaplonowych", 200, 8));
    katalog.push_back(Usterka("Naprawa ukladu wydechowego", 450, 15));
    katalog.push_back(Usterka("Geometria kol", 150, 10));
    katalog.push_back(Usterka("Diagnostyka komputerowa", 100, 5));     
    katalog.push_back(Usterka("Wymiana filtra paliwa", 180, 12));     
    katalog.push_back(Usterka("Wymiana pompy wody", 650, 25));        
    katalog.push_back(Usterka("Regeneracja turbosprezarki", 1800, 35));
    katalog.push_back(Usterka("Serwis i odgrzybianie klimy", 300, 10));
    katalog.push_back(Usterka("Wymiana pior wycieraczek", 80, 2));     
    katalog.push_back(Usterka("Wymiana termostatu", 350, 15));        
    katalog.push_back(Usterka("Czyszczenie filtra DPF", 700, 20));

    return katalog;
}
//baza usterek do losowania
vector<Usterka> pobierzDodatkoweUsterki() {
    vector<Usterka> dodatkowe;
    dodatkowe.push_back(Usterka("Zuzyte klocki hamulcowe tyl", 350, 12));
    dodatkowe.push_back(Usterka("Nieszczelny amortyzator", 400, 15));
    dodatkowe.push_back(Usterka("Zuzyty pasek klinowy", 180, 8));
    dodatkowe.push_back(Usterka("Niska jakosc plynu hamulcowego", 120, 6));
    dodatkowe.push_back(Usterka("Peknieta tuleja wahacza", 300, 14));
    dodatkowe.push_back(Usterka("Zuzyte lozysko kola", 450, 18));
    dodatkowe.push_back(Usterka("Przeciekajaca chlodnica", 550, 20));
    dodatkowe.push_back(Usterka("Uszkodzony katalizator", 1200, 25));
    dodatkowe.push_back(Usterka("Zuzyte mocowanie silnika", 280, 12));
    dodatkowe.push_back(Usterka("Przepalona zarowka stopu", 50, 3));      
    dodatkowe.push_back(Usterka("Wyciek oleju z miski olejowej", 350, 18));
    dodatkowe.push_back(Usterka("Luzy na drazkach kierowniczych", 220, 12));
    dodatkowe.push_back(Usterka("Skorodowane przewody hamulcowe", 400, 20));
    dodatkowe.push_back(Usterka("Uszkodzona sonda Lambda", 450, 15));     
    dodatkowe.push_back(Usterka("Peknieta sprezyna zawieszenia", 300, 16));
    dodatkowe.push_back(Usterka("Zatarty zacisk hamulcowy", 250, 14));
    dodatkowe.push_back(Usterka("Uszkodzony czujnik ABS", 200, 10));

    return dodatkowe;
}
//czysty getline
void clearing()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
//bezpiczne wprowadzanie danych 
int wczytajInt(const string& txt, int min, int max)
{
    int wynik;
    while (true)
    {
        cout << YELLOW << txt << RESET ;
        if (cin >> wynik && wynik >= min && wynik <= max)
        {
            clearing();
            return wynik;
        }
        cout <<"\n"<<RED<< "BLAD: Podaj liczbe z zakresu " << min << "-" << max << "!"<<endl;
        clearing();
    }
}
// GUI menu warsztatu
void wyswietlMenu()
{
    cout << BLUE << "  _      __                 _       _   " << RESET << endl;
    cout << BLUE << " | | /| / /__ ________ ____| |_    | |  " << RESET << endl;
    cout << BLUE << " | |/ |/ / _ `/ __/_ // (_-< __|   |_|  " << RESET << endl;
    cout << BLUE << " |__/|__/\\_,_/_/ /___/___/\\__|     (_)  " << RESET << endl;
    cout << BLUE << "   SYSTEM ZARZADZANIA WARSZTATEM        " << RESET << endl;
    cout << BLUE << "========================================" << RESET << endl;
    cout << endl;
    cout << "  " << YELLOW << "[1]" << RESET << " Przyjmij nowe auto do serwisu" << endl;
    cout << "  " << YELLOW << "[2]" << RESET << " Wyswietl liste zlecen" << endl;
    cout << "  " << YELLOW << "[3]" << RESET << " Wyswietl gotowe do odbioru" << endl;
    cout << "  " << YELLOW << "[4]" << RESET << " Wydaj pojazd (Finalizacja)" << endl;
    cout << "  " << YELLOW << "[5]" << RESET << " Generuj raport dzienny" << endl;
    cout << "  " << YELLOW << "[6]" << RESET << " Statystyki warsztatu (Wykres)" << endl;
    cout << endl;
    cout << "  " << RED << "[0] ZAKONCZ PRACE" << RESET << endl;
    cout << endl;
    cout << BLUE << "========================================" << RESET << endl;
}

void wyswietlInfoPojazdu(const IPojazd& pojazd, double mnoznikCeny)
{   

    cout << BLUE << "\n+=============================================+" << RESET << "\n";
    cout << BLUE << "|  ZLECENIE: " << RESET << BOLD << setw(31) << left << pojazd.getNumerRejestracyjny() << RESET << BLUE << " |" << RESET << "\n";
    cout << BLUE << "+=============================================+" << RESET << "\n";
    cout << "  " << YELLOW << "Klient: " << RESET << pojazd.getImieKlienta() << "\n";
    cout << "  " << YELLOW << "Typ:    " << RESET << pojazd.getTypPojazdyString() << "\n";
    cout << "  " << YELLOW << "Pojazd: " << RESET << pojazd.getMarka() << " " << pojazd.getModel()<< " (" << BOLD << pojazd.getRokProdukcji() << RESET << ")\n";
    cout << "  " << YELLOW << "Status: " << RESET << GREEN << pojazd.getStatusString() << RESET;

    if (pojazd.getStatus() == status_zlecenia::w_naprawie)
    {
        cout << " (pozostalo: " << pojazd.pozostalyCzas() << "s)";
    } 
    cout << "\n\n";
    cout <<RED<<BOLD<< "  USTERKI:\n"<<RESET;
    const auto& usterki = pojazd.getUsterki();
    for (auto i = 0; i < usterki.size(); ++i)
    {
        cout<< CYAN <<BOLD << (i + 1) << ". " << usterki[i].nazwa<<RESET;
        if (usterki[i].wykryta_przez_mechanika)
        {
            cout << RED<<BOLD<<" [WYKRYTA PRZEZ MECHANIKA]"<<RESET;
        }
        cout << "\n";
    }
    cout << GREEN << BOLD << "\n  CALKOWITY KOSZT: " << fixed << setprecision(2) << pojazd.obliczCalkowityKoszt(mnoznikCeny) << " PLN\n" << RESET;
    cout << "-----------------------------------------------\n";
}

void przyjmijKlienta(vector<unique_ptr<IPojazd>>& zlecenia, map<string, InfoMarki>& bazeMarek, const vector<InfoMarki>& dostepneMarki)
{
    try
    {
        cout << BLUE << "=========================================" << RESET << endl;
        cout << BLUE << "|      PRZYJMOWANIE NOWEGO POJAZDU      |" << RESET << endl;
        cout << BLUE << "=========================================" << RESET << endl;

        string imie;
        cout << "\n" << YELLOW << "[KROK 1/5] Dane Klienta" << RESET << endl;
        getline(cin >> ws, imie);

        cout << "\n" << YELLOW << "[KROK 2/5] Wybor Marki" << RESET << endl;
        cout << " Dostepne marki w bazie:\n";
        for (auto i = 0; i < dostepneMarki.size(); ++i)
        {
            cout << "   " << YELLOW << "[" << (i + 1) << "]" << RESET << " " << dostepneMarki[i] << "\n";
        }

        int wyborMarki = wczytajInt("\nWybierz marke: ", 1, dostepneMarki.size());
        InfoMarki wybranaMarka = dostepneMarki[wyborMarki - 1];

        string model;
        cout << "\n" << YELLOW << "[KROK 3/5] Model Pojazdu" << RESET << endl;
        getline(cin >> ws, model);

        int rok = wczytajInt("\nRok produkcji (1970-2026): ", 1970, 2026);

        string numer_rejestracyjny;
        cout << "\n" << YELLOW << "[KROK 4/5] Numer Rejestracyjny Pojazdu" << RESET << endl;
        getline(cin >> ws, numer_rejestracyjny);

        cout << "\n" << YELLOW << "[KROK 5/5] Typ Pojazdu" << RESET << endl;
        cout << "   " << YELLOW << "[1]" << RESET << " Samochod osobowy " << endl;
        cout << "   " << YELLOW << "[2]" << RESET << " Motocykl " << GREEN << "(-25% ceny)" << RESET << endl;
        cout << "   " << YELLOW << "[3]" << RESET << " Ciezarowka " << RED << "(+25`0% ceny)" << RESET << endl; 
        int typPojazdu = wczytajInt("Wybor: ", 1, 3);

        unique_ptr<Pojazd> pojazd;
        switch (typPojazdu)
        {
        case 1: pojazd = make_unique<Samochod>(wybranaMarka.getNazwa(), model, rok, numer_rejestracyjny, imie); break;
        case 2: pojazd = make_unique<Motocykl>(wybranaMarka.getNazwa(), model, rok, numer_rejestracyjny, imie); break;
        case 3: pojazd = make_unique<Ciezarowka>(wybranaMarka.getNazwa(), model, rok, numer_rejestracyjny, imie); break;
        }

        cout <<MAGENTA<<"\n[ROZMOWA Z KLIENTEM]\n"<<RESET;
        cout <<CYAN<< "Klient " << imie << " zglasza usterki...\n\n"<<RESET;

        vector<Usterka> katalog = pobierzKatalogUsterek();
        cout << BLUE << "=================================================================\n" << RESET;
        cout << BLUE << "=                    KATALOG USTEREK                            =\n" << RESET;
        cout << BLUE << "=================================================================\n" << RESET;
        for (auto i = 0; i < katalog.size(); ++i)
        {
            cout << "  " << (i + 1) << ". " << katalog[i] << "\n";
        }
        cout << " 0. Zakoncz dodawanie\n";

        bool dodanoUsterke = false;
        int wybor;
        
        do
        {
            wybor = wczytajInt("Wybierz usterke: ", 0, katalog.size());
            if (wybor > 0)
            {
                pojazd->dodajUsterke(katalog[wybor - 1]);
                cout <<GREEN<< "Dodano: " << katalog[wybor - 1].nazwa << "\n"<<RESET;
                dodanoUsterke = true;
            }
        } while (wybor != 0);

        if (!dodanoUsterke)
        {
            cout <<MAGENTA<< "\n Brak usterek. Anulowano przyjecie pojazdu.\n"<<RESET;
            return;
        }

        cout << CYAN<<"[MECHANIK] Trwa diagnostyka pojazdu...\n"<<RESET;

        vector<Usterka> dodatkowe = pobierzDodatkoweUsterki();
        vector<string> wykryte = pojazd->wykonajDiagnoze(dodatkowe, wybranaMarka.getMnoznikCzasu());

        if (!wykryte.empty())
        {
            cout <<RED<< "  WYKRYTO DODATKOWE USTERKI:\n"<<RESET;
            for (const auto& nazwa : wykryte) cout << "  "<<RED<<BOLD << nazwa << RESET<<"\n";
        }
        else
        {
            cout <<BLUE<< "Brak ukrytych usterek.\n"<<RESET;
        }
        cout << "\n" << BLUE << "=========================================" << RESET << endl;
        cout << BLUE << "|            WYCENA KONCOWA             |" << RESET << endl;
        cout << BLUE << "=========================================" << RESET << endl;

        double kosztCalkowity = pojazd->obliczCalkowityKoszt(wybranaMarka.getMnoznikCeny());
        int czasKoncowy = pojazd->getCalkowityCzasNaprawy();
        cout << "  Koszt naprawy:   " << GREEN << BOLD << fixed << setprecision(2)<< kosztCalkowity << " PLN" << RESET << endl;
        cout << "  Czas trwania:    " << BOLD << czasKoncowy << " sekund" << RESET << endl;
        

        cout << "\n";
        cout << BLUE << "+=================================================+" << RESET << "\n";
        cout << BLUE << "|              FAKTURA KOSZTORYSOWA               |" << RESET << "\n";
        cout << BLUE << "+=================================================+" << RESET << "\n";
        double sumaBazowa = 0;
        const auto& listaUsterek = pojazd->getUsterki();

        for (auto i = 0; i < listaUsterek.size(); ++i)
        {
            sumaBazowa += listaUsterek[i].koszt;
            cout << " " << (i + 1) << ". " << listaUsterek[i].nazwa << "- " << listaUsterek[i].koszt << " PLN" << endl;
        }
        cout <<endl;
        cout << YELLOW << "  [1] Suma bazowa: " << RESET<<BOLD<< sumaBazowa << " zl\n"<<RESET;
        cout << YELLOW << "  [2] Mnoznik marki (" << wybranaMarka.getNazwa() << "):" << RESET<< "x " <<BOLD<< wybranaMarka.getMnoznikCeny() << "\n"<<RESET;
        double mnoznik_typ = 1.0;
        if (pojazd->getTypPojazdu() == typ_pojazdu::motocykl)
        {
            mnoznik_typ = 0.75;
            cout << YELLOW << "  [3] Znizka na motocykl: " << RESET << mnoznik_typ << " (-25%)\n ";
        }
        else if (pojazd->getTypPojazdu() == typ_pojazdu::ciezarowka)
        {
            mnoznik_typ = 2.5;
            cout << YELLOW << "  [3] Podwyzka na pojazd ciezarowy: " << RESET << mnoznik_typ << " (+250)% \n";
        }
        cout << BLUE << "---------------------------------------------------" << RESET << "\n";


        cout << GREEN << BOLD << "  DO ZAPLATY (BRUTTO):" << RESET<< GREEN << BOLD <<  kosztCalkowity << " zl" << RESET << "\n";

        cout << BLUE << "===================================================" << RESET << "\n";

        cout << "\n" << CYAN << "  SZACOWANY CZAS SERWISU:\n" << RESET;

        cout << BLUE << "---------------------------------------------------" << RESET << "\n";
        cout << CYAN << BOLD << "    TERMIN REALIZACJI:" << RESET<< right << CYAN << BOLD  << czasKoncowy << " s" << RESET << "\n";

        cout << BLUE << "=========================================" << RESET << endl;
        cout << "\n" << YELLOW << "[PYTANIE] Czy klient akceptuje kosztorys?" << RESET << endl;
        cout << "   " << GREEN << "[1] TAK" << RESET << " - Przyjmij auto do serwisu\n";
        cout << "   " << RED << "[2] NIE" << RESET << " - Klient rezygnuje (Anuluj)\n";
        int decyzja = wczytajInt("\n > Twoja decyzja: ", 1, 2);
        if (decyzja == 1)
        {
            zlecenia.push_back(move(pojazd));
            cout <<MAGENTA<< "\nZlecenie przyjete! Auto w naprawie.\n"<<RESET;
        }
        else
        {
            cout <<RED<< "\nKlient zrezygnowal.\n"<<RESET;
        }
    }
    catch (const wyjatki& e)
    {
        cerr << "\nBLAD KRYTYCZNY: " << e.what() << "\n";
    }
}

void wyswietlWszystkieZlecenia(vector<unique_ptr<IPojazd>>& zlecenia, const map<string, InfoMarki>& bazeMarek)
{
    if (zlecenia.empty())
    {
        cout << "\n[INFO] Brak aktywnych zlecen.\n";
        return;
    }

    for (auto& pojazd : zlecenia)
    {
        pojazd->aktualizujStatus();
        auto it = bazeMarek.find(pojazd->getMarka());
        double mnoznik = (it != bazeMarek.end()) ? it->second.getMnoznikCeny() : 1.0;
        wyswietlInfoPojazdu(*pojazd, mnoznik);
    }
}

void wyswietlGotowe(vector<unique_ptr<IPojazd>>& zlecenia, const map<string, InfoMarki>& bazeMarek)
{
    bool znaleziono = false;
    auto ileGotowych = count_if(zlecenia.begin(), zlecenia.end(), [](const unique_ptr<IPojazd>& p)
        {
            return p->getStatus() == status_zlecenia::gotowe_do_odbioru;
        });

    if (ileGotowych == 0)
    {
        cout << "\n[INFO] Brak aut gotowych do odbioru.\n";
        return;
    }

    for (auto& pojazd : zlecenia)
    {
        pojazd->aktualizujStatus();

        if (pojazd->getStatus() == status_zlecenia::gotowe_do_odbioru)
        {
            auto it = bazeMarek.find(pojazd->getMarka());
            double mnoznik = (it != bazeMarek.end()) ? it->second.getMnoznikCeny() : 1.0;
            wyswietlInfoPojazdu(*pojazd, mnoznik);
            znaleziono = true;
        }
    }
}

void odbierzPojazd(vector<unique_ptr<IPojazd>>& zlecenia, vector<unique_ptr<IPojazd>>& historia,const map<string, InfoMarki>& bazeMarek)
{
    cout << BLUE << "=========================================" << RESET << endl;
    cout << BLUE << "|      WYDAWANIE POJAZDU KLIENTOWI      |" << RESET << endl;
    cout << BLUE << "=========================================" << RESET << endl;

    vector<int> indeksyGotowych;
    for (auto i = 0; i < zlecenia.size(); ++i) {
        zlecenia[i]->aktualizujStatus();

        if (zlecenia[i]->getStatus() == status_zlecenia::gotowe_do_odbioru)
        {
            indeksyGotowych.push_back(i);
            auto it = bazeMarek.find(zlecenia[i]->getMarka());
            double mnoznik = (it != bazeMarek.end()) ? it->second.getMnoznikCeny() : 1.0;

            cout << "  " << indeksyGotowych.size() << ". " << zlecenia[i]->getNumerRejestracyjny() << " | " << zlecenia[i]->getImieKlienta() << " | Do zaplaty: " << fixed << setprecision(2) << zlecenia[i]->obliczCalkowityKoszt(mnoznik) << " PLN\n";
        }
    }

    if (indeksyGotowych.empty())
    {
        cout << "\n[INFO] Brak aut gotowych do odbioru.\n";
        return;
    }

    cout << "  0. Powrot\n";
    int wybor = wczytajInt("Wybierz pojazd: ", 0, indeksyGotowych.size());

    if (wybor > 0)
    {
        int indexWZleceniach = indeksyGotowych[wybor - 1];
        cout << "\n Wydano pojazd klientowi: " << zlecenia[indexWZleceniach]->getImieKlienta() << "\n";
        cout <<BOLD<< " Przyjeto zaplate.\n"<<RESET;  
        historia.push_back(move(zlecenia[indexWZleceniach]));
        zlecenia.erase(zlecenia.begin() + indexWZleceniach);
    }
}
//wykres gotowych i w naprawie
void wyswietlWykresStatusow(vector<unique_ptr<IPojazd>>& zlecenia)
{
    if (zlecenia.empty()) return;

    int suma = zlecenia.size();
    int gotowe = 0;

    for (auto& p : zlecenia) {
        p->aktualizujStatus();
        if (p->getStatus() == status_zlecenia::gotowe_do_odbioru) gotowe++;
    }
    int wNaprawie = suma - gotowe; // Reszta to te w naprawie
    cout << "\n--- STATYSTYKI ---\n";
    cout << "W naprawie [" << string((wNaprawie * 20) / suma, '#') << "] "<< wNaprawie << " (" << (double)wNaprawie / suma * 100.0 << "%)\n";
    cout << "Gotowe     [" << string((gotowe * 20) / suma, '#') << "] "<< gotowe << " (" << (double)gotowe / suma * 100.0 << "%)\n";
}
int main() 
{
    srand(time(NULL));
    vector<unique_ptr<IPojazd>> zlecenia;
    vector<unique_ptr<IPojazd>> historiaZlecen;
    map<string, InfoMarki> bazaMarekMap;
    vector<InfoMarki> dostepneMarki;

    try
    {
        dostepneMarki = marki_plik::wczytaj_marki("marki.txt");
        for (const auto& m : dostepneMarki) bazaMarekMap[m.getNazwa()] = m;
    }
    catch (const wyjatki& e)
    {
        cerr << "BLAD KRYTYCZNY PODCZAS STARTU: " << e.what() << endl;
        return 1;
    }

    while (true)
    {
        wyswietlMenu();
        int wybor = wczytajInt("Twoj wybor: ", 0, 6);

        try
        {
            switch (wybor)
            {
            case 1: przyjmijKlienta(zlecenia, bazaMarekMap, dostepneMarki); break;
            case 2: wyswietlWszystkieZlecenia(zlecenia, bazaMarekMap); break;
            case 3: wyswietlGotowe(zlecenia, bazaMarekMap); break;
            case 4: odbierzPojazd(zlecenia, historiaZlecen, bazaMarekMap); break;
            case 5:
                raport_plik::zapiszRaport("raport_dnia.txt", zlecenia, historiaZlecen, bazaMarekMap);
                cout << "\nRaport zapisano w pliku 'raport_dnia.txt'\n";
                break;
            case 6:
                cout << "Liczba zlecen w toku: " << zlecenia.size() << "\n";
                wyswietlWykresStatusow(zlecenia);
                break;
            case 0:
                cout << "Zamykanie systemu..\n";
                return 0;
            }
        }
        catch (const wyjatki& e)
        {
            cerr << "\nBLAD KRYTYCZNY: " << e.what() << "\n";
        }
        cout << "\nNacisnij ENTER aby kontynuowac..";
        cin.get();
    }
}