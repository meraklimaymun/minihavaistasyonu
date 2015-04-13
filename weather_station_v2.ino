
/*
*******************************************************************************************
**                                                                                       **
**     _____ ______   _______   ________  ________  ___  __    ___       ___             **
**    |\   _ \  _   \|\  ___ \ |\   __  \|\   __  \|\  \|\  \ |\  \     |\  \            **
**    \ \  \\\__\ \  \ \   __/|\ \  \|\  \ \  \|\  \ \  \/  /|\ \  \    \ \  \           **
**     \ \  \\|__| \  \ \  \_|/_\ \   _  _\ \   __  \ \   ___  \ \  \    \ \  \          **
**      \ \  \    \ \  \ \  \_|\ \ \  \\  \\ \  \ \  \ \  \\ \  \ \  \____\ \  \         **
**       \ \__\    \ \__\ \_______\ \__\\ _\\ \__\ \__\ \__\\ \__\ \_______\ \__\        **
**        \|__|     \|__|\|_______|\|__|\|__|\|__|\|__|\|__| \|__|\|_______|\|__|        **
**                                                                                       **
**                                                                                       **
**                                                                                       **
**    _____ ______   ________      ___    ___ _____ ______   ___  ___  ________          **
**    |\   _ \  _   \|\   __  \    |\  \  /  /|\   _ \  _   \|\  \|\  \|\   ___  \       **
**    \ \  \\\__\ \  \ \  \|\  \   \ \  \/  / | \  \\\__\ \  \ \  \\\  \ \  \\ \  \      **
**     \ \  \\|__| \  \ \   __  \   \ \    / / \ \  \\|__| \  \ \  \\\  \ \  \\ \  \     **
**      \ \  \    \ \  \ \  \ \  \   \/  /  /   \ \  \    \ \  \ \  \\\  \ \  \\ \  \    **
**       \ \__\    \ \__\ \__\ \__\__/  / /      \ \__\    \ \__\ \_______\ \__\\ \__\   **
**        \|__|     \|__|\|__|\|__|\___/ /        \|__|     \|__|\|_______|\|__| \|__|   **
**                                \|___|/                                                **
**                                                                                       **
**                                                                                       **
**                    https://www.youtube.com/user/MerakliMaymunn                        **
**                                                                                       **
**     MİNİ HAVA İSTASYONU - ÇAĞAN ÇELİK TARAFINDAN TASARLANIP & PROGRAMLANMIŞTIR.       **
**                 KAYNAK GÖSTERMEK ŞARTIYLA ÖZGÜRCE PAYLAŞABİLİR,                       **
**             TİCARİ VEYA BİREYSEL PROJELERİNİZDE KULLANABİLİRSİNİZ...                  **
**                                    Bu taslakta;                                       **
**             Henning Karlsen tarafından yazılan 5110 LCD kütüphanesinden,              **
**              http://henningkarlsen.com/electronics/library.php?id=47                  **
**         ladyada tarafından yazılan DHT22 ısı ve nem sensörü kütüphanesinden,          **
**                    https://github.com/adafruit/DHT-sensor-library                     **
**  SparkFun Electronics'ten V10 Mike Grusin tarafından yazılan BMP180 kütüphanesinden   **
**                 https://github.com/sparkfun/BMP180_Breakout                          **
**                                  faydalanılmıştır...                                  **
**                                                                                       **
**                 Açık kaynak donanım ve yazılım ruhu sizinle olsun.                    **
**                                                                                       **
*******************************************************************************************
*/

#include "DHT.h" //DHT sensörleri kitaplığını taslağımıza dahil ediyoruz.
#include <SFE_BMP180.h> //Basınç sensörü kitaplığını taslağımıza dahil ediyoruz.
#include <Wire.h> //Wire kütüphanesini taslağımıza dahil ediyoruz.
#include <LCD5110_Graph.h> //5110 grafik kütüphanesini taslağımıza dahil ediyoruz.

#define DHTPIN 2 //DHT22'nin veri pinini Arduino'daki hangi pine bağladığımızı belirtiyoruz.
#define DHTTYPE DHT22 //Hangi DHT sensörünü kullandığımızı belirtiyoruz.
#define ALTITUDE 788.0 //Bulunduğunuz şehrin rakım değeri. Ben Eskişehir'deyim, yani 788. 
//Yaşadığınız şehrin rakımına buradan bakın: http://tr.wikipedia.org/wiki/T%C3%BCrkiye_il_merkez_rak%C4%B1mlar%C4%B1

