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

// Replace with your network credentials
const char* ssid = "bao"; // tên wifi
const char* password = "12345678"; // password của wifi
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
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));

    // Config UART1

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

  const uart_port_t uart_num1 = UART_NUM_2;
  uint8_t data1[128];
  int length1 = 0;
  ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num1, (size_t*)&length1));
  length1 = uart_read_bytes(uart_num1, data1, length1, 100);
  if (length1 > 0) {
      button = (char*) data1;
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
            client.print(R"(
            <!DOCTYPE html>
            <html lang="en">

            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Traffic Light Animation</title>
                <link rel="stylesheet" href="./bootstrap/dist/css/bootstrap.css">
                <link rel="stylesheet" href="./style.css">
                <style>
                    body { display: flex; justify-content: center; align-items: center; flex-direction: column; margin: 0; background-color: black; padding-bottom: 2000px; }
                    .inner-container { margin-top: 100px; position: relative; width: 1200px; height: 1200px; display: flex; align-items: center; justify-content: center; background-image: url("image/traffic.png"); background-size: cover; background-position: center; }
                    .Col { position: absolute; transform: rotate(90deg); align-items: center; text-align: center; display: flex; }
                    .tf1 { margin-right: 350px; }
                    .tf2 { margin-left: 350px; transform: rotate(180deg); }
                    .tf3 { margin-right: 400px; }
                    .tf4 { transform: rotate(180deg); margin-left: 350px; }
                    .Row { position: absolute; display: flex; }
                    .traffic-signal { width: 100px; height: 300px; background-color: #333; border-radius: 10px; display: flex; flex-direction: column; justify-content: space-between; align-items: center; }
                    .light { width: 60px; height: 60px; border-radius: 50%; margin: 10px; transition: background-color 0.9s; }
                    @keyframes blinkRed { 0%, 49% { background-color: red; } 50%, 100% { background-color: black; } }
                    .red { background-color: red; }
                    .yellow { background-color: yellow; animation: blinkYellow 9s infinite 3s; }
                    .green { background-color: green; animation: blinkGreen 9s infinite 6s; }
                    .button-container { margin-top: 20px; display: flex; justify-content: center; }
                    .btn { background-color: #007bff; border: none; color: white; padding: 10px 20px; margin: 0 5px; border-radius: 5px; transition: background-color 0.3s, transform 0.3s; }
                    .btn:hover { background-color: #0056b3; transform: translateY(-2px); }
                    .btn:active { transform: translateY(0); background-color: #003d7a; }
                    .red { background-color: red; }
                    .yellow { background-color: yellow; }
                    .green { background-color: green; }
                    .light {background-color: black;}
                    .number {display: flex;justify-content: center;margin-top: 10px;}
                    .number-display {font-size: 240px;font-weight: bold;margin-top: 50px;color: white;display: inline-block;border: 3px solid white;padding: 0px 30px;border-radius: 20%;}
                    .number-display1,.number-display3{margin-right: 50px;}
                    .colon {font-size: 240px;font-weight: bold;margin: 0 5px;color: white;}
                </style>
            </head>

            <body>

                <div class="inner-container">
                    <div class="Col">
                        <div class="tf1">
                            <div class="traffic-signal ">
                                <div class="red red1 light"  ></div>
                                <div class="yellow yellow1 light"></div>
                                <div class="green green1 light"></div>
                            </div>
                        </div>
                        <div class="tf2">
                            <div class="traffic-signal">
                                <div class="red red1 light"  ></div>
                                <div class="yellow yellow1 light"></div>
                                <div class="green green1 light"></div>
                            </div>
                        </div>
                    </div>
                    <div class="Row">
                        <div class="tf3">
                            <div class="traffic-signal">
                                <div class="red red2 light"></div>
                                <div class="yellow yellow2 light"></div>
                                <div class="green green2 light"></div>
                                
                            </div>
                        </div>
                        <div class="tf4">
                            <div class="traffic-signal">
                                <div class="red red2 light"></div>
                                <div class="yellow yellow2 light"></div>
                                <div class="green green2 light"></div>
                            </div>
                        </div>
                    </div>
                </div>
                <br>
                <br>
                <div class="inner-button">
                    <button type="button" class="btn btn-primary" id="button-1">Button 1</button>
                    <button type="button" class="btn btn-primary" id="button-2">Button 2</button>
                    <button type="button" class="btn btn-primary" id="button-3">Button 3</button>
                    <button type="button" class="btn btn-primary" id="button-4">Button 4</button>
                </div>
                <div class="number">
                    <div class="number-display number-display1"></div>
                    <div class="number-display number-display2"></div>
                    <div class="colon">:</div>
                    <div class="number-display number-display3"></div>
                    <div class="number-display number-display4"></div>
                </div>
                <script src="bootstrap/js/bootstrap.min.js"></script>
                <script>
                    const button1 = document.getElementById('button-1');
                    const button2 = document.getElementById('button-2');
                    const button3 = document.getElementById('button-3');
                    const button4 = document.getElementById('button-4');
                    
                    const redLights = document.querySelectorAll('.red');
                    const yellowLights = document.querySelectorAll('.yellow');
                    const greenLights = document.querySelectorAll('.green');
                    
                    const redLights1 = document.querySelectorAll('.red1');
                    const yellowLights1 = document.querySelectorAll('.yellow1');
                    const greenLights1 = document.querySelectorAll('.green1');
                    
                    const redLights2 = document.querySelectorAll('.red2');
                    const yellowLights2 = document.querySelectorAll('.yellow2');
                    const greenLights2 = document.querySelectorAll('.green2');
                    
                    const numberDisplays1 = document.querySelectorAll('.number-display1');
                    const numberDisplays2 = document.querySelectorAll('.number-display2');
                    const numberDisplays3 = document.querySelectorAll('.number-display3');
                    const numberDisplays4 = document.querySelectorAll('.number-display4');
                    
                    let redBlinkingInterval;
                    let yellowBlinkingInterval;
                    let greenBlinkingInterval;
                    let currentState = 0;
                    let maxRed = 5;
                    let maxYellow = 2;
                    let maxGreen = 3;
                    let temp;
                    let i_1 = maxRed, j_1 = maxYellow, t_1 = maxGreen;
                    let i_2 = maxRed, j_2 = maxYellow, t_2 = maxGreen;
                    let counter = 0;
                    let counter2 = 1;
                    let intervalId;
                    //let button1,button2,button3,button4;
                    
                    // Cập nhật hiển thị mỗi giây
                    function startUpdating() {
                        intervalId = setInterval(getCurrentTimeInMinutesAndSeconds, 1000);
                    }
                    
                    function stopUpdating() {
                        clearInterval(intervalId);
                    }
                    
                    function LED_RED_1() {
                        yellowLights1.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        greenLights1.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        redLights1.forEach(light => {
                            light.style.backgroundColor = 'red';
                        });
                    }
                    
                    function LED_YELLOW_1() {
                        redLights1.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        greenLights1.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        yellowLights1.forEach(light => {
                            light.style.backgroundColor = 'yellow';
                        });
                    }
                    
                    function LED_GREEN_1() {
                        redLights1.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        yellowLights1.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        greenLights1.forEach(light => {
                            light.style.backgroundColor = 'green';
                        });
                    }
                    
                    function LED_RED_2() {
                        yellowLights2.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        greenLights2.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        redLights2.forEach(light => {
                            light.style.backgroundColor = 'red';
                        });
                    }
                    
                    function LED_YELLOW_2() {
                        redLights2.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        greenLights2.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        yellowLights2.forEach(light => {
                            light.style.backgroundColor = 'yellow';
                        });
                    }
                    
                    function LED_GREEN_2() {
                        redLights2.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        yellowLights2.forEach(light => {
                            light.style.backgroundColor = 'black';
                        });
                        greenLights2.forEach(light => {
                            light.style.backgroundColor = 'green';
                        });
                    }
                    
                    function getCurrentTimeInMinutesAndSeconds() {
                        // Cập nhật trạng thái và hiển thị cho bộ đèn 1
                        switch (counter) {
                            case 0:
                                LED_RED_1();
                                display1(i_1);
                                i_1--;
                                if (i_1 == 0) {
                                    i_1 = maxRed;
                                    counter++;
                                }
                                break;
                            case 1:
                                LED_GREEN_1();
                                display1(t_1);
                                t_1--;
                                if (t_1 == 0) {
                                    t_1 = maxGreen;
                                    counter++;
                                }
                                break;
                            case 2:
                                LED_YELLOW_1();
                                display1(j_1);
                                j_1--;
                                if (j_1 == 0) {
                                    j_1 = maxYellow;
                                    counter = 0;
                                }
                                break;
                        }
                    
                        // Cập nhật trạng thái và hiển thị cho bộ đèn 2
                        switch (counter2) {
                            case 0:
                                LED_RED_2();
                                display2(i_2);
                                i_2--;
                                if (i_2 == 0) {
                                    i_2 = maxRed;
                                    counter2++;
                                }
                                break;
                            case 1:
                                LED_GREEN_2();
                                display2(t_2);
                                t_2--;
                                if (t_2 == 0) {
                                    t_2 = maxGreen;
                                    counter2++;
                                }
                                break;
                            case 2:
                                LED_YELLOW_2();
                                display2(j_2);
                                j_2--;
                                if (j_2 == 0) {
                                    j_2 = maxYellow;
                                    counter2 = 0;
                                }
                                break;
                        }
                    }
                    
                    function display1(a) {
                        document.querySelectorAll('.number-display1').forEach(display => display.textContent = Math.floor(a / 10));
                        document.querySelectorAll('.number-display2').forEach(display => display.textContent = a % 10);
                    }
                    
                    function display2(a) {
                        numberDisplays3.forEach(display => {
                            display.textContent = Math.floor(a / 10); 
                        });
                    
                        numberDisplays4.forEach(display => {
                            display.textContent = a % 10;
                        });
                    }
                    
                    
                    function button1(){
                        clearInterval(redBlinkingInterval);
                        clearInterval(yellowBlinkingInterval);
                        clearInterval(greenBlinkingInterval);

                        currentState = currentState + 1;
                        if(currentState == 4){
                            currentState = 0;
                        }
                        
                    
                        switch (currentState) {
                            case 0:
                                if(maxRed != maxYellow + maxGreen){
                                    maxYellow = maxRed - maxGreen;
                                }
                                i_1 = maxRed, j_1 = maxYellow, t_1 = maxGreen;
                                i_2 = maxRed, j_2 = maxYellow, t_2 = maxGreen;
                                startUpdating();
                                break;
                            case 1:
                                stopUpdating();
                                display1(currentState);
                                display2(maxRed);
                                yellowLights.forEach(light => {
                                    light.style.backgroundColor = 'black';
                                });
                                greenLights.forEach(light => {
                                    light.style.backgroundColor = 'black';
                                });
                                redLights.forEach(light => {
                                    light.style.backgroundColor = 'black';
                                });
                                redBlinkingInterval = setInterval(() => {
                                    redLights.forEach(redLight => {
                                        redLight.style.backgroundColor = (redLight.style.backgroundColor === 'red') ? 'black' : 'red';
                                    });
                                }, 1000);
                                temp = maxRed;
                                break;
                    
                            case 2:
                                display1(currentState);
                                display2(maxYellow);
                                yellowLights.forEach(light => {
                                    light.style.backgroundColor = 'black';
                                });
                                greenLights.forEach(light => {
                                    light.style.backgroundColor = 'black';
                                });
                                redLights.forEach(light => {
                                    light.style.backgroundColor = 'black';
                                });
                                yellowBlinkingInterval = setInterval(() => {
                                    yellowLights.forEach(yellowLight => {
                                        yellowLight.style.backgroundColor = (yellowLight.style.backgroundColor === 'yellow') ? 'black' : 'yellow';
                                    });
                                }, 1000);
                                temp = maxYellow;
                                break;
                            case 3:
                                display1(currentState);
                                display2(maxGreen);
                                yellowLights.forEach(light => {
                                    light.style.backgroundColor = 'black';
                                });
                                greenLights.forEach(light => {
                                    light.style.backgroundColor = 'black';
                                });
                                redLights.forEach(light => {
                                    light.style.backgroundColor = 'black';
                                });
                                greenBlinkingInterval = setInterval(() => {
                                    greenLights.forEach(greenLight => {
                                        greenLight.style.backgroundColor = (greenLight.style.backgroundColor === 'green') ? 'black' : 'green';
                                    });
                                }, 1000);
                                temp = maxGreen;
                                break;
                        }
                    };
                    )");
                    if (header.indexOf("GET /data") >= 0) {
                      if (String(button) == "1") {
                          client.println("button1();");
                      } else if (String(button) == "2") {
                          client.println("button2();");
                      } else if (String(button) == "3") {
                          client.println("button3();");
                      } else if (String(button) == "4"){
                          client.println("button4();");
                      }
                      }
                    client.print(R"(
                    startUpdating();

                    function button2(){
                        temp++;
                        display2(temp);
                    };
                    function button3(){
                        switch (currentState) {
                        case 1:
                            maxRed = temp;
                            break;
                        case 2:
                            maxYellow = temp;
                            break;
                        case 3:
                            maxGreen = temp;
                            break;
                        }
                    };

                    function button4() {
                        temp--;
                        display2(temp);
                    };
                    
                    ;</script>
            </body>

            </html>
            )");
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