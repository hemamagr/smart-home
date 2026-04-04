import firebase_admin
from firebase_admin import credentials, firestore
import serial
import serial.tools.list_ports
import json
import time
import datetime

# ==========================================
# 1. CONFIGURATION FIREBASE
# ==========================================
try:
    cred = credentials.Certificate(r"C:\Users\PC1\Documents\Arduino\smart_home\Raspberry\cle_firebase.json")
    firebase_admin.initialize_app(cred)
    db = firestore.client()
    print(" Connexion Firebase : RÉUSSIE")
except Exception as e:
    print(f" Erreur de connexion Firebase : {e}")
    exit()

# ==========================================
# 2. DÉTECTION AUTOMATIQUE DE L'ARDUINO
# ==========================================
def trouver_port_arduino():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        # On cherche des mots-clés typiques des drivers Arduino/USB
        if any(keyword in p.description for keyword in ["Arduino", "USB", "CH340", "CP210"]):
            return p.device
    return None

def connecter_serie():
    port = trouver_port_arduino()
    if port:
        try:
            ser = serial.Serial(port, 9600, timeout=1)
            time.sleep(2) # Pause pour laisser l'Arduino rebooter
            print(f" Arduino détecté sur {port}")
            return ser
        except:
            return None
    return None

# Initialisation de la connexion série
ser = connecter_serie()

if not ser:
    print(" Aucun Arduino trouvé. MODE SIMULATION activé.")

# ==========================================
# 3. BOUCLE PRINCIPALE (LE PONT)
# ==========================================
print("\n--- Passerelle en service (Ctrl+C pour quitter) ---")

while True:
    try:
        data_a_envoyer = None

        # CAS A : Lecture du vrai Arduino
        if ser and ser.in_waiting > 0:
            ligne = ser.readline().decode('utf-8').strip()
            
            # On vérifie si c'est du JSON (commence par { et finit par })
            if ligne.startswith("{") and ligne.endswith("}"):
                try:
                    data_a_envoyer = json.loads(ligne)
                    print(f" Reçu de l'Arduino : {data_a_envoyer}")
                except json.JSONDecodeError:
                    print(f"Erreur de formatage JSON : {ligne}")

        # CAS B : Simulation manuelle (si pas d'Arduino ou pas de message)
        elif not ser:
            simu = input("\nSimulation (Tape le JSON ou 'exit') : ")
            if simu.lower() == 'exit': break
            try:
                data_a_envoyer = json.loads(simu)
            except:
                print(" Format invalide. Exemple : {\"temp\": 25, \"hum\": 60}")

        # ENVOI VERS FIREBASE
        if data_a_envoyer:
            maintenant = datetime.datetime.now()
            data_a_envoyer['timestamp'] = maintenant
            
            # 2. MISE À JOUR "TEMPS RÉEL" (On écrase pour l'affichage direct sur l'app)
            db.collection('maison').document('salon').set(data_a_envoyer, merge=True)
            
            # 3. CRÉATION DE L'HISTORIQUE (On ajoute une nouvelle fiche à chaque fois)
            # .add() génère un identifiant unique automatiquement
            db.collection('maison').document('salon').collection('historique').add(data_a_envoyer)
            
            print(f" Synchronisation Cloud : Temps Réel OK + Historique archivé ({maintenant.strftime('%H:%M:%S')})")
    except KeyboardInterrupt:
        print("\nArrêt de la passerelle...")
        break
    except Exception as e:
        print(f" Erreur imprévue : {e}")
        time.sleep(2) # Pause pour éviter les boucles d'erreurs rapides