//Definicion LEDs
const byte LED_ROJO = 9;
const byte LED_VERDE = 5;
const byte LED_AMARILLO = 3;

//Definicion BOTONES
const byte BTN_ROJO = 10;
const byte BTN_VERDE = 4;
const byte BTN_AMARILLO = 2;

//Definicion Auxiliares
const byte buzzer = 14;

//Definición de variables
byte secuencia[99];
byte nivel;
int velocidad;
bool gameOver;
int digit[11] = { 0b0111111,  //0
                  0b0000110,  //1
                  0b1011011,  //2
                  0b1001111,  //3
                  0b1100110,  //4
                  0b1101101,  //5
                  0b1111101,  //6
                  0b0000111,  //7
                  0b1111111,  //8
                  0b1101111,  //9
                  0b0000000}; //reset
int digit1, digit2;
unsigned long previousMillis = 0;

void setup() {
  //Configuracion LEDs
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);

  //Configuracion BOTONES
  pinMode(BTN_ROJO, INPUT);
  pinMode(BTN_VERDE, INPUT);
  pinMode(BTN_AMARILLO, INPUT);

  //Configuracion Auxiliares
  pinMode(buzzer, OUTPUT);
  pinMode(19, OUTPUT);  //a
  pinMode(18, OUTPUT);  //b
  pinMode(6, OUTPUT);   //c
  pinMode(8, OUTPUT);   //d
  pinMode(7, OUTPUT);   //e
  pinMode(17, OUTPUT);  //f
  pinMode(11, OUTPUT);  //g
  pinMode(15, OUTPUT);  //Digit 1
  pinMode(16, OUTPUT);  //Digit 2
  
  //Configuracion de variables
  nivel = 0;
  velocidad = 1050;
  gameOver = false;
  
  //Semilla para generar numeros aleatorios distintos a cada inicio
  randomSeed(analogRead(6));
}

void loop() {
  if(!gameOver){
    generar_secuencia();
    previousMillis=millis();
    while(previousMillis+2000 > millis())
      show_number(nivel);
    display_off();
    mostrar_secuencia();
    leer_secuencia();
  }
  else{
    game_over();
  }
}

//Aumenta un led la secuencia
void generar_secuencia(){
  int numero = random(0,3);
  if      (numero == 0)  numero = LED_AMARILLO;
  else if (numero == 1)  numero = LED_VERDE;
  else if (numero == 2)  numero = LED_ROJO;

  secuencia[nivel] = numero;
  nivel++;
  if(velocidad > 250) velocidad -= 50;
}

//Muestra todos los leds de la secuencia
void mostrar_secuencia(){
  for (int i = 0; i < nivel; i++) {
    delay(velocidad*0.4);
    digitalWrite(secuencia[i],HIGH);
    if      (secuencia[i] == LED_AMARILLO)  tone(buzzer, 600);
    else if (secuencia[i] == LED_VERDE)  tone(buzzer, 800);
    else if (secuencia[i] == LED_ROJO)  tone(buzzer, 1000);
    delay(velocidad);
    digitalWrite(secuencia[i],LOW);
    noTone(buzzer);
  }
}

//Lee los botones que se pulsan, y comprueba que coinciden con la secuencia
void leer_secuencia(){
  byte pulsacion = 39;  //El 39 es un numero cualquiera, que nunca va a coincidir con ningun pin. 
  byte x = 0;
  
  while(!gameOver && x<nivel){
      while(pulsacion == 39){
        
        if(digitalRead(BTN_AMARILLO) == HIGH){
          pulsacion = LED_AMARILLO;
          digitalWrite(LED_AMARILLO,HIGH);
          while(digitalRead(BTN_AMARILLO) == HIGH)  tone(buzzer, 600);
          noTone(buzzer);
          digitalWrite(LED_AMARILLO,LOW);
        }
        
        if(digitalRead(BTN_VERDE) == HIGH){
          pulsacion = LED_VERDE;
          digitalWrite(LED_VERDE,HIGH);
          while(digitalRead(BTN_VERDE) == HIGH) tone(buzzer, 800);
          noTone(buzzer);
          digitalWrite(LED_VERDE,LOW);
        }
        if(digitalRead(BTN_ROJO) == HIGH){
          pulsacion = LED_ROJO;
          digitalWrite(LED_ROJO,HIGH);
          while(digitalRead(BTN_ROJO) == HIGH)  tone(buzzer, 1000);
          noTone(buzzer);
          digitalWrite(LED_ROJO,LOW);
        }
        
    }
  
    if(pulsacion != secuencia[x]) gameOver = true;
    else{
      x++;
      pulsacion = 39;
    }
  }
}

void game_over(){
  tone(buzzer, 200);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 150);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 100);
  delay(250);
  digitalWrite(LED_VERDE,LOW);
  digitalWrite(LED_AMARILLO,LOW);
  digitalWrite(LED_ROJO,LOW);
  noTone(buzzer);
  
  
  
  while(true){
    previousMillis=millis();
    /*digitalWrite(LED_VERDE,!digitalRead(LED_VERDE));
    digitalWrite(LED_AMARILLO,!digitalRead(LED_AMARILLO));
    digitalWrite(LED_ROJO,!digitalRead(LED_ROJO));*/
    digitalWrite(LED_VERDE,LOW);
    digitalWrite(LED_AMARILLO,LOW);
    digitalWrite(LED_ROJO,LOW);
    while(previousMillis+500>millis())
      show_number(nivel);
    display_off();
    delay(250);
  }
}

void show_digit(int num)   
{  
  digitalWrite(19, bitRead(digit[num], 0));
  digitalWrite(18, bitRead(digit[num], 1));
  digitalWrite(6, bitRead(digit[num], 2));
  digitalWrite(8, bitRead(digit[num], 3));
  digitalWrite(7, bitRead(digit[num], 4));
  digitalWrite(17, bitRead(digit[num], 5));
  digitalWrite(11, bitRead(digit[num], 6)); 
}

void show_number(int j){
  digit2 = j / 10;
  digit1 = j % 10;
  digitalWrite(15, HIGH);
  digitalWrite(16, LOW);
  show_digit(digit2);
  delay(10);
  digitalWrite(16, HIGH);
  digitalWrite(15, LOW);
  show_digit(digit1);
  delay(10);
}

void display_off(){
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);
  show_digit(10);
}
