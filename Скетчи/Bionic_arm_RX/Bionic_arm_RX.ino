//---------------------------------------------RX-------------------------------------


#include <Adafruit_PWMServoDriver.h>
#include <SPI.h> 
#include "nRF24L01.h" 
#include "RF24.h" 
#include <Wire.h>

RF24 radio(7,8); // "создать" модуль на пинах 9 и 10 Для Уно 
                  //RF24 radio(9,53); // для Меги 

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50
#define SIG_POWER RF24_PA_MIN
#define SIG_SPEED RF24_1MBPS
//#define CH_AMOUNT 8   // число каналов
#define CH_NUM 0x60   // номер канала (должен совпадать)


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

float x = 90.0; 
float recieved_data; // массив принятых данных
byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"}; //возможные номера труб 
byte pipeNo;


void setup(void){ 
  Serial.begin(9600); //открываем порт для связи с ПК 
 
  pwm.begin(); 
  pwm.setPWMFreq(FREQUENCY); 

  radio.begin();            //активировать модуль 
/*  radio.setAutoAck(1);      //режим подтверждения приёма, 1 вкл 0 выкл 
  radio.setRetries(0, 15);  //(время между попыткой достучаться, число попыток) 
  radio.enableAckPayload(); //разрешить отсылку данных в ответ на входящий сигнал */
  radio.setPayloadSize(32); //размер пакета, в байтах 

  radio.openReadingPipe(1,address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x60);            //выбираем канал (в котором нет шумов!) 

  radio.setPALevel (RF24_PA_LOW);   //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX 
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS 
                                    //должна быть одинакова на приёмнике и передатчике! 
                                    //при самой низкой скорости имеем самую высокую чувствительность и дальность!! 

  radio.powerUp(); //начать работу 
  radio.startListening(); //начинаем слушать эфир, мы приёмный модуль 

} 

void loop(void) { 
  
 
    while (radio.available(&pipeNo)) {                                 // слушаем эфир
        radio.read(&recieved_data,sizeof(recieved_data));
             if (recieved_data > x) { 
                 pwm.setPWM(0, 0, pulseWidth(45)); 
                 pwm.setPWM(1, 0, !pulseWidth(45)); 
                 pwm.setPWM(2, 0, pulseWidth(60)); 
                 pwm.setPWM(3, 0, pulseWidth(70)); 
                 pwm.setPWM(4, 0, pulseWidth(45)); 
                   delay(1000); 
             }  else if ( recieved_data < x){ 
                 pwm.setPWM(0, 0, pulseWidth(0)); 
                 pwm.setPWM(1, 0, !pulseWidth(0)); 
                 pwm.setPWM(2, 0, pulseWidth(0)); 
                 pwm.setPWM(3, 0, pulseWidth(0)); 
                 pwm.setPWM(4, 0, pulseWidth(0)); 
                   delay(1000);
             }      
     Serial.print("Recieved: "); Serial.println(recieved_data); 
               
   } 
 
delay (50);  
}

int pulseWidth(int angel) { 
  int pulse_wide, analog_value; 
  pulse_wide = map(angel , 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH); 
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096); 
return analog_value; 
} 


 
  


