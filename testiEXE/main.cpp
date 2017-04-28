#include <QApplication>
#include "tietokanta.h"
#include <QDebug>

QTableView *createView(QSqlTableModel *model, const QString &title = "")
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle(title);
    return view;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // luodaan tietokanta-niminen olio heap- eli pinomuistiin
    Tietokanta* tietokanta;
    tietokanta = new Tietokanta();

    // debuggaus paalle
    tietokanta->virheenTarkastus(true);

    // tarkistetaan loytyyko kortti tietokannasta
    qDebug() << "idTili: " << tietokanta->tarkistaKortti("1234"); // parametriksi QString kortin numero. palautuksena tulee idTili tai false

    // palauttaa idTilin jos kortin numero ja pin-koodi tasmaa
    qDebug() <<  "tunnusluku: " << tietokanta->tarkistaPin("1234", 1234); // parametriksi QString kortin numero ja int tunnusluku. palautuksena tulee true/false

    // palauttaa tilin saldon
    qDebug() << "Saldo: " << tietokanta->tarkistaSaldo( tietokanta->tarkistaKortti("1234") ); // parametriksi idTili, joka asadaan tarkitaKortti-funktiolta

    // talletetaan tilille rahaa
    tietokanta->talletaRahaa(1, 100); // parametriksi idTili ja summa
    qDebug() << "Talletetaan rahaa...";
    qDebug() << "Saldo talletuksen jalkeen: " << tietokanta->tarkistaSaldo( tietokanta->tarkistaKortti("1234") );

    // nostetaan tililtä rahaa
    tietokanta->nostaRahaa(1, 100); // parametriksi idTili ja summa
    qDebug() << "Nostetaan rahaa...";
    qDebug() << "Saldo noston jalkeen: " << tietokanta->tarkistaSaldo( tietokanta->tarkistaKortti("1234") );

     /* <-------------- tulostetaan tilitapahtumat alkaa -------------> */
    qDebug() << "Haetaan tilitapahtumat...";
    // luodaan modeli-olio johon haetaan tietokannasta tapahtumat
    QSqlTableModel model;
    tietokanta->haeTilitapahtumat(&model, 1);

    // luodaan table view olio ja esitetaan tieto
    QTableView *view1 = createView(&model, QObject::tr("Tilitapahtumat"));
    view1->show();
    /* <-------------- tulostetaan tilitapahtumat loppuu-------------> */

    // tuhotaan olio
    delete tietokanta;

    return a.exec();
}
