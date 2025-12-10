#!/usr/bin/env bash
# exit on error
set -o errexit

# 1. Instalar dependencias de Python
pip install -r requirements.txt

# 2. Compilar el C++ (Esto genera el ejecutable 'ProyectoPED' para Linux)
# Asegúrate de listar TODOS tus .cpp aquí
g++ -o ProyectoPED *.cpp -std=c++11

echo "Build finalizado con éxito."