void HTTP_init(void) {

  HTTP.on("/configs.json", handle_ConfigJSON); // Формирование configs.json страницы для передачи данных в web интерфейс
  //  // API для устройства
  HTTP.on("/ssdp", handle_Set_Ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
  HTTP.on("/ssid", handle_Set_Ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
  HTTP.on("/ssidap", handle_Set_Ssidap); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  HTTP.on("/restart", handle_Restart);   // Перезагрузка модуля по запросу вида /restart?device=ok
  // HTTP.on("/ , handle_);  // Резерв //
  // Добавляем функцию Update для перезаписи прошивки по WiFi при 1М(256K SPIFFS) и выше
  httpUpdater.setup(&HTTP);
  // Запускаем HTTP сервер
  HTTP.begin();
}

// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handle_Set_Ssdp() {
  SSDP_Name = HTTP.arg("ssdp");       // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  saveConfig();                       // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}
// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_Set_Ssid() {
  _ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  _password = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  _ip0 =  HTTP.arg("ip0").toInt();     // Получаем значение ip-0 из запроса
  _ip1 =  HTTP.arg("ip1").toInt();     // Получаем значение ip-1 из запроса
  _ip2 =  HTTP.arg("ip2").toInt();     // Получаем значение ip-2 из запроса
  _ip3 =  HTTP.arg("ip3").toInt();     // Получаем значение ip-3 из запроса
  _ip4 =  HTTP.arg("ip4").toInt();     // Получаем значение ip-4 из запроса
  _IPaddress = HTTP.arg("ip0")+"."+HTTP.arg("ip1")+"."+HTTP.arg("ip2")+"."+HTTP.arg("ip4");  // Формируем адрес и сохраняем в глобальную переменную
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");  // отправляем ответ о выполнении

}
//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handle_Set_Ssidap() {              //
  _ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальную переменную
  _passwordAP = HTTP.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальную переменную
  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = HTTP.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                        // Если значение равно Ок
    HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                              // перезагружаем модуль
  }
  else {                                        // иначе
    HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
  }
}

void handle_ConfigJSON() {
  String root = "{}";  //Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","IPaddress":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(root);
  //Заполняем поля json
  json["SSDP"] = SSDP_Name;
  json["ssidAP"] = _ssidAP;
  json["passwordAP"] = _passwordAP;
  json["ssid"] = _ssid;
  json["password"] = _password;
  json["ip0"] = _ip0;
  json["ip1"] = _ip1;
  json["ip2"] = _ip2;
  json["ip3"] = _ip3;
  json["ip4"] = _ip4;
  json["IPaddress"] = _IPaddress;
  json["timezone"] = timezone;
  json["ip"] = WiFi.localIP().toString();
  json["time"] = GetTime();
  json["date"] = GetDate();
  // Помещаем созданный json в переменную root
  root = "";
  json.printTo(root);
  HTTP.send(200, "text/json", root);
}

// Пример обновление страницы через каждые 5 секунд
// http://wikihandbk.com/wiki/ESP8266:
//
String prepareHtmlPage()
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // 
                                       // 
            "Refresh: 5\r\n" +         // 
                                       // 
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "Analog input:  " + String(analogRead(A0)) +
            "</html>" +
            "\r\n";
  return htmlPage;
}
