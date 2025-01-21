/*
   Симистор через оптопару
   подключен к 4 пину, 
   детектор нуля ко 2 пину.
   Переменная Dimmer - величина диммирования, от 0 до 255
   В этом коде на пин А0 подключен потенциометр для управления яркостью
*/

#define dimPin 4
#define zeroPin 2
#include <CyberLib.h> 
volatile int tic, Dimmer;

void setup() {
  Serial.begin(9600);
  pinMode(dimPin, OUTPUT);
  digitalWrite(dimPin, 0);
  pinMode(zeroPin, INPUT);                 // настраиваем порт на вход для отслеживания прохождения сигнала через ноль
  attachInterrupt(0, detect_up, FALLING);  // настроить срабатывание прерывания interrupt0 на pin 2 на низкий уровень

  StartTimer1(timer_interrupt, 40);        // время для одного разряда ШИМ
  StopTimer1();                            // остановить таймер

  Serial.println("Start");
}

void loop() {

  if (Serial.available()) {
    Dimmer = Serial.parseInt();
    Serial.println(Dimmer);
  }
  
   Dimmer = map(analogRead(0), 0, 1023, 240, 0);
}

//----------------------ОБРАБОТЧИКИ ПРЕРЫВАНИЙ--------------------------
void timer_interrupt() {       // прерывания таймера срабатывают каждые 40 мкс
  tic++;                       // счетчик
  if (tic > Dimmer)            // если  время включать ток
    digitalWrite(dimPin, 1);   // ключить ток
}

void  detect_up() {    // обработка внешнего прерывания на пересекание нуля снизу
  tic = 0;                                  // обнулить счетчик
  ResumeTimer1();                           // перезапустить таймер
  attachInterrupt(0, detect_down, RISING);  // перенастроить прерывание
}

void  detect_down() {  // обработка внешнего прерывания на пересекание нуля сверху
  tic = 0;                                  // обнулить счетчик
  StopTimer1();                             // остановить таймер
  digitalWrite(dimPin, 0);                  // выкючить ток
  attachInterrupt(0, detect_up, FALLING);   // перенастроить прерывание
}

