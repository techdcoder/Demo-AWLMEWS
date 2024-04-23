#include "ESP8266WiFi.h"

#include "config.h"
#include "Awlmews-Ultrasonic.h"

WiFiClient server;
Ultrasonic ultrasonic(D6,D5);

bool wifi_connected = false;
bool server_connected = false;
const float prototype_height = 30.48;
const float critical_height = 25;

const int siren = D7;

  void update_state(){
  wifi_connected = WiFi.status() == WL_CONNECTED;
  server_connected = server.connected();
}

void connect_wifi(){
  static bool connecting = false;
  
  if(!connecting) WiFi.begin(WIFI_NAME,WIFI_PASSWORD);
  connecting = true;
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Connecting to ");
    Serial.println(WIFI_NAME);
  }else if(WiFi.status() == WL_CONNECTED){
    Serial.print("Connected to ");
    Serial.println(WIFI_NAME);

    connecting = false;
  }
}

void connect_server(){
  server.connect(SERVER_IP,SERVER_PORT);
  Serial.print("Connecting to ");
  Serial.print(SERVER_IP);
  Serial.print(":");
  Serial.println(SERVER_PORT);
}

void setup_pins(){
  pinMode(siren,OUTPUT);
}

void setup() {
  Serial.begin(115200);

  setup_pins();
}

void upload_reading(ull interval){
  static ull last_time = 0;
  ull current_time = millis();

  if(current_time - last_time > interval){
    last_time = current_time;

    float reading = ultrasonic.read_averaged(5);
    reading = prototype_height - reading + 10;
    reading = max(reading,0.0f);

    if(reading > critical_height){
      digitalWrite(siren,LOW);
    }else{
      digitalWrite(siren,HIGH);
    }

    Serial.print(reading);
    Serial.println(" cm");

    server.print(reading);
    server.print(",");
    server.flush();
  }
}

void loop() {
  update_state();
  if(!wifi_connected){
    connect_wifi();
    return;
  }

  if(!server_connected){
    connect_server();
    return;
  }

  upload_reading(200);
}
