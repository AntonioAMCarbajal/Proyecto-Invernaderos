# Centro de Innovación e Integración de Tecnologías Avanzadas | Unidad Veracruz

### Proyecto de monitoreo de invernadero tecnificado
### Fecha: 17 de abril de 2023

### Elaborado por:
1. Jefe del Departamento de Electrónica: **Antonio del Angel Moreno Carbajal**
2. Residente del departamento: **Maria Isabel Cruz Solis**

## Introducción

### Identificación del problema 
Para obtener un buen cultivo de cualquier hortaliza es necesario que se encuentre dentro de las condiciones ambientales adecuadas como lo son principalmente: temperatura, humedad y radiación, esto por lo general no puede ser garantizado en ambientes no controlados como son los cultivos al aire libre. El desconocimiento de los parámetros anteriormente mencionados, afectan las camas de cultivo provocando una baja en la producción de la planta y la calidad del fruto, incluso no ser apto para consumo humano. 

### Objetivo general:
Diseñar un sistema de monitoreo en un invernadero dentro del Centro de Innovación e Integración de Tecnologías Avanzadas, Unidad Veracruz, que permita la visualización de las variables atmosféricas mediante gráficas. 

### Objetivos específicos: 
- Identificar los sensores a utilizar en el sistema de monitoreo del invernadero 
- Diseñar el código para la tarjeta de desarrollo que vincule los sensores y la comunicación inalámbrica. 
- Desarrollar las gráficas en plataformas libres, a partir de los datos recolectados por la tarjeta de desarrollo ESP32. 

### Equipo
1. Cautin
2. Mototool Dremel
3. Multimetro

### Material
1. Devkit ESP32 V1
2. Sensor de temperatura
3. Sensor de humedad
4. Sensor de radiación UV
5. Placa fenolica
6. Estaño
7. Pasta para soldadura
8. Cable
9. Bateria de 5 - 9 V
10. Cortador 1/32 y 1/16
11. Cloruro ferrico

### Instalación de programas
#### 1.- Instalación de Arduino
Arduino ofrece hardware y software, a través de tarjetas de desarrollo y entornos de desarrollo integrado, este ultimo se ha actualizado para permitir desarrollar código en placas de desarrollo diferentes, tal es el caso de las ESP32. La descarga del entorno de desarrollo Arduino IDE lo podrá encontrar en el siguiente enlace: https://www.arduino.cc/en/software

#### 2.- Instalación de Node-RED
Node-RED es una herramienta de programación gráfica para conectar dispositivos de hardware, API y servicios en línea. Los pasos para instalar esta herramienta los encontrará en el siguiente enlace: https://nodered.org/docs/getting-started/windows

Para instalar en otros Sistemas Operativos: https://nodered.org/docs/getting-started/local

#### 3.- Instalación de XAMPP
XAMPP es una distribución de Apache completamente gratuita y fácil de instalar que contiene MariaDB, PHP y Perl. El paquete de instalación de XAMPP ha sido diseñado para ser fácil de instalar y usar. Consulte el siguiente enlace: https://www.apachefriends.org/es/index.html


## Desarrollo tecnico del proyecto

Enlace para descargar la tarjeta ESP32: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json

### Diagrama de conexión
![Administrador de tarjetas en arduino](https://github.com/AntonioAMCarbajal/Proyecto-Invernaderos/blob/main/Administrador%20de%20tarjetas%20en%20arduino.PNG)
![Diagrama general del sistema](https://github.com/AntonioAMCarbajal/Proyecto-Invernaderos/blob/main/Diagrama%20general%20del%20sistema.PNG)
### Codigo general

### Dashboard en Node-RED

### Base de datos 

### Pruebas

### Resultados

## Conclusiones