DHT dht(DHTPIN, DHTTYPE); //Belirttiğimiz değişkenleri kaydettiriyoruz.
LCD5110 myGLCD(8,9,10,11,12); //5110 LCD'nin takılı olduğu pinleri söylüyoruz.
SFE_BMP180 bmp180;
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];
extern uint8_t TinyFont[];
extern uint8_t nem_bitmap[];
extern uint8_t sicaklik_bitmap[];
extern uint8_t intro[];
extern uint8_t basinc_bitmap[];
extern uint8_t yagmurlu[];
extern uint8_t gunesli[];
extern uint8_t karli[];
extern uint8_t uparrow[];
extern uint8_t downarrow[];
extern uint8_t equal[];

int gecenzaman=0;
int basinckarsilastirma[24] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //İçinde 24 adet değişken saklayan bir Array yaratıyoruz. Bu değişkenlerin her biri saat başı basınç değerlerini tutacak.
int firsttime =0; //Mini Hava İstasyonu'nun ilk defa açılıp açılmadığını kontrol etmek için oluşturduğumuz değişken.
const int switchpin =6; //Switch (anahtar)ı bağladığımız pin 6.
int toplam=0;
int ortalama=0;
int fark=0;
int tahmin;
int yonelim;

void setup(){
  pinMode(switchpin, INPUT);
  pinMode(A3, INPUT);
  pinMode(7, OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(13,LOW);
  dht.begin(); //DHT22 ısı ve nem sensörünü ölçümlere başlattırıyoruz.
  bmp180.begin(); //  BMP180 basınç sensörünü ölçümlere başlattırıyoruz.
  myGLCD.InitLCD(); //LCD başlatılıyor.
  myGLCD.setContrast(60); //Kontrast değeri belirleniyor.

}

void loop() {
  
int nem = dht.readHumidity(); //DHT22 sensöründen nem okunuyor ve nem değişkenine yazılıyor.
int sicaklik = dht.readTemperature(); //DHT22 sensöründen sıcaklık okunuyor ve nem değişkenine yazılıyor.
int basinc = readPressure(); //BMP180 sensöründen basınç okunuyor ve basinc değişkenine yazılıyor.
  
float sicaklikf = dht.readTemperature(); //Detaylı bir şekilde göstermek için bir de float (küsüratlı) şeklinde değişken yapıyoruz.
float nemf = dht.readHumidity();  //Detaylı bir şekilde göstermek için bir de float (küsüratlı) şeklinde değişken yapıyoruz.
float basincf = readPressure(); //Detaylı bir şekilde göstermek için bir de float (küsüratlı) şeklinde değişken yapıyoruz.


toplam = basinckarsilastirma[0] + basinckarsilastirma[1] + basinckarsilastirma[2] + basinckarsilastirma[3] + basinckarsilastirma[4] + basinckarsilastirma[5] + basinckarsilastirma[6] + basinckarsilastirma[7] + basinckarsilastirma[8] + basinckarsilastirma[9] + basinckarsilastirma[10] + basinckarsilastirma[11] + basinckarsilastirma[12] + basinckarsilastirma[13] + basinckarsilastirma[14] + basinckarsilastirma[15] + basinckarsilastirma[16] + basinckarsilastirma[17] + basinckarsilastirma[18] + basinckarsilastirma[19] + basinckarsilastirma[20] + basinckarsilastirma[21] + basinckarsilastirma[22] + basinckarsilastirma[23]; //Belirli aralıklarla ölçülen basınç değerlerini topluyoruz.
ortalama = toplam / 24; //Sonra 3'e bölerek ortalamasını almış oluyoruz.
fark = basinc - ortalama; //Daha sonra güncel basınç değerini hesaplamış olduğumuz ortalamadan çıkararak aradaki farkı buluyoruz.


// DÜŞÜK BASINÇ UYARISI (KIRMIZI LED)
if ((fark) <=-4) {   //4 hPa'lık düşüş olursa. 
digitalWrite(3, HIGH); //Kırmızı yanar.
tahmin=3;
}

//OTOMATİK LCD ARKAPLAN IŞIĞI
int parlaklik = analogRead(A3); 
if (parlaklik <=175) {  //Eğer ortamda ışık yetersizse, 
digitalWrite(7, HIGH);  // 5110 ekranının arkaplan ışığını yak.
}
else {
digitalWrite(7, LOW); //  Yoksa söndür.
}

  if (digitalRead(6) == LOW) { //ÇALIŞMA MODUNU BELİRLE. SWITCH AÇIKSA NORMAL MODDA ÇALIŞ.
    myGLCD.disableSleep(); //Ekranın güç tasarrufu modunu kapatıyoruz.
    
  if (firsttime == 0) { //İlk açılışta intromuzu gösteriyoruz ve sensörlerin okunduğu bilgisini veriyoruz...
  //Aşağıda İntro Animasyonu var...
    myGLCD.clrScr();
    myGLCD.drawBitmap(0, 0, intro, 84, 48);
    for (int i=84; i>=7; i--) {
    myGLCD.setFont(TinyFont);
    myGLCD.print("MINI HAVA ISTASYONU", i, 37);
    delay(20);
    myGLCD.update();
    }
    
    for (int i=48; i>=43; i--) {
    myGLCD.clrScr();
    myGLCD.drawBitmap(0, 0, intro, 84, 48);
    myGLCD.setFont(TinyFont);
    myGLCD.print("MINI HAVA ISTASYONU", 7, 37);
    myGLCD.print("SENSORLER OKUNUYOR", 8, i);
    delay(50);
    myGLCD.update();
    }
    
    delay(5000); //Sensörlerin okunması için gerekli zamanı sağlıyoruz.
    firsttime= 1; // Hava İstasyonu'nun açıldığını ve introyu bir daha göstermeye gerek olmadığını belirtiyoruz.
  
  }
  else {
  }

  //SICAKLIK GÖSTERİLİR...
      myGLCD.clrScr();
      myGLCD.drawBitmap(0, 0, sicaklik_bitmap, 84, 48);
      myGLCD.setFont(BigNumbers);
      myGLCD.printNumF(sicaklikf,1,12 ,18);
      myGLCD.setFont(SmallFont);
      myGLCD.print("C",72,27);
      myGLCD.setFont(TinyFont);
      myGLCD.print("o",70,22);
      myGLCD.update();
      delay(2500);
 
      
  // NEM GÖSTERİLİR...
      myGLCD.clrScr();
      myGLCD.drawBitmap(0, 0, nem_bitmap, 84, 48);
      myGLCD.setFont(BigNumbers);
      myGLCD.printNumF(nemf,1,20, 20);
      myGLCD.setFont(SmallFont);
      myGLCD.print("%",9,28);
      myGLCD.update();
      delay(2500);

      
   // BASINÇ GÖSTERİLİR...   
  
      myGLCD.clrScr();
      myGLCD.drawBitmap(0, 0, basinc_bitmap, 84, 48);
      myGLCD.setFont(BigNumbers);
      myGLCD.printNumF(basincf,1,0,18);
      myGLCD.setFont(TinyFont);
      myGLCD.print("hPa",40,43);
      myGLCD.update();
      delay(2500);
      
       // BASINÇ ÇİZELGESİ 
  
      myGLCD.clrScr();
      myGLCD.drawRect(35, 10, 61, 47);
      myGLCD.setFont(TinyFont);
      myGLCD.print("BASINC (SON 24 SAAT)",6,0);
      myGLCD.print("1000 hPa",0,42);
      myGLCD.print("1050 hPa",0,11);
   
    for (int i=36; i<61; i++)
    {
     int koordinat;
     switch (i) {  
     case 36:
     koordinat= map(basinckarsilastirma[0],990,1050,46,11);
     break;   
     case 37:
     koordinat= map(basinckarsilastirma[1],990,1050,46,11);
     break;
     case 38:
     koordinat= map(basinckarsilastirma[2],990,1050,46,11);
     break;
     case 39:
     koordinat= map(basinckarsilastirma[3],990,1050,46,11);
     break;
     case 40:
     koordinat= map(basinckarsilastirma[4],990,1050,46,11);
     break;
     case 41:
     koordinat= map(basinckarsilastirma[5],990,1050,46,11);
     break;
     case 42:
     koordinat= map(basinckarsilastirma[6],990,1050,46,11);
     break;
     case 43:
     koordinat= map(basinckarsilastirma[7],990,1050,46,11);
     break;
     case 44:
     koordinat= map(basinckarsilastirma[8],990,1050,46,11);
     break;
     case 45:
     koordinat= map(basinckarsilastirma[9],990,1050,46,11);
     break;
     case 46:
     koordinat= map(basinckarsilastirma[10],990,1050,46,11);
     break;
     case 47:
     koordinat= map(basinckarsilastirma[11],990,1050,46,11);
     break;
     case 48:
     koordinat= map(basinckarsilastirma[12],990,1050,46,11);
     break;
     case 49:
     koordinat= map(basinckarsilastirma[13],990,1050,46,11);
     break;
     case 50:
     koordinat= map(basinckarsilastirma[14],990,1050,46,11);
     break;
     case 51:
     koordinat= map(basinckarsilastirma[15],990,1050,46,11);
     break;
     case 52:
     koordinat= map(basinckarsilastirma[16],990,1050,46,11);
     break;
     case 53:
     koordinat= map(basinckarsilastirma[17],990,1050,46,11);
     break;
     case 54:
     koordinat= map(basinckarsilastirma[18],990,1050,46,11);
     break;
     case 55:
     koordinat= map(basinckarsilastirma[19],990,1050,46,11);
     break;
     case 56:
     koordinat= map(basinckarsilastirma[20],990,1050,46,11);
     break;
     case 57:
     koordinat= map(basinckarsilastirma[21],990,1050,46,11);
     break;
     case 58:
     koordinat= map(basinckarsilastirma[22],990,1050,46,11);
     break;
     case 59:
     koordinat= map(basinckarsilastirma[23],990,1050,46,11);
     break;
     }
   
      myGLCD.invPixel(i,koordinat);
      myGLCD.update();
      delay(20);
    }
    
  if (yonelim == 1) { //Eğer basınç yükselme eğilimindeyse, yukarı doğru ok animasyonu gösteriyoruz.
    
  for (int c=0; c<4; c++) { //İlk for döngüsü animasyonu 4 kere tekrar etmemizi sağlıyor.
    
     for (int i=20; i>10; i--){  //İkinci for döngüsü animasyonun ilk hareketini.
     myGLCD.drawBitmap(64, i, uparrow, 24, 24);
     myGLCD.update();
     delay(75);
     }
     for (int y=10; y<20; y++) {  //Üçüncü for döngüsü animasyonun ikinci hareketini yapıyor.
     myGLCD.drawBitmap(64, y, uparrow, 24, 24);
     myGLCD.update();
     delay(75);
     }
  }
  }
  
   if (yonelim == 0) {  //Eğer basınç değerleri aynı çizgide ilerliyorsa, sağa ve sola ok animasyonu gösteriyoruz.
    
  for (int c=0; c<4; c++) { //İlk for döngüsü animasyonu 4 kere tekrar etmemizi sağlıyor.
    
     for (int i=62; i<65; i++){ //İkinci for döngüsü animasyonun ilk hareketini.
     myGLCD.drawBitmap(i, 20, equal, 22, 17);
     myGLCD.update();
     delay(125);
     }
     for (int y=65; y>62; y--) { //Üçüncü for döngüsü animasyonun ikinci hareketini yapıyor.
     myGLCD.drawBitmap(y, 20, equal, 22, 17);
     myGLCD.update();
     delay(125);
     }
  }
  }
  
  if (yonelim == -1) { //Eğer basınç düşme eğilimindeyse, aşağı doğru ok animasyonu gösteriyoruz.
    
  for (int c=0; c<4; c++) { //İlk for döngüsü animasyonu 4 kere tekrar etmemizi sağlıyor.
      
     for (int i=10; i<20; i++) {   //İkinci for döngüsü animasyonun ilk hareketini.
     myGLCD.drawBitmap(64, i, downarrow, 24, 24);
     myGLCD.update();
     delay(75);
      }
     for (int y=20; y>10; y--) { //Üçüncü for döngüsü animasyonun ikinci hareketini yapıyor.
     myGLCD.drawBitmap(64, y, downarrow, 24, 24);
     myGLCD.update();
     delay(75);
    }
  }
  }

  delay(2000);


//BASINÇ ORTALAMALARI
      myGLCD.clrScr();
      myGLCD.setFont(TinyFont);
      myGLCD.print("BASINC (SON 24 SAAT)",6,0);
      myGLCD.setFont(SmallFont);
      myGLCD.print("Ortalama: ",0,10);
      myGLCD.printNumI(ortalama,55,10);
      myGLCD.print("Guncel: ",0,25);
      myGLCD.printNumI(basinc,55,25);
      myGLCD.print("Fark: ",5,40);
      myGLCD.printNumI(fark,55,40);
      myGLCD.update();
      delay(5000);  

      ++gecenzaman; //'gecenzaman' değişkenin 1 artmasını söylüyoruz.
      //Yukarıdaki birçok delay komutu sebebiyle 'gecenzaman' isimli değişkenin 1 artması için 20 saniye geçmesi gerekiyor.
      //Bu sayede Arduino açık olduğundan bu yana geçen zamanı hesaplayabiliriz. 'gecenzaman' değişkeni 180 sayısına ulaşırsa
      //1 saat geçmiş, 360'a ulaşırsa 2 saat geçmiş olur.. Böyle devam ettirerek hesaplayabiliriz.

   }
 else {         // EĞER SWITCH KAPALI KONUMA GETİRİLİRSE GÜÇ TASARRUFU ÇALIŞIR.. GÜÇ TASARRUFU MODUNDA ÖLÇÜMLER DEVAM EDER.
     myGLCD.enableSleep();
     digitalWrite(7,LOW);
     ++gecenzaman;
     delay(20000);
 }

 switch (gecenzaman){  //Saatlik ölçümler...
   case 180:
   basinckarsilastirma[0] = basinc;
   break;
   
   case 360:
   basinckarsilastirma[1] = basinc;
   if ((basinckarsilastirma[0]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[0]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[0]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 540:
   basinckarsilastirma[2] = basinc;
   if ((basinckarsilastirma[1]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[1]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[1]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 720:
   basinckarsilastirma[3] = basinc;
   if ((basinckarsilastirma[2]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[2]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[2]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 900:
   basinckarsilastirma[4] = basinc;
   if ((basinckarsilastirma[3]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[3]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[3]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 1080:
   basinckarsilastirma[5] = basinc;
   if ((basinckarsilastirma[4]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[4]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[4]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 1260:
   basinckarsilastirma[6] = basinc;
   if ((basinckarsilastirma[5]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[5]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[5]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 1440:
   basinckarsilastirma[7] = basinc;
   if ((basinckarsilastirma[6]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[6]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[6]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 1620:
   basinckarsilastirma[8] = basinc;
   if ((basinckarsilastirma[7]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[7]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[7]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 1800:
   basinckarsilastirma[9] = basinc;
   if ((basinckarsilastirma[8]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[8]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[8]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 1980:
   basinckarsilastirma[10] = basinc;
   if ((basinckarsilastirma[9]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[9]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[9]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 2160:
   basinckarsilastirma[11] = basinc;
   if ((basinckarsilastirma[10]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[10]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[10]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 2340:
   basinckarsilastirma[12] = basinc;
   if ((basinckarsilastirma[11]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[11]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[11]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 2520:
   basinckarsilastirma[13] = basinc;
   if ((basinckarsilastirma[12]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[12]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[12]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;

 case 2700:
   basinckarsilastirma[14] = basinc;
   if ((basinckarsilastirma[13]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[13]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[13]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 2880:
   basinckarsilastirma[15] = basinc;
   if ((basinckarsilastirma[14]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[14]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[14]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 3060:
   basinckarsilastirma[16] = basinc;
   if ((basinckarsilastirma[15]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[15]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[15]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 3240:
   basinckarsilastirma[17] = basinc;
   if ((basinckarsilastirma[16]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[16]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[16]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 3420:
   basinckarsilastirma[18] = basinc;
   if ((basinckarsilastirma[17]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[17]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[17]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 3600:
   basinckarsilastirma[19] = basinc;
   if ((basinckarsilastirma[18]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[18]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[18]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 3780:
   basinckarsilastirma[20] = basinc;
   if ((basinckarsilastirma[19]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[19]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[19]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 3960:
   basinckarsilastirma[21] = basinc;
   if ((basinckarsilastirma[20]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[20]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[20]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
   case 4140:
   basinckarsilastirma[22] = basinc;
   if ((basinckarsilastirma[21]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[21]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[21]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
   
      case 4320:
   basinckarsilastirma[23] = basinc;
   if ((basinckarsilastirma[22]) > (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden düşükse bunu yönelim değişkenine yazıyoruz.
   yonelim=-1;
   }
   if ((basinckarsilastirma[22]) < (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerinden büyükse bunu yönelim değişkenine yazıyoruz.
   yonelim=1;
   }
   if ((basinckarsilastirma[22]) == (basinc)) { //Eğer bir önceki ölçüm, güncel basınç değerine eşitse bunu yönelim değişkenine yazıyoruz.
   yonelim=0;
   }
   break;
 } 

}


//En başta girdiğimiz rakım bilgisini kullanarak, ham basınç değerleri deniz seviyesi basınç değerlerine dönüştürülüyor.
float readPressure()
{
  char status;
  double T,P,p0,a;

  status = bmp180.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = bmp180.getTemperature(T);
    if (status != 0)
    { 
      status = bmp180.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = bmp180.getPressure(P,T);
        if (status != 0)
        {
          p0 = bmp180.sealevel(P,ALTITUDE);       
          return p0;
        }
      }
    }
  }
}
