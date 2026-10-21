#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;
//ata sınıf
class DevreElemani {
public:
    virtual void matriseGom(double** matris, double* akimlar) = 0; //ana kural
    virtual int mnaBoyutKatkisi() {
        return 0;
    }
    virtual void setMnaIndeks(int indeks) {}
    virtual void zamanIlerlet(double* voltajlar) {}
};
//direnc
class Direnc : public DevreElemani {
public:
    double ohm;
    int dugum1;
    int dugum2;

    Direnc(double _ohm, int _d1, int _d2) {
        ohm = _ohm;
        dugum1 = _d1;
        dugum2 = _d2;
    }
    void matriseGom(double** matris, double* akimlar) override {
        if (ohm == 0) return;
        double G = 1.0 / ohm;
        if (dugum1) matris[dugum1 - 1][dugum1 - 1] += G;
        if (dugum2) matris[dugum2 - 1][dugum2 - 1] += G;
        if (dugum1 && dugum2) {
            matris[dugum1 - 1][dugum2 - 1] -= G;
            matris[dugum2 - 1][dugum1 - 1] -= G;
        }
    }
};
//pil
class VoltajKaynagi : public DevreElemani {
public:
    double voltaj;
    int dugumArti;
    int dugumEksi;
    int mnaIndeks;

    VoltajKaynagi(double v, int dArti, int dEksi) {
        voltaj = v;
        dugumArti = dArti;
        dugumEksi = dEksi;
        mnaIndeks = 0;
    }
    int mnaBoyutKatkisi()override { return 1; }
    void setMnaIndeks(int indeks)override {
        mnaIndeks = indeks;
    }
    void matriseGom(double** matris, double* akimlar) override {
        if (dugumArti) matris[mnaIndeks][dugumArti - 1] += 1.0;
        if (dugumEksi) matris[mnaIndeks][dugumEksi - 1] -= 1.0;
        if (dugumArti) matris[dugumArti - 1][mnaIndeks] += 1.0;
        if (dugumEksi) matris[dugumEksi - 1][mnaIndeks] -= 1.0;
        akimlar[mnaIndeks] = voltaj;
    }
};
//akim kaynagi
class AkimKaynagi : public DevreElemani {
public:
    double amper;
    int dugumGiren;
    int dugumCikan;

    AkimKaynagi(double a, int dGiren, int dCikan) {
        amper = a;
        dugumGiren = dGiren;
        dugumCikan = dCikan;
    }
    void matriseGom(double** matris, double* akimlar) override {
        if (dugumGiren) {
            akimlar[dugumGiren - 1] += amper;
        }
        if (dugumCikan) {
            akimlar[dugumCikan - 1] -= amper;
        }
    }
};
class ACKaynak : public DevreElemani {
public:
    int dugum1, dugum2;
    double genlik, frekans, dt;
    double icZaman = 0.0, guncelVoltaj = 0.0;

    ACKaynak(double g, int d1, int d2, double f, double _dt) {
        genlik = g;
        dugum1 = d1;
        dugum2 = d2;
        frekans = f;
        dt = _dt;
    }
    void zamanIlerlet(double* voltajlar)override {
        icZaman += dt;
        guncelVoltaj = genlik * sin(2 * 3.14159 * frekans * icZaman);
    }
    void matriseGom(double** matris, double* akimlar)override {
        double sanalG = 1000000000.0;
        if (dugum1 != 0) {
            matris[dugum1 - 1][dugum1 - 1] += sanalG;
            akimlar[dugum1 - 1] += guncelVoltaj * sanalG;
        }
        if (dugum2 != 0) {
            matris[dugum2 - 1][dugum2 - 1] += sanalG;
            akimlar[dugum2 - 1] -= guncelVoltaj * sanalG;
        }
        if (dugum1 && dugum2 != 0) {
            matris[dugum1 - 1][dugum2 - 1] -= sanalG;
            matris[dugum2 - 1][dugum1 - 1] -= sanalG;
        }
    }
};
//kapasitor
class Kapasitor : public DevreElemani {
public:
    double farad;
    int dugum1, dugum2;
    double dt;                 //delta t
    double eskiVoltaj;

