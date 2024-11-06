// Definimos el pin donde está conectado el potenciómetro
const int potPin = 35; // Pin D35 del ESP32 para la lectura analógica

// Definimos la resolución máxima del ADC (12 bits -> 0-4095)
const int maxADCValue = 4095;
const float referenceVoltage = 3.3; // Voltaje de referencia del ESP32 (3.3V)

void setup() {
  // Iniciamos la comunicación serial
  Serial.begin(115200);

  // Configuramos el pin como entrada analógica
  pinMode(potPin, INPUT);
}

void loop() {
  // Leemos el valor del potenciómetro (0 - 4095)
  int adcValue = analogRead(potPin);

  // Convertimos la lectura del ADC al voltaje correspondiente
  float voltage = (adcValue * referenceVoltage) / maxADCValue;

  // Mostramos la lectura y el voltaje en el Monitor Serial
  Serial.print("Lectura ADC: ");
  Serial.print(adcValue);
  Serial.print("  - Voltaje: ");
  Serial.print(voltage);
  Serial.println(" V");

  // Pausa de 500ms para evitar demasiada saturación de datos en el monitor
  delay(500);
}