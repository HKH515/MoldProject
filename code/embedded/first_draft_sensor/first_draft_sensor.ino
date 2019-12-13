#include <LEAmDNS.h>
#include <ESP8266mDNS_Legacy.h>
#include <ESP8266mDNS.h>
#include <LEAmDNS_lwIPdefs.h>
#include <LEAmDNS_Priv.h>



#include <LiquidCrystal.h>

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <DHT.h>;
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <PubSubClient.h>
//#include <Adafruit_SleepyDog.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
//#define CHECK_FINGERPRINT
//#include "secrets.h"

#define DHTPIN 2
#define DHTTYPE DHT22 
//#define CHECK_CA_ROOT
//#define CHECK_PUB_KEY
DHT dht(DHTPIN, DHTTYPE);

#ifndef STASSID
#define STASSID "iot-research" // Your WiFi SSID
#define STAPSK  "yNUtn6uwGa7PFrLdv3hgqDRpf89MsTHb"; //Your WiFi password
#endif


//#ifndef SECRET

  #define HOSTNAME "192.168.1.122"
  const char* ssid = STASSID;  
  const char*  password = STAPSK; 
  
  const char MQTT_HOST[] = "192.168.1.122";
  const int MQTT_PORT = 8883;
  const char MQTT_USER[] = "alex";// leave blank if no credentials used
  const char MQTT_PASS[] = "testing";// leave blank if no credentials used
  


  #ifdef CHECK_CA_ROOT
