#ifndef TIETOKANTA_H
#define TIETOKANTA_H

/*
    RAJAPINTALUOKKA TIETOKANTAAN
    Antti Heikkilä TVT16SMO
*/

#include "tietokanta_global.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableView>

class TIETOKANTASHARED_EXPORT Tietokanta
{
    bool debug;                                             // muuttuja debuggaustilalle

public:

    Tietokanta();                                           // muodostin

    void luoYhteys();                                       // luodaan yhteys tietokantaan
    void virheenTarkastus(bool);                            // debuggaus paalle tai pois
    int tarkistaKortti(QString);                            // tarkistaa loytyyko kortti tietokannasta palauttaa idTilin
    bool tarkistaPin(QString, int);                         // ottaa vastaan kortin numeron ja pin-koodin. palauttaa true/false
    float tarkistaSaldo(int);                               // ottaa vastaan idTilin ja palauttaa tilin saldon
    void haeTilitapahtumat(QSqlTableModel *model, int);     // hakee tilitapahtumat modeliin
    bool nostaRahaa(int, float);                            // annetaan idTili ja summa, palauttaa truen:n jos nosto onnistui
    void talletaRahaa(int, float);                          // annetaan idTili ja summa, ei palauta mitaan

    ~Tietokanta();                                          // tuhoaja

};

#endif // TIETOKANTA_H
