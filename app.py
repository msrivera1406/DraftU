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

# def detect_specific_conflict(data):
#     subjects = data 
    
#     def schedules_overlap(sched1, sched2):
#         for s1 in sched1:
#             for s2 in sched2:
#                 if s1['day'] == s2['day']: 
#                     start1 = int(s1['start'].replace(':', ''))
#                     end1 = int(s1['end'].replace(':', ''))
#                     start2 = int(s2['start'].replace(':', ''))
#                     end2 = int(s2['end'].replace(':', ''))
                    
#                     if max(start1, start2) < min(end1, end2):
#                         return True
#         return False

#     for i in range(len(subjects)):
#         for j in range(i + 1, len(subjects)):
#             sub_a = subjects[i]
#             sub_b = subjects[j]
            
          
#             conflict_exists = True 
            
#             for group_a in sub_a['groups']:
#                 for group_b in sub_b['groups']:
#                     if not schedules_overlap(group_a['schedules'], group_b['schedules']):
#                         conflict_exists = False
#                         break 
#                 if not conflict_exists:
#                     break
        
#             if conflict_exists:
#                 return f"Incompatible: '{sub_a['name']}' y '{sub_b['name']}' se empalman en todas sus opciones."

#     return "Conflicto complejo (más de 2 materias involucradas o falta de cupos)."

def time_to_minutes(time_str):
    if not time_str: return 0
    try:
        hours, minutes = map(int, time_str.split(':'))
        return hours * 60 + minutes
    except: 
        return 0

@app.route('/simulation')
def simulation():
    return render_template('simulation.html')

@app.route('/results')
def results():
    return render_template('results.html')

@app.route('/builder')
def builder():
    return render_template('builder.html')

@app.route('/generate-schedule', methods=['POST'])
def generate_schedule():

    frontend_data = request.json 
    data=request.json
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
    try:
        cmd = RUN_COMMAND + ["--headless", OUTPUT_FILE, INPUT_FILE]
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