static const char digicert[] PROGMEM = R"EOF(
MIIFpDCCBIygAwIBAgIJAI3xqeXm2/QKMA0GCSqGSIb3DQEBDQUAMGoxFzAVBgNV
BAMMDkFuIE1RVFQgYnJva2VyMRYwFAYDVQQKDA1Pd25UcmFja3Mub3JnMRQwEgYD
VQQLDAtnZW5lcmF0ZS1DQTEhMB8GCSqGSIb3DQEJARYSbm9ib2R5QGV4YW1wbGUu
bmV0MB4XDTE5MTIxMjEyMjc1N1oXDTMyMTIwODEyMjc1N1owazEYMBYGA1UEAwwP
cmFzcGJlcnJ5cGlhZGFtMRYwFAYDVQQKDA1Pd25UcmFja3Mub3JnMRQwEgYDVQQL
DAtnZW5lcmF0ZS1DQTEhMB8GCSqGSIb3DQEJARYSbm9ib2R5QGV4YW1wbGUubmV0
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA7dXWVye7PArCDnpueFPB
u78ZjFZmoGSWZAIn0EVhKzk+zGgIeEPCaiCB4/cLqFCDho/xUJHPForycyfnFBKy
ai7Ac21zVjjFy21hrqs43ExIxRAt9wTCTMOFfzTdn50RTJHM7VKuXQrSQwqjqApO
MtsHClkAkC3pZtZb4Wwn0iBZlxt7rgqoM83Wy/IW8hLdOYwa/rTc7RKjs3mpqTlR
VY3naL8PvB4jePUXhMB1n6cG3o6BESqpsrt8z3wxyY7YIvIMLpffsJpPLmzLipAp
Uh2m6NK/tLAgD/p7d77JWLutX7ZzOAc505II6ylawi0xcTwyBvjQzzsHoJiK2P2e
SQIDAQABo4ICSjCCAkYwDAYDVR0TAQH/BAIwADARBglghkgBhvhCAQEEBAMCBkAw
CwYDVR0PBAQDAgXgMCEGCWCGSAGG+EIBDQQUFhJCcm9rZXIgQ2VydGlmaWNhdGUw
HQYDVR0OBBYEFFSo00gVhEyLbi2RTFRHzqhtIhRXMIGcBgNVHSMEgZQwgZGAFKzn
FCZCC64DdORJmqhbrkhhfAaKoW6kbDBqMRcwFQYDVQQDDA5BbiBNUVRUIGJyb2tl
cjEWMBQGA1UECgwNT3duVHJhY2tzLm9yZzEUMBIGA1UECwwLZ2VuZXJhdGUtQ0Ex
ITAfBgkqhkiG9w0BCQEWEm5vYm9keUBleGFtcGxlLm5ldIIJANmzmvhhMmqpMIGr
BgNVHREEgaMwgaCHBKwSAAGHEP6AAAAAAAAAuyGGpb1n7zuHBKwRAAGHBKn+KgSH
EP6AAAAAAAAASlUT5GFU6K2HBKn+lKqHEP6AAAAAAAAAeEUC//4lYLyHBMCoAXqH
EP6AAAAAAAAAapbro8nKYQWHBH8AAAGHEAAAAAAAAAAAAAAAAAAAAAGCFXJhc3Bi
ZXJyeXBpYWRhbS5sb2NhbIIJbG9jYWxob3N0MIGGBgNVHSAEfzB9MHsGAysFCDB0
MBwGCCsGAQUFBwIBFhBodHRwOi8vbG9jYWxob3N0MFQGCCsGAQUFBwICMEgwEBYJ
T3duVHJhY2tzMAMCAQEaNFRoaXMgQ0EgaXMgZm9yIGEgbG9jYWwgTVFUVCBicm9r
ZXIgaW5zdGFsbGF0aW9uIG9ubHkwDQYJKoZIhvcNAQENBQADggEBAGF33xC76Iu0
FC/+Svnoi+BWLVFpjWtcyR03Wr2CxlwOq0t1Rag7aPcmIjG0AoYVzpnbQ1K0k+Dl
eBLs4R7qQ3K/y+O0Maem2QyzVkgFOvqylS+ikix5XK2cqbEYgsqllxIvv4zj4++T
HNELr8eiPf+CQs/J4MpV8x85C2mQ5StHf9pmUJl1B5FbFeWynI56fu2IVhrj1J1M
XLRLLZKMvzr0pc6zbfLiWq6bpRrCHbCtof6QUj2V+ReEv3mKR2kaqXSv2IeXAug3
YdJMTHDiIOEAW5M/TBgtFIq86ahvz+NtFB9wxwivdu2LwK1zvZB5cUU+TLLWHpP9
rZsm5EWp6pg=
)EOF";
      #endif


  #ifdef CHECK_PUB_KEY
  // Extracted by: openssl x509 -pubkey -noout -in ca.crt
  static const char pubkey[] PROGMEM = R"KEY(
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAl+bwWRPjp1slCdH/X2Sg
C8cg01v3Z3C3P4/di+6PoOun4nmEsSxbD1VXvhCHzyz34WY+Hq286Kj7/DCjcgIu
TbUsklcCPc+ZCor6JWugby3zC9pj/TTVELPehjkguZJDss87dy1TsieB7cup+SlC
0xW7ZW9db5zl16NsJPihWnEhdhvCMKBwgJWtkD2xJk6831d1sifS490srtr7DmGH
6A/9ilz8diSKP6u5KjxCoMinpvG1LIPDEYzIZJDl2qQGC605xBs/W48gRethduT6
U8vRHqX4fidxZOAY2qCt8zol6RRtKSTzp62D0bPqTIJQhFzGDbeb1++R7AX8O+ai
cQIDAQAB
-----END PUBLIC KEY-----
  )KEY";
  #endif

  #ifdef CHECK_FINGERPRINT
  // Extracted by: openssl x509 -fingerprint -in ca.crt
    static const char fp[] PROGMEM = "1B:03:30:EF:6C:A7:1A:D3:84:93:BF:A7:C3:D0:C9:B7:B5:1F:E9:1F";
    #endif
//  #endif


const byte ledPin = 0; // Pin with LED on Adafruit Huzzah
//const char* mqtt_server = "192.168.1.122"; 

