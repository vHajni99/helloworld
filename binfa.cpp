// z3a7.cpp
//
// Copyright (C) 2011, 2012, Bátfai Norbert, nbatfai@inf.unideb.hu, nbatfai@gmail.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#include <iostream>		// mert olvassuk a std::cin, írjuk a std::cout csatornákat
#include <cmath>		// mert vonunk gyököt a szóráshoz: std::sqrt
#include <fstream>		// fájlból olvasunk, írunk majd
#include <stdexcept>

class LZWBinFa
{
public:
    LZWBinFa ():fa (&gyoker) {}

    ~LZWBinFa () {
        szabadit(gyoker.egyesGyermek ());
        szabadit(gyoker.nullasGyermek ());
    }

    int cZeros = 0, cOnes = 0;

    void operator<< (char b)
    {
        // Mit kell betenni éppen, '0'-t?
        if (b == '0')
        {
            cZeros ++;
            if (!fa->nullasGyermek ())
            {
                Csomopont *uj = new Csomopont ('0');
                fa->ujNullasGyermek (uj);
                fa = &gyoker;
            }
            else
            {
                fa = fa->nullasGyermek ();
            }
        }

        else
        {
	    cOnes ++;
            if (!fa->egyesGyermek ())
            {
                Csomopont *uj = new Csomopont ('1');
                fa->ujEgyesGyermek (uj);
                fa = &gyoker;
            }
            else
            {
                fa = fa->egyesGyermek ();
            }
        }
    }

    void kiir (void)
    {
        melyseg = 0;
        kiir (&gyoker, std::cout);
    }

    int getMelyseg (void)
    {
        melyseg = maxMelyseg = 0;
        rmelyseg (&gyoker);
        return maxMelyseg - 1;
    }

    double getAtlag (void)
    {
        melyseg = atlagosszeg = atlagdb = 0;
        ratlag (&gyoker);
        atlag = ((double) atlagosszeg) / atlagdb;
        return atlag;
    }

    double getSzoras (void)
    {
        atlag = getAtlag ();
        szorasosszeg = 0.0;
        melyseg = atlagdb = 0;

        rszoras (&gyoker);

        if (atlagdb - 1 > 0)
            szoras = std::sqrt (szorasosszeg / (atlagdb - 1));
        else
            szoras = std::sqrt (szorasosszeg);

        return szoras;
    }

    friend std::ostream & operator<< (std::ostream & os, LZWBinFa & bf)
    {
        bf.kiir (os);
        return os;
    }

    void kiir (std::ostream & os)
    {
        melyseg = 0;
        kiir (&gyoker, os);
    }

private:
    class Csomopont {
    public:
        Csomopont (char b = '/'):betu (b), balNulla (0), jobbEgy (0)
        {
        };
        ~Csomopont ()
        {
        };

        Csomopont *nullasGyermek () const
        {
            return balNulla;
        }

        Csomopont *egyesGyermek () const
        {
            return jobbEgy;
        }

        void ujNullasGyermek (Csomopont * gy)
        {
            balNulla = gy;
        }

        void ujEgyesGyermek (Csomopont * gy)
        {
            jobbEgy = gy;
        }

        char getBetu () const
        {
            return betu;
        }

    private:
        // friend class LZWBinFa; /* mert ebben a valtozatban az LZWBinFa metódusai nem közvetlenül
        // a Csomopont tagjaival dolgoznak, hanem beállító/lekérdező üzenetekkel érik el azokat */

        // Milyen betűt hordoz a csomópont
        char betu;
        // Melyik másik csomópont a bal oldali gyermeke? (a C változatból "örökölt" logika:
        // ha hincs ilyen csermek, akkor balNulla == null) igaz
        Csomopont *balNulla;
        Csomopont *jobbEgy;
        // nem másolható a csomópont (ökörszabály: ha van valamilye a szabad tárban,
        // letiltjuk a másoló konstruktort, meg a másoló értékadást)
        Csomopont (const Csomopont &); //másoló konstruktor
        Csomopont & operator= (const Csomopont &);
    };

    /* Mindig a fa "LZW algoritmus logikája szerinti aktuális" csomópontjára mutat */
    Csomopont *fa;
    // technikai
    int melyseg, atlagosszeg, atlagdb;
    double szorasosszeg;
    // szokásosan: nocopyable
    LZWBinFa (const LZWBinFa &);
    LZWBinFa & operator= (const LZWBinFa &);

    /* Kiírja a csomópontot az os csatornára. A rekurzió kapcsán lásd a korábbi K&R-es utalást... */
    void kiir (Csomopont * elem, std::ostream & os)
    {
        // Nem létező csomóponttal nem foglalkozunk... azaz ez a rekurzió leállítása
        if (elem != NULL)
        {
            ++melyseg;
            kiir (elem->egyesGyermek (), os);
            // ez a postorder bejáráshoz képest
            // 1-el nagyobb mélység, ezért -1
            for (int i = 0; i < melyseg; ++i)
                os << "---";
            os << elem->getBetu () << "(" << melyseg - 1 << ")" << std::endl;
            kiir (elem->nullasGyermek (), os);
            --melyseg;
        }
    }
    void szabadit (Csomopont * elem)
    {
        // Nem létező csomóponttal nem foglalkozunk... azaz ez a rekurzió leállítása
        if (elem != NULL)
        {
            szabadit (elem->egyesGyermek ());
            szabadit (elem->nullasGyermek ());
            // ha a csomópont mindkét gyermekét felszabadítottuk
            // azután szabadítjuk magát a csomópontot:
            delete elem;
        }
    }

protected:
    /* A fában tagként benne van egy csomópont, ez erősen ki van tüntetve, Ő a gyökér: */
    Csomopont gyoker;
    int maxMelyseg;
    double atlag, szoras;

