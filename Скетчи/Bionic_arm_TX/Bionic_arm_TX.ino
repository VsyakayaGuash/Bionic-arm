//----------------------------------TX-------------------------------


#include <SPI.h>      // библиотека для работы с шиной SPI 
#include "nRF24L01.h" // библиотека радиомодуля 
#include "RF24.h"     // ещё библиотека радиомодуля 

RF24 radio(7, 8); // "создать" модуль на пинах 9 и 10 Для Уно 
                  //RF24 radio(9,53); // для Меги 

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб 

#define SIG_POWER RF24_PA_LOW
#define SIG_SPEED RF24_1MBPS
//#define CH_AMOUNT 8   // число каналов (должно совпадать с приёмником)
#define CH_NUM 0x60   // номер канала (должен совпадать с приёмником)



void setup() { 
  Serial.begin(9600);       //открываем порт для связи с ПК 
 
  radio.begin();            //активировать модуль 
 /* radio.setAutoAck(1);      //режим подтверждения приёма, 1 вкл 0 выкл 
  radio.setRetries(0, 15);  //(время между попыткой достучаться, число попыток) 
  radio.enableAckPayload(); //разрешить отсылку данных в ответ на входящий сигнал */
  radio.setPayloadSize(32); //размер пакета, в байтах 

  radio.openWritingPipe(address[0]); //мы - труба 0, открываем канал для передачи данных 
  radio.setChannel(0x60);            //выбираем канал (в котором нет шумов!) 

  radio.setPALevel (RF24_PA_LOW);   //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX 
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS 
                                    //должна быть одинакова на приёмнике и передатчике! 
                                    //при самой низкой скорости имеем самую высокую чувствительность и дальность!! 

  radio.powerUp();       //начать работу 
  radio.stopListening(); //не слушаем радиоэфир, мы передатчик 
} 

void loop() { 
  Serial.print("Sent: ");  
  int sensorValue = analogRead(A0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0) * 100; // print out the value you read:
  
  radio.write(&voltage, sizeof(voltage)); 
  Serial.println(voltage);
delay(500); 
}

  


