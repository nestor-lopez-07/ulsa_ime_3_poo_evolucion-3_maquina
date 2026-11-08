# Práctica Unidad IV: La evolución de una máquina

Programación Orientada a Objetos · Ingeniería Mecatrónica · Tercer semestre


## Fase 1. Entender el problema

**1.1 El problema con mis propias palabras**

La planta necesita un programa para controlar las máquinas que trabajan en su línea de producción, Cada máquina debe registrar cuántas piezas procesa, cuánto tiempo trabaja y cuánto tarda en completar un ciclo. El programa debe permitir ejecutar turnos y generar reportes para conocer el rendimiento de cada máquina evitando errores usando restricciones y reglas.

**1.2 Reglas que siempre deben cumplirse en una máquina** 

1. El nombre de la máquina no debe estar vacío.
2. La cantidad de piezas procesadas nunca debe ser negativa.
3. El tiempo trabajado nunca debe ser negativo.
4. El tiempo de cada ciclo de procesamiento debe ser mayor que cero.



**1.3 ¿Quién más usa la clase `Maquina`? ¿Qué podría salir mal si otro equipo crea una máquina nueva sin conocer tus reglas?**

Otros equipos también van a utilizar la clase Maquina para crear sus propias máquinas. Si no conocen las reglas, podrían programar máquinas con datos incorrectos, como tiempos negativos o nombres vacíos. Esto podría causar errores en los resultados y en el funcionamiento del programa.

## Fase 2. Diseñar la solución

**2.1 Diagrama inicial (Etapa 0)**

![Diagrama inicial](diseno_inicial.png)

**2.2 Diagrama final (Etapa 9)**

![Diagrama final](diseno_solucion.png)

**2.3 ¿Qué cambió entre los dos diagramas?**

| Elemento | Etapa 0 | Etapa 9 | Etapa que lo cambió |
| --- | --- | --- | --- |
| Visibilidad de los atributos de `Maquina` | _____ | _____ | _____ |
| `procesar()` | _____ | _____ | _____ |
| `tiempoCicloSeg()` | _____ | _____ | _____ |
| Destructor de `Maquina` | _____ | _____ | _____ |
| ¿Se pueden crear objetos `Maquina`? | _____ | _____ | _____ |
| Relación `ControlDeLinea` - `Maquina` | _____ | _____ | _____ |
| Relación `Torno` - `Motor` | _____ | _____ | _____ |
| Relación `BandaTransportadora` - `Motor` | _____ | _____ | _____ |

## Fase 3. Implementar

**3.0 Etapa 0. Una máquina sin reglas**

a) ¿Qué imprime `ejecutarTurno()` para el torno y la fresadora? ¿Por qué no imprime "torneando" ni "fresando"?

Porque la función recibe una referencia de tipo Maquina y utiliza el método de la clase base, ya que todavía no usamos virtual.

b) ¿Por qué el torno acumula 65 s y no 135 s?

Porque primero procesa directamente y suma 45 segundos. Después, en ejecutarTurno(), realiza dos ciclos genéricos de 10 segundos cada uno. Por eso suma 65 segundos.

**3.1 Etapa 1. Invariantes y constructores explicit**

a) ¿Qué regla de la sección 1.2 rompió `torno.piezasProcesadas = -5;`?

Rompió la regla de que las piezas procesadas no pueden ser negativas.

b) ¿De dónde salió la máquina "Fantasma"? ¿Qué hizo el compilador sin que se lo pidieras?

El compilador convirtió automáticamente el texto "Fantasma" en un objeto Maquina, porque el constructor todavía no tenía explicit.

c) ¿Qué tuviste que cambiar en `Torno` y `Fresadora` y por qué?

Tuve que cambiar la forma de acceder a los atributos de Maquina. Reemplacé el acceso directo a nombre por getNombre(), y utilicé registrarPieza() y agregarTiempo() para modificar las piezas y el tiempo. Esto fue necesario porque los atributos ahora son privados y solo se pueden modificar mediante los métodos protegidos de la clase base.

**3.2 Etapa 2. Virtual y override**

a) La fresadora acumula 180 s, pero reporta 360 pzas/h. ¿Por qué un cálculo usó 60 s y el otro 10 s?

El const cuenta como parte de la firma del método. tiempoCicloSeg() de la fresadora no sobrescribía al de la base era un método nuevo. procesar() usó el de la fresadora 60 s, pero capacidadPorHora() es const y solo podía ver el de la base 10 s. Por eso 3600 / 10 = 360.

