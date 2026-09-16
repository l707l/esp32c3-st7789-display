# ESP32-C3 Super Mini + LCD TFT 1.69" ST7789V3 (240x280)

Firmware personalizado para microcontrolador **ESP32-C3 Super Mini** (arquitectura RISC-V a 160 MHz) y pantalla a color **LCD IPS TFT de 1.69 pulgadas con controlador ST7789V3** y resolución de **240x280 píxeles** (8 pines SPI).

---

## 📸 Características

- **Compensación de Offset (Centrado 240x280):** Corrección automática del desfase de memoria interno de 20 píxeles.
- **Inversión de Color IPS:** Colores precisos y vivos mediante `tft.invertDisplay(true)`.
- **Hardware SPI de Alta Velocidad:** Pines asignados al periférico hardware SPI nativo para máxima tasa de refresco sin parpadeos.
- **Pantalla de Bienvenida:** Logo estilizado, recuadros redondeados y barra de progreso animada al arrancar.
- **Dashboard de Telemetría en Tiempo Real:**
  - Información de CPU y reloj (RISC-V @ 160 MHz).
  - Estado de memoria Flash (4 MB QIO) y Heap/RAM libre en vivo.
  - Escáner de redes Wi-Fi en segundo plano con contador de redes detectadas y dirección MAC física.
  - Barra de estado animada y contador de tiempo de actividad (*Uptime*).
- **Control de Luz de Fondo (BLK):** Salida por GPIO 1 para control de encendido o modulación PWM de brillo.
- **Telemetría Serial:** Salida de diagnóstico a 115200 bps por USB CDC nativo.

---

## 🔌 Diagrama de Conexiones (Pinout)

Conecta los 8 pines del módulo LCD al ESP32-C3 Super Mini según la siguiente tabla:

| Pin LCD (8 pines) | Pin ESP32-C3 Super Mini | Tipo | Descripción |
| :---: | :---: | :---: | :--- |
| **GND** | **GND** | Alimentación | Tierra común |
| **VCC** | **3.3V** *(o 5V)* | Alimentación | Alimentación de la pantalla |
| **SCL** | **GPIO 6** | Salida SPI | Reloj SPI (SPI Clock / SCK) |
| **SDA** | **GPIO 7** | Salida SPI | Datos SPI (SPI MOSI) |
| **RES** | **GPIO 3** | Salida Digital | Reset del controlador ST7789 |
| **DC** | **GPIO 4** | Salida Digital | Selección Datos / Comando (D/C) |
| **CS** | **GPIO 10** | Salida Digital | Chip Select (CS) |
| **BLK** | **GPIO 1** *(o 3.3V)* | Salida / PWM | Control de retroiluminación |

> **Nota sobre BLK:** Puedes conectarlo a `GPIO 1` para apagar/encender la pantalla por software, o directamente al pin `3.3V` de la placa para mantenerla siempre encendida.

---

## 🛠️ Requisitos de Software y Librerías

Para compilar este proyecto en **Arduino IDE** o mediante **arduino-cli**:

1. **Placa ESP32 en el Gestor de Placas:**
   - URL del gestor: `https://downloads.arduino.cc/packages/package_index.tar.bz2`
   - Paquete: `esp32` de Espressif Systems (v3.0.0 o superior).
   - Placa recomendada: `Nologo ESP32C3 Super Mini` o `ESP32C3 Dev Module`.
   - Opciones importantes:
     - **USB CDC On Boot:** `Enabled`
     - **Flash Frequency:** `80MHz`
     - **Flash Mode:** `QIO`

2. **Librerías requeridas:**
   - `Adafruit GFX Library` (v1.12.0+)
   - `Adafruit ST7735 and ST7789 Library` (v1.11.0+)
   - `Adafruit BusIO`

Instalación rápida por consola con `arduino-cli`:
```bash
arduino-cli lib install "Adafruit GFX Library" "Adafruit ST7735 and ST7789 Library"
```

---

## 🚀 Compilación y Carga

### Desde la consola (arduino-cli):
```bash
# Compilar
arduino-cli compile --fqbn esp32:esp32:nologo_esp32c3_super_mini ESP32C3_ST7789_Display.ino

# Subir al microcontrolador (reemplaza COM10 por tu puerto)
arduino-cli upload -p COM10 --fqbn esp32:esp32:nologo_esp32c3_super_mini ESP32C3_ST7789_Display.ino
```

### Desde Arduino IDE:
1. Abre el archivo `ESP32C3_ST7789_Display.ino`.
2. Selecciona la placa **Nologo ESP32C3 Super Mini** (o **ESP32C3 Dev Module**).
3. Selecciona el puerto COM correspondiente a tu dispositivo.
4. Presiona **Subir** (Upload).
5. Abre el **Monitor Serie** configurado a **115200 baudios**.

---

## 📄 Licencia
Este proyecto es de código abierto bajo la licencia MIT.
