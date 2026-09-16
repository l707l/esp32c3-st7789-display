#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <WiFi.h>

/*
 ==============================================================================
  FIRMWARE A LA MEDIDA: ESP32-C3 SUPER MINI + LCD TFT 1.69" (ST7789V3 240x280)
 ==============================================================================
  
  DIAGRAMA DE CONEXION:
  -------------------------------------------------------------
  PANTALLA LCD (8 Pines)    ESP32-C3 SUPER MINI     NOTAS
  -------------------------------------------------------------
  GND                       GND                     Tierra común
  VCC                       3.3V (o 5V)             Alimentación
  SCL                       GPIO 6                  SPI Clock (SCK)
  SDA                       GPIO 7                  SPI Data (MOSI)
  RES                       GPIO 3                  Reset
  DC                        GPIO 4                  Data / Command
  CS                        GPIO 10                 Chip Select
  BLK                       GPIO 1                  Backlight (Control PWM)
  -------------------------------------------------------------
*/

// --- Definición de pines ---
#define TFT_SCL   6   // Reloj SPI por hardware
#define TFT_SDA   7   // Datos SPI (MOSI) por hardware
#define TFT_RES   3   // Reset
#define TFT_DC    4   // Data / Command
#define TFT_CS   10   // Chip Select
#define TFT_BLK   1   // Retroiluminación (Backlight)
#define BOARD_LED 8   // LED azul integrado en el ESP32-C3 Super Mini (activo en LOW)

// Instancia de pantalla
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RES);

// Paleta de colores RGB565 atractiva
#define COLOR_BG        0x0842  // Azul oscuro / noche
#define COLOR_CARD      0x18E5  // Gris azulado moderno
#define COLOR_ACCENT    0x07E0  // Verde neón
#define COLOR_CYAN      0x07FF  // Cyan vibrante
#define COLOR_ORANGE    0xFD20  // Naranja
#define COLOR_WHITE     0xFFFF  // Blanco puro
#define COLOR_GRAY      0x7BEF  // Gris suave

int animCounter = 0;
unsigned long lastUpdate = 0;
int wifiNetworksFound = 0;

void setup() {
  // Inicializar puerto serie USB CDC
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[SISTEMA] Iniciando ESP32-C3 Super Mini...");

  // Configuración de LED integrado
  pinMode(BOARD_LED, OUTPUT);
  digitalWrite(BOARD_LED, HIGH); // Apagado inicialmente (activo en LOW)

  // Configuración del pin de luz de fondo (BLK)
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, HIGH); // Encender luz de fondo

  // Inicializar bus SPI en los pines definidos
  SPI.begin(TFT_SCL, -1, TFT_SDA, TFT_CS);

  // Inicializar ST7789 para resolución 240x280
  Serial.println("[TFT] Inicializando ST7789V3 (240x280)...");
  tft.init(240, 280);

  // Invertir colores (casi todas las pantallas IPS ST7789V3 lo requieren)
  tft.invertDisplay(true);

  // Orientación vertical estándar (0 o 2 para vertical, 1 o 3 para horizontal)
  // 0: 240 ancho x 280 alto
  tft.setRotation(0);

  // Pantalla de bienvenida
  drawBootScreen();
  delay(2000);

  // Escaneo rápido de Wi-Fi inicial para telemetría
  Serial.println("[WIFI] Escaneando redes cercanas...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  wifiNetworksFound = WiFi.scanNetworks();
  Serial.printf("[WIFI] Redes encontradas: %d\n", wifiNetworksFound);

  // Dibujar plantilla base del Dashboard
  drawDashboardUI();
}

void drawBootScreen() {
  tft.fillScreen(ST77XX_BLACK);

  // Marcos estéticos redondeados
  tft.drawRoundRect(8, 8, 224, 264, 12, COLOR_CYAN);
  tft.drawRoundRect(10, 10, 220, 260, 10, COLOR_ORANGE);

  // Título
  tft.setTextSize(2);
  tft.setTextColor(COLOR_WHITE);
  tft.setCursor(30, 45);
  tft.print("ESP32-C3");

  tft.setTextSize(1);
  tft.setTextColor(COLOR_ACCENT);
  tft.setCursor(30, 70);
  tft.print("SUPER MINI EDITION");

  // Barra divisora
  tft.drawFastHLine(30, 85, 180, COLOR_GRAY);

  // Datos de pantalla
  tft.setTextColor(COLOR_WHITE);
  tft.setCursor(30, 105);
  tft.print("Display: ST7789V3");

  tft.setCursor(30, 125);
  tft.print("Resolucion: 240x280");

  tft.setCursor(30, 145);
  tft.print("Bus: Hardware SPI");

  // Animación de barra de carga
  tft.drawRoundRect(30, 180, 180, 18, 6, COLOR_WHITE);
  for (int i = 0; i <= 172; i += 6) {
    tft.fillRoundRect(34, 184, i, 10, 4, COLOR_CYAN);
    digitalWrite(BOARD_LED, (i % 12 == 0) ? LOW : HIGH);
    delay(25);
  }
  digitalWrite(BOARD_LED, HIGH);

  tft.setTextColor(COLOR_ACCENT);
  tft.setCursor(70, 215);
  tft.print("SISTEMA LISTO!");
}

