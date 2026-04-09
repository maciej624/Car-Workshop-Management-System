#pragma once
#include <memory>
#include <string>
#include <fstream>
#include <map>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace chrono;


enum class status_zlecenia
{
    w_naprawie,
    gotowe_do_odbioru
};
enum class typ_pojazdu
{
    samochod,
    motocykl,
    ciezarowka
};
//wyjatki pliki i zle dane
class wyjatki :public runtime_error
{
public:
    wyjatki(const string& msg) :runtime_error(msg) {}
};

class plik_nie_istnieje : public wyjatki
{
public:
    plik_nie_istnieje(const string& msg) :wyjatki("Nie mozna otworzyc pliku > " + msg) {}
};

class nieprawidlowe_dane : public wyjatki
{
public:
    nieprawidlowe_dane(const string& msg) :wyjatki("Zosta³y wprowadzone zle dane > " + msg) {}
};
//struktura pojedynczej usterki
struct Usterka
{
    string nazwa;
    double koszt;
    int czas_naprawy;
    bool wykryta_przez_mechanika;

    Usterka(const string& n, double k, int c, bool wykryta = false) : nazwa(n), koszt(k), czas_naprawy(c), wykryta_przez_mechanika(wykryta) {}

    friend ostream& operator<<(ostream& os, const Usterka& u)
    {
        os << u.nazwa << " (" << u.koszt << " PLN, " << u.czas_naprawy << "s)";
        return os;
    }
};
//klasa trzymajaca mnozniki dla kazdej marki z pliku
class InfoMarki
{
private:
    string nazwa;
    double mnoznikCeny;
    double mnoznikCzasu;
public:
    InfoMarki(const string& n = "", double cena = 1.0, double czas = 1.0) : nazwa(n), mnoznikCeny(cena), mnoznikCzasu(czas) 
    { 
    if (cena <= 0) throw nieprawidlowe_dane("Mnoznik ceny musi byc > 0");
    if (czas <= 0) throw nieprawidlowe_dane("Mnoznik czasu musi byc > 0");
    }
    string getNazwa() const { return nazwa; }
    double getMnoznikCeny() const { return mnoznikCeny; }
    double getMnoznikCzasu() const { return mnoznikCzasu; }
    friend ostream& operator<<(ostream& os, const InfoMarki& m)
    {
        os << m.nazwa << " (cena x" << fixed << setprecision(2) << m.mnoznikCeny << ", czas x" << m.mnoznikCzasu << ")";
        return os;
    }
};
//klasa abstrakcyjna pojazdow 
class IPojazd
{
public:
    virtual string getMarka() const = 0;
    virtual string getModel() const = 0;
    virtual int getRokProdukcji() const = 0;
    virtual string getNumerRejestracyjny() const = 0;
    virtual string getImieKlienta() const = 0;
    virtual status_zlecenia getStatus() const = 0;
    virtual string getStatusString() const = 0;
    virtual double obliczCalkowityKoszt(double mnoznikMarki) const = 0;
    virtual typ_pojazdu getTypPojazdu() const = 0;
    virtual string getTypPojazdyString() const = 0;
    virtual int pozostalyCzas() const = 0;
    virtual int getCalkowityCzasNaprawy() const = 0;
    virtual const vector<Usterka>& getUsterki() const = 0;
    virtual void aktualizujStatus() = 0;
    virtual ~IPojazd() {}
};
//glowna klasa bazowa
class Pojazd : public IPojazd
{
private:
    string marka,model,numerRejestracyjny,imieKlienta;
    int rokProdukcji,calkowityCzasNaprawy;
    vector<Usterka> usterki;
    time_point<steady_clock> czasRozpoczeciaNaprawy;
    status_zlecenia status;

protected:
    void setStatus(status_zlecenia nowyStatus) { status = nowyStatus; }
    void setCzasRozpoczecia(time_point<steady_clock> czas) { czasRozpoczeciaNaprawy = czas; }

public:
    Pojazd(const string& mrk, const string& mdl, int rok, const string& nr, const string& imie) : marka(mrk), model(mdl), rokProdukcji(rok), numerRejestracyjny(nr), imieKlienta(imie), calkowityCzasNaprawy(0), status(status_zlecenia::w_naprawie)
    {
        if (mrk.empty()) throw nieprawidlowe_dane("Marka nie moze byc pusta");
        if (mdl.empty()) throw nieprawidlowe_dane("Model nie moze byc pusty");
        if (rok < 1970 || rok > 2026) throw nieprawidlowe_dane("Rok musi byc w zakresie 1970-2026");
        if (nr.empty()) throw nieprawidlowe_dane("Numer rejestracyjny nie moze byc pusty");
        if (imie.empty()) throw nieprawidlowe_dane("Imie klienta nie moze byc puste");
    }
    virtual ~Pojazd() {}

    void dodajUsterke(const Usterka& usterka)
    {
        usterki.push_back(usterka);
        calkowityCzasNaprawy += usterka.czas_naprawy;
    }

