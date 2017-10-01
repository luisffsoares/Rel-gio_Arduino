/*  UNIVERSIDADE ESTADUAL DE MARINGÁ
 *  Projetos de sistemas microcontrolados 
 *  Professor Rubens Zenko
 *  
 *  Relógio digital utilizando Arduino UNO e display SMA420561KX-2
 *  
 *  Autores:
 *  Luís Felipe Favaro Soares  91695
 *  Renan Alves Casarotto      88348
 *  Rodrigo Kenji Imafuku      88874
 *  
 *  
 *  Mais Informações em: https://github.com/luisffsoares/Rel-gio_Arduino
 *  Setembro de 2017
 *  
 */

#include <TimerOne.h> //Biblioteca utilizada

//Declarração de variáveis

//Portas digitais relacionadas com os segmentos do display
int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int dot = 9;

//Portas digitais relacionadas com os Digitos do display
int d1 = 10;
int d2 = 11;
int d3 = 12;
int d4 = 13;

//Variaveis de contagem de tempo e operações
int cont = 0;
int i = 0, j = 0;
int dia = 0;
long int h,m,s;
long int ah=0,am=0;
long int segundos =  57892;  //Segundo inicial

int hora,minuto,n,hd1,hd2,md1,md2;

int disp = 1;

// Botões
const int b0 = A0;            
const int b1 = A1;            
const int b2 = A2;           
const int b3 = A3;            
const int buzzer = A4; //Buzzer conectado na porta Analógica 4          
const int b5 = A5;

// Leitura dos pinos de entrada
int buttonState0;              
int buttonState1;              
int buttonState2;              

//Debounce
unsigned long lastDebounceTime0 = 0;  
unsigned long lastDebounceTime1 = 0;  
unsigned long lastDebounceTime2 = 0;  
unsigned long debounceDelay = 50;

//Leituras antesriores dos botões    
int lastButtonState0 = LOW;   
int lastButtonState1 = LOW;   
int lastButtonState2 = LOW;   

//Variáveis de operação dos botões
int contabotao = 0;
int contabotao0 = 0;
int contabotao1 = 0;
int contaalarme = 0;

//Variávei de opração dos menus
bool espera = LOW;
bool incrementaminuto = LOW;
bool incrementahora = LOW;
bool menuincrementa = LOW;
bool modoalteracao = LOW;
bool menualarme = LOW;
bool alarme = LOW;

void setup() 
{
  inicializa_display(); //Declara os pinos utilizados do display como saída
  
  // Set do timer em 5000 microsegundos 
  Timer1.initialize(5000); 
  // Rotina de interrupção
  Timer1.attachInterrupt( timerIsr ); 

//Setup como entradas e saídas dos botões, buzzer e LEDs
 pinMode(b0, INPUT);
 pinMode(b1, INPUT);
 pinMode(b2, INPUT);
 pinMode(b3, OUTPUT);
 pinMode(buzzer, OUTPUT);
 pinMode(b5, OUTPUT);

}
 
void loop(){
  
  // leitura do estado dos botões
  int reading0 = digitalRead(b0);
  int reading1 = digitalRead(b1);
  int reading2 = digitalRead(b2);


  if (reading0 != lastButtonState0) {
    // reseta do tempo de debounce
    lastDebounceTime0 = millis();
  }

  if ((millis() - lastDebounceTime0) > debounceDelay) {
    
    if (reading0 != buttonState0) {
      buttonState0 = reading0;
    }
  }

  if (reading1 != lastButtonState1) {
    
    lastDebounceTime1 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
    }
  }

 if (reading2 != lastButtonState2) {
    // reset the debouncing timer
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime2) > debounceDelay) {
    
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
    }
  }
  

 lastButtonState0 = reading0;
 lastButtonState1 = reading1;
 lastButtonState2 = reading2;
}
 
/// --------------------------
/// Rotina do timer
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

  botoes();
  
    if(menualarme == LOW){
      decodifica_tempo_disp(disp);
    }
    else{
      decodifica_alarme_disp(disp);
    }
    disp = disp+1;

    //botoes();


}