void drawDashboardUI() {
  tft.fillScreen(COLOR_BG);

  // Encabezado superior
  tft.fillRoundRect(10, 10, 220, 38, 8, COLOR_CARD);
  tft.setTextSize(2);
  tft.setTextColor(COLOR_CYAN);
  tft.setCursor(20, 21);
  tft.print("C3 MONITOR");

  // Indicador de actividad (círculo)
  tft.fillCircle(210, 29, 6, COLOR_ACCENT);

  // Tarjeta 1: CPU & Memoria
  tft.fillRoundRect(10, 56, 220, 68, 8, COLOR_CARD);
  tft.setTextSize(1);
  tft.setTextColor(COLOR_GRAY);
  tft.setCursor(20, 66);
  tft.print("CHIP & MEMORIA");

  tft.setTextColor(COLOR_WHITE);
  tft.setCursor(20, 82);
  tft.print("CPU: RISC-V @ 160 MHz");

  tft.setCursor(20, 96);
  tft.printf("Flash: %d MB | FlashMode: QIO", ESP.getFlashChipSize() / (1024 * 1024));

  tft.setCursor(20, 110);
  tft.printf("RAM Libre: %d KB", ESP.getFreeHeap() / 1024);

  // Tarjeta 2: Redes Wi-Fi detectadas
  tft.fillRoundRect(10, 132, 220, 52, 8, COLOR_CARD);
  tft.setTextColor(COLOR_GRAY);
  tft.setCursor(20, 142);
  tft.print("CONECTIVIDAD");

  tft.setTextColor(COLOR_ORANGE);
  tft.setCursor(20, 158);
  tft.printf("Wi-Fi Scan: %d redes cerca", wifiNetworksFound);

  tft.setTextColor(COLOR_WHITE);
  tft.setCursor(20, 170);
  tft.printf("MAC: %s", WiFi.macAddress().c_str());

  // Tarjeta 3: Telemetría en vivo y animación
  tft.fillRoundRect(10, 192, 220, 78, 8, COLOR_CARD);
  tft.setTextColor(COLOR_GRAY);
  tft.setCursor(20, 202);
  tft.print("ESTADO EN VIVO");
}

void loop() {
  unsigned long currentMillis = millis();

  // Actualizar datos y animación cada 100 ms
  if (currentMillis - lastUpdate >= 100) {
    lastUpdate = currentMillis;
    animCounter++;

    // Parpadeo suave del LED integrado cada segundo
    if (animCounter % 10 == 0) {
      digitalWrite(BOARD_LED, !digitalRead(BOARD_LED));
    }

    // Actualizar Uptime y Heap en la tarjeta inferior
    tft.fillRect(20, 218, 200, 20, COLOR_CARD);
    tft.setTextSize(1);
    tft.setTextColor(COLOR_WHITE);
    tft.setCursor(20, 220);
    long seg = currentMillis / 1000;
    tft.printf("Uptime: %02ldm %02lds", seg / 60, seg % 60);

    tft.setCursor(140, 220);
    tft.printf("RAM: %dK", ESP.getFreeHeap() / 1024);

    // Barra de onda animada (visualizador gráfico)
    int barWidth = (animCounter % 30) * 6;
    tft.fillRoundRect(20, 246, 200, 12, 4, 0x10A2); // Fondo de barra
    tft.fillRoundRect(20, 246, barWidth + 10, 12, 4, COLOR_ACCENT);

    // Mensaje periódico por puerto serie
    if (animCounter % 50 == 0) {
      Serial.printf("[TELEMETRIA] Uptime: %lds | Heap: %d bytes | Display 240x280 OK\n",
                    seg, ESP.getFreeHeap());
    }
  }
}