    void dodajMnoznikCzasu(double mnoznik)
    {
        if (mnoznik <= 0) throw nieprawidlowe_dane("Mnoznik czasu musi byc > 0");
        calkowityCzasNaprawy = static_cast<int>(calkowityCzasNaprawy * mnoznik);
    }

    vector<string> wykonajDiagnoze(const vector<Usterka>& mozliweDodatkoweUsterki, double mnoznikCzasu)
    {
        vector<string> wykryteUsterki;
        //SYMULACJA - losowosc nowej usterki
        for (const auto& usterka : mozliweDodatkoweUsterki)
        {
            int los = (rand() % 100) + 1;
            if (los <= 7) 
            {
                Usterka nowaUsterka = usterka;
                nowaUsterka.wykryta_przez_mechanika = true;
                usterki.push_back(nowaUsterka);
                calkowityCzasNaprawy += nowaUsterka.czas_naprawy;
                wykryteUsterki.push_back(nowaUsterka.nazwa);
            }
        }
        dodajMnoznikCzasu(mnoznikCzasu); //mnozymy calkowity czas naprawy
        czasRozpoczeciaNaprawy = steady_clock::now();
        status = status_zlecenia::w_naprawie;
        return wykryteUsterki;
    }

    void aktualizujStatus()
    {
        if (status == status_zlecenia::w_naprawie)
        {
            auto teraz = steady_clock::now();
            auto uplynelo = duration_cast<seconds>(teraz - czasRozpoczeciaNaprawy).count();

            if (uplynelo >= calkowityCzasNaprawy)
            {
                status = status_zlecenia::gotowe_do_odbioru;
            }
        }
    }

    virtual double obliczCalkowityKoszt(double mnoznikMarki) const override
    {
        double suma = 0;
        for (const auto& usterka : usterki) {
            suma += usterka.koszt;
        }
        return suma * mnoznikMarki;
    }

    string getMarka() const override { return marka; }
    string getModel() const override { return model; }
    int getRokProdukcji() const override { return rokProdukcji; }
    string getNumerRejestracyjny() const override { return numerRejestracyjny; }
    string getImieKlienta() const override { return imieKlienta; }
    status_zlecenia getStatus() const override { return status; }
    const vector<Usterka>& getUsterki() const override { return usterki; }
    int getCalkowityCzasNaprawy() const override { return calkowityCzasNaprawy; }

    int pozostalyCzas() const override
    {
        if (status != status_zlecenia::w_naprawie) return 0;

        auto teraz = steady_clock::now();
        auto uplynelo = duration_cast<seconds>(teraz - czasRozpoczeciaNaprawy).count();
        int zostalo = calkowityCzasNaprawy - static_cast<int>(uplynelo);
        return (zostalo > 0) ? zostalo : 0;
    }

    string getStatusString() const override
    {
        switch (status)
        {
        case status_zlecenia::w_naprawie:
            return "W naprawie";
        case status_zlecenia::gotowe_do_odbioru:
            return "Gotowe do odbioru";
        default:
            return "NIEZNANY";
        }
    }

    virtual typ_pojazdu getTypPojazdu() const override = 0;

    string getTypPojazdyString() const override
    {
        switch (getTypPojazdu())
        {
        case typ_pojazdu::samochod: return "Samochod osobowy";
        case typ_pojazdu::motocykl: return "Motocykl";
        case typ_pojazdu::ciezarowka: return "Ciezarowka";
        default: return "Nieznany";
        }
    }

    friend ostream& operator<<(ostream& os, const Pojazd& p) {
        os << p.getTypPojazdyString() << " " << p.marka << " " << p.model << " (" << p.rokProdukcji << "), " << p.numerRejestracyjny;
        return os;
    }
};
class Samochod : public Pojazd
{
public:
    Samochod(const string& mrk, const string& mdl, int rok, const string& nr, const string& imie) : Pojazd(mrk, mdl, rok, nr, imie) {}
    typ_pojazdu getTypPojazdu() const override { return typ_pojazdu::samochod; }
};

class Motocykl : public Pojazd
{
public:
    Motocykl(const string& mrk, const string& mdl, int rok, const string& nr, const string& imie) : Pojazd(mrk, mdl, rok, nr, imie) {}
    typ_pojazdu getTypPojazdu() const override { return typ_pojazdu::motocykl; }
    double obliczCalkowityKoszt(double mnoznikMarki) const override
    {
        return Pojazd::obliczCalkowityKoszt(mnoznikMarki) * 0.75;
    }
};

