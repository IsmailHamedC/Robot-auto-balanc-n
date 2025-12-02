# Proyecto robot auto balancín mecatrónica 
ME4250-2 Mecatrónica 2025-2

Integrantes: Bruno Muñoz, Esteban Zamora, Ismail Hamed, Joaquín Roco, Tomás Agurto

Dentro de este repositorio se pueden encontrar distintas carpetas para entender la fabricación, ensamblaje y funcionamiento de nuestro robot balancín

# Indice
1.- CAD y Componentes

2.- Ensamble

3.-Diagrama electrónico robot balanín

4.-Código Arduino

# 1 CAD y Componentes
En la carpeta [CAD](https://github.com/IsmailHamedC/Robot-auto-balanc-n/tree/main/CAD) se encuentran los archivos .f3d para la fabricación de elementos que aportan a la estructura del robot, mientras que en la carpeta [Componentes](https://github.com/IsmailHamedC/Robot-auto-balanc-n/tree/main/Componentes) se puede ver el listado de elementos electrónicos que permiten el control del robot.

![Vista ISO](https://github.com/IsmailHamedC/Robot-auto-balanc-n/blob/main/Imagenes%20CAD/ISOOFRENTE.png)
# 2 Ensamble

El archivo [Ensamble](https://github.com/IsmailHamedC/Robot-auto-balanc-n/blob/main/Ensamble.f3z) muestra en Fusion el ensamble de todos los componenentes electrónicos y estructurales que le dan la forma al robot balancín

# 3 Diagrama electrónico

![Diagrama electrónico](https://github.com/IsmailHamedC/Robot-auto-balanc-n/blob/main/Componentes/Diagrama%20electronico%20robot%20balancin.png)

# 4 Código Arduino

En [Código robot balancín](https://github.com/IsmailHamedC/Robot-auto-balanc-n/blob/main/C%C3%B3digo%20robot%20balanc%C3%ADn.ino) se puede encontrar el código que incluye las funciones para el movimiento de motores, identificación del sensor y controlador PID cuyas constantes fueron calibradas a Kp = 25; Ki = 2; Kd = 6