    Kapasitor(double _f, int _d1, int _d2, double _dt) {
        farad = _f;
        dugum1 = _d1;
        dugum2 = _d2;
        dt = _dt;
        eskiVoltaj = 0.0;
    }
    void matriseGom(double** matris, double* akimlar) override {
        double G = farad / dt;
        if (dugum1)matris[dugum1 - 1][dugum1 - 1] += G;
        if (dugum2)matris[dugum2 - 1][dugum2 - 1] += G;
        if (dugum1 && dugum2) {
            matris[dugum1 - 1][dugum2 - 1] -= G;
            matris[dugum2 - 1][dugum1 - 1] -= G;
        }
        double sanalAkim = G * eskiVoltaj;
        if (dugum1) { akimlar[dugum1 - 1] += sanalAkim; }
        if (dugum2) { akimlar[dugum2 - 1] -= sanalAkim; }
    }
    void zamanIlerlet(double* voltajlar)override {
        double v1 = 0.0, v2 = 0.0;
        if (dugum1 != 0) {
            v1 = voltajlar[dugum1 - 1];
        }
        if (dugum2 != 0) {
            v2 = voltajlar[dugum2 - 1];
        }
        eskiVoltaj = v1 - v2;
    }
};
//bobin
class Bobin : public DevreElemani {
public:
    double henry;
    int dugum1, dugum2;
    double dt;
    double eskiAkim;

    Bobin(double _h, int _d1, int _d2, double _dt) {
        henry = _h;
        dugum1 = _d1;
        dugum2 = _d2;
        dt = _dt;
        eskiAkim = 0.0;
    }
    void matriseGom(double** matris, double* akimlar) override {
        double G = dt / henry;
        if (dugum1)matris[dugum1 - 1][dugum1 - 1] += G;
        if (dugum2)matris[dugum2 - 1][dugum2 - 1] += G;
        if (dugum1 && dugum2) {
            matris[dugum1 - 1][dugum2 - 1] -= G;
            matris[dugum2 - 1][dugum1 - 1] -= G;
        }
        if (dugum1) { akimlar[dugum1 - 1] -= eskiAkim; }
        if (dugum2) { akimlar[dugum2 - 1] += eskiAkim; }
    }
    void zamanIlerlet(double* voltajlar) override {
        double v1 = 0, v2 = 0;
        if (dugum1 != 0) {
            v1 = voltajlar[dugum1 - 1];
        }
        if (dugum2 != 0) {
            v2 = voltajlar[dugum2 - 1];
        }
        eskiAkim += ((v1 - v2) / henry) * dt;;
    }
};
double* matrisCozucu(double** matris, double* akimlar, int boyut) {
    for (int k = 0; k < boyut - 1; k++) {
        for (int i = k + 1; i < boyut; i++) {
            if (matris[k][k] == 0) continue;
            double carpan = matris[i][k] / matris[k][k];
            for (int j = k; j < boyut; j++) {
                matris[i][j] -= (carpan * matris[k][j]);
            }
            akimlar[i] -= carpan * akimlar[k];
        }
    }
    double* voltajlar = new double[boyut];
    for (int i = boyut - 1; i >= 0; i--) {
        double sagTaraf = akimlar[i];
        for (int j = i + 1; j < boyut; j++) {
            sagTaraf -= matris[i][j] * voltajlar[j];
        }
        if (matris[i][i] == 0) {
            voltajlar[i] = 0;
        }
        else {
            voltajlar[i] = sagTaraf / matris[i][i];
        }
    }

    cout << "\n========================================" << endl;
    cout << "        SIMULASYON SONUCLARI            " << endl;
    cout << "========================================" << endl;
    for (int i = 0; i < boyut; i++) {
        cout << "V [" << i << "] = " << setprecision(4) << voltajlar[i] << endl;
    }
    cout << "========================================" << endl;
    return voltajlar;
}

