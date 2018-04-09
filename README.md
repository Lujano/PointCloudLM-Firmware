# PointCloud-Firmware

  En el siguiente repositorio, se encuentran los distintos códigos y proyectos de CodeWarrior
empleados durante la elaboración del Firmware utilizado para programar el microcontrolador
MC9S08QE128, el cual se encarga del control en la elaboración de una nube de puntos

## Prerrequisitos

 1. Tener instalado el IDE CodeWarrior para editar, compilar y hacer "debug" del firmware empleado
 en la programación del microcontrolador MC9S08QE128
 
 2. Poseer una placa DEMOQE128 Rev. C con el microcontrolador MC9S08QE128 instalado

![alt text](https://github.com/Lujano/PointCloud-Firmware/blob/master/ReadmeDEMOQE128.jpg)
 
## Instalación de CodeWarrior

 * Para la instalación del software CodeWarrior dirigirse al siguiente [enlace](http://wikitronica.labc.usb.ve/index.php/Codewarrior_10.3).<br>
 donde se encontrará una guía detallada.
 
 * El instalador del Codewarrior se puede descargar a través del siguiente [enlace](http://community.nxp.com/docs/DOC-1090).<br>
 
 ## Ejecutando el proyecto en CodeWarrior
 
 1. Descargar los archivos contenidos en la carpeta Integracion_Fase1
 
 2. Crear un proyecto en Codewarrior ("Bareboard Project") cuyo nombre sea exactamente "Integracion_Fase1" y verificar las siguientes
 configuraciones:
 
 ![alt text](https://github.com/Lujano/PointCloud-Firmware/blob/master/ReadmePaso1.jpg)
 
 ![alt text](https://github.com/Lujano/PointCloud-Firmware/blob/master/ReadmePaso2.jpg)
 
 ![alt text](https://github.com/Lujano/PointCloud-Firmware/blob/master/ReadmePaso3.jpg)
 
 ![alt text](https://github.com/Lujano/PointCloud-Firmware/blob/master/ReadmePaso4.jpg)
 
 ![alt text](https://github.com/Lujano/PointCloud-Firmware/blob/master/ReadmePaso5.jpg)
 
 3. Ir a la carpeta contenedora del proyecto y eliminar los siguientes archivos:
 
 ![alt text](https://github.com/Lujano/PointCloud-Firmware/blob/master/ReadmePaso6.jpg)
 
 4. Copiar los archivos descargados de Integracion_Fase1 y pegar dentro de la carpeta del proyecto.
 
 5. Realizar un Clean del proyecto
 
 El proyecto debe estar listo para su uso.
 
 ## Usos del proyecto
 
  Entre los usos que puede tener esta sección del proyecto "Generación de nube de puntos haciendo uso de fusión sensorial" se pueden destacar los siguientes:
  
  * Manejo y control de una base Pan-Tilt construida a partir de dos (2) Servos Motores
  
  * Control, Adquisición y manejo de Datos de proximidad, obtenidos a través de un sensor ultrasonido modelo HCSR05

  * Control, Adquisición y manejo de Datos de proximidad, obtenidos a través de un sensor infrarrojo modelo SHARP GP2Y0A21
  
  * Envío de datos vía puerto serial, esto a través de un protocolo de comunicación que transmite 5 canales analógico-digital

[Home](https://github.com/Lujano/PointCloudLM/wiki)