b) ¿Tu compilador mostró alguna advertencia antes de usar `override`? ¿Qué mensaje dio después de agregar `override`?

No me salio ninguna advertencia. En cambio, al poner override el compilador sí marcó error: marked 'override', but does not override. Corregí la firma a int tiempoCicloSeg() const override; y ya dio 60 pzas/h.]

**3.3 Etapa 3. La línea de producción**

a) ¿Por qué la línea procesó todo como "generico" si `procesar()` ya es virtual?

hace slicing con un std::vector<Maquina> guarda copias de tipo Maquina al agregar un torno solo se copia la parte de Maquina y se pierde lo demás.

b) ¿Por qué el torno y la fresadora originales quedaron con 0 piezas?

Porque la línea trabajaba con copias, y los originales nunca se tocaron.

c) La relación entre `ControlDeLinea` y `Maquina`, ¿es composición o agregación? ¿Por qué?

Es agregación. La línea solo conoce a las máquinas, pero no las crea ni las destruye. Si la línea desaparece, las máquinas siguen existiendo.

**3.4 Etapa 4. Destructores**

| Momento | Mensajes de destrucción que aparecieron |
| --- | --- |
| `delete temporal;` sin destructor virtual | solo se observa Destruyendo Maquina al ejecutar delete temporal |
| `delete temporal;` con destructor virtual | e ejecutan Destruyendo Torno, Destruyendo Motor y Destruyendo Maquina |
| Al terminar `main` (torno y fresadora) | ejecutan los destructores automáticamente al salir de main. Primero se destruye Fresadora y después Torno |

a) ¿Qué advertencia mostró el compilador? ¿Por qué decimos que ya te estaba avisando del problema?

Mostró una advertencia porque estaba intentando destruir un objeto Torno mediante un puntero Maquina, pero el destructor de Maquina no era virtual.

b) ¿Por qué el torno de `main` sí se destruyó completo aunque el destructor no fuera virtual? ¿A qué máquina corresponde el primer "Destruyendo Maquina" después de "Fin de main"?

Porque el torno local se destruyó usando su tipo real, mientras que con delete temporal se utilizó un puntero de tipo Maquina. Al no tener un destructor virtual, no se llamó correctamente a los destructores de Torno y Motor.

**3.5 Etapa 5. La clase abstracta**

a) Copia los mensajes de error y explica con tus palabras qué dice cada uno.

cannot declare variable 'pulidora' to be of abstract type 'Pulidora': no puedo crear una pulidora porque le falta implementar procesar(), que en la base es virtual puro.
cannot declare variable 'generica' to be of abstract type 'Maquina': Maquina ya es abstracta, entonces no se pueden crear objetos de ella.

b) ¿Por qué es mejor este error de compilación que el comportamiento "generico" de antes?

Antes el error pasaba sin avisar y terminaba en un reporte incorrecto. Ahora el compilador lo detecta antes de ejecutar y dice qué falta.

**3.6 Etapa 6. El contrato que el compilador no ve**

a) ¿Qué partes del contrato violó `InspeccionVisual`?

no registró la pieza ni el tiempo, y devolvió un tiempo de ciclo de 0, que no es mayor que cero. Por eso salió con 0 piezas y capacidad inf.

b) ¿Por qué el compilador no lo detectó?

Porque el compilador solo revisa que las firmas estén bien (nombres, parámetros, tipos), pero no revisa lo que el método hace realmente.

c) ¿Qué pasaría si el próximo equipo tampoco lee el comentario del contrato?

Pasaría lo mismo. Un comentario solo sirve si el otro equipo lo lee y lo respeta. Eso lo resuelve la Etapa 7.

**3.7 Etapa 7. La base hace cumplir el contrato**

a) ¿Cómo puede una clase derivada sobrescribir un método que es privado en la base?

Porque private controla quién puede llamar al método, mientras que virtual permite que las clases derivadas lo sobrescriban.

b) ¿Qué ocurre ahora con una máquina cuyo tiempo de ciclo es 0? Compáralo con la Etapa 6.

Ahora el programa detecta el error y muestra un mensaje indicando qué máquina tiene un tiempo inválido. Antes se podía obtener una capacidad infinita.

c) ¿Por qué la línea ignoró el "atajo" del torno, pero la llamada directa no? ¿Cómo corregiste el diseño?

