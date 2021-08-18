#define trigger 4          // Declara que o pino de trigger estará na porta 4
#define echo 5             // Declara que o pino de echo estará na porta 5
float distancia1 = 0;      // Cria a variável para receber o valor da distância
float distancia2 = 0;      // Cria a variável para receber o valor da distância
float distancia3 = 0;      // Cria a variável para receber o valor da distância
float media=0;             //
float var=0;               //
float cont=0;              // Variaéveis
float cont2=0;             //// Cria a variável para recer o valor do tempo lido do pino 5
float tempo = 0;
float a=0;                 //Leitura da tensao
float b=0;                 //Leitrua da tensao

int ledE=6;                 // LED da espera
int ledA=7;                 // LED da água
int ledS=8;                 // LED do sabão
int botao=9;                // botão no pino 9 (PEDAL)
int estado=0;               // Guarda o valor do estado
int b1=10;                  // Bomba de sabão
int b2=11;                  // Bomba de água
int ledf=2;
int ativ=12;                //Aciona a Beteria
int i = 0;
int mediaTensao = 0;


 
void setup() 
  {
  pinMode(trigger, OUTPUT);        // Configura o trigger como saída
  digitalWrite(trigger, LOW);      // Configura o trigger em nível baixo inicialmente
  pinMode(echo, INPUT);            // Configura o echo como entrada de sinal digital
  Serial.begin(9600);              // Inicia a comunicação serial
  pinMode(ledS,OUTPUT);             
  pinMode(ledE,OUTPUT);
  pinMode(ledA,OUTPUT);  
  pinMode(ledf,OUTPUT); 
  pinMode(b1,OUTPUT);
  pinMode(b2,OUTPUT);  
  pinMode(botao,INPUT); 
  pinMode(ativ, OUTPUT);           
  }


 
void loop() {

  
  TesteLuz();

  if(estado==0){
    distancia1=0;
    estado=0; 
    cont=0; 
    distancia1= AcionarTrigger(); // Retrona a distancia 
  
    if (distancia1 < 1){                   // Testa se existe alguem em uma distancia menor do que um metro
      distancia2=distancia2+distancia1;
      var=var+1;
      delay(50); 

      if (var==10){ // Testa se a pessoa ficou na frete por 10 ciclos
  
        media = CalcularMedia(distancia2);
        var=0;
        distancia2=0;                                              
        if (media <= 0.3 && media >0){          // Testa se tem pessoa perto pela media da distancia sendo menor que 0,3m
          digitalWrite(ledf, HIGH);  
          delay(1000);
          if (digitalRead(botao) == HIGH)  {      //Testa se a pessoa pisou no pedal
            
            LigarEstado0(); // Liga o estado 0
            estado=1;
            cont2=0;
            cont=0;
          }
        }else{
            cont2=cont2+1;
            if(cont2==3){
              digitalWrite(ledf, LOW);
              cont2=0;
              estado=0;
           }
        }
      }
    }else{     
      cont=cont+1;
      if(cont==5){        
        digitalWrite(ledf, LOW);
        cont=0;
      }
    }
  }
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  

  if(estado==1){
  
    distancia1= AcionarTrigger();
  
    if (distancia1 <1){                    // Testa se existe alguem em uma distancia menor do que um metro
  
      distancia2=distancia2+distancia1;
      var=var+1;
      delay(50); 

      if (var==10){
  
        media = CalcularMedia(distancia2); 
        var=0;
        distancia2=0; 
   
        if (media <= 0.3 && media >0){ // Testa se tem pessoa perto pela media da distancia sendo menor que 0,3m
          LigarEstado1(); //Liga o estado 1
          estado=2;
          cont2=0;
          cont=0;
    
        }else{
          cont2=cont2+1;
          if(cont2==3){
            digitalWrite(ledf, LOW);
            cont2=0;
            estado=0;
          }
        }
      }
    }else{
      cont=cont+1;
      if(cont==5){
        digitalWrite(ledf, LOW);
        cont=0;
      }
    }

  }

 
  if(estado==2){
   
    distancia1= AcionarTrigger();
  
    if (distancia1 <1){                      // Testa se existe alguem em uma distancia menor do que um metro
  
      distancia2=distancia2+distancia1;
      var=var+1;
      delay(50); 

      if (var==10){
  
        media = CalcularMedia(distancia2);
        var=0;
        distancia2=0;
  
        if (media <= 0.3 && media >0){ // Testa se tem pessoa perto pela media da distancia sendo menor que 0,3m
          LigarEstado2(); // Liga o estado2
          estado=0;
          cont2=0;
          cont=0;
    
        }else{
          cont2=cont2+1;
          if(cont2==3){
            digitalWrite(ledf, LOW);
            cont2=0;
            estado=0;
         }
        }
      }
    }else{
      cont=cont+1;
      if(cont==5){
        digitalWrite(ledf, LOW);
        cont=0;
      }
    }
  
  }

}


