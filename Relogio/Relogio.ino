#include <TimerOne.h>
// Teste

int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int dot = 9;

int d1 = 10;
int d2 = 11;
int d3 = 12;
int d4 = 13;

// oi han
int cont = 0;
int i = 0, j = 0;
int dia = 0;
long int h,m,s;
long int segundos =  57892;  //Segundo inicial

int hora,minuto,n,hd1,hd2,md1,md2;

int disp = 1;

const int b0 = A0;            // the number of the pushbutton pin
const int b1 = A1;            // the number of the pushbutton pin

int buttonState0;              // the current reading from the input pin
int buttonState1;              // the current reading from the input pin


unsigned long lastDebounceTime0 = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int lastButtonState0 = LOW;   // the previous reading from the input pin
int lastButtonState1 = LOW;   // the previous reading from the input pin

int contabotao = 0;
int contabotao0 = 0,contabotao1 = 0;

bool espera = LOW;
bool incrementaminuto = LOW;
bool incrementahora = LOW;
bool menuincrementa = LOW;


void setup() 
{
  inicializa_display();
  
  // set a timer of length 5000 microseconds 
  Timer1.initialize(5000); 
  // attach the service routine here
  Timer1.attachInterrupt( timerIsr ); 

 pinMode(b0, INPUT);
 pinMode(b1, INPUT);

}
 
void loop(){
  
  // read the state of the switch into a local variable:
  int reading0 = digitalRead(b0);
  int reading1 = digitalRead(b1);


  if (reading0 != lastButtonState0) {
    // reset the debouncing timer
    lastDebounceTime0 = millis();
  }

  if ((millis() - lastDebounceTime0) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading0 != buttonState0) {
      buttonState0 = reading0;
    }
  }

  

  if (reading1 != lastButtonState1) {
    // reset the debouncing timer
    lastDebounceTime1 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
    }
  }
 
  
// save the reading. Next time through the loop, it'll be the lastButtonState:
 lastButtonState0 = reading0;
 lastButtonState1 = reading1;









  

  /*
  if(reading0 == HIGH){
      j++;
      delay(200);
  }

  if(j == 0  &&  reading1 == HIGH){
      segundos += 60;
      delay(200);
  }

  if(j == 1  &&  reading1 == HIGH){
      segundos += 3600;
      delay(200);
  }
  if(j == 2){
    j=0;
  }
   */
  

 
}
 
/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{   
    apaga();
    
    h = segundos/3600;
    m = (segundos / 60) % 60;
    s = segundos%60;
    
    if (cont >= 200){
      segundos = segundos +1;
      cont = 0;
    }

    if (cont <= 100){
      //digitalWrite(d2,HIGH);
      digitalWrite(dot,LOW); 
    }else{
      digitalWrite(dot,HIGH);
      
    }

    cont = cont + 1;
    
    if (segundos >= 86400){
        segundos = 0;
        dia = dia + 1;
    }

    if (disp > 4){
      disp = 1;
    }
    
    decodifica_tempo_disp(disp);
    disp  = disp+1;

    botoes();


}

void botoes(){

  if (buttonState0 == HIGH & buttonState1 == HIGH){
    contabotao = contabotao+1;
  }

  if (contabotao == 200){
    //segundos = segundos +60;
    //contabotao = 0;
    incrementaminuto = HIGH;
    
  }

  if (contabotao == 400){
    //segundos = segundos +60;
    contabotao = 0;
    incrementaminuto = LOW;
    incrementahora = LOW;
    
  }

  if (incrementaminuto == HIGH){
    if(buttonState0 == HIGH & buttonState1 == LOW){
     contabotao0 = contabotao0+1;
    }
    if (contabotao0 == 100){
      incrementaminuto = LOW;
      contabotao0 = 0;
      incrementahora = HIGH;
    }

    if (espera == LOW){
      if (buttonState0 == LOW & buttonState1 == HIGH){
        segundos = segundos+60; 
        espera = HIGH;
      }
    }

    if (espera = HIGH){
      contabotao1 = contabotao1+1;
    }
    if (contabotao1 == 100){
      contabotao1 = 0;
      espera = LOW;
    }

  

    
  }

  if (incrementahora == HIGH){
    if(buttonState0 == HIGH & buttonState1 == LOW){
     contabotao0 = contabotao0+1;
    }
    if (contabotao0 == 100){
      contabotao0 = 0;
      incrementahora = LOW;
    }


    if (espera == LOW){
      if (buttonState0 == LOW & buttonState1 == HIGH){
        segundos = segundos+3600; 
        espera = HIGH;
      }
    }

    if (espera = HIGH){
      contabotao1 = contabotao1+1;
    }
    if (contabotao1 == 100){
      contabotao1 = 0;
      espera = LOW;
    }
  }
  
}

