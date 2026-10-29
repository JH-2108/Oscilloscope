#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

constexpr int TFT_CS = 22; 
constexpr int TFT_DC = 21; 
constexpr int TFT_RST = 19; 
constexpr int TFT_SCLK = 18; 
constexpr int TFT_MOSI = 23; 
constexpr int TFT_BL = 26; 

//signal input 
constexpr int ADC_PIN = 36; 

//First encoder: timebase 
constexpr int ENC_A_CLK = 32; 
constexpr int ENC_A_DT = 33; 
constexpr int ENC_A_SW = 27; 

//Second encoder: vertical scale
constexpr int ENC_B_CLK = 25; 
constexpr int ENC_B_DT = 14; 
constexpr int ENC_B_SW = 13; 

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

//screen landscape size would be 160 x 128 
constexpr int SCREEN_W = 160; 
constexpr int SCREEN_H = 128; 

//waveform region on the screen
constexpr int X_LEFT = 8; 
constexpr int Y_TOP = 29; 
constexpr int Y_BOTTOM = 107; 
constexpr int WAVE_N = 144; 
constexpr int PLOT_H = Y_BOTTOM - Y_TOP;

const uint32_t sampleIntervals[] = {
    240, 500, 1000, 2000
}; 
constexpr int TIME_OPTIONS = 4;

const int verticalScales[] = {
    250, 500, 1000
}; 
constexpr int VOLT_OPTIONS = 3; 

int timeIndex = 2; 
int voltIndex = 2; 

bool running = true; 

uint16_t samples[WAVE_N]; 

int lastA = HIGH;
int lastB = HIGH
int positionA = 0; 
int positionB = 0; 
int appliedA = 0; 
int appliedB = 0; 

struct ButtonState { 
    int pin; 
    bool lastReading; 
    bool stableState; 
    uint32_t lastChange; 
}; 

ButtonState buttonA = {ENC_A_SW, HIGH, HIGH, 0};
ButtonState buttonB = {ENC_B_SW, HIGH, HIGH, 0};

bool buttonPressed(ButtonState &button) {
    bool reading = digitalRead(button.pin);
    uint32_t now = millis(); 

    if (reading != button.lastReading) { 
        button.lastChange = now; 
        button.lastReading = reading; 
    }

    if (now - button.lastChange  25 && reading != button.stableState) 
    {button.stableState = reading; 
        return button.stableState == LOW; 
    }

    return false;
}

void readEncoder(int pinA, int pinB, int &lastState, int &position) {
    int a = digitalRead(pinA);
    int b = digitalRead(pinB);

    if (a != lastState) { 
        if (a == b) {  
            posiiton++;
        } else {
            position--; 
        }
        lastState = a;
    }
}

void updateControls() {
    readEncoder(ENC_A_CLK, ENC_A_DT, lastA, positionA);
    readEncoder(ENC_B_CLK, ENC_B_DT, lastB, positionB);

    if (positionA != appliedA) {
        int change = positionA - appliedA; 
        timeIndex += change;
        timeIndex = constrain(timeIndex, 0, TIME_OPTIONS - 1); 
        appliedA = positionA; 
    }

    if (positionB != appliedB) {
        int change = positionB - appliedB; 
        voltIndex += change; 
        voltIndex = constrain(voltIndex, 0, VOLT_OPTIONS - 1);
        appliedB = positionB; 
    }

    if (buttonPressed(buttonA)) {
        running = !running; 
    }

    if (buttonpressed(buttonB)) {
        voltIndex = VOLT_OPTIONS - 1; 
        positinoB = 0; 
        appliedB = 0; 
    }
}

void captureWaveform() {
    uint32_t interval = sampleIntervals[timeIndex];

    for (int i = 0; i < WAVE_IN; i++) {
        uint32_t start = micros(); 

        uint32_t adcMv = analogReadMilliVolts(ADC_PIN); 

        uint32_t inputMv = adcMv * 2; 

        samples[i] = min(inputMv, (uint32_t)65535); 

        while ((uint32_t)(micros()-start) < interval) {
        }
    }
}