void botoes(){
  if (buttonState0 == HIGH & buttonState1 == HIGH){
    contabotao = contabotao+1;
  }
  if (buttonState2 == HIGH){
    contaalarme = contaalarme+1;
  }
  if (buttonState0 == LOW & buttonState1 == HIGH){
    if(menualarme == LOW    &&    menuincrementa == LOW){
      h = m;
      m = s;
    }
  } 
  if (contaalarme == 200){
    alarme = !alarme;
    digitalWrite(b3, alarme);
    contaalarme = 0;
  }

  if (contabotao == 200){
    incrementaminuto = HIGH;
    incrementahora = LOW;
    menuincrementa = HIGH; 
    menualarme = LOW;
  }

  if (contabotao >= 400){
    //segundos = segundos +60;
    contabotao = 0;
    menuincrementa = LOW; 
    incrementaminuto = HIGH;
    incrementahora = LOW;
    menualarme = HIGH;
  }

  if(menuincrementa == HIGH){
    if (incrementaminuto == HIGH){
      if(buttonState0 == HIGH & buttonState1 == LOW){
        contabotao0 = contabotao0+1;
      }
      if (contabotao0 >= 100){
        incrementaminuto = LOW;
        contabotao0 = 0;
        incrementahora = HIGH;
      }
  
      if(buttonState0 == LOW & buttonState1 == HIGH){
        contabotao1 = contabotao1+1;
      }
      if (contabotao1 >= 100){
        contabotao = 0;
        contabotao0 = 0;
        contabotao1 = 0;
        segundos = segundos+60 - segundos%60; 
      }
    }
  
    if (incrementahora == HIGH){
      if(buttonState0 == HIGH & buttonState1 == LOW){
       contabotao0 = contabotao0+1;
      }
      if (contabotao0 >= 100){
        contabotao0 = 0;
        contabotao = 0;
        incrementahora = LOW;
      }
  
      if(buttonState0 == LOW & buttonState1 == HIGH){
       contabotao1 = contabotao1+1;
      }
      if (contabotao1 >= 100){
        contabotao = 0;
        contabotao0 = 0;
        contabotao1 = 0;
        segundos = segundos+3600;
      }   
    }
  }
  
  if(menualarme == HIGH){
    digitalWrite(b5,HIGH);
    if (incrementaminuto == HIGH){
      if(buttonState0 == HIGH & buttonState1 == LOW){
        contabotao0 = contabotao0+1;
      }
      if (contabotao0 >= 100){
        incrementaminuto = LOW;
        contabotao = 0;
        contabotao0 = 0;
        incrementahora = HIGH;
      }
  
      if(buttonState0 == LOW & buttonState1 == HIGH){
        contabotao1 = contabotao1+1;
      }
      if (contabotao1 >= 100){
        contabotao = 0;
        contabotao0 = 0;
        contabotao1 = 0;
        am = am + 1;
        if(am>=60){
          am = 0;
        }
      }
    }
  
    if (incrementahora == HIGH){
      if(buttonState0 == HIGH & buttonState1 == LOW){
       contabotao0 = contabotao0+1;
      }
      if (contabotao0 >= 100){
        contabotao = 0;
        contabotao0 = 0;
        incrementahora = LOW;
        menualarme = LOW;
        digitalWrite(A2, LOW);
      }
  
      if(buttonState0 == LOW & buttonState1 == HIGH){
       contabotao1 = contabotao1+1;
      }
      if (contabotao1 >= 100){
        contabotao = 0;
        contabotao0 = 0;
        contabotao1 = 0;
        ah = ah + 1;
        if(ah>24){
          ah = 0;
        }
      }   
    }  
  }
  else{
    digitalWrite(b5,LOW);
  }

  if(alarme){
    if(h==ah    &&    m==am){
      tone(buzzer, 440);
    }
  }
  else{
    noTone(buzzer);
  }
  
}

void decodifica_tempo_disp(int dp){

  if (dp == 1){
    if (incrementahora == HIGH){
      if (cont <= 100){
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

void decodifica_alarme_disp(int dp){

  if (dp == 1){
    if (incrementahora == HIGH){
      if (cont <= 100){
      
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
    
    if (ah < 10){
      mostra_display(0);
    }
  
    if (ah >= 10){
      hd1 = ah / 10;
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
    
    if (ah < 10){
      mostra_display(ah);
    }
    if (ah >= 10){
      hd2 = ah % 10;
      mostra_display(hd2);
    }
  }

  if (dp == 3){

    if (incrementaminuto == HIGH){
      if (cont <= 100){
      
      digitalWrite(d3,HIGH); 
      }
    if (cont>100){
      digitalWrite(d3,LOW);
    }
    }
    if (incrementaminuto == LOW){
    digitalWrite(d3,HIGH);
    }

    if (am < 10){
      mostra_display(0);
    }
    if (am >= 10){
      md1 = am / 10;
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

    if (am < 10){
      mostra_display(am);
    }
    if (am >= 10){
      md2 = am % 10;
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

