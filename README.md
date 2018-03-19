# PointCloud-Firmware

  En el siguiente repositorio, se encuentran los distintos códigos y proyectos de CodeWarrior
empleados durante la elaboración del Firmware utilizado para programar el microcontrolador
MC9S08QE128, el cual se encarga del control en la elaboración de una nube de puntos

## Prerrequisitos

 1. Tener instalado el IDE CodeWarrior para editar, compilar y hacer "debug" del firmware empleado
 en la programación del microcontrolador MC9S08QE128
 
 2. Poseer una placa DEMOQE128 Rev. C con el microcontrolador MC9S08QE128 instalado

(introduce imagen de DemoQE128)
 3. 
## Instalación de CodeWarrior

 * Para la instalación del software CodeWarrior dirigirse al siguiente [enlace](http://wikitronica.labc.usb.ve/index.php/Freescale_Codewarrior_6.3).<br>
 donde se encontrará una guía detallada.
 
 * El instalador del Codewarrior se puede descargar a través del siguiente [enlace](http://community.nxp.com/docs/DOC-1090).<br>
 
 ## Ejecutando el proyecto en CodeWarrior
 
 1. Descargar los archivos contenidos en la carpet
 
 a Integracion_Fase1
 
 2. Crear un proyecto en Codewarrior ("Bareboard Project") cuyo nombre sea exactamente "Integracion_Fase1" y seleccionar el
 microcontrolador a utilizar ("MC9S08QE128") además de incluir el Processor Expert.
 
 3. Ir a la carpeta contenedora del proyecto y eliminar los siguientes archivos:
 
 ![alt text](PointCloud-Firmware/AbrirProyectoCodeWarrior.jpg)
 
 4. Copiar los archivos descargados de Integracion_Fase1 y pegar dentro de la carpeta del proyecto.
 
 5. Realizar un Clean del proyecto
 
 El proyecto debe estar listo para su uso.
