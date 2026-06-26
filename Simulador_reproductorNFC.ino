#include <Wire.h>
#include <HardwareSerial.h>
#include <Adafruit_PN532.h>
#include <DFRobotDFPlayerMini.h>

// 📌 Configuración de Pines I2C (Pines seguros)
#define SDA_PIN 5
#define SCL_PIN 6
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

// 📌 Configuración de Pines Hardware Serial UART1
#define RX_PIN 21  
#define TX_PIN 20  
HardwareSerial dfSerial(1);
DFRobotDFPlayerMini myDFPlayer;

bool dfPlayerListo = false;
String tarjetaActual = ""; 
bool sonando = false;

// ⏱️ CONFIGURACIÓN DEL FILTRO DE TOLERANCIA
int lecturasFallidas = 0;
const int MAX_FALLOS_TOLERADOS = 6; // Número de ciclos que esperamos antes de apagar la música
                                    // 6 ciclos x ~80ms de bucle = ~500ms (medio segundo de tolerancia)

void setup() {
  Serial.begin(115200);
  delay(1000); 
  Serial.println(F("\n--- REPRODUCTOR CON TOLERANCIA ANTIRREBOTE ---"));

  Wire.begin(SDA_PIN, SCL_PIN);
  
  dfSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  if (!myDFPlayer.begin(dfSerial)) {
    Serial.println(F("⚠️ ADVERTENCIA: DFPlayer no detectado. Modo simulación activo."));
    dfPlayerListo = false;
  } else {
    Serial.println(F("¡DFPlayer Mini detectado con éxito!"));
    myDFPlayer.volume(20);
    dfPlayerListo = true;
  }

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println(F("❌ ERROR CRÍTICO: No se encontró el módulo PN532."));
    while (1);
  }
  
  nfc.SAMConfig();
  Serial.println(F("\n✅ Filtro activado. Coloca un disco..."));
}

void loop() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLength;                        
  
  // Timeout muy bajo (30ms) para mantener el sistema ágil
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 30);
  
  if (success) {
    // Si lee la tarjeta con éxito, reseteamos el contador de fallos inmediatamente
    lecturasFallidas = 0;

    String uidString = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) uidString += "0";
      uidString += String(uid[i], HEX);
    }
    uidString.toUpperCase();

    // Si es una tarjeta nueva (o una que vuelve tras haberse apagado del todo)
    if (uidString != tarjetaActual) {
      tarjetaActual = uidString;
      sonando = true;
      
      Serial.println(F("============================================="));
      Serial.print(F("💿 Disco detectado firmemente! UID: "));
      Serial.println(uidString);

      // Selección de pista según el UID real que tengas
      if (uidString == "1D228EDF081080") { 
        Serial.println(F("🎵 Sonando: 0001.mp3 (Disco 1)"));
        if(dfPlayerListo) myDFPlayer.playMp3Folder(1); 
      } 
      else if (uidString == "04070A434F6180") {
        Serial.println(F("🎵 Sonando: 0002.mp3 (Disco 2)"));
        if(dfPlayerListo) myDFPlayer.playMp3Folder(2); 
      } 
      else if (uidString == "041CC8414F6181") {
        Serial.println(F("🎵 Sonando: 0003.mp3 (Disco 3)"));
        if(dfPlayerListo) myDFPlayer.playMp3Folder(3); 
      } 
      else if (uidString == "041456424F6180") {
        Serial.println(F("🎵 Sonando: 0004.mp3 (Disco 4)"));
        if(dfPlayerListo) myDFPlayer.playMp3Folder(4); 
      } 
      else if (uidString == "0474DFAA8F6180") {
        Serial.println(F("🎵 Sonando: 0005.mp3 (Disco 5)"));
        if(dfPlayerListo) myDFPlayer.playMp3Folder(5); 
      } 
      else if (uidString == "048485414F6180") {
        Serial.println(F("🎵 Sonando: 0006.mp3 (Disco 6)"));
        if(dfPlayerListo) myDFPlayer.playMp3Folder(6); 
      } 
      else {
        Serial.println(F("❓ Disco no registrado en el sistema."));
        if(dfPlayerListo) myDFPlayer.stop();
        sonando = false;
      }
      Serial.println(F("============================================="));
    }
  } 
 else {
    // Si no detecta tarjeta en este ciclo específico...
    if (sonando) {
      lecturasFallidas++; // Añadimos un voto de duda
      
      // Imprime un aviso sutil en el monitor serie para que veas el "ruido" físico
      Serial.print(F(".")); 

      // Sólo si los fallos acumulados superan nuestro límite, confirmamos que se ha quitado
      if (lecturasFallidas >= MAX_FALLOS_TOLERADOS) {
        Serial.println(F("\n🛑 Disco retirado definitivamente. Deteniendo música."));
        
        if (dfPlayerListo) {
          myDFPlayer.stop(); 
        }
        
        tarjetaActual = ""; 
        sonando = false;    
        lecturasFallidas = 0;
        Serial.println(F("---------------------------------------------"));
      }
    }
  }

  // Tiempo de espera entre lecturas (ajustado a 50ms)
  delay(50); 
}