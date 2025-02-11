// int ledVermelho = 2; Em vez de atribuir um valor para ledVermelho
#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4 // Definir que LED_VERMELHO é 4
#define LED_AZUL 5
#define INDEFINIDO -1

#define BOTAO_VERDE 8
#define BOTAO_AMARELO 9
#define BOTAO_VERMELHO 10
#define BOTAO_AZUL 10

#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500

#define TAMANHO_SEQUENCIA 4

enum Estados{
  PRONTO_PARA_PROX_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};

//nosso array de LEDs
int sequenciaLuzes[TAMANHO_SEQUENCIA];

int rodada = 0;

int ledsRespondidos = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();
}

void iniciaJogo(){

  int jogo = analogRead(0);

  randomSeed(jogo);

  for(int contador = 0; contador < TAMANHO_SEQUENCIA; contador ++){
    sequenciaLuzes[contador] = sorteiaCor(); 
  }
}

int sorteiaCor(){
  return random(LED_VERDE, LED_AZUL + 1); //2-5 (6)
}

// Função responsável por iniciar as portas 
void iniciaPortas(){
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);
}

void loop() {
  switch(estadoAtual()){
    case PRONTO_PARA_PROX_RODADA:
      Serial.println("Pronto para proxima rodada");
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      Serial.println("Usuario respondendo");
      processaRespostaUsuario();
      break;
    case JOGO_FINALIZADO_SUCESSO:
      Serial.println("Jogo finalizado com Sucesso");
      jogoFinalizadoSucesso();
      break;
    case JOGO_FINALIZADO_FALHA:
      Serial.println("Jogo finalizado com Falha");
      jogoFinalizadoFalha();
      break;
  }
  delay(500);
}

void processaRespostaUsuario(){

  int resposta = checaRespostaJogador();

  if(resposta == INDEFINIDO){
    return;
  }

  if(resposta == sequenciaLuzes[ledsRespondidos]){
    ledsRespondidos++;
  }else{
    Serial.println("Resposta errada");
    rodada = TAMANHO_SEQUENCIA + 2;
  }

  ledsRespondidos++;

}

void preparaNovaRodada(){
  rodada++;
  ledsRespondidos = 0; 
  if(rodada <= TAMANHO_SEQUENCIA){
    tocaLedsRodada();
  }
}

int estadoAtual(){
  if (rodada <= TAMANHO_SEQUENCIA){

    if (ledsRespondidos == rodada){
      return PRONTO_PARA_PROX_RODADA;
    }else{
      return USUARIO_RESPONDENDO;
    }

  }else if(rodada == TAMANHO_SEQUENCIA + 1){
    return JOGO_FINALIZADO_SUCESSO;
  }else {
    return JOGO_FINALIZADO_FALHA;
  }
}

void tocaLedsRodada(){
  for(int contador = 0; contador < rodada; contador ++){
    piscaLed(sequenciaLuzes[contador]);
  }
}


int checaRespostaJogador(){
  if(digitalRead(BOTAO_VERDE) == LOW){
    return piscaLed(LED_VERDE);
  }
  if(digitalRead(BOTAO_AMARELO) == LOW){
    return piscaLed(LED_AMARELO);
  }
  if(digitalRead(BOTAO_VERMELHO) == LOW){
    return piscaLed(LED_VERMELHO);
  }
  if(digitalRead(BOTAO_AZUL) == LOW){
    return piscaLed(LED_AZUL);
  }

  return INDEFINIDO;

}


// Função que guarda a primeira sequencia de de leds
void jogoFinalizadoSucesso(){
  piscaLed(LED_VERDE);
  piscaLed(LED_AMARELO);
  piscaLed(LED_VERMELHO);
  piscaLed(LED_AZUL);
  delay(MEIO_SEGUNDO);
}

// Função que guarda a segunda sequencia de de leds
void jogoFinalizadoFalha(){
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  delay(MEIO_SEGUNDO);
}

// Função responsavel por piscar os leds
int piscaLed(int portaLed){

  digitalWrite(portaLed, 1); // digitalWrite(2, HIGH);  HIGH --> Liga
  delay(UM_SEGUNDO);
  digitalWrite(portaLed, 0); //   digitalWrite(2, LOW); LOW  --> Desliga
  delay(MEIO_SEGUNDO);
  return portaLed;
}