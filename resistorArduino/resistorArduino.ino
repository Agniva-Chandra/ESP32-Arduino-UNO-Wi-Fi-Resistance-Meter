const int sensorPin = A0;      // Analog input pin
float Vin = 5.00;              // Arduino's 5V pin voltage
float Rref = 1000.0;           // Known resistor (1kΩ)
float Vout = 0.0;              // Measured voltage
float R = 0.0;                 // Calculated unknown resistor

// E24 standard resistor values
const float standardResistors[] = {
  1000, 1100, 1200, 1300, 1500, 1600, 1800, 2000, 2200, 2400,
  2700, 3000, 3300, 3600, 3900, 4300, 4700, 5100, 5600, 6200,
  6800, 7500, 8200, 9100, 10000
};
const int numStandard = sizeof(standardResistors) / sizeof(float);
const float tolerance = 0.05;  // 5% tolerance

float roundToStandard(float value) {
  for (int i = 0; i < numStandard; i++) {
    float lowBound = standardResistors[i] * (1 - tolerance);
    float highBound = standardResistors[i] * (1 + tolerance);
    if (value >= lowBound && value <= highBound) {
      return standardResistors[i];
    }
  }
  return value; // if no match, return original
}

void setup() {
  // Serial.begin(115200);     // Debug (PC)
  Serial.begin(115200);    // Send to ESP32
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Vout = (Vin * sensorValue) / 1023.0;

  String outText = "";

  if (Vout > 0 && Vout < Vin) {
    R = Rref * (Vout / (Vin - Vout));
    float roundedR = roundToStandard(R);

    if (roundedR >= 1000) {
      outText += String(roundedR / 1000, 1) + " kΩ";
    } else {
      outText += String(roundedR, 0) + " Ω";
    }
  } 
  else if (Vout >= Vin) {
    outText = "0 Ω";
  } 
  else {
    outText = "Open";
  }

  // Send to ESP32
  Serial.println(outText);

  // Debug to PC
  // Serial.println(outText);

  delay(5);
}