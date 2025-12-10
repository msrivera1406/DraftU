from flask import Flask, render_template, request, jsonify
import os
import json
import subprocess
app=Flask(__name__)

EXECUTABLE_NAME = "ProyectoPED.exe"  # O el nombre de tu exe
OUTPUT_FILE = "output.json"
INPUT_FILE = "input.json"

if os.name == 'nt': # 'nt' significa Windows
    EXECUTABLE_NAME = "ProyectoPED.exe"
    RUN_COMMAND = [EXECUTABLE_NAME] # En Windows se llama directo
else: # Linux (Render)
    EXECUTABLE_NAME = "./ProyectoPED" # Sin extensión .exe
    RUN_COMMAND = [EXECUTABLE_NAME]

def time_to_minutes(time_str):
    if not time_str: return 0
    try:
        hours, minutes = map(int, time_str.split(':'))
        return hours * 60 + minutes
    except: 
        return 0

@app.route('/results')
def results():
    return render_template('results.html')

@app.route('/builder')
def builder():
    return render_template('builder.html')

@app.route('/generate-schedule', methods=['POST'])
def generate_schedule():
    """
    1. Recibe el JSON del Frontend.
    2. Lo transforma al formato que C++ necesita.
    3. Ejecuta C++.
    4. Devuelve el resultado.
    """
    
    # 1. RECIBIR DATOS DEL FRONTEND
    frontend_data = request.json 
    # 2. TRANSFORMAR PARA C++ (Adaptador)
    cpp_input = []
    
    for subject in frontend_data:
        # Convertimos cada materia
        cpp_subject = {
            "nombre": subject['name'],
            "opciones": [] # Aquí irán los grupos
        }
        
        for group in subject['groups']:
            # Convertimos cada grupo
            cpp_group = {
                "id_grupo": str(group['id']), # ID único
                "profesor": group['professor'],
                "bloques": []
            }
            
            for schedule in group['schedules']:
                # AQUÍ OCURRE LA MAGIA: Convertimos String -> Int
                cpp_group['bloques'].append({
                    "dia": schedule['day'], # 0=Lunes, 1=Martes... (Asegúrate que C++ use 0-6 o ajusta aquí)
                    "inicio": time_to_minutes(schedule['start']), # 420
                    "fin": time_to_minutes(schedule['end'])       # 540
                })
            
            cpp_subject['opciones'].append(cpp_group)
        
        cpp_input.append(cpp_subject)
    # 2. GUARDAR input.json
    try:
        with open(INPUT_FILE, 'w', encoding='utf-8') as f:
            json.dump(cpp_input, f, indent=4)
    except Exception as e:
        return jsonify({"status": "error", "message": f"Error escribiendo input: {str(e)}"}), 500
    
    # 3. EJECUTAR C++
    # Comando: ProyectoPED.exe --headless output.json input.json
    try:
        cmd = RUN_COMMAND + ["--headless", OUTPUT_FILE, INPUT_FILE]
        
        # shell=True solo es necesario en Windows
        subprocess.run(cmd, check=True, shell=(os.name=='nt'))
            
    except subprocess.CalledProcessError:
        return jsonify({"status": "error", "message": "El motor C++ falló al ejecutarse."}), 500
    except FileNotFoundError:
        return jsonify({"status": "error", "message": "No se encuentra el ejecutable .exe"}), 500

    # 4. LEER RESULTADOS
    try:
        with open(OUTPUT_FILE, 'r', encoding='utf-8') as f:
            results = json.load(f)
        return jsonify(results)
    except Exception as e:
        return jsonify({"status": "error", "message": f"No se pudo leer output.json: {str(e)}"}), 500


@app.route('/')
def index():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')