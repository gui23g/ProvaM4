#include <WiFi.h>
#include <HTTPClient.h>


#define led_verde 2 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int pino_botao = 18;  // O pino utilizado para o botão
int estado_botao = 0;  // Variavel de status do botão

const int pino_ldr = 4;  // O pino utilizado para o ldr
int limite =600;
// Adiciona um contador pro led
int contador = 0;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(pino_botao, INPUT); // Inicializando o botão como um input

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  estado_botao = digitalRead(pino_botao);
  if (estado_botao == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {

  // Adiciona o millis para registrar o tempo
  unsigned long tempo = millis();
  estado_botao = digitalRead(pino_botao);

  int ldr_status=analogRead(pino_ldr);

  if(ldr_status<=limite ){
    Serial.print("its dark turn on led");
    Serial.println(ldr_status);
    if(digitalRead(led_amarelo) == LOW && tempo > contador){
      digitalWrite(led_amarelo, HIGH);
      contador = tempo + 1000;
    } else if(digitalRead(led_amarelo) == HIGH && tempo < contador){
      digitalWrite(led_amarelo, LOW);
      contador = tempo + 1000;
    }

  }else{
    Serial.print("its bright turn off light");
    Serial.println(ldr_status);

    if(estado_botao == HIGH && digitalRead(led_vermelho) == HIGH){
      contador = tempo + 1000;
    }

    if(digitalRead(led_verde) == LOW && digitalRead(led_vermelho) == LOW && digitalRead(led_amarelo) == LOW){
      contador = tempo + 3000;
      digitalWrite(led_verde,HIGH);   
      }
    if(digitalRead(led_verde) == HIGH && tempo > contador){
          digitalWrite(led_amarelo, HIGH);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_vermelho, LOW);
    contador = tempo + 2000;
    } else if(digitalRead(led_amarelo) == HIGH && tempo > contador){
            digitalWrite(led_vermelho, HIGH);
            digitalWrite(led_verde, LOW);
            digitalWrite(led_amarelo, LOW);
    contador = tempo + 5000;
    } else if(digitalRead(led_vermelho) == HIGH && tempo > contador){
          digitalWrite(led_verde, HIGH);
          digitalWrite(led_amarelo, LOW);
          digitalWrite(led_vermelho, LOW);
          contador = tempo + 3000;
    }
  }
}