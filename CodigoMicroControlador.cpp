#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "PIX"
#define WIFI_PASSWORD "12345678"

#define API_KEY "AIzaSyDNhIcF1_umGHVktZhlNTStZpbvD4sL5rA"

#define DATABASE_URL "https://esp32led-9fa8f-default-rtdb.firebaseio.com/"

#define USER_EMAIL "teste@gmail.com"
#define USER_PASSWORD "123456789"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

const int ledPin1 = 4;
const int ledPin2 = 5;
const int ledPin3 = 18;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("\nWi-Fi Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.timeout.serverResponse = 10 * 1000;  

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Serial.print("Autenticando no Firebase");
  while (auth.token.uid == "") {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nAutenticação bem-sucedida!");
}

void loop() {

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    int ledState1 = LOW, ledState2 = LOW, ledState3 = LOW;


    if (Firebase.RTDB.getInt(&fbdo, "/led1/state")) {
      ledState1 = fbdo.intData();
      digitalWrite(ledPin1, ledState1);
      Serial.print("LED 1: ");
      Serial.println(ledState1);
    } else {
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getInt(&fbdo, "/led2/state")) {
      ledState2 = fbdo.intData();
      digitalWrite(ledPin2, ledState2);
      Serial.print("LED 2: ");
      Serial.println(ledState2);
    } else {
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getInt(&fbdo, "/led3/state")) {
      ledState3 = fbdo.intData();
      digitalWrite(ledPin3, ledState3);
      Serial.print("LED 3: ");
      Serial.println(ledState3);
    } else {
      Serial.println(fbdo.errorReason());
    }
  }
}