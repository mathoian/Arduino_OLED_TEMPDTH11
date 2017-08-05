/*
 * 
 Mathoian Santos https://github.com/mathoian | marcelohenrik@gmail.com | https://www.facebook.com/mclo.henrique
 Link Projeto GITHUB https://github.com/mathoian/Arduino_OLED_TEMPDTH11
 Link video youtube  https://www.youtube.com/watch?v=qpGbQmF6fA4
 programa: arduino com  OLED 128X32
 Assunto: **
 SO utilizado: arch linux x64

 -- Materiais -- 

Display OLED 128X32
Sensor de temperatura DTH11 // temperatura e humidade
Arduino (USEI ARDUINO NANO)
Jumper +- 10 unidades
photoboard


                                      +-----+
         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |         GND/RST2  [ ][ ]            |
         |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   C5 
         |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   C4
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[ ] |   B5
         | [ ]v.ref                 MISO/12[ ] |   .
         | [ ]RST                   MOSI/11[ ]~|   .
         | [ ]3V3    +---+               10[ ]~|   .
         | [ ]5v     | A |                9[ ]~|   .
         | [ ]GND   -| R |-               8[ ] |   B0
         | [ ]GND   -| D |-                    |
         | [ ]Vin   -| U |-               7[b] |   D7
         |          -| I |-               6[ ]~|   .
         | [ ]A0    -| N |-               5[ ]~|   .
         | [ ]A1    -| O |-               4[ ] |   .
         | [ ]A2     +---+           INT1/3[ ]~|   .
         | [ ]A3                     INT0/2[ ] |   .
         | [y]A4/SDA  RST SCK MISO     TX>1[ ] |   .
         | [x]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   D0
         |            [ ] [ ] [ ]              |
         |  UNO_R3    GND MOSI 5V  ____________/
          \_______________________/


 -- OBS -- 
PINOS:
GND 0
VCC +5V
SCL = SLK, CLK (CLOCK) A5 =x
SDA ANALOG A4 = y
SIGNAL DTH11 = b 

*/
// **- Bibliotecas  **-
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <dht.h>   

// **- Mapeamento de Hardware **-
#define    dht_pin    7   //pin dth11 
#define VISOR 32

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);   // para o OLED
dht   my_dht;                           //para o DHT11
 
#if (SSD1306_LCDHEIGHT != VISOR)
#error("Please fix file Adafruit_SSD1306.h! 0_o");
#endif

//Protótipo das Funções
void _temp(); //mostrar a temperatura em Graus Celsius
void _printDisplay(); //mostrar a umidade relativa do ar
void printSerial();

// **- Variáveis Globais **-
byte   temperatura = 0x00,   //armazena a temperatura
       umidade     = 0x00,   //armazena a umidade
       cont     = 0x00;   //contador auxiliar

boolean control = 0x00;      //flag de controle


// **- Interrupção **-
ISR(TIMER2_OVF_vect){
  
    TCNT2=100;            //Reinicializa o registrador do Timer2
    cont++;              //incrementa cont
    
    if(cont == 400 ){   //cont igual a 400? //case yes!
      cont = 0x00;      //reinicia cont
      
      digitalWrite(13, !digitalRead(13)); //Inverte o estado da saída
       control = !control;
    }    
}


// **- Configurações Iniciais **-
void setup(){    

  Serial.begin(9600);   //serial em 9600 baud rate   

  pinMode(13, OUTPUT);  //saída digital 13 (LED onboard) "da Placa"
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Inicializa OLED com endereço I2C 0x3C (para 128x64)
  display.clearDisplay();
  display.display();

  TCCR2A = 0x00;   //Timer operando em modo normal
  TCCR2B = 0x07;   //Prescaler 1:1024
  TCNT2  = 100;    //10 ms overflow again
  TIMSK2 = 0x01;   //Habilita interrupção do Timer2
  
} //end setup

// **- Loop Infinito **-
void loop(){
   
   my_dht.read11(dht_pin); //lendo o pin do sensor DTH11 

   temperatura = my_dht.temperature;
   umidade     = my_dht.humidity;
   
   printSerial();
   
   _printDisplay(); //senão, mostra a umidade relativa do ar
   
   display.clearDisplay(); //limpa o display OLED
  
}


// **- Funções **-


void _printDisplay(){
  
   //  ** Feat temperatura **  
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(60,0);
   display.print("Temperatura");
   display.setTextSize(2);
   display.setCursor(70,12);
   display.setTextColor(WHITE);
   display.print(temperatura);
   display.print(" C");
      
  //  ** Feat umidade **
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(8,0);
   display.print("Umidade");
   display.setTextSize(2);
   display.setTextColor(WHITE);
   display.setCursor(6,12);
   display.print(umidade);
   display.print(" %");
   display.display();
   delay(200);
}

void printSerial(){ // serial print =]

  // bom para verificar via serial caso nao tenha display =[
   Serial.print(temperatura);
   Serial.print(" ");
   Serial.println(umidade);
}










 
 

