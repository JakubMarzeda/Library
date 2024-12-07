#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>

#define MAX_BIBLIOTEKI 100
#define MAX_KSIAZKI 1000
#define MAX_AUTORZY 500

// POMOCNICZA FUNKCJA DO GENEROWANIA UNIKALNEGO ID

int generujId(int obecnyLicznik) {
    return obecnyLicznik + 1;
}

// KLASY JAKO MODELE TABEL(PLIKÓW) ABY LATWIEJ BYLO ZARAZAC

class Autor {
    int id;
    string imie;
    string nazwisko;
    string dataUrodzenia;

    public:
    Autor() : id(0), imie(""), nazwisko(""), dataUrodzenia("") {}

    Autor(int id, string imieAutora, string nazwiskoAutora, string dataUrodzenia)
        : id(id), imie(imieAutora), nazwisko(nazwiskoAutora), dataUrodzenia(dataUrodzenia) {} // lista inicjalizacyjna (krótszy zapis konstruktora)

    void wypiszInformacje() {
        cout << "ID: " << this->id << ", Imie: " << this->imie  << " ,Nazwisko: " << this->nazwisko << " , Data urodzenia: " << this->dataUrodzenia << endl;
    }

    bool czyJestTenSam(string imieAutora, string nazwiskoAutora, string dataUr) {
        return imie == imieAutora && nazwisko == nazwiskoAutora && dataUrodzenia == dataUr;
    }

    int getId() const { // const czyli że funckja nie zmienia stanu obiektu tylko odczyt nie modyfikacja
        return id;
    }

    string getImie() const {
        return imie;
    }

    string getNazwisko() const {
        return nazwisko;
    }

    string getDataUrodzenia() const {
        return dataUrodzenia;
    }
};

class Ksiazka {
    int id;
    string tytul;
    string dataWydania;
    int idBiblioteki;
    int idAutora;

    public:
    Ksiazka() : id(0), tytul(""), dataWydania(""), idBiblioteki(0), idAutora(0) {}

    Ksiazka(int id, string tytulKsiazki, string dataWydania, int idBiblioteki, int idAutora)
        : id(id), tytul(tytulKsiazki), dataWydania(dataWydania), idBiblioteki(idBiblioteki), idAutora(idAutora) {} // lista inicjalizacyjna

    void wypiszInformacje() {
        cout << "ID: " << this->id << " ,Tyuł książki: " << this->tytul << ", Data wydania: " << this->dataWydania << endl;
    }

    int getId() const {
        return id;
    }

    string getDataWydania() const {
        return dataWydania;
    }

    string getTytul() const {
        return tytul;
    }

    int getIdBiblioteki() const {
        return idBiblioteki;
    }

    int getIdAutora() const {
        return idAutora;
    }
};

class Biblioteka {
    int id;
    string nazwa;
    string ulica;

    public:
    Biblioteka() : id(0), nazwa(""), ulica("") {}

    Biblioteka(int id, string nazwaBiblioteki, string ulica)
        : id(id), nazwa(nazwaBiblioteki), ulica(ulica) {} // lista inicjalizacyjna

    void wypiszInformacje() {
        cout << "ID: " << this->id << " ,Nazwa: " << this->nazwa << " ,Ulica: " << this->ulica << endl;
    }

    int getId() const {
        return id;
    }

    string getNazwa() const {
        return nazwa;
    }

    string getUlica() const {
        return ulica;
    }
};

// WCZYTYWANIE

void wczytajBiblioteki(const char* nazwaPliku, Biblioteka biblioteki[], int& licznikBibliotek) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Nie mozna otworzyc pliku!" << nazwaPliku << endl;
        return;
    }

    string linia;
    licznikBibliotek = 0;

    while (getline(plik, linia)) {
        stringstream ss(linia);
        string idStr, nazwa, ulica;
        if (getline(ss, idStr, '|') && getline(ss, nazwa, '|') && getline(ss, ulica)) {
            int id = stoi(idStr); // funkcja stoi konwertuje id ktore jest stringiem na int
            biblioteki[licznikBibliotek] = Biblioteka(id, nazwa, ulica);
            licznikBibliotek++;

            if (licznikBibliotek >= MAX_BIBLIOTEKI) {
                cerr << "Osiągnięto maksymalną liczbę bibliotek: " << MAX_BIBLIOTEKI << endl;
                break;
            }
        } else {
            cerr << "Błąd w formacie danych w linii" << linia << endl;
        }
    }
    plik.close();
}

