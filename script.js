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
let i_1 = maxRed, j_1 = maxYellow, t_1 = maxGreen;
let i_2 = maxRed, j_2 = maxYellow, t_2 = maxGreen;
let counter = 0;
let counter2 = 1;

let intervalId;

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
    numberDisplays1.forEach(display => {
        display.textContent = Math.floor(a / 10); 
    });

    numberDisplays2.forEach(display => {
        display.textContent = a % 10; 
    });
}

function display2(a) {
    numberDisplays3.forEach(display => {
        display.textContent = Math.floor(a / 10); 
    });

    numberDisplays4.forEach(display => {
        display.textContent = a % 10;
    });
}


button1.addEventListener('click', () => {
    clearInterval(redBlinkingInterval);
    clearInterval(yellowBlinkingInterval);
    clearInterval(greenBlinkingInterval);

    currentState = currentState + 1;
    if(currentState == 4){
        currentState = 0;
    }
    

    switch (currentState) {
        case 0:
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
            if(maxRed != maxYellow + maxGreen){
                maxYellow = maxRed - maxGreen;
            }
            break;
    }
});

startUpdating();

button2.addEventListener('click', () => {
    if(currentState == 1){
        maxRed++;
        display2(maxRed);
    }
    else if(currentState == 2){
        maxYellow++;
        display2(maxYellow);
    }
    else if(currentState == 3){
        maxGreen++;
        display2(maxGreen);
    }
});

button3.addEventListener('click', () => {
    
});

button4.addEventListener('click', () => {
    if(currentState == 1){
        maxRed--;
        display2(maxRed);
    }
    else if(currentState == 2){
        maxYellow--;
        display2(maxYellow);
    }
    else  if(currentState == 3){
        maxGreen--;
        display2(maxGreen);
    }
});