void TesteLuz(){
  
  a = (float)analogRead(A0);                          
  a = a*(12.00/1023.00);
  if(a==0){                                    // Teste pra saber se tem luz

    for(i=0;i<3;i++){
      a = (float)analogRead(A0);                          
      a = a*(12.00/1023.00);
      if(a==0){ 
        b = (float)analogRead(A1);
        b = b*(12.00/1023);
        mediaTensao = mediaTensao + b;              //Calculando a media da tensao
      }
    }
    mediaTensao = mediaTensao/3;
    Serial.println("Teste:");
    Serial.println(mediaTensao);
    if(mediaTensao>=9){                                // Teste pra saber se a bateria tem carga
      digitalWrite(ativ, HIGH);
    }
  } else{
    digitalWrite(ativ, LOW);
  }
}


float AcionarTrigger (){

    digitalWrite(trigger, LOW);
    delayMicroseconds(5);                          
    digitalWrite(trigger, HIGH);                              // Aciona o trigger 
    delayMicroseconds(1000);                                  // Delay de por 1000 microsegundos
    digitalWrite(trigger, LOW);                               // Desliga o trigger
    tempo = pulseIn(echo, HIGH);                              // Lê o tempo em que echo permanece em nivel alto, tempo dado em microsegundos
    tempo = tempo/1000000;                                    // Converte o tempo para segundos
    return ((tempo*340)/2);                                   //Retorna o calculo a distancia com base na equação fornecida no datasheet
  
  
}

float CalcularMedia(float distancia){ // Retorna a media da distancia
    float retorno;
    retorno = distancia/10;
    Serial.println("Media Medida em Metros");             // Apresenta no monitor a distancia calculada anteriormente em metros
    Serial.println(retorno);
    delay(100); 
  return retorno;
  
}

void PiscarLed (int led){ // Faz que o led pisque 3 vezes
        int repete;
        for(repete=0;repete<3;repete++){
           digitalWrite(led, HIGH); // liga o led
           delay(500);
           digitalWrite(led, LOW);  // desliga o led
           delay(500);   
        }
}

void LigarEstado0(){ // Estado 0 e responsavel pelo acionamento da bomba da agua e sabao
            
            
            PiscarLed (ledA);
            
            digitalWrite(ledA, HIGH); // liga o led
            digitalWrite(b2, HIGH); // liga A BOMBA 2(agua)
            delay(1500);
            digitalWrite(b2, LOW); // desliga A BOMBA 2
            digitalWrite(ledA, LOW);
   
            PiscarLed (ledS);
           
            digitalWrite(ledS, HIGH); // liga o led
            digitalWrite(b1, HIGH); // liga A BOMBA 1 (sabao)
            delay(1000);
            digitalWrite(ledS, LOW);
            digitalWrite(b1, LOW); // desliga A BOMBA 1
    
}


void LigarEstado1(){ // Tempo de espera para enxaguar as mãos
          
          digitalWrite(ledf, HIGH);
          PiscarLed (ledE);
          digitalWrite(ledE, HIGH); // liga o led
          delay(20000);
          digitalWrite(ledE, LOW);  // desliga o led
  
}

void LigarEstado2(){ // Liga a bomba de agua para retirada do sabao
          digitalWrite(ledf, HIGH); 
          PiscarLed (ledA);
          digitalWrite(ledA, HIGH); // liga o led
          digitalWrite(b2, HIGH); // liga A BOMBA 2
          delay(10000);
          digitalWrite(b2, LOW); // desliga A BOMBA 2
          digitalWrite(ledA, LOW);
}
