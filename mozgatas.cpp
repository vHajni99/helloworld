/*
 * ============================================================================
 *
 *       Filename:  binary_tree.cpp
 *
 *    Description:  Build a binary tree from the human genom
 *
 *        Version:  0.4
 *        Created:  2014-03-23
 *   Last Updated:  2014-05-04
 *
 *         Author:  Zoltan Dalmadi
 *         E-mail:  dmz985@gmail.com
 *
 * ============================================================================
 *
 *      Changelog:
 *
 *            0.1: initial release
 *            0.2: added C++11 features
 *            0.3: added deep copy "test"
 *            0.4: added move semantics "test"
 *
 * ============================================================================
 */

#include <iostream>
#include <fstream>
#include "BT.h"

int main(int argc, const char *argv[])
{
  // ==========================================================================

  // Leellenőrizzük a megadott paramétereket
  // Ha a paraméterek száma nem 3 (programnév + bemeneti file + kimeneti file)
  if (argc != 3)
  {
    // Ha van bemeneti file, de nincs kimeneti
    if (argv[1] && !argv[2])
    {
      // Kiírjuk a helyes használatot a standard error-ra
      std::cerr << "Nincs kimeneti file!" << std::endl \
        << "Hasznalat: " << argv[0] << " bemeneti_file kimeneti_file" \
        << std::endl;

      // és megszakítjuk programunk működését
      return -1;
    }

    // Ha nincs se bemeneti, se kimeneti file
    std::cerr << "Hasznalat: " << argv[0] << " bemeneti_file kimeneti_file" \
      << std::endl;

    // és megszakítjuk programunk működését
    // Azért különbözik a 2 return szám, mert különböző esetekre tudnánk így
    // hivatkozni, ha implementálnánk komolyabb hibekezelő eljárásokat. Ebben
    // a programban nincs rá szükség, de jó szokás külön hibakóddal kezelni
    // különböző hibaeseteket. A hibás eseteket gyakran jelöljük negatív
    // számokkal, mert könnyen felismerhetőek pozitív társaik mellett
    return -2;
  }

  // ==========================================================================

  // Létrehozzuk a bemeneti file-t kezelő objektumunkat
  std::ifstream input(argv[1]);

  // Megvizsgáljuk, hogy a bemeneti file olvasható-e (létezik-e)
  if (!input)
  {
    std::cerr << "Bemeneti file nem olvashato!" << std::endl;
    return -3;
  }

  // Megvizsgáljuk, hogy a bemeneti file üres-e. Erre a beépített ifstream
  // osztály peek() függvényét használjuk, ami annyit csinál, hogy beolvassa
  // a file következő karakterét anélkül, hogy eltárolná. Ha következő
  // (ebben az esetben az első) karakter EOF (End of File, azaz file a végét
  // jelző karakter), akkor a file üres
  if (input.peek() == EOF)
  {
    std::cerr << "Bemeneti file ures!" << std::endl;
    return -4;
  }

  // Létrehozzuk a kimeneti file-t kezelő objektumot is
  std::ofstream output(argv[2]);

  // ==========================================================================

  // Dinamikusan foglalunk memóriát az aktuálisan beolvasott karakternek
  char* currentChar = new char;

  // Létrehozzuk a bináris fa objektumunkat
  BT BinTree;

  // Komment-e az aktuális karakterünk?
  bool isComment = false;

  // Elkezdjük a beolvasási procedúrát
  while(input.read(currentChar, sizeof(char)))
  {
    // Megvizsgáljuk az aktuális karaktert
    switch (*currentChar)
    {
      // Ha >, akkor az egy komment elejét jelenti a file-ban
      case '>':

        // tehát a komment változónkat igazra állítjuk
        isComment = true;

        // és folytatjuk a ciklus következő lépésével,
        // vagyis lépünk a következő karakterre
        continue;

      // Ha sor végekarakterhez értünk, ott vége a kommentnek
      case '\n':

        // tehát a komment változónkat hamisra állítjuk
        isComment = false;

        // és ugyanúgy lépünk a következő karakterre
        continue;

      // Az N betűk nem hordoznak információt, így azokat is átlépjük
      case 'N':
        continue;
    }

    // Ha a komment változónk igaz, az azt jelenti, hogy az aktuális karakter
    // éppen egy komment. Ugyanúgy lépünk a következő karakterre
    // (sorvége karakterig, lásd fentebb)
    if(isComment)
    {
      continue;
    }

    // Az aktuális karakter bitjeit behelyezzük a fába, a túlterhelt <<
    // operátorunk segítségével. Használhatnánk az addNode() függvényt is.
    // Azért 8-ig iterálunk, mert a char adattípus 8 bites
    for (int i = 0; i < 8; i++)
    {
      // Bitmaszkolással a legfelső bitet megvizsgáljuk. A maszk 0x80, azaz
      // 1000_0000. Ha egy bitet 1-el ÉS-elünk, akkor mindig megkapjuk az
      // eredeti bitünket. Így ezzel a módszerrel meg tudjuk állapítani a
      // maszkban az 1-es helyén álló bit pozíciójánál (ami ebben az esetben
      // a legfelső bit) a karakterünk bitjének értékét
      if(*currentChar & 0x80)
      {
        // Ha a logikai ÉS művelet igaz igazságértékkel bírt, az azt jelenti,
        // hogy az aktuális karakter legfelső bitjén 1-es van, tehát a bináris
        // fánkba 1-est kell helyezni
        BinTree << '1';
      }
      else
      {
        // Ellenkező esetben 0 volt a legfelső bit, így 0-t helyezünk el
        BinTree << '0';
      }
      // és az aktuális karakteren biteltolást hajtunk végre balra, hogy a
      // következő bitje kerüljön a legfelső pozícióba
      *currentChar <<= 1;
    }
  }

  // ==========================================================================

  // Mivel a bemeneti file-ra már nincs szükségünk, így lezárjuk azt
  input.close();

  // Meghívjuk a fánk kiíró függvényét
  BinTree.printAllNodes(BinTree.getRoot(), output);

  // Meghívjuk a mélységet kiíró függvényt
  BinTree.printHeight(BinTree.getRoot(), output);

  // Meghívjuk az átlagot kiíró függvényt
  BinTree.printAverage(BinTree.getRoot(), output);

  // Meghívjuk a szórást kiíró függvényt
  BinTree.printVariance(BinTree.getRoot(), output);

  // ========================== MÁSOLÁS "TESZT" ===============================

  // Példányosítunk egy új binfa objektumot
  BT BinTree2;

  // Az egyenlőségjel operátorral meghívódik a másoló értékadás.
  // Mivel előtte példányosítottuk az új objektumot, így az már létezik, és
  // nem a másoló konstruktor hívódik meg, hanem a túlterhelt egyenlőségjel
  // operátorunk.
  // Ha példányosítás közben másolnánk (BT BinTree2 = BinTree), akkor a normál
  // másoló konstruktor hívódna meg, mivel a második objektumot előtte
  // létre kell hozni

  BinTree2 = BinTree;

  // Meghívjuk a másolt objektumra is a kiíró függvényeket.
  // Jól látható a kimeneti file-ban, hogy a fák tartalma megegyezik, azonban
  // minden egyes csomópont címe különböző.
  output << std::endl << "******* MÁSOLAT *******" << std::endl << std::endl;
  BinTree2.printAllNodes(BinTree2.getRoot(), output);
  BinTree2.printHeight(BinTree2.getRoot(), output);
  BinTree2.printAverage(BinTree2.getRoot(), output);
  BinTree2.printVariance(BinTree2.getRoot(), output);

  // ========================= MOZGATÁS "TESZT" ===============================

  // Példányosítunk egy új binfa objektumot
  BT BinTree3;

  // Hasonlóan a másoló konstruktorhoz, ha az objektumunk a mozgatás előtt már
  // létezett, akkor a mozgató értékadás hívódik meg (ami nem más, mint az
  // egyenlőségjel operátorunk egy újabb túlterhelése)
  // Ebben az esetben is, ha példányosítás közben mozgatnánk, akkor a mozgató
  // konstruktort hívná meg.
  // std::move függvény: technikailag a paraméterében megadott balértékből
  // jobbértéket csinál, aminek eredménye a mozgató konstruktor/értékadás
  // meghívódása
  BinTree3 = std::move(BinTree);

  // Meghívjuk a mozgatott objektumra is a kiíró függvényeket.
  // A kimeneten látható, hogy a mozgatott fa csomópontjainak címei megegyeznek
  // az első fánk csomópontjainak címeivel, tehát sikeres volt a mozgatás
  output << std::endl << "******* MOZGATOTT *******" << std::endl << std::endl;
  BinTree3.printAllNodes(BinTree3.getRoot(), output);
  BinTree3.printHeight(BinTree3.getRoot(), output);
  BinTree3.printAverage(BinTree3.getRoot(), output);
  BinTree3.printVariance(BinTree3.getRoot(), output);

  // ==========================================================================

  // Nem használjuk tovább a kimeneti filet, így lezárjuk
  output.close();

  // töröljük az aktuális karaktert tároló változót, illetve az általa
  // használt dinamikusan lefoglalt memóriát felszabadítjuk
  delete currentChar;
  currentChar = nullptr;

  return 0;
}