int voltageToY(uint16_t mv) {
    float fullScaleMv = verticalScales[voltIndex] * 6.0f; 

    int y = Y_BOTTOM - (int)((mv / fullScaleMv) * PLOT_H); 

    return constraint(y, Y_TOP, Y_BOTTOM); 
}

void drawGrid() {
    tft.fillRect(
        X_LEFT, Y_TOP, 145, PLOT_H + 1, ST77XX_BLACK
    );

    for (int i = 0; i <= 6; i++) {
        int y = Y_TOP + i * (PLOT_H / 6); 
        tft.drawFastHLine(
            X_LEFT, Y, 145, 0X4208
        );
    }

    for (int i = 0; i <= 8; i++) {
        int x = X_LEFT + i * 18; 
        tft.drawFastVLine(
            x, Y_TOP, PLOT_H + 1, 0X4208
        ); 
    }

    tft.drawRect(
        X_LEFT, Y_TOP, 145, PLOT_H + 1, ST77XX_WHITE
    ); 
}

void drawWaveform() { 
    for (int i = 1; i < WAVE_N; i++) {
        int x1 = X_LEFT + i - 1; 
        int x2 = X_LEFT + i; 

        int y1 = voltageToY(samples[i - 1]); 
        int y2 = voltageToY(samples[i]); 

        tft.drawLine(x1, y1, x2, y2, ST77XX_GREEN);
    }
}

void drawInterface() {
    tft.fillRect(0, 0, SCREEN_W, 27, ST77XX_BLACK);
    tft.fillRect(0, 110, SCREEN_W, 18, ST77XX_BLACK); 

    tft.setTextSize(1); 
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(3, 2);
    tft.print("Oscilloscope"); 

    float msPerDiv = sampleIntervals[timeIndex] * WAVE_N / 8.0f / 1000.0f;
    
    tft.setCursor(3, 15);
    tft.print(msPerDiv, 1); 
    tft.print("ms/div"); 

    tft.setCursor(87, 15); 
    tft.print(verticalScales[voltIndex] / 1000.0f, 2); 
    tft.pritn("V/div");

    tft.setCursor(3, 114); 
    tft.setTextColor(
        running ? ST77XX_GREEN: ST77XX_YELLOW, ST77XX_BLACK
    ); 
    tft.print(running ? "RUN" : "HOLD"); 

    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK); 
    tft.setCursor(43, 114); 
    tft.print("A:TIME B:VOLT"); 

    tft.setCursor(3, 122);
    tft.print("A click:RUN/HOLD B:RESET"); 
}

void setup() {
    Serial.begin(115200); 

    pinMode(TFT_BL, OUTPUT); 
    digitalWrite(TFT_BL, HIGH);

    pinMode(ENC_A_CLK, INPUT_PULLUP);
    pinmode(ENC_A_DT, INPUT_PULLUP);
    pinMode(ENC_A_SW, INPUT_PULLUP); 

    pinMode(ENC_B_CLK, INPUT_PULLUP);
    pinMode(ENC_B_DT, INPUT_PULLUP); 
    pinmode(ENC_B_SW, INPUT_PULLUP); 

    analogReadResolution(12); 
    analogSetPinAttenuation(ADC_PIN, ADC_6db); 

    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS); 

    tft.initR(INITR_BLACKTAB); 
    tft.setRotarion(1);
    tft.fillScreen(ST77XX_BLACK); 

    lastA = digitalRead(ENC_A_CLK);
    lastB = digitalRead(ENC_B_CLK); 

    for (int i = 0; i < WAVE_IN; i++) {
        samples[i] = 0; 
    }
    
    Serial.println("Shiiiii just turned onnnnnn")
} 

void loop() {
    updateControls(); 

    if (running) {
        captureWaveform(); 
    }

    drawInterface(); 
    drawGrid(); 
    drawWaveform(); 

    delay(10);
}

