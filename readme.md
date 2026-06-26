# 💿 ESP32-C3 NFC Jukebox: Reproductor Musical Interactivo

¡Bienvenido al proyecto **ESP32-C3 NFC Jukebox**! Este es un proyecto de hardware abierto que transforma un microcontrolador compacto en un reproductor de música interactivo por presencia. 

El concepto está inspirado en los tocadiscos y reproductores de CD tradicionales: al colocar un "disco" (una tarjeta o llavero NFC) sobre la superficie lectora, la música asociada comienza a sonar de inmediato. Al retirar el disco, la música se detiene.

---

## 👶 Ideal como Juguete Infantil Educativo
Este proyecto fue diseñado con la mente puesta en los más pequeños de la casa. Al no requerir pantallas, menús complejos ni botones pequeños, **un niño pequeño puede usarlo de forma 100% autónoma**. 

* **Interacción causa-efecto:** Aprender que colocar su "personaje" o "disco" favorito activa una canción o un audiocuento estimula su motricidad y comprensión.
* **Sin pantallas:** Una alternativa perfecta para entretener y educar sin sobreestimulación visual.
* **Seguro y robusto:** Al gestionar el software mediante un filtro de tolerancia (histéresis), el sistema ignora los temblores o pequeños movimientos del niño al colocar el disco, evitando que la música se corte o se reinicie constantemente.

---

## 🚀 Características Técnicas
* **Reproducción por presencia:** Control en tiempo real del estado del lector NFC.
* **Filtro antirrebote por software:** Algoritmo optimizado para evitar microcortes por vibraciones o falsos negativos de la antena.
* **Gestión eficiente de pines:** Configuración de pines totalmente remapeada para evitar los pines de *strapping* críticos del ESP32-C3 (GPIO 2, 8 y 9), garantizando un arranque del chip limpio y seguro.
* **Modo Simulación:** El código incluye un sistema de depuración que permite probar y escanear tarjetas NFC a través del Monitor Serie, incluso si el módulo de audio o la tarjeta MicroSD no están presentes.

---

## 🔌 Configuración de Pines (Seguros)

Para evitar conflictos en el encendido del ESP32-C3 Super Mini, se han modificado los buses por defecto utilizando la matriz de GPIO del chip:

| Componente | Pin Componente | Pin ESP32-C3 Super Mini | Tipo de Bus |
| :--- | :--- | :--- | :--- |
| **PN532 (NFC)** | SDA | **GPIO 5** | I2C Remapeado |
| **PN532 (NFC)** | SCL | **GPIO 6** | I2C Remapeado |
| **DFPlayer Mini**| RX (Recibe) | **GPIO 20** (TX del ESP) | UART Hardware |
| **DFPlayer Mini**| TX (Envía) | **GPIO 21** (RX del ESP) | UART Hardware |

> ⚠️ *Nota:* Se recomienda alimentar el DFPlayer Mini a 5V y colocar una resistencia de 1 kΩ en serie en la línea TX del ESP32 (GPIO 20) hacia el RX del DFPlayer para eliminar ruidos parásitos en el altavoz.

---

## 📦 El Contenedor: Creando la "Magia" del Proyecto
Para que este proyecto pase de ser un montón de cables a un juguete real e intuitivo, **es fundamental diseñar o encontrar una buena caja**. 

### Ideas de personalización:
1. **Caja de Madera o Cartón Decorada:** Una pequeña caja de té o una caja de zapatos decorada puede ocultar la electrónica por completo. El lector NFC puede leer a través de materiales no metálicos de hasta 2-3 cm de grosor.
2. **Impresión 3D:** Diseñar una carcasa con forma de tocadiscos retro o de radio antigua donde haya una zona plana delimitada (el "plato") para posar los discos.
3. **Los "Discos" NFC:** Puedes pegar las etiquetas NFC detrás de tarjetas de cartón con dibujos de las canciones, dentro de figuras de plástico (estilo figuras interactivas) o en discos de madera pintados.

---

## 🛠️ Cómo Utilizar este Repositorio

1. Instala las librerías `Adafruit PN532` y `DFRobotDFPlayerMini` en Arduino IDE.
2. **Preparar la MicroSD:** Formatea una tarjeta MicroSD en **FAT32**. Crea una carpeta en la raíz llamada `mp3` (en minúsculas) y guarda tus archivos nombrados exactamente como: `0001.mp3`, `0002.mp3`, `0003.mp3`, etc.
3. **Escanear Tarjetas:** Carga el código de la carpeta `/src` en tu ESP32-C3 usando Arduino IDE. Abre el Monitor Serie a **115200 bps** y pasa tus tarjetas para copiar sus códigos de identificación (UID).
4. **Asignar Canciones:** Reemplaza los UIDs de ejemplo en los condicionales `if` del código por tus códigos reales y vuelve a subir el programa.