class Ciezarowka : public Pojazd
{
public:
    Ciezarowka(const string& mrk, const string& mdl, int rok, const string& nr, const string& imie) : Pojazd(mrk, mdl, rok, nr, imie) {}
    typ_pojazdu getTypPojazdu() const override { return typ_pojazdu::ciezarowka; }
    double obliczCalkowityKoszt(double mnoznikMarki) const override
    {
        return Pojazd::obliczCalkowityKoszt(mnoznikMarki) * 2.5;
    }
};
class marki_plik
{
public:
    static vector<InfoMarki> wczytaj_marki(const string& nazwaPliku)
    {
        vector <InfoMarki> marki;
        ifstream plik(nazwaPliku);
        if (!plik.is_open())
        {
            ofstream nowyPlik(nazwaPliku);
            if (!nowyPlik.is_open())
            {
                throw plik_nie_istnieje(nazwaPliku);
            }
            nowyPlik << "Toyota 1.0 0.9\nBMW 1.5 1.3\nMercedes 1.6 1.4\nAudi 1.4 1.2\n";
            nowyPlik << "Ford 1.0 1.0\nVolkswagen 1.2 1.1\nFiat 0.8 0.9\nSkoda 0.9 0.9\n";
            nowyPlik << "Renault 1.0 1.0\nPeugeot 1.1 1.1\nOpel 0.95 0.95\nCitroen 1.0 1.2\n";
            nowyPlik << "Honda 1.1 0.9\nMazda 1.1 1.0\nNissan 1.0 1.0\nMitsubishi 1.0 1.0\n";
            nowyPlik << "Hyundai 0.95 0.9\nKia 0.95 0.9\nSuzuki 0.9 0.8\nSubaru 1.2 1.2\n";
            nowyPlik << "Volvo 1.3 1.2\nLexus 1.6 1.1\nPorsche 2.0 1.5\nJaguar 1.7 1.5\n";
            nowyPlik << "LandRover 1.8 1.6\nAlfaRomeo 1.4 1.5\nJeep 1.3 1.2\nMini 1.4 1.2\n";
            nowyPlik << "Dacia 0.7 0.8\nSeat 1.1 1.0\nChevrolet 1.1 1.1\nTesla 1.5 1.4\n";
            nowyPlik << "Saab 1.2 1.3\nLancia 1.1 1.3\nSmart 1.2 1.1\n";
            nowyPlik.close();
            plik.open(nazwaPliku);
        }

        string nazwa;
        double mnoznikCeny, mnoznikCzasu;

        while (plik >> nazwa >> mnoznikCeny >> mnoznikCzasu)
        {
            try
            {
                InfoMarki marka(nazwa, mnoznikCeny, mnoznikCzasu);
                marki.push_back(marka);
            }
            catch (const nieprawidlowe_dane& e)
            {
                cerr << "Blad wczytywania marki " << nazwa << ": " << e.what() << endl;
            }
        }
        plik.close();
        return marki;
    }
};
class raport_plik
{
public:
    static void zapiszRaport(const string& nazwaPliku,const vector<unique_ptr<IPojazd>>& zlecenia,const vector<unique_ptr<IPojazd>>& historia,const map<string, InfoMarki>& bazaMarek)
    {
        ofstream raport(nazwaPliku);
        if (!raport.is_open()) throw plik_nie_istnieje(nazwaPliku);

        auto teraz = system_clock::now();
        auto czasT = system_clock::to_time_t(teraz);

        raport << "\n"; 
        raport << "-------------------------------------\n";
        raport << "|        RAPORT DZIENNY WARSZTATU    |\n";
        raport << "-------------------------------------\n";
        raport << "Data: " << ctime(&czasT);

        double przychod = 0;
        int gotowe = 0, wNaprawie = 0, usterkiWykryte = 0, usterkiZgloszone = 0;
        raport << "\nZLECENIA:\n---------------------------------------\n";
        int licznik = 1;
        auto przetworzPojazd = [&](const unique_ptr<IPojazd>& pojazd)
            {
                raport << "\n#" << licznik++ << " " << pojazd->getImieKlienta() << "\n";
                raport << pojazd->getMarka() << " " << pojazd->getModel() << "\n";
                raport << "Status: " << pojazd->getStatusString() << "\n";
                raport << "Usterki:\n";
                for (const auto& usterka : pojazd->getUsterki())
                {
                    raport << "    - " << usterka.nazwa;
                    if (usterka.wykryta_przez_mechanika)
                    {
                        raport << " [Wykryta]";
                        usterkiWykryte++;
                    }
                    else
                    {
                        usterkiZgloszone++;
                    }
                    raport << " (" << usterka.koszt << " PLN)\n";
                }

                auto szukaj_po_marce = bazaMarek.find(pojazd->getMarka());
                double mnoznik = (szukaj_po_marce != bazaMarek.end()) ? szukaj_po_marce->second.getMnoznikCeny() : 1.0;
                double koszt = pojazd->obliczCalkowityKoszt(mnoznik);
                raport << "Koszt: " << fixed << setprecision(2) << koszt << " PLN\n";
                przychod += koszt;

                if (pojazd->getStatus() == status_zlecenia::gotowe_do_odbioru) gotowe++;
                else if (pojazd->getStatus() == status_zlecenia::w_naprawie) wNaprawie++;
            };
        for (const auto& p : zlecenia) przetworzPojazd(p);
        for (const auto& p : historia) przetworzPojazd(p);

        raport << "\n-------------------------\n";
        raport << "PODSUMOWANIE:\n";
        raport << "Lacznie zlecen: " << (zlecenia.size() + historia.size()) << "\n";
        raport << "W tym zakonczonych: " << historia.size() << "\n";
        raport << "PRZYCHOD CALKOWITY: " << fixed << setprecision(2) << przychod << " PLN\n";
        raport << "-------------------------\n";
        raport.close();
    }
};
