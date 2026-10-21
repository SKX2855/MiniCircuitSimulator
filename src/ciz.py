import csv
from turtle import color  
import matplotlib.pyplot as plt   #as plt derken kısaltmayı plt yaptık

zaman = []
v1 = []
v2 = []

#okuma işi 
with open('osiloskop.csv','r') as oskiloskop:
    okuyucu = csv.reader(oskiloskop)
    next(okuyucu)         #ilk satırdaki yazıları atlar
    for satir in okuyucu:
        zaman.append(float(satir[0]))
        v1.append(float(satir[1]))
        v2.append(float(satir[2]))

plt.plot(zaman, v1, color='blue')
plt.plot(zaman, v2, color='red')
plt.title("Canlı veri")
print("Grafik aciliyor...")
plt.savefig("cıktı.png")
plt.show()