    void rmelyseg (Csomopont * elem)
    {
        if (elem != NULL)
        {
            ++melyseg;
            if (melyseg > maxMelyseg)
                maxMelyseg = melyseg;
            rmelyseg (elem->egyesGyermek ());
            // ez a postorder bejáráshoz képest
            // 1-el nagyobb mélység, ezért -1
            rmelyseg (elem->nullasGyermek ());
            --melyseg;
        }
    }

    void ratlag (Csomopont * elem)
    {
        if (elem != NULL)
        {
            ++melyseg;
            ratlag (elem->egyesGyermek ());
            ratlag (elem->nullasGyermek ());
            --melyseg;
            if (elem->egyesGyermek () == NULL && elem->nullasGyermek () == NULL)
            {
                ++atlagdb;
                atlagosszeg += melyseg;
            }
        }
    }

    void rszoras (Csomopont * elem)
    {
        if (elem != NULL)
        {
            ++melyseg;
            rszoras (elem->egyesGyermek ());
            rszoras (elem->nullasGyermek ());
            --melyseg;
            if (elem->egyesGyermek () == NULL && elem->nullasGyermek () == NULL)
            {
                ++atlagdb;
                szorasosszeg += ((melyseg - atlag) * (melyseg - atlag));
            }
        }
    }
};

void usage (void)
{
    std::cout << "Usage: lzwtree in_file -o out_file" << std::endl;
}

int main (int argc, char *argv[])
{
    try {
        // a kiírás szerint ./lzwtree in_file -o out_file alakra kell mennie, ez 4 db arg:
        if (argc != 4)
        {
            usage ();
            throw std::invalid_argument("arg");
            return -1;
        }

        // "Megjegyezzük" a bemenő fájl nevét
        // char *inFile = *++argv;
        char *inFile = argv[1];

        // a -o kapcsoló jön?
        //if (*((*++argv) + 1) != 'o')
        if(argv[2][1] != 'o')
        {
            usage ();
            return -2;
        }

        // ha igen, akkor az 5. előadásból kimásoljuk a fájlkezelés C++ változatát:
        std::fstream beFile (inFile, std::ios_base::in);

        // fejlesztgetjük a forrást: http://progpater.blog.hu/2011/04/17/a_tizedik_tizenegyedik_labor
        if (!beFile)
        {
            std::cout << inFile << " nem letezik..." << std::endl;
            throw std::ios::failure("file");
            usage ();
            return -3;
        }

        std::fstream kiFile (argv[3], std::ios_base::out);

        unsigned char b;		// ide olvassik majd a bejövő fájl bájtjait
        LZWBinFa binFa;		// s nyomjuk majd be az LZW fa objektumunkba

        // a bemenetet binárisan olvassuk, de a kimenő fájlt már karakteresen írjuk, hogy meg tudjuk
        // majd nézni... :) l. az említett 5. ea. C -> C++ gyökkettes átírási példáit

        while (beFile.read ((char *) &b, sizeof (unsigned char)))
            if (b == 0x0a) {
                break;
                throw std::ios::failure("file");
            }

        bool kommentben = false;

        while (beFile.read ((char *) &b, sizeof (unsigned char)))
        {
            if (b == 0x3e) // '>' karakter
            {
                kommentben = true;
                throw std::ios::failure("file");
                continue;
            }

            if (b == 0x0a) // újsor
            {
                kommentben = false;
                throw std::ios::failure("file");
                continue;
            }

            if (kommentben)
            {
                throw std::ios::failure("file");
                continue;
            }

            if (b == 0x4e)	// 'N' betű
            {
                throw std::ios::failure("file");
                continue;
            }

            for (int i = 0; i < 8; ++i)
            {
                if (b & 0x80) // '1'
                    binFa << '1';
                else
                    binFa << '0';
                b <<= 1;
            }

        }

        kiFile << binFa;		// ehhez kell a globális operator<< túlterhelése

        kiFile << "depth = " << binFa.getMelyseg () << std::endl;
        kiFile << "mean(átlag) = " << binFa.getAtlag () << std::endl;
        kiFile << "var(szórás) = " << binFa.getSzoras () << std::endl;

	std::cout << "0-ák száma: "<< binFa.cZeros << std::endl;
	std::cout << "1-ek száma: "<< binFa.cOnes << std::endl;

        kiFile.close ();
        beFile.close ();

        return 0;
    }

    catch (std::invalid_argument& e) {
        std::cout << "Hiba történt: ";
        std::cout << e.what() << std::endl;
    }

    catch (std::ios::failure& e) {
        std::cout << "Hiba történt: ";
        std::cout << e.what() << std::endl;
    }
}
