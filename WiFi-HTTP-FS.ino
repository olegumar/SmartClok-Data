#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h>
#include <ArduinoJson.h>         //не выще версии 5
#include <time.h>
#include <ESP8266HTTPUpdateServer.h>

// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Объект для обнавления с web страницы
ESP8266HTTPUpdateServer httpUpdater;
// Для файловой системы 
File fsUploadFile;

// Определяем переменные
String _ssid     = "";        // Для хранения SSID "RTK70"
String _password = "";        // Для хранения пароля "77596501392"
int _ip0 = 0;                 // Для хранения определения нашего (желаемого)адресса
int _ip1 = 0;                 // Для хранения определения нашего (желаемого)адресса
int _ip2 = 0;                 // Для хранения определения нашего (желаемого)адресса
int _ip3 = 0;                 // Для хранения определения нашего (желаемого)адресса
int _ip4 = 0;                 // Для хранения настройки адресса клиента
String _IPaddress = "";       // Для хранения адресса клиента
String _ssidAP = "WiFi";          // SSID AP точка доступа
String _passwordAP = "";          // Пароль точки доступа
String SSDP_Name = "Update-Time"; // Имя нашего устройства SSDP
int timezone = 5;                 // Часовой пояс GTM
String jsonConfig = "{}";

void setup() {
  // 
  Serial.begin(115200);
  // Запускаем файловую систему
  Serial.println("Start - FS");
  FS_init();
  Serial.println("Step - FileConfig");
  loadConfig();
  Serial.println("Start - WIFI");
  // Запускаем WIFI
  WIFIinit();
  Serial.println("Start - Time");
  // Получаем время из сети
  //Time_init();
  // /Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start - SSDP");
  SSDP_init();
  //Р 
  Serial.println("Start - WebServer");
  HTTP_init();
}

void loop() {
  HTTP.handleClient();
  delay(1);
}