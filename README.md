Özel MNA Devre Simülatörü ve Görselleştirici

Bu proje, tamamen sıfırdan C++ ile geliştirilmiş harici kütüphanesiz bir devre simülasyon motorudur. Düğüm voltajlarını sıfırdan yazılan Gauss Eliminasyon matris çözücüsü ile hesaplar ve transient (geçici hal) tepkilerini Python ile görselleştirir.

## Temel Özellikler
* Harici Kütüphane Yok (C++):** Eigen veya Boost gibi hazır kütüphaneler kullanılmadan, matris çözücü ve MNA motoru sıfırdan yazılmıştır.
* Dinamik Eleman Desteği:** Direnç, Kapasitör, Bobin, AC Voltaj Kaynağı, DC Voltaj Kaynağı ve Akım Kaynağını destekler.
* Transient (Geçici Hal) Analizi:** Bobin şarjı, faz farkı ve gerilim düşümlerini doğru bir şekilde simüle eder.
* Veri Boru Hattı (Pipeline):** Simülasyon sonuçlarını otomatik olarak CSV dosyasına aktarır, Python betiği bu veriyi okuyup osiloskop ekranı gibi çizer.

## Simülasyon Çıktısı
AC Kaynak (Mavi) ile Bobin (Kırmızı) arasındaki faz kaymasını gösteren bir RL devre simülasyonu örneği:

![Simülasyon Çıktısı](docs/cıktı.png)

## Nasıl Çalıştırılır?

1. C++ Motorunu derleyin ve çalıştırın.

cd src
g++ main.cpp -o engine
./engine

2. osiloskop.csv dosyasını ciz.py ile aynı klasöre koyup ciz.py programını çalıştırın.

3. Veriyi görselleştirin.
python ciz.py
