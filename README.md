# CARM
Minería de Reglas de Asociación para Clasificación

La **Minería de Reglas de Asociación** (ARM) es un área informática basada en la búsqueda de co-ocurrencias entre los ítems presentes en las transacciones de un dataset. Teóricamente podríamos incluirla entre las técnicas de *Estadística Descriptiva*, sin embargo se usa en el ámbito de la *Estadística Inferencial*. 

Su origen es puramente informático. Permite resolver el problema de extracción de itemsets frecuentes (FIM) en tiempos y con consumos de recursos razonables. Este proyecto está enfocado a

El **Aprendizaje Automático** es un área informática que persigue entrenar a las máquinas para que hagan tareas para las que están mucho mejor preparadas que nosotros. El Problema de Clasificación se resuelve con técnicas que aprenden leyendo grandes colecciones de datos y usan ese aprendizaje en la estimación...

...

Esta implementación se basa en Apriori [...]. Tiene en cuenta las características de los Datasets de Clasificación Estándar (SCD), su estructura y el objetivo del Problema de Clasificación. Los cambios introducidos al algoritmo original van desde la definición de soporte mínimo hasta la estructura usada para la ejecución del algoritmo -Trie (prefix tree)- pasando por las reglas de generación de candidatos y de poda.

1. El soporte mínimo en Apriori es un único valor, expresado generalmente como porcentaje. Un soporte mínimo del 5%, p.ej., provoca que no se estudien los ítems que aparecen en menos del 5% de las transacciones en estudio, recogidas en el dataset D. El primer problema que provoca este umbral en el Problema de Clasificación es que si una clase no aparece al menos en el 5% de las transacciones de D no será tenida en cuenta por el algoritmo. No podremos obtener ningún resultado que dé alguna pista sobre esa clase "infrecuente" o "rara".
- Para resolverlo usaremos múltiples soportes mínimos. Cada clase tendrá el suyo propio, así nos aseguramos de tener información válida para todas las clases en estudio. Esto supone una carga de datos en memoria RAM, pero tratándose de clases infrecuentes serán pocos los datos que tendremos que incorporar.
2. La experiencia nos ha enseñado que hay SCD con atributos de valor único. En ARM se convierte en un valor que aparece en todas las transacciones en estudio, por lo que aparecerá como el más frecuente de todos los ítems del dataset y, por tanto, el más importante desde el punto de vista de su representatividad. Este ítem aparecerá en todas las relaciones buscadas a través del Trie L, consumiendo gran cantidad de RAM. En el Problema de Clasificación este ítem no aporta nada absolutamente, no podemos deducir nada de un individuo que toma ese valor en ese atributo porque eso es "lo único que nos dicen los datos, que ese atributo toma siempre ese valor". En la fase de entrenamiento del Problema de Clasificación, si usamos ARM, es una información que no aporta nada nuevo y ocupa demasiada memoria RAM, facilitando la aparición del *dilema del ítem raro*.
- Para resolverlo eliminamos el atributo del SCD, informando al investigador que lo hemos eliminado.
