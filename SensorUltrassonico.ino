// ----------------------------------------------------
// Projeto: Estacionamento Inteligente (ESP32 + HC-SR04)
// ----------------------------------------------------
#include <WiFi.h>
#include <HTTPClient.h>

// ----------------------
// CONFIGURAÇÃO DE REDE
// ----------------------
const char* ssid = "name"; 
const char* password = "password"; 
const char* serverUrl = "http://192.168.0.14/sensor/receber.php"; // SUBSTITUIR

// ----------------------
// CONFIGURAÇÃO DO SENSOR E VARIÁVEIS GLOBAIS
// ----------------------
const int TRIG_PIN = 32;
const int ECHO_PIN = 34;
const String SENSOR_NOME = "Vaga 1"; 

// AJUSTES DE FILTRAGEM E PRECISÃO
const int NUM_LEITURAS = 11;         
const int LEITURAS_A_DESCARTAR = 3;  

// AJUSTES DE LIMITES (em metros)
const float LIMITE_OCUPADO_MAX = 1.00; // MÁX: 100 cm
const float LIMITE_MIN_VALIDO = 0.10;  // MÍNIMO: 10 cm (Ruído ou Crosstalk)

// VARIÁVEIS GLOBAIS PARA ESTABILIDADE (DEBOUNCE)
String lastReportedStatus = "LIVRE"; 
int statusChangeCounter = 0;         
const int CONFIRMATION_READINGS = 3; // 3 Confirmações (15 segundos)

// ----------------------
// FUNÇÕES DE REDE
// ----------------------
void conectarWifi() {
  Serial.println("\nConectando ao WiFi...");
  WiFi.begin(ssid, password);
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConectado ao WiFi!");
      WiFi.mode(WIFI_MODE_STA);
      Serial.print("IP local: ");
      Serial.println(WiFi.localIP());
  } else {
      Serial.println("\nFalha na conexão WiFi. Tentando novamente no loop.");
  }
}

// ----------------------
// FUNÇÕES DE MEDIÇÃO OTIMIZADA
// ----------------------
float medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); 

  if (duration == 0) return -1; 
  float distanciaMetros = (duration * 0.0343) / 200.0; 
  return distanciaMetros;
}

float medirDistanciaMedia() {
  float leituras[NUM_LEITURAS];
  int validas = 0;

  for (int i = 0; i < NUM_LEITURAS; i++) {
    float d = medirDistancia(TRIG_PIN, ECHO_PIN); 
    if (d > 0) { 
      leituras[validas] = d;
      validas++;
    }
    delay(50); 
  }

  if (validas == 0) return -1;

  // Ordena as leituras válidas (Bubble Sort)
  for (int i = 0; i < validas - 1; i++) {
    for (int j = 0; j < validas - i - 1; j++) {
      if (leituras[j] > leituras[j + 1]) {
        float temp = leituras[j];
        leituras[j] = leituras[j + 1];
        leituras[j + 1] = temp;
      }
    }
  }

  float soma = 0;
  int leituras_descartadas = LEITURAS_A_DESCARTAR * 2;
  int leituras_a_somar = validas - leituras_descartadas;
  
  if (leituras_a_somar <= 0) {
      for (int i = 0; i < validas; i++) {
          soma += leituras[i];
      }
      return soma / validas;
  }
  
  for (int i = LEITURAS_A_DESCARTAR; i < validas - LEITURAS_A_DESCARTAR; i++) {
    soma += leituras[i];
  }

  return soma / leituras_a_somar;
}

// ----------------------
// Função: Verificar vaga (Lógica com Histerese de Ruído)
// ----------------------
String verificarVaga(String currentReportedStatus) { // NOVO PARÂMETRO
  float d = medirDistanciaMedia();

  if (d == -1) {
    // Se a leitura falhar (timeout), mantenha o status anterior para estabilidade.
    return currentReportedStatus; 
  }

  // 1. Deteção de Ruído Extremo de Proximidade (Abaixo de 10 cm)
  if (d < LIMITE_MIN_VALIDO) {
      
      // SE o status anterior era OCUPADA, IGNORE o ruído e mantenha OCUPADA.
      if (currentReportedStatus == "OCUPADA") {
          Serial.print(" --- Falha de Sensor (");
          Serial.print(d * 100);
          Serial.println(" cm). Mantendo OCUPADA. --- ");
          return "OCUPADA";
      }
      // SE o status anterior era LIVRE, o ruído é descartado como LIVRE.
      else {
          Serial.print(" --- Ruído Descartado (Distância: ");
          Serial.print(d * 100);
          Serial.println(" cm) --- ");
          return "LIVRE";
      }
  } 
  
  // 2. Lógica de Ocupação Normal (10 cm a 80 cm)
  else if (d <= LIMITE_OCUPADO_MAX) {
      return "OCUPADA";
  } 
  
  // 3. Lógica de Liberdade (Acima de 80 cm)
  else {
      return "LIVRE";
  }
}

// ----------------------
// SETUP
// ----------------------
void setup() {
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  delay(100); 

  conectarWifi(); 
}

// ----------------------
// LOOP PRINCIPAL (Com Lógica de Debounce)
// ----------------------
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    
    // NOVO: Passa o status reportado anteriormente para a função de verificação
    String currentMeasuredStatus = verificarVaga(lastReportedStatus); 

    // 1. Lógica de Debounce (Confirmação de Estado)
    if (currentMeasuredStatus != lastReportedStatus) {
      statusChangeCounter++;
      
      Serial.print("Status: ");
      Serial.print(currentMeasuredStatus);
      Serial.print(" (Aguardando Confirmação ");
      Serial.print(statusChangeCounter);
      Serial.print("/");
      Serial.print(CONFIRMATION_READINGS);
      Serial.println(")");

      if (statusChangeCounter >= CONFIRMATION_READINGS) {
        lastReportedStatus = currentMeasuredStatus;
        statusChangeCounter = 0; 
        
        Serial.println(">>> NOVO STATUS CONFIRMADO! ENVIANDO POST <<<");
      } else {
        Serial.println("---------------------");
        delay(5000); 
        return; // Pula o envio do POST neste ciclo
      }
    } else {
      statusChangeCounter = 0;
    }
    
    // 2. Envio do POST
    Serial.print(SENSOR_NOME); 
    Serial.print(" (Status: ");
    Serial.print(lastReportedStatus); 
    Serial.print(", Média: ");
    Serial.print(medirDistanciaMedia() * 100); 
    Serial.println(" cm)");

    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"vaga\":\"" + SENSOR_NOME + "\",\"status\":\"" + lastReportedStatus + "\"}"; 

    int httpResponseCode = http.POST(json);

    Serial.print("Resposta do servidor: ");
    Serial.println(httpResponseCode);

    http.end();
  
  } else {
    Serial.println("WiFi desconectado, tentando reconectar...");
    conectarWifi();
  }

  Serial.println("---------------------");
  delay(5000); 
}