BearSSL::WiFiClientSecure espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  
//  NTP.begin();
  dht.begin();
  WiFi.hostname(HOSTNAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

    // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    uint8_t macAddr[6];
    WiFi.macAddress(macAddr);
    Serial.printf("Connected, mac address: %02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  #ifdef CHECK_CA_ROOT
      BearSSL::X509List cert(digicert);
      espClient.setTrustAnchors(&cert);
      Serial.println("ROOT");
  #endif
  #ifdef CHECK_PUB_KEY
      BearSSL::PublicKey key(pubkey);
      espClient.setKnownKey(&key);
      Serial.println("PUBKEY");
  #endif
  #ifdef CHECK_FINGERPRINT
      espClient.setFingerprint(fp);
      Serial.println("FINGER");
  #endif
  #if (!defined(CHECK_PUB_KEY) and !defined(CHECK_CA_ROOT) and !defined(CHECK_FINGERPRINT))
      espClient.setInsecure();
  #endif
 
 client.setServer(MQTT_HOST, MQTT_PORT);
 client.setCallback(callback);


}


  
void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  if (!client.connected()) {
  reconnect();
 }
 client.loop();
 char charH[10];
 char charT[10];

  ftoa(h, charH, 3);
// client.publish("humidity_S1", dtostrf(h, 4, 3, charH)); 
// client.publish("temperature_S1", dtostrf(t, 4, 3, charT));
 client.publish("humidity_S1", charH); 
 //client.publish("temperature_S1", charT);
 
 Serial.println("Going into deep sleep for 20 seconds");
 ESP.deepSleep(10e6); // 20e6 is 20 microseconds
}
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Callback");
// Serial.print("Message arrived [");
// Serial.print(topic);
// Serial.print("] ");
// for (int i=0;i<length;i++) {
//  char receivedChar = (char)payload[i];
//  Serial.print(receivedChar);
//  if (receivedChar == '0')
//  // ESP8266 Huzzah outputs are "reversed"
//  digitalWrite(ledPin, HIGH);
//  if (receivedChar == '1')
//   digitalWrite(ledPin, LOW);
//  }
//  Serial.println();
}

 
void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT connecting ... ");
    if (client.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {

      Serial.println("connected.");
      //client.subscribe("humidity", );
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println(". Try again in 5 seconds.");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}
// Taken from: http://www.ars-informatica.ca/eclectic/ftoa-convert-a-floating-point-number-to-a-character-array-on-the-arduino/ 
// At: 13/12 2019 18:20
void ftoa(float f, char *str, uint8_t precision) {
  uint8_t i, j, divisor = 1;
  int8_t log_f;
  int32_t int_digits = (int)f;             //store the integer digits
  float decimals;
  char s1[12];

  memset(str, 0, sizeof(f));  
  memset(s1, 0, 10);

  if (f < 0) {                             //if a negative number 
    str[0] = '-';                          //start the char array with '-'
    f = abs(f);                            //store its positive absolute value
  }
  log_f = ceil(log10(f));                  //get number of digits before the decimal
  if (log_f > 0) {                         //log value > 0 indicates a number > 1
    if (log_f == precision) {              //if number of digits = significant figures
      f += 0.5;                            //add 0.5 to round up decimals >= 0.5
      itoa(f, s1, 10);                     //itoa converts the number to a char array
      strcat(str, s1);                     //add to the number string
    }
    else if ((log_f - precision) > 0) {    //if more integer digits than significant digits
      i = log_f - precision;               //count digits to discard
      divisor = 10;
      for (j = 0; j < i; j++) divisor *= 10;    //divisor isolates our desired integer digits 
      f /= divisor;                             //divide
      f += 0.5;                            //round when converting to int
      int_digits = (int)f;
      int_digits *= divisor;               //and multiply back to the adjusted value
      itoa(int_digits, s1, 10);
      strcat(str, s1);
    }
    else {                                 //if more precision specified than integer digits,
      itoa(int_digits, s1, 10);            //convert
      strcat(str, s1);                     //and append
    }
  }

  else {                                   //decimal fractions between 0 and 1: leading 0
    s1[0] = '0';
    strcat(str, s1);
  }

  if (log_f < precision) {                 //if precision exceeds number of integer digits,
    decimals = f - (int)f;                 //get decimal value as float
    strcat(str, ".");                      //append decimal point to char array

    i = precision - log_f;                 //number of decimals to read
    for (j = 0; j < i; j++) {              //for each,
      decimals *= 10;                      //multiply decimals by 10
      if (j == (i-1)) decimals += 0.5;     //and if it's the last, add 0.5 to round it
      itoa((int)decimals, s1, 10);         //convert as integer to character array
      strcat(str, s1);                     //append to string
      decimals -= (int)decimals;           //and remove, moving to the next
    }
  }
}