Porque procesar() ya no es virtual. La línea utiliza el método de Maquina, que contiene las reglas del ciclo. La llamada directa utiliza el método propio de Torno. Para corregirlo, se elimina el atajo y se conserva el método de la clase base.

**3.8 Etapa 8. Herencia mal usada**

a) Da al menos dos razones por las que `BandaTransportadora : public Motor` es un error de diseño.

Porque una banda transportadora no es un motor, es una máquina que utiliza un motor. 
Al heredar de Motor permite que otras partes del programa cambien directamente su velocidad y no puede agregarse correctamente a la línea de producción como una Maquina.

b) La relación entre `BandaTransportadora` y `Motor`, ¿es composición o agregación? ¿Por qué?

Es composición porque la banda tiene su propio motor como parte de ella. La banda controla cuándo y cómo utiliza ese motor.

**3.9 Etapa 9. La librería y la prueba final**

a) Después de agregar la `Cortadora`, ¿qué archivos mostró `git status`? ¿Cuántos archivos de la librería tuviste que modificar?

[Inserta aquí tu respuesta]

b) ¿Qué pasó al escribir `-lplanta` antes de `src/main.cpp`? ¿Por qué?

[Inserta aquí tu respuesta]

**3.10 Bitácora de dudas**

| # | Etapa | Duda | Cómo la resolví | Fuente |
| --- | --- | --- | --- | --- |
| 1 | _____ | _____ | _____ | _____ |
| 2 | _____ | _____ | _____ | _____ |
| 3 | _____ | _____ | _____ | _____ |

## Fase 4. Probar y mejorar

**4.1 Tabla de pruebas**

| # | Caso | Resultado esperado | Resultado obtenido | ¿Pasa? |
| --- | --- | --- | --- | --- |
| 1 | Máquina con nombre vacío | Excepción con mensaje claro | _____ | _____ |
| 2 | Reporte de una línea vacía | "La linea no tiene maquinas" | _____ | _____ |
| 3 | `agregar(nullptr)` | Devuelve `false` | _____ | _____ |
| 4 | Reporte antes de ejecutar el turno | 0 piezas, 0 s y capacidad calculada | _____ | _____ |
| 5 | Turno de 0 ciclos | Nada cambia | _____ | _____ |
| 6 | Torno con 3 ciclos | 3 piezas, 135 s, 80 pzas/h | _____ | _____ |
| 7 | Máquina con `tiempoCicloSeg()` igual a 0 | Excepción con mensaje claro | _____ | _____ |
| 8 | Máquina con `tiempoCicloSeg()` negativo | Excepción con mensaje claro | _____ | _____ |
| 9 | La misma máquina agregada dos veces | _____ | _____ | _____ |

**4.2 Bitácora de mejoras**

| # | Falla o mejora detectada | Qué cambié | Por qué |
| --- | --- | --- | --- |
| 1 | _____ | _____ | _____ |
| 2 | _____ | _____ | _____ |

Retos opcionales que intenté: _____

## Fase 5. Publicar en GitHub

**5.1 Enlace a mi fork**

[Inserta aquí el enlace a tu fork]

## Cierre y reflexión

**6.1 Errores que ya no pueden ocurrir**

| Error | Etapa en que apareció | Restricción que lo impide ahora | ¿Quién lo detecta: el compilador o el programa? |
| --- | --- | --- | --- |
| Contadores con valores inválidos | _____ | _____ | _____ |
| Máquina "fantasma" creada a escondidas | _____ | _____ | _____ |
| Método que no sobrescribe por un `const` | _____ | _____ | _____ |
| Máquinas copiadas y "rebanadas" | _____ | _____ | _____ |
| Destrucción incompleta | _____ | _____ | _____ |
| Máquina sin `procesar()` propio | _____ | _____ | _____ |
| Tiempo de ciclo inválido | _____ | _____ | _____ |
| Derivada que no registra sus piezas | _____ | _____ | _____ |

**6.2 ¿Cuál de los errores fue el más peligroso: el de compilación, la advertencia o el que compilaba sin problemas? ¿Por qué?**

[Inserta aquí tu respuesta]

**6.3 ¿Qué aprendiste en esta práctica?**

[Inserta aquí tu respuesta]

**6.4 ¿Qué cambiarías de tu proceso la próxima vez?**

[Inserta aquí tu respuesta]