void decodifica_tempo_disp(int dp){

  if (dp == 1){
    if (incrementahora == HIGH){
      if (cont <= 100){
      //digitalWrite(d2,HIGH);
      digitalWrite(d1,HIGH); 
      }
    if (cont>100){
      digitalWrite(d1,LOW);
    }
    }
    if (incrementahora == LOW){
    digitalWrite(d1,HIGH);
    digitalWrite(dot,HIGH);
    }
    
   
    if (h < 10){
      mostra_display(0);
    }
  
    if (h >= 10){
      hd1 = h / 10;
      mostra_display(hd1);
    }
  }
    
  if (dp == 2){

    if (incrementahora == HIGH){
      if (cont <= 100){
      //digitalWrite(d2,HIGH);
      digitalWrite(d2,HIGH);; 
      }
    if (cont>100){
      digitalWrite(d2,LOW);
    }
    }
    if (incrementahora == LOW){
    digitalWrite(d2,HIGH);
    }
    
    
    if (h < 10){
      mostra_display(h);
    }
    if (h >= 10){
      hd2 = h % 10;
      mostra_display(hd2);
    }
  }

  if (dp == 3){

    if (incrementaminuto == HIGH){
      if (cont <= 100){
      //digitalWrite(d2,HIGH);
      digitalWrite(d3,HIGH); 
      }
    if (cont>100){
      digitalWrite(d3,LOW);
    }
    }
    if (incrementaminuto == LOW){
    digitalWrite(d3,HIGH);
    }

    if (m < 10){
      mostra_display(0);
    }
    if (m >= 10){
      md1 = m / 10;
      mostra_display(md1);
    }
  }

  if (dp == 4){

    if (incrementaminuto == HIGH){
      if (cont <= 100){
      //digitalWrite(d2,HIGH);
      digitalWrite(d4,HIGH); 
      }
    if (cont>100){
      digitalWrite(d4,LOW);
    }
    }
    if (incrementaminuto == LOW){
    digitalWrite(d4,HIGH);
    }

    if (m < 10){
      mostra_display(m);
    }
    if (m >= 10){
      md2 = m % 10;
      mostra_display(md2);
    }
  }
}
 

void mostra_display(int n){

  
  if (n == 0){
    
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,LOW);
    digitalWrite(f,LOW);
    digitalWrite(g,HIGH);
      
  }
  
  if (n == 1){
    
    digitalWrite(a,HIGH);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,HIGH);
    digitalWrite(e,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);
      
  }

  if (n == 2){

    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,HIGH);
    digitalWrite(d,LOW);
    digitalWrite(e,LOW);
    digitalWrite(f,HIGH);
    digitalWrite(g,LOW);
      
  }

  if (n == 3){

    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,LOW);

  }

  if (n == 4){

    digitalWrite(a,HIGH);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,HIGH);
    digitalWrite(e,HIGH);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);
      
  }

  if (n == 5){

    digitalWrite(a,LOW);
    digitalWrite(b,HIGH);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,HIGH);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);

  }

  if (n == 6){

    digitalWrite(a,LOW);
    digitalWrite(b,HIGH);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,LOW);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);
      
  }

  if (n == 7){

    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,HIGH);
    digitalWrite(e,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);
      
  }

  if (n == 8){

    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,LOW);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);

      
  }

  if (n == 9){

    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,HIGH);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);

  }

  
}

void apaga(){

  digitalWrite(d1,LOW);
  digitalWrite(d2,LOW);
  digitalWrite(d3,LOW);
  digitalWrite(d4,LOW);
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
  digitalWrite(dot,HIGH);

  
}

void inicializa_display(){
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(dot, OUTPUT);
  
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);

  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
  digitalWrite(dot,HIGH);

  digitalWrite(d1,LOW);
  digitalWrite(d2,LOW);
  digitalWrite(d3,LOW);
  digitalWrite(d4,LOW);

}

