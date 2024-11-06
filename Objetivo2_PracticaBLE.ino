// Definimos el pin donde está conectado el LED
const int ledPin = 32; // Pin 32 del ESP32

void setup() {
  // Iniciamos la comunicación serial
  Serial.begin(115200);

  // Configuramos el pin del LED como salida
  pinMode(ledPin, OUTPUT);

  // Apagamos el LED al inicio
  digitalWrite(ledPin, LOW);
  
  // Mensaje inicial
  Serial.println("Introduce 'S' para encender el LED o 'N' para apagarlo.");
}

void loop() {
  // Verificamos si hay datos disponibles en el Monitor Serial
  if (Serial.available() > 0) {
    // Leemos el comando enviado por el Monitor Serial
    char command = Serial.read();

    // Procesamos los comandos 'S' o 'N'
    if (command == 'S' || command == 's') {
      // Encendemos el LED
      digitalWrite(ledPin, HIGH);
      Serial.println("LED Encendido.");
    }
    else if (command == 'N' || command == 'n') {
      // Apagamos el LED
      digitalWrite(ledPin, LOW);
      Serial.println("LED Apagado.");
    }
    else {
      // Comando no válido
      Serial.println("Comando no válido. Introduce 'S' para encender o 'N' para apagar.");
    }
  }
}
