// Load Wi-Fi library
#include <WiFi.h>
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)

#define TXD2_PIN (GPIO_NUM_4)  // UART1 TX
#define RXD2_PIN (GPIO_NUM_5)  // UART1 RX
// Replace with your network credentials
const char* ssid = "BJT"; // tên wifi
const char* password = "123123123"; // password của wifi
// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String output25State = "off";
String output27State = "off";
// Assign output variables to GPIO pins
const int output25 = 2;
const int output27 = 2;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  

 // Config UART2
    const uart_port_t uart_num = UART_NUM_2;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_modes = UART_DATA_8_modeS,
        .parity = UART_PARITY_DISABLE,
        .stop_modes = UART_STOP_modeS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));

    // Config UART1
    const uart_port_t uart_num1 = UART_NUM_1;
    
    ESP_ERROR_CHECK(uart_param_config(uart_num1, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, TXD2_PIN, RXD2_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));




  // Initialize the output variables as outputs
  pinMode(output25, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output25, LOW);
  digitalWrite(output27, LOW);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

String strTest;
String strTest1;

void loop() {
  WiFiClient client = server.available(); 
  char* button;

  const uart_port_t uart_num1 = UART_NUM_1;
  uint8_t data1[128];
  int length1 = 0;
  ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num1, (size_t*)&length1));
  length1 = uart_read_bytes(uart_num1, data1, length1, 100);
  if (length1 > 0) {
      char* button = (char*) data1;
  }

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        // Kiểm tra yêu cầu cho các endpoint
        
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // Display the new HTML web page
            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"en\">");
            client.println("<head>");
            client.println("<style>");
            client.println("body { display: flex; justify-content: center; align-items: center; flex-direction: column; margin: 0; background-color: black; padding-bottom: 2000px; }");
            client.println(".inner-container { margin-top: 100px; position: relative; width: 1200px; height: 1200px; display: flex; align-items: center; justify-content: center; background-image: url("image/traffic.png"); background-size: cover; background-position: center; }");
            client.println(".Col { position: absolute; transform: rotate(90deg); align-items: center; text-align: center; display: flex; }");
            client.println(".tf1 { margin-right: 350px; }");
            client.println(".tf2 { margin-left: 350px; transform: rotate(180deg); }");
            client.println(".tf3 { margin-right: 400px; }");
            client.println(".tf4 { transform: rotate(180deg); margin-left: 350px; }");
            client.println(".Row { position: absolute; display: flex; }");
            client.println(".traffic-signal { width: 100px; height: 300px; background-color: #333; border-radius: 10px; display: flex; flex-direction: column; justify-content: space-between; align-items: center; }");
            client.println(".light { width: 60px; height: 60px; border-radius: 50%; margin: 10px; transition: background-color 0.9s; }");
            client.println("@keyframes blinkRed { 0%, 49% { background-color: red; } 50%, 100% { background-color: black; } }");
            client.println(".red { background-color: red; }");
            client.println(".yellow { background-color: yellow; animation: blinkYellow 9s infinite 3s; }");
            client.println(".green { background-color: green; animation: blinkGreen 9s infinite 6s; }");
            client.println(".button-container { margin-top: 20px; display: flex; justify-content: center; }");
            client.println(".btn { background-color: #007bff; border: none; color: white; padding: 10px 20px; margin: 0 5px; border-radius: 5px; transition: background-color 0.3s, transform 0.3s; }");
            client.println(" .btn:hover { background-color: #0056b3; transform: translateY(-2px); }");
            client.println(".btn:active { transform: translateY(0); background-color: #003d7a; }");
            client.println(".red { background-color: red; }");
            client.println(".yellow { background-color: yellow; }");
            client.println(".green { background-color: green; }");
            client.println(".light {background-color: black;}");
            client.println(".number {display: flex;justify-content: center;margin-top: 10px;}");
            client.println(".number-display {font-size: 240px;font-weight: bold;margin-top: 50px;color: white;display: inline-block;border: 3px solid white;padding: 0px 30px;border-radius: 20%;}");
            client.println(".number-display1,.number-display3{margin-right: 50px;}");
            client.println(".colon {font-size: 240px;font-weight: bold;margin: 0 5px;color: white;}");
            client.println("    </style>");
            client.println("</head>");
            client.println("<body>");
            client.println("    <div class=\"inner-container\">>");
            client.println("        <div class=\"Col\">");
            client.println("            <div class=\"tf1\">");
            client.println("                <div class=\"traffic-signal\">");
            client.println("                    <div class=\"red red1 light\"></div>");
            client.println("                    <div class=\"yellow yellow1 light\"></div>");
            client.println("                    <div class=\"green green1 light\"></div>");
            client.println("                </div>");
            client.println("            </div>");
            client.println("            <div class=\"tf2\">");
            client.println("                <div class=\"traffic-signal\">");
            client.println("                    <div class=\"red red1 light\"></div>");
            client.println("                    <div class=\"yellow yellow1 light\"></div>");
            client.println("                    <div class=\"green green1 light\"></div>");
            client.println("                </div>");
            client.println("            </div>");
            client.println("        </div>");
            client.println("        <div class=\"Row\">");
            client.println("            <div class=\"tf3\">");
            client.println("                <div class=\"traffic-signal\">");
            client.println("                    <div class=\"red red2 light\"></div>");
            client.println("                    <div class=\"yellow yellow2 light\"></div>");
            client.println("                    <div class=\"green green2 light\"></div>");
            client.println("                </div>");
            client.println("            </div>");
            client.println("            <div class=\"tf4\">");
            client.println("                <div class=\"traffic-signal\">");
            client.println("                    <div class=\"red red2 light\"></div>");
            client.println("                    <div class=\"yellow yellow2 light\"></div>");
            client.println("                    <div class=\"green green2 light\"></div>");
            client.println("                </div>");
            client.println("            </div>");
            client.println("        </div>");
            client.println("    </div>");
            client.println("    <div class=\"button-container\">");
            client.println("        <button type=\"button\" class=\"btn btn-primary\" id=\"button-1\">Button 1</button>");
            client.println("        <button type=\"button\" class=\"btn btn-primary\" id=\"button-2\">Button 2</button>");
            client.println("        <button type=\"button\" class=\"btn btn-primary\" id=\"button-3\">Button 3</button>");
            client.println("        <button type=\"button\" class=\"btn btn-primary\" id=\"button-4\">Button 4</button>");
            client.println("    </div>");
            client.println("    <div class=\"number\">");
            client.println("        <div class=\"number-display number-display1\"></div>");
            client.println("        <div class=\"number-display number-display2\"></div>");
            client.println("        <div class=\"colon\">:</div>");
            client.println("        <div class=\"number-display number-display3\"></div>");
            client.println("        <div class=\"number-display number-display4\"></div>");
            client.println("    </div>");
            client.println("    <script src=\"bootstrap/js/bootstrap.min.js\"></script>");
            client.println("    <script>");
            client.println("        const redLights1 = document.querySelectorAll('.red1');");
            client.println("        const yellowLights1 = document.querySelectorAll('.yellow1');");
            client.println("        const greenLights1 = document.querySelectorAll('.green1');");
            client.println("        const redLights2 = document.querySelectorAll('.red2');");
            client.println("        const yellowLights2 = document.querySelectorAll('.yellow2');");
            client.println("        const greenLights2 = document.querySelectorAll('.green2');");
            client.println("        let redBlinkingInterval, yellowBlinkingInterval, greenBlinkingInterval;");
            client.println("        let currentState = 0;");
            client.println("        let maxRed = 5, maxYellow = 2, maxGreen = 3;");
            client.println("        let i_1 = maxRed, j_1 = maxYellow, t_1 = maxGreen;");
            client.println("        let i_2 = maxRed, j_2 = maxYellow, t_2 = maxGreen;");
            client.println("        let intervalId, temp;");
            client.println("        let counter = 0, let counter2 = 1;");
            client.println("        function startUpdating() {");
            client.println("            intervalId = setInterval(getCurrentTimeInMinutesAndSeconds, 1000);");
            client.println("        }");
            client.println("        function stopUpdating() {");
            client.println("            clearInterval(intervalId);");
            client.println("        }");
            client.println("        function LED_RED_1() {");
            client.println("            yellowLights1.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            greenLights1.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            redLights1.forEach(light => light.style.backgroundColor = 'red');");
            client.println("        }");
            client.println("        function LED_YELLOW_1() {");
            client.println("            redLights1.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            greenLights1.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            yellowLights1.forEach(light => light.style.backgroundColor = 'yellow');");
            client.println("        }");
            client.println("        function LED_GREEN_1() {");
            client.println("            redLights1.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            yellowLights1.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            greenLights1.forEach(light => light.style.backgroundColor = 'green');");
            client.println("        }");
            client.println("        function LED_RED_2() {");
            client.println("            yellowLights2.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            greenLights2.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            redLights2.forEach(light => light.style.backgroundColor = 'red');");
            client.println("        }");
            client.println("        function LED_YELLOW_2() {");
            client.println("            redLights2.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            greenLights2.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            yellowLights2.forEach(light => light.style.backgroundColor = 'yellow');");
            client.println("        }");
            client.println("        function LED_GREEN_2() {");
            client.println("            redLights2.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            yellowLights2.forEach(light => light.style.backgroundColor = 'black');");
            client.println("            greenLights2.forEach(light => light.style.backgroundColor = 'green');");
            client.println("        }");
            client.println("        function getCurrentTimeInMinutesAndSeconds() {");
            client.println("            switch (currentState) {");
            client.println("                case 0: LED_RED_1(); display1(i_1--); if (i_1 < 0) { i_1 = maxRed; currentState++; } break;");
            client.println("                case 1: LED_GREEN_1(); display1(t_1--); if (t_1 < 0) { t_1 = maxGreen; currentState++; } break;");
            client.println("                case 2: LED_YELLOW_1(); display1(j_1--); if (j_1 < 0) { j_1 = maxYellow; currentState = 0; } break;");
            client.println("            }");
            client.println("            switch (currentState) {");
            client.println("                case 0: LED_RED_2(); display2(i_2--); if (i_2 < 0) { i_2 = maxRed; currentState++; } break;");
            client.println("                case 1: LED_GREEN_2(); display2(t_2--); if (t_2 < 0) { t_2 = maxGreen; currentState++; } break;");
            client.println("                case 2: LED_YELLOW_2(); display2(j_2--); if (j_2 < 0) { j_2 = maxYellow; currentState = 0; } break;");
            client.println("            }");
            client.println("        }");
            client.println("        function display1(a) {");
            client.println("            document.querySelectorAll('.number-display1').forEach(display => display.textContent = Math.floor(a / 10));");
            client.println("            document.querySelectorAll('.number-display2').forEach(display => display.textContent = a % 10);");
            client.println("        }");
            client.println("        function display2(a) {");
            client.println("            document.querySelectorAll('.number-display3').forEach(display => display.textContent = Math.floor(a / 10));");
            client.println("            document.querySelectorAll('.number-display4').forEach(display => display.textContent = a % 10);");
            client.println("        }");
            client.println("function button1(){");
            client.println("  clearInterval(redBlinkingInterval);");
            client.println("  clearInterval(yellowBlinkingInterval);");
            client.println("  clearInterval(greenBlinkingInterval);");
            client.println("  currentState = currentState + 1;");
            client.println("  if(currentState == 4)currentState = 0;");
            client.println("  switch (currentState) {");
            client.println("  case 0:");
            client.println("if(maxRed != maxYellow + maxGreen) maxYellow = maxRed - maxGreen;");
            client.println("      i_1 = maxRed, j_1 = maxYellow, t_1 = maxGreen;");
            client.println("      i_2 = maxRed, j_2 = maxYellow, t_2 = maxGreen;");
            client.println("      startUpdating();");
            client.println("      break;");
            client.println("  case 1:");
            client.println("      stopUpdating();");
            client.println("      display1(currentState);");
            client.println("      display2(maxRed);");
            client.println("      yellowLights.forEach(light => {");
            client.println("          light.style.backgroundColor = 'black';");
            client.println("      });");
            client.println("     greenLights.forEach(light => {");
            client.println("          light.style.backgroundColor = 'black';");
            client.println("      });");
            client.println("     redLights.forEach(light => {");
            client.println("         light.style.backgroundColor = 'black';");
            client.println("     });");
            client.println("      redBlinkingInterval = setInterval(() => {");
            client.println("         redLights.forEach(redLight => {");
            client.println("            redLight.style.backgroundColor = (redLight.style.backgroundColor === 'red') ? 'black' : 'red';");
            client.println("         });");
            client.println("     }, 1000);");
            client.println("      temp = maxRed;");
            client.println("      break;");

            client.println("  case 2:");
            client.println("    display1(currentState);");
            client.println("    display2(maxYellow);");
            client.println("    yellowLights.forEach(light => {");
            client.println("         light.style.backgroundColor = 'black';");
            client.println("     });");
            client.println("    greenLights.forEach(light => {");
            client.println("        light.style.backgroundColor = 'black';");
            client.println("    });");
            client.println("    redLights.forEach(light => {");
            client.println("        light.style.backgroundColor = 'black';");
            client.println("    });");
            client.println("    yellowBlinkingInterval = setInterval(() => {");
            client.println("        yellowLights.forEach(yellowLight => {");
            client.println("            yellowLight.style.backgroundColor = (yellowLight.style.backgroundColor === 'yellow') ? 'black' : 'yellow';");
            client.println("         });");
            client.println("   }, 1000);");
            client.println("   temp = maxYellow;");
            client.println("   break;");

            client.println(" case 3:");
            client.println("   display1(currentState);");
            client.println("    display2(maxGreen);");
            client.println("    yellowLights.forEach(light => {");
            client.println("        light.style.backgroundColor = 'black';");
            client.println("    });");
            client.println("    greenLights.forEach(light => {");
            client.println("       light.style.backgroundColor = 'black';");
            client.println("    });");
            client.println("    redLights.forEach(light => {");
            client.println("      light.style.backgroundColor = 'black';");
            client.println("    });");
            client.println("     greenBlinkingInterval = setInterval(() => {");
            client.println("        greenLights.forEach(greenLight => {");
            client.println("             greenLight.style.backgroundColor = (greenLight.style.backgroundColor === 'green') ? 'black' : 'green';");
            client.println("        });");
            client.println("    }, 1000);");
            client.println("     if(maxRed != maxYellow + maxGreen){");
            client.println("         maxYellow = maxRed - maxGreen;");
            client.println("       }");
            client.println("       temp = maxGreen;");
            client.println("       break;");
            client.println("  }");
            client.println("}");

            client.println("function button2(){");
            client.println("  temp++;");
            client.println("  display2(temp);");
            client.println("}");

            client.println("function button3(){");
            client.println("switch (currentState) {");
            client.println("case 1:");
            client.println("    maxRed = temp;");
            client.println("    break;");
            client.println("case 2:");
            client.println("    maxYellow = temp;");
            client.println("    break;");
            client.println("case 3:");
            client.println("    maxGreen = temp;");
            client.println("    break;");
            client.println("}");


            client.println("function button4(){");
            client.println("  temp++;");
            client.println("  display2(temp);");
            client.println("}");
            if (header.indexOf("GET /data") >= 0) {
              switch (button){
              case "1":
                client.println("button1();");
                break;
              case "2":
                client.println("button2();");
                break;
              case "3":
                client.println("button3();");
                break;
              case "4":
                client.println("button4();");
                break;
              }
            }
            
            client.println("        startUpdating();");
            client.println("    </script>");
            client.println("</body>");
            client.println("</html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  delay(1000);
}