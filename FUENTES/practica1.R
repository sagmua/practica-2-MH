#LIBRERIAS UTILIZADAS:
library("foreign") #para leer archivos .arff
library(foreach) #iterar
library(iterators)



#leemos los datos deseados:
datos_sonar = read.arff("../BIN/sonar.arff")
datos_spam = read.arff("../BIN/spambase-460.arff")
datos_cancer = read.arff("../BIN/wdbc.arff")

#una vez leidos los datos, creamos una funcion para normalizar dichos datos:
normalizarDatos = function(datos, nombre_clase){
  
  normalise <- function(x) {
    ranx <- range(x)
    (x - ranx[1]) / diff(ranx)
  }
  sin_etiquetas = -which(names(datos) %in% nombre_clase)
  datos[sin_etiquetas] = apply(datos[sin_etiquetas], 2, normalise)
  
  
  datos
}

#Ahora normalizamos los tres conjuntos de datos, indicandole cual es la columna
# de las etiquetas para que no la normalice:
datos_cancer = normalizarDatos(datos_cancer, "class")
datos_spam = normalizarDatos(datos_spam, "is_spam")
datos_sonar = normalizarDatos(datos_sonar, "Class")


write.csv(datos_cancer, "../BIN/cancer_normalizado.csv")
write.csv(datos_spam, "../BIN/spam_normalizado.csv")
write.csv(datos_sonar, "../BIN/sonar_normalizado.csv")