int main() {
    vector<DevreElemani*> devre;
    int toplamDugumSayisi = 0;
    int secim;

    cout << "=== MINI DEVRE SIMULATORU ===" << endl;

    while (true) {
        cout << "\n[ANA MENU]" << endl;
        cout << "1. Direnc Ekle" << endl;
        cout << "2. Kapasitor Ekle" << endl;
        cout << "3. Bobin Ekle" << endl;
        cout << "4. Voltaj Kaynagi (Pil) Ekle" << endl;
        cout << "5. AC Kaynagi Ekle" << endl;
        cout << "6. Akim Kaynagi Ekle" << endl;
        cout << "7. Devre Durumunu Goruntule" << endl;
        cout << "9. Simule Et " << endl;
        cout << "10. Programdan Cik" << endl;
        cout << "Seciminiz: ";
        cin >> secim;

        if (secim == 1) {
            double ohm;
            int d1, d2;
            cout << "Direnc Degeri (Ohm): ";
            cin >> ohm;
            cout << "1. Baglandigi Dugum (Toprak icin 0): ";
            cin >> d1;
            cout << "2. Baglandigi Dugum (Toprak icin 0): ";
            cin >> d2;

            if (d1 > toplamDugumSayisi) toplamDugumSayisi = d1;
            if (d2 > toplamDugumSayisi) toplamDugumSayisi = d2;

            devre.push_back(new Direnc(ohm, d1, d2));
            cout << "-> Direnc eklendi!" << endl;
        }
        else if (secim == 2) {
            double farad, dt;
            int d1, d2;
            cout << "Kapasitor Degeri (Farad): ";
            cin >> farad;
            cout << "Zaman adimi (dt) ne olsun?: ";
            cin >> dt;
            cout << "1. Baglandigi Dugum (Toprak icin 0): ";
            cin >> d1;
            cout << "2. Baglandigi Dugum (Toprak icin 0): ";
            cin >> d2;

            if (d1 > toplamDugumSayisi) toplamDugumSayisi = d1;
            if (d2 > toplamDugumSayisi) toplamDugumSayisi = d2;

            devre.push_back(new Kapasitor(farad, d1, d2, dt));
            cout << "-> Kapasitor eklendi!" << endl;
        }
        else if (secim == 3) {
            double henry, dt;
            int d1, d2;
            cout << "Bobin Degeri (Henry): ";
            cin >> henry;
            cout << "Zaman adimi (dt) ne olsun?: ";
            cin >> dt;
            cout << "1. Baglandigi Dugum (Toprak icin 0): ";
            cin >> d1;
            cout << "2. Baglandigi Dugum (Toprak icin 0): ";
            cin >> d2;

            if (d1 > toplamDugumSayisi) toplamDugumSayisi = d1;
            if (d2 > toplamDugumSayisi) toplamDugumSayisi = d2;

            devre.push_back(new Bobin(henry, d1, d2, dt));
            cout << "-> Bobin eklendi!" << endl;
        }
        else if (secim == 4) {
            double v;
            int dArti, dEksi;
            cout << "Voltaj Degeri (Volt): ";
            cin >> v;
            cout << "Arti Uc Dugum Numarasi: ";
            cin >> dArti;
            cout << "Eksi Uc Dugum Numarasi (Toprak icin 0): ";
            cin >> dEksi;

            if (dArti > toplamDugumSayisi) toplamDugumSayisi = dArti;
            if (dEksi > toplamDugumSayisi) toplamDugumSayisi = dEksi;

            devre.push_back(new VoltajKaynagi(v, dArti, dEksi));
            cout << "-> Voltaj kaynagi eklendi!" << endl;
        }
        else if (secim == 5) {
            double genlik, frekans, dt;
            int d1, d2;
            cout << "Genlik Degerini Giriniz: ";
            cin >> genlik;
            cout << "Frekans Degerini Giriniz: ";
            cin >> frekans;
            cout << "Zaman adimi (dt): ";
            cin >> dt;
            cout << "1. Dugum Numarasi: ";
            cin >> d1;
            cout << "2. Dugum Numarasi: ";
            cin >> d2;

            if (d1 > toplamDugumSayisi) toplamDugumSayisi = d1;
            if (d2 > toplamDugumSayisi) toplamDugumSayisi = d2;

            devre.push_back(new ACKaynak(genlik, d1, d2, frekans, dt));
            cout << "-> AC Kaynak eklendi!" << endl;
        }
        else if (secim == 6) {
            double a;
            int dGiren, dCikan;
            cout << "Akim Degeri (Amper): ";
            cin >> a;
            cout << "Giren Dugum Numarasi: ";
            cin >> dGiren;
            cout << "Cikan Dugum Numarasi: ";
            cin >> dCikan;

            if (dGiren > toplamDugumSayisi) toplamDugumSayisi = dGiren;
            if (dCikan > toplamDugumSayisi) toplamDugumSayisi = dCikan;

            devre.push_back(new AkimKaynagi(a, dGiren, dCikan));
            cout << "-> Akim kaynagi eklendi!" << endl;
        }
        else if (secim == 7) {
            cout << "\n--- DEVRE DURUMU ---" << endl;
            cout << "Mevcut En Buyuk Dugum Sayisi: " << toplamDugumSayisi << endl;
            cout << "Toplam Devre Elemani Sayisi: " << devre.size() << endl;
        }
        else if (secim == 9) {
            if (toplamDugumSayisi == 0) {
                cout << "Devrede hic eleman yok. Once eleman ekleyin!" << endl;
                continue;
            }
            double toplamSure, dt;
            cout << "Simulasyon kac saniye sursun?: ";
            cin >> toplamSure;
            cout << "Simulasyon zaman adimi (dt) nedir?: ";
            cin >> dt;

            int boyut = toplamDugumSayisi;
            for (size_t i = 0; i < devre.size(); i++) {
                if (devre[i]->mnaBoyutKatkisi() > 0) {
                    devre[i]->setMnaIndeks(boyut);
                    boyut++;
                }
            }
            cout << "Matris Boyutu: " << boyut << "x" << boyut << endl;

            double** matris = new double* [boyut];
            for (int i = 0; i < boyut; i++) {
                matris[i] = new double[boyut]();
            }
            double* akimlar = new double[boyut]();

            ofstream dosya("osiloskop.csv");
            dosya << "Zaman,V1,V2\n";

            cout << "Simulasyon basladi..." << endl;

            for (double t = 0; t < toplamSure; t += dt) {
                for (int i = 0; i < boyut; i++) {
                    akimlar[i] = 0.0;
                    for (int j = 0; j < boyut; j++) {
                        matris[i][j] = 0.0;
                    }
                }
                for (size_t i = 0; i < devre.size(); i++) {
                    devre[i]->matriseGom(matris, akimlar);
                }
                double* voltajlar = matrisCozucu(matris, akimlar, boyut);

                dosya << t << "," << voltajlar[0] << "," << voltajlar[1] << "\n";

                cout << "t=" << t << " saniye -> ";
                for (int i = 0; i < boyut; i++) {
                    cout << "V" << i + 1 << ": " << voltajlar[i] << "V | ";
                }
                cout << endl;

                for (size_t i = 0; i < devre.size(); i++) {
                    devre[i]->zamanIlerlet(voltajlar);
                }

                delete[] voltajlar;
            }
            dosya.close();
            cout << "Simulasyon Tamamlandi! Veriler osiloskop.csv'ye yazildi." << endl;
            for (int i = 0; i < boyut; i++) {
                delete[] matris[i];
            }
            delete[] matris;
            delete[] akimlar;
        }
        else if (secim == 10) {
            cout << "Programdan cikiliyor..." << endl;
            for (size_t i = 0; i < devre.size(); i++) {
                delete devre[i];
            }
            break;
        }
        else {
            cout << "Gecersiz secim!" << endl;
        }
    }
    return 0;
}