// const char* nie mozna modyfikowac zawartosci na ktora wskazuje wskaznik czyli funckja nie zmienia nazwy pliku
// int& licznik ksiazek - umozliwia modyfikacje zmiennej poza jej zakresem czyli nie tworzy kopii i zmiany są widoczne na zewnątrz
void wczytajKsiazki(const char* nazwaPliku, Ksiazka ksiazki[], int& licznikKsiazek) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Nie można otworzyć pliku: " << nazwaPliku << endl;
        return;
    }

    string linia;
    licznikKsiazek = 0;

    while (getline(plik, linia)) {
        stringstream ss(linia);
        string idStr, tytul, dataWydania, idBibliotekiStr, idAutoraStr;

        if (getline(ss, idStr, '|') && getline(ss, tytul, '|') && getline(ss, dataWydania, '|') &&
            getline(ss, idBibliotekiStr, '|') && getline(ss, idAutoraStr)) {
            int id = stoi(idStr);
            int idBiblioteki = stoi(idBibliotekiStr);
            int idAutora = stoi(idAutoraStr);
            ksiazki[licznikKsiazek] = Ksiazka(id, tytul, dataWydania, idBiblioteki, idAutora);
            licznikKsiazek++;

            if (licznikKsiazek >= MAX_KSIAZKI) {
                cerr << "Osiągnięto maksymalną liczbę książek " << MAX_KSIAZKI << endl;
                break;
            }
        } else {
            cerr << "Błąd w formacie danych w linii: " << linia << endl;
        }
    }

    plik.close();
};

void wczytajAutorow(const char* nazwaPliku, Autor autorzy[], int& licznikAutorow) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Nie można otworzyć pliku: " << nazwaPliku << endl;
        return;
    }

    string linia;
    licznikAutorow = 0;
    while (getline(plik, linia)) {
        stringstream ss(linia);
        string idStr, imie, nazwisko, dataUrodzenia;

        if (getline(ss, idStr, '|') && getline(ss, imie, '|') && getline(ss, nazwisko, '|') && getline(ss, dataUrodzenia)) {
            int id = stoi(idStr);
            autorzy[licznikAutorow] = Autor(id, imie, nazwisko, dataUrodzenia);
            licznikAutorow++;

            if (licznikAutorow >= MAX_AUTORZY) {
                cerr << "Osiągnięto maksymalną liczbę książek " << MAX_KSIAZKI << endl;
                break;
            }
        } else {
            cerr << "Błąd w formacie danych w linii: " << linia << endl;
        }
    }
    plik.close();
}

// DODAWANIE OBIEKTOW

void dodajBiblioteke(Biblioteka biblioteki[], int& licznikBibliotek) {
    if (licznikBibliotek >= MAX_BIBLIOTEKI) {
        cerr << "Nie można dodać nowej biblioteki. Osiągnięto maksymalną liczbę bibliotek: " << MAX_BIBLIOTEKI << endl;
        return;
    }

    string nazwa, ulica;

    cout << "Podaj nazwę biblioteki: ";
    getline(cin, nazwa);
    cout << "Podaj ulicę biblioteki: ";
    getline(cin, ulica);

    int id = generujId(licznikBibliotek);
    biblioteki[licznikBibliotek] = Biblioteka(id, nazwa, ulica);
    licznikBibliotek++;
    cout << "Dodano nową bibliotekę o ID " << id << " i nazwie \"" << nazwa << "\"." << endl;
}

void dodajKsiazke(Ksiazka ksiazki[], int& licznikKsiazek, Biblioteka biblioteki[], int licznikBibliotek, Autor autorzy[], int licznikAutorow) {
    if (licznikKsiazek >= MAX_KSIAZKI) {
        cerr << "Nie można dodać nowej książki. Osiągnięto maksymalną liczbę książek: " << MAX_KSIAZKI << endl;
        return;
    }

    string tytul, dataWydania;
    int idBiblioteki, idAutora;

    cout << "Podaj tytuł książki: ";
    getline(cin, tytul);
    cout << "Podaj datę wydania książki (np. 2024-06-12): ";
    getline(cin, dataWydania);

    bool bibliotekaZnaleziono = false;
    do {
        cout << "Podaj ID biblioteki, w której znajduje się książa: ";
        cin >> idBiblioteki;
        cin.ignore();

        for (int i = 0; i < licznikBibliotek; i++) {
            if (biblioteki[i].getId() == idBiblioteki) {
                bibliotekaZnaleziono = true;
                break;
            }
        }
        if (!bibliotekaZnaleziono) {
            cerr << "Nie znaleziono biblioteki o ID: " << idBiblioteki << ". Spróbuj ponownie." << endl;
        }
    } while(!bibliotekaZnaleziono);

    bool autorZnaleziono = false;
    do {
        cout << "Podaj ID autora książki: ";
        cin >> idAutora;
        cin.ignore();

        for (int i = 0; i < licznikAutorow; i++) {
            if (autorzy[i].getId() == idAutora) {
                autorZnaleziono = true;
                break;
            }
        }

        if (!autorZnaleziono) {
            cerr << "Nie znaleziono autora o ID: " << idAutora << ". Spróbuj ponownie." << endl;
        }
    } while(!autorZnaleziono);

    int idKsiazki = generujId(licznikKsiazek);

    ksiazki[licznikKsiazek] = Ksiazka(idKsiazki, tytul, dataWydania, idBiblioteki, idAutora);

    licznikKsiazek++;
    cout << "Dodano nową książkę o ID " << idKsiazki << " i tytule \"" << tytul << "\"." << endl;
}

