#include "tietokanta.h"


Tietokanta::Tietokanta()
{
    // oletuksena debuggaus pois paalta
    debug = false;

    // luodaan yhteys tietokanta
    this->luoYhteys();
}

void Tietokanta::luoYhteys()
{
    // muodostetaan yhteys
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.33.10");
    db.setDatabaseName("pankkiautomaatti");
    db.setUserName("root");
    db.setPassword("1234");

    // tarkistetaan saatiinko yhteys
    if (!db.open())
    {
        if (debug == true)
        qDebug() << "Yhteyttä ei voitu luoda" << endl;
    } else {
        if (debug == true)
        qDebug() << "Yhteys muodostettu tietokantaan!" << endl;
    }
}

int Tietokanta::tarkistaKortti(QString kortti)
{
    // luodaan muuttuja palautettavaksi
    int idTili = 0;

    // valmistellaan ja suoritetaan query
    QSqlQuery query;
    query.prepare("SELECT idTili FROM Tili JOIN Asiakas ON Tili.idAsiakas = Asiakas.idAsiakas WHERE pankkikortinNumero = :kortti");
    query.bindValue(":kortti", kortti);
    query.exec();

    if( query.size() > 0 ) {
        if (debug == true)
            qDebug() << "Kortti loytyi!";
    } else {
        if (debug == true)
        qDebug() << "Korttia ei loytynyt!";
    }
    // haetaan queryn tulokset idTili-muuttujaan
    while (query.next()) {
        QString tulos = query.value(0).toString();
        idTili = tulos.toInt();
    }

    // palautetaan tilinumero
    return idTili;
}

bool Tietokanta::tarkistaPin(QString kortti, int pin)
{
    // luodaan muuttuja palautettavaksi
    int pinTasmaa = false;

    // valmistellaan ja suoritetaan query
    QSqlQuery query;
    query.prepare("SELECT idTili FROM Tili JOIN Asiakas ON Tili.idAsiakas = Asiakas.idAsiakas WHERE Asiakas.pankkikortinNumero = :kortti AND Asiakas.tunnusluku = :tunnusluku");
    query.bindValue(":kortti", kortti);
    query.bindValue(":tunnusluku", pin);
    query.exec();

    // jos saadaan tuloksia niin kortti ja tunnus tasmaa
    if (query.size() > 0) {
        pinTasmaa = true;

        if (debug == true)
            qDebug() << "Tunnusluku tasmaa!";
    } else {
        qDebug() << "Tunnusluku ei tasmaa!";
    }

    // tasmaako pin-koodi tiliin
    return pinTasmaa;
}

float Tietokanta::tarkistaSaldo(int tili)
{
    // luodaan muuttuja palautettavaksi
    float saldo;

    // valmistellaan ja suoritetaan query
    QSqlQuery query;
    query.prepare("SELECT saldo FROM Tili WHERE idTili = :id");
    query.bindValue(":id", tili);
    query.exec();

    // haetaan queryn tulokset saldo-muuttujaan
    while (query.next()) {
        QString tulos = query.value(0).toString();
        saldo = tulos.toFloat();

        if (debug == true)
            qDebug() << saldo;
    }

    // palautetaan saldo
    return saldo;
}

void Tietokanta::haeTilitapahtumat(QSqlTableModel *model, int tili)
{
    // modelin asetukset
    model->setTable("Tilitapahtumat");              // taulu
    QString intToString = QString::number(tili);    // muutetaan tilinumero QStringiksi
    QString idTili = "idTili='"+intToString+"'";    // Muodostetaan filtteri
    model->setFilter(idTili);
    model->select();

    // asetetaan columnien otsikot
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PVM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("TAPAHTUMA"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("KUVAUS"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SUMMA"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("TILI"));
}

bool Tietokanta::nostaRahaa(int tili, float summa)
{
    // luodaan muuttuja, joka ilmoittaa pystytaanko rahaa nostamaan
    bool nostetiinko = false;

    // muuttuja uutta saldoa varten
    float uusiSaldo = 0;

    // tarkistetaan riittääkö tilillä katetta ja nostetaan rahaa jos riittää
    if( summa <= this->tarkistaSaldo(tili) ) {

        // vahennetaan saldostasta atripuuttina saatu summa jos tililla riittaa katetta
        uusiSaldo = ( this->tarkistaSaldo(tili) - summa );

        // valmistellaan ja suoritetaan query
        QSqlQuery query;
        query.prepare("UPDATE Tili SET saldo = :uusiSaldo WHERE idTili = :id");
        query.bindValue(":uusiSaldo", uusiSaldo);
        query.bindValue(":id", tili);
        query.exec();

        // saatiin nostettua joten false muutuu trueksi
        nostetiinko = true;

        // lisataan viela tapahtuma tietokantaan
        QSqlQuery event;
        event.prepare("INSERT INTO Tilitapahtumat values(0, NOW(), 'Nosto', 'Automaattinosto', :summa, :id)");
        event.bindValue(":summa", summa);
        event.bindValue(":id", tili);
        event.exec();

        if (debug == true)
            qDebug() << "Nostettiin " << summa;

     } else {
        if (debug == true)
            qDebug() << "Tililla ei riita kate!";
    }

    // saatiinko nostettua
    return nostetiinko;
}

void Tietokanta::talletaRahaa(int tili, float summa)
{
    // lisataan saldoon atripuuttina saatu summa
    float uusiSaldo = 0;
    uusiSaldo = ( this->tarkistaSaldo(tili) + summa );

    // valmistellaan ja suoritetaan query
    QSqlQuery query;
    query.prepare("UPDATE Tili SET saldo = :uusiSaldo WHERE idTili = :id");
    query.bindValue(":uusiSaldo", uusiSaldo);
    query.bindValue(":id", tili);
    query.exec();

    // lisataan viela tapahtuma tietokantaan
    QSqlQuery event;
    event.prepare("INSERT INTO Tilitapahtumat values(0, NOW(), 'Pano', 'Tilisiirto', :summa, :id)");
    event.bindValue(":summa", summa);
    event.bindValue(":id", tili);
    event.exec();

    if (debug == true)
        qDebug() << "Talletettiin: " << summa;
}

Tietokanta::~Tietokanta()
{
    if (debug == true)
         qDebug()  << "tuhottiin olio";
}

void Tietokanta::virheenTarkastus(bool x)
{
    debug = x;
}
