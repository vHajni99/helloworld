/*
 * ============================================================================
 *
 *       Filename:  BT.cpp
 *
 *    Description:  BT class implementation for binary tree application
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
 *            0.3: added deep copy implementation
 *            0.4: added move semantics implementation
 *
 * ============================================================================
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include "BT.h"

// ============================================================================

// Konstruktor
BT::BT() :
  // Létrehozzuk a gyökér csomópontot
  root(createNode('/')),
  // És ráállítjuk az aktuális csomópont mutatót
  currentNode(root),
  // A fa magasságát 0-ra állítjuk
  treeHeight(0)
{
}

// Destruktor
BT::~BT()
{
  // Leellenőrizzük, hogy a gyökér mutató nem üres-e
  // (mozgató konstruktor/értékadás után történik meg, mert ezek a függvények
  // nullptr-re állítják a mozgatott objektum gyökér mutatóját. Ha nullptr-re
  // hívnánk meg a felszabadító függvényt és a delete kulcsszót, szegmentációs
  // hibát eredményezne)
  if (root != nullptr)
  {
    // Az osztály használata után a dinamikusan foglalt memóriaterületet
    // felszabadítjuk
    destroyNode(root->right);
    destroyNode(root->left);

    // ne felejtsük el a gyökeret se felszabadítani (köszi, valgrind)
    delete root;
  }
}

// Másoló konstruktor
BT::BT(const BT& copied) : BT() // konstruktor delegáció, C++11
{
  // Leellenőrizzük, hogy a másolt fa nem üres-e, és csak akkor indítjuk el
  // a másolási procedúrát.
  // Ellenkező esetben üres marad (a konstruktor inicializálta)
  if (copied.root->left != nullptr && copied.root->right != nullptr)
  {
    // Rekurzív másoló függvény hívások jobb és bal oldalra is
    root->right = copyNode(copied.root->right);
    root->left = copyNode(copied.root->left);
  }
}


// Másoló értékadás operátor
BT& BT::operator= (const BT& copied)
{
  // ön-értékadás tesztelése
  if (this == &copied)
    return *this;

  // Ha a fa üres
  if (copied.root->left == nullptr && copied.root->right == nullptr)
    return *this;

  // Rekurzív másoló függvény hívások jobb és bal oldalra is
  root->right = copyNode(copied.root->right);
  root->left = copyNode(copied.root->left);

  // Visszatérünk az objektummal, így lehet láncolni a kifejezéseket
  return *this;
}

// Mozgató konstruktor
BT::BT(BT&& moved) :
  // Nem hívjuk meg a normál konstruktort.
  // A tagokat egyenlővé tesszük az átmozgatott objektum tagjaival
  root(moved.root),
  currentNode(moved.root),
  treeHeight(moved.treeHeight)
{
  // És a mozgatott objektum gyökerét "kinullázzuk"
  moved.root = nullptr;
}

// Mozgató értékadás
BT& BT::operator= (BT&& moved)
{
  // ön-értékadás tesztelése
  if (this == &moved)
    return *this;

  // mivel a mozgató értékadás akkor hívódik meg, ha az objektum már létezett
  // előtte, a konstruktor már lefutott, és így a gyökérelemnek le van
  // foglalva a diamikus memóriában hely. Így ezt törölnünk kell, mert ha nem
  // tennénk meg, nem tudnánk arra a memóriaterületre hivatkozni, ami memória
  // túlcsorduláshoz vezet (köszi, valgrind)
  delete root;

  // A tagokat egyenlővé tesszük az átmozgatott objektum tagjaival
  root = moved.root;
  currentNode = moved.root;
  treeHeight = moved.treeHeight;

  // És a mozgatott objektum gyökerét "kinullázzuk"
  moved.root = nullptr;

  // Visszatérünk az objektummal, így lehet láncolni a kifejezéseket
  return *this;
}

// Rekurzív másoló függvény
BT::Node* BT::copyNode(const Node* Ptr)
{
  // Az if blokkon kívül létrehozunk egy csomópont mutatót. Ha a blokkon belül
  // hoznánk létre, a hatókörből való kilépés után megsemmisülne, és nem
  // tudnánk visszatérni vele
  Node* newNode = nullptr;

  // Ha a csomópont nem üres
  if (Ptr != nullptr)
  {
    // Az elkészített mutatóra meghívjuk a csomópont készítő függvényünket,
    // ami pont egy csomópont mutatóval tér vissza, paraméterként megkapja
    // a másolt fa aktuális csomópontjának értékét
    newNode = createNode(Ptr->value);

    // Majd a rekurzió folytatódik a jobb és bal gyerekeire is
    newNode->right = copyNode(Ptr->right);
    newNode->left = copyNode(Ptr->left);
  }

  // Visszatérünk a csomóponttal
  return newNode;
}

// ============================================================================

// Csomópont készítő függvény
// Az x paramétert referencia szerint adjuk át, hogy ne történjen felesleges
// másolás, ezáltal kicsit növelve programunk teljesítményét. Valamint, hogy
// biztosak legyünk benne, hogy a függvény nem változtatja meg a paramétert,
// konstansként jelöljük meg
BT::Node* BT::createNode(const char& x)
{
  // Dinamikusan lefoglalunk memóriaterületet az új csomópontnak
  Node* newNode = new Node;

  // Átmásoljuk a paraméterben megadott 0 vagy 1-es betűt
  newNode->value = x;

  // A bal és jobb oldali gyermek mutatókat 0-ra állítjuk. Ez azt jelenti,
  // hogy nem mutatnak semmire
  newNode->left = nullptr;
  newNode->right = nullptr;

  // Visszaadjuk a csomópontot
  return newNode;
}

// Csomópont megsemmisítő függvény
// Mivel dinamikusan foglaltunk memóriát a csomópontok számára, így azok
// helyeit a program futásának végén fel is kell szabadítani, hogy ne legyen
// memória-túlcsordulás
void BT::destroyNode(Node* Ptr)
{
  // Ha az aktuális csomópont nem üres
  if (Ptr != nullptr)
  {
    // rekurzív hívás a bal és jobb gyerekekre
    destroyNode(Ptr->right);
    destroyNode(Ptr->left);

    // delete kulcsszó: memória felszabadítás. Ha úgy tetszik, a new
    // ellentettje. Utána még jó szokás a pointereknek null értéket megadni,
    // így biztosan nem mutatnak semmire
    delete Ptr;
    Ptr = nullptr;
  }
}

// Publikus csomópont hozzáadó függvény. Ezt használjuk majd a fába
// pakoláshoz
void BT::addNode(const char& x)
{
  // Egyszerűen meghívjuk a privát csomópont hozzáadó függvényt, az aktuális
  // csomópont mutatót megadva paraméterként. Így nem kell törődnünk a
  // behelyezés helyével, hanem csak magával a karakterrel
  addNodePrivate(x, currentNode);
}

// Túlterheljük a << operátort. Ugyanazt csinálja, mint a fenti függvény, de
// így egyszerűbben lehet majd a fába csomópontokat illeszteni
void BT::operator<< (const char& x)
{
  addNodePrivate(x, currentNode);
}

// Privát csomópont hozzáadó függvény
// Itt már második paraméterként az aktuális csomópont mutatóval is dolgozunk
void BT::addNodePrivate(const char& x, Node* Ptr)
{
  // Ha a behelyezendő karakter 0
  if (x == '0')
  {
    // ha az aktuális csomópont bal oldali gyermek mutatója nem 0 (nem üres)
    if (Ptr->left != nullptr)
    {
      // ráállítjuk az aktuális csomópont mutatót
      currentNode = Ptr->left;
    }
    // ha pedig 0 (üres)
    else
    {
      // a csomópont készítő tagfüggvénnyel új csomópontot hozunk létre
      Ptr->left = createNode(x);

      // és az aktuális csomópont mutatót a gyökér csomópontra állítjuk
      currentNode = root;
    }
  }
  // Ha a behelyezendő karakter 1
  else if (x == '1')
  {
    // Ha az aktuális csomópont jobb oldali gyermek mutatója nem 0 (nem üres)
    if (Ptr->right != nullptr)
    {
      // ráállítjuk az aktuális csomópont mutatót
      currentNode = Ptr->right;
    }
    // Ha pedig 0 (üres)
    else
    {
      // a csomópont készítő tagfügvénnyel új csomópontot hozunk létre
      Ptr->right = createNode(x);

      // és az aktuális csomópont mutatót a gyökér csomópontra állítjuk
      currentNode = root;
    }
  }
}

// ============================================================================

// Getter függvény a gyökér mutatóhoz. Azért van rá szükség, mert a gyökér
// mutató privát, és direktben nem lehet elérni. Így közvetlenül nem tudják
// egyéb osztályok módosítani
BT::Node* BT::getRoot() const
{
  return root;
}

// ============================================================================

// Fa tartalmát kiíró függvény
void BT::printAllNodes(Node* Ptr, std::ostream& output)
{
  // Ha az aktuális csomópont nem üres
  if (Ptr != nullptr)
  {
    // Fa mélységének növelése
    ++treeHeight;

    // Rekurzív hívás az aktuális csomópont jobb oldali (1-es) gyermekére
    printAllNodes(Ptr->right, output);

    // Mikor visszatért a rekurzív hívások után a vezérlés ide, annyiszor
    // 3 db kötőjelet rakunk a kimenetbe, amilyen mély volt a fánk, majd
    // zárójelbe beírjuk a magasság értékét (azért vonunk le 1-et, mert
    // a mélység 0-ról indult)
    for (int i = 0; i < treeHeight; i++)
      output << "---";
    output << Ptr->value << "(" << treeHeight - 1 << ") : " << Ptr << std::endl;

    // Rekurzív hívás az aktuális csomópont bal oldali (0-s) gyermekére
    // ugyanez lejátszódik a bal oldali gyermekekre is
    printAllNodes(Ptr->left, output);

    // Fa mélységének csökkentése
    treeHeight--;
  }
}

// ============================================================================

// Fa mélységét lekérő függvény
int BT::getHeight(Node* Ptr)
{
  // Nullázzuk a fa mélységét és maximum mélységét
  treeHeight = maxTreeHeight = 0;

  // Meghívjuk a fa mélységét számító függvényt
  calculateHeight(Ptr);

  // Visszaadjuk a maximum mélységet (azért vonunk le 1-et, mert 0-ról indult)
  return maxTreeHeight - 1;
}

// fa mélységét számító függvény
void BT::calculateHeight(Node* Ptr)
{
  // Ha az aktuális csomópont nem üres
  if (Ptr != nullptr)
  {
    // Fa mélységének növelése
    ++treeHeight;

    // Ha a fa aktuális mélysége magasabb, mint az éppen beállított maximum
    // mélység, egyenlővé tesszük vele
    if (treeHeight > maxTreeHeight)
    {
      maxTreeHeight = treeHeight;
    }

    // Rekurzívan meghívjuk a jobb és bal gyermekekre is
    calculateHeight(Ptr->right);
    calculateHeight(Ptr->left);

    // Fa mélységének csökkentése
    treeHeight--;
  }
}

// ============================================================================

// Átlagot lekérő függvény
double BT::getAverage(Node* Ptr)
{
  // Nullázzuk az átlagszámításhoz szükséges változóinkat és a fa magasságát
  averageTemp = averageSum = treeHeight = 0;

  // Meghívjuk az átlagot számító függvényt
  calculateAverage(Ptr);

  // Kiszámítjuk az átlagot, double-re tipuskényszerítünk, hogy a törtrész ne
  // vesszen el
  average = double (averageSum) / averageTemp;

  // Visszaadjuk az átlagot
  return average;
}

void BT::calculateAverage(Node* Ptr)
{
  // Ha az aktuális csomópont nem üres
  if (Ptr != nullptr)
  {
    // Fa mélységének növelése
    ++treeHeight;

    // rekurzívan meghívjuk a jobb és bal gyermekekre is
    calculateAverage(Ptr->right);
    calculateAverage(Ptr->left);

    // fa mélységének csökkentése
    treeHeight--;

    // ha az aktuális csomópont bal és jobb oldali gyereke üres, az azt
    // jelenti, hogy az adott ág utolsó csomópontja
    if (Ptr->right == nullptr && Ptr->left == nullptr) {

      // növeljük az átlagszámításhoz szükséges átmeneti változót
      ++averageTemp;

      // növeljük az átlag összeget a fa magasságával
      averageSum += treeHeight;
    }

  }
}

// ============================================================================

// Szórást lekérő függvény
double BT::getVariance(Node* Ptr)
{
  // Lekérjük az átlagot
  average = getAverage(Ptr);

  // Nullázzuk a szükséges változókat
  varianceSum = 0.0;
  averageTemp = treeHeight = 0;

  // Meghívjuk a szórást számító függvényt
  calculateVariance(Ptr);

  // Ha (átlag átmeneti változó - 1) pozitív
  if (averageTemp - 1 > 0)

    // Akkor elosztjuk vele a szórásösszeget, mielőtt gyököt vonunk
    variance = std::sqrt(varianceSum / (averageTemp-1));

    // Egyébként simán gyököt vonunk a szórásösszegből
  else
    variance = std::sqrt(varianceSum);

  // Visszaadjuk a szórást
  return variance;
}

// Szórást számító függvény
void BT::calculateVariance(Node* Ptr)
{
  // Ha az aktuális csomópont nem üres
  if (Ptr != nullptr)
  {
    // Fa mélységének növelése
    ++treeHeight;

    // rekurzívan meghívjuk a jobb és bal gyermekekre is
    calculateVariance(Ptr->right);
    calculateVariance(Ptr->left);

    // fa mélységének csökkentése
    treeHeight--;

    // ha az aktuális csomópont bal és jobb oldali gyereke üres, az azt
    // jelenti, hogy az adott ág utolsó csomópontja
    if (Ptr->right == nullptr && Ptr->left == nullptr) {

      // növeljük az átlagszámításhoz szükséges átmeneti változót
      averageTemp++;

      // növeljük a szórás összegét
      varianceSum += ((treeHeight - average)*(treeHeight - average));
    }

  }
}

// ============================================================================

// Mélységet kiíró függvény
void BT::printHeight(Node* Ptr, std::ostream& output)
{
  output << "Melyseg: " << getHeight(Ptr) << std::endl;
}

// Átlagot kiíró függvény
void BT::printAverage(Node* Ptr, std::ostream& output)
{
  output << "Atlag: " << getAverage(Ptr) << std::endl;
}

// Szórást kiíró függvény
void BT::printVariance(Node* Ptr, std::ostream& output)
{
  output << "Szoras: " << getVariance(Ptr) << std::endl;
}