void dodajAutora(Autor autorzy[], int& licznikAutorow) {
    if (licznikAutorow >= MAX_AUTORZY) {
        cerr << "Nie można dodać nowego autora. Osiągnięto maksymalną liczbę autorów: " << MAX_AUTORZY << endl;
        return;
    }

    string imie, nazwisko, dataUrodzenia;
    cout << "Podaj imie autora: ";
    getline(cin, imie);
    cout << "Podaj nazwisko autora: ";
    getline(cin, nazwisko);
    cout << "Podaj data rodzenia: ";
    getline(cin, dataUrodzenia);

    int id = generujId(licznikAutorow);
    autorzy[licznikAutorow] = Autor(id, imie, nazwisko, dataUrodzenia);
    licznikAutorow++;
    cout << "Dodano nowego autora o ID " << id << " - " << imie << " " << nazwisko << "." << endl;
}

// WYSZUKIWANIE
void znajdzKsiazkiWBibliotekach(const string& tytul, Ksiazka ksiazki[], int licznikKsiazek, Biblioteka biblioteki[], int licznikBibliotek) {
    bool znaleziona = false;
    for (int i = 0; i < licznikKsiazek; i++) {
        if (ksiazki[i].getTytul() == tytul) {
            for (int j = 0; j < licznikBibliotek; j++) {
                if (ksiazki[i].getIdBiblioteki() == biblioteki[j].getId()) {
                    cout << "Znaleziono książkę \"" << tytul << "\" w bibliotece: " << biblioteki[j].getNazwa() << endl;
                    ksiazki[i].wypiszInformacje();
                    znaleziona = true;
                }
            }
        }
    }

    if (!znaleziona) {
        cout << "Nie znaleziono książek o tytule \"" << tytul << "\" w żadnej bibliotece: " << endl;
    }
}

void znajdzKsiazkiAutora(const string& imie, const string& nazwisko, const string& dataUrodzenia, Ksiazka ksiazki[], int licznikKsiazek, Autor autorzy[], int licznikAutorow) {
    bool znaleziono = false;
    for (int i = 0; i < licznikKsiazek; i++) {
        int idAutora = ksiazki[i].getIdAutora();
        for (int j = 0; j < licznikAutorow; j++) {
            if (autorzy[j].getId() == idAutora && autorzy[j].czyJestTenSam(imie, nazwisko, dataUrodzenia)) {
                cout << "Znaleziono książkę autorstwa " << imie << " " << nazwisko << " (" << dataUrodzenia << ")." << endl;
                ksiazki[i].wypiszInformacje();
                znaleziono = true;
            }
        }
    }

    if (!znaleziono) {
        cout << "Nie znaleziono książek tego autora." << endl;
    }
}

void wypiszKsiazkiWBibliotece(int idBiblioteka, Ksiazka ksiazki[], int licznikKsiazek) {
    bool znaleziono = false;

    for (int i = 0; i < licznikKsiazek; i++) {
        if (ksiazki[i].getIdBiblioteki() == idBiblioteka) {
            ksiazki[i].wypiszInformacje();
            znaleziono = true;
        }
    }

    if (!znaleziono) {
        cout << "Nie znaleziono książek w tej bibliotece: " << endl;
    }
}

// ZAPIS DO PLIKOW

void zapiszBiblioteki(const char* nazwaPliku, Biblioteka biblioteki[], int licznikBibliotek) {
    ofstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Nie można otworzyć pliku do zapisu!" << endl;
        return;
    }

    for(int i = 0; i < licznikBibliotek; i++) {
        plik << biblioteki[i].getId() << "|" << biblioteki[i].getNazwa() << "|" << biblioteki[i].getUlica() << endl;
    }

    plik.close();
}

void zapiszKsiazki(const char* nazwaPliku, Ksiazka ksiazki[], int licznikKsiazek) {
    ofstream plik(nazwaPliku);

    if (!plik.is_open()) {
        cerr << "Nie można otworzyć pliku do zapisu!" << endl;
        return;
    }

    for (int i = 0; i < licznikKsiazek; i++) {
        plik << ksiazki[i].getId() << "|" << ksiazki[i].getTytul() << "|" << ksiazki[i].getDataWydania() << "|" << ksiazki[i].getIdBiblioteki() << "|" << ksiazki[i].getIdAutora() << endl;
    }

    plik.close();
}

void zapiszAutorow(const char* nazwaPliku, Autor autorzy[], int licznikAutorow) {
    ofstream plik(nazwaPliku);

    if (!plik.is_open()) {
        cerr << "Nie można otworzyć pliku do zapisu!" << endl;
        return;
    }

    for (int i = 0; i < licznikAutorow; i++) {
        plik << autorzy[i].getId() << "|" << autorzy[i].getImie() << "|" << autorzy[i].getNazwisko() << "|" << autorzy[i].getDataUrodzenia() << endl;
    }

    plik.close();
}

void wyswietlMenu() {
    cout << "\n--- MENU ---\n";
    cout << "1. Dodaj bibliotekę\n";
    cout << "2. Dodaj książkę\n";
    cout << "3. Dodaj autora\n";
    cout << "4. Wyszukaj książkę w bibliotekach\n";
    cout << "5. Wyszukaj książki autora\n";
    cout << "6. Wypisz książki w wybranej bibliotece\n";
    cout << "7. Zapisz dane do plików\n";
    cout << "8. Zakończ program\n";
}

int main() {
    Autor autorzy[MAX_AUTORZY];
    Ksiazka ksiazki[MAX_KSIAZKI];
    Biblioteka biblioteki[MAX_BIBLIOTEKI];

    int licznikAutorow = 0;
    int licznikKsiazek = 0;
    int licznikBibliotek = 0;

    wczytajAutorow("autorzy.txt", autorzy, licznikAutorow);
    wczytajKsiazki("ksiazki.txt", ksiazki, licznikKsiazek);
    wczytajBiblioteki("biblioteki.txt", biblioteki, licznikBibliotek);

    bool dzialaj = true;

    while (dzialaj) {
        wyswietlMenu();

        int wybor;
        cout << "Wybierz opcję: ";
        cin >> wybor;
        cin.ignore();

        switch (wybor) {
            case 1: {
                dodajBiblioteke(biblioteki, licznikBibliotek);
                break;
            }

            case 2: {
                dodajKsiazke(ksiazki, licznikKsiazek, biblioteki, licznikBibliotek, autorzy, licznikAutorow);
                break;
            }

            case 3: {
                dodajAutora(autorzy, licznikAutorow);
                break;
            }

            case 4: {
                string tytul;
                cout << "Podaj tytul książki do wyszukania: ";
                getline(cin, tytul);
                znajdzKsiazkiWBibliotekach(tytul, ksiazki, licznikKsiazek, biblioteki, licznikBibliotek);
                break;
            }

            case 5: {
                string imie, nazwisko, dataUrodzenia;
                cout << "Podaj imie: ";
                getline(cin, imie);
                cout << "Podaj nazwisko: ";
                getline(cin, nazwisko);
                cout << "Podaj dataUrodzenia: ";
                getline(cin, dataUrodzenia);
                znajdzKsiazkiAutora(imie, nazwisko, dataUrodzenia, ksiazki, licznikKsiazek, autorzy, licznikAutorow);
                break;
            }

            case 6: {
                cout << "Lista bibliotek: " << endl;
                for (int i = 0; i < licznikAutorow; i++) {
                    biblioteki[i].wypiszInformacje();
                }
                int idBiblioteki;
                cout << "Podaj ID biblioteki, dla której chcesz wypisać książki: ";
                cin >> idBiblioteki;
                cin.ignore();
                wypiszKsiazkiWBibliotece(idBiblioteki, ksiazki, licznikKsiazek);
                break;
            }

            case 7: {
                zapiszAutorow("autorzy.txt", autorzy, licznikAutorow);
                zapiszKsiazki("ksiazki.txt", ksiazki, licznikKsiazek);
                zapiszBiblioteki("biblioteki.txt", biblioteki, licznikBibliotek);
                cout << "Dane zapisano do plików." << endl;
                break;
            }

            case 8: {
                dzialaj = false;
                cout << "Zakończenie programu." << endl;
                break;
            }

            default: {
                cerr << "Nieprawidłowy wybór. Spróbuj ponownie." << endl;
            }
        }
    }
    return 0;
}