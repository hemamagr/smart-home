import firebase_admin
from firebase_admin import credentials, firestore
import serial
import serial.tools.list_ports
import json
import time

# ==========================================
# 1. FIREBASE INIT
# ==========================================
try:
    cred = credentials.Certificate(r"C:\Users\PC1\Documents\Arduino\smart_home\Raspberry\cle_firebase.json")
    firebase_admin.initialize_app(cred)
    db = firestore.client()

    print(" Connexion Firebase RÉUSSIE")

    USER_ID = "test_user_L3"

except Exception as e:
    print(f" Erreur Firebase : {e}")
    exit()

# ==========================================
# 2. QUOTA HISTORIQUE
# ==========================================
INTERVALLE_HISTORIQUE = 900  # 900s pour enregistrer chaque 15min , ne pas saturer 
derniere_archive = 0

# ==========================================
# 3. ARDUINO DETECTION
# ==========================================
def trouver_port_arduino():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        if any(x in p.description for x in ["Arduino", "USB", "CH340", "CP210"]):
            return p.device
    return None

ser = None
port = trouver_port_arduino()

if port:
    try:
        ser = serial.Serial(port, 9600, timeout=1)
        time.sleep(2)
        print(f" Arduino détecté sur {port}")
    except:
        print(" Impossible d'ouvrir le port série")
else:
    print(" MODE SIMULATION")

# ==========================================
# 4. LOOP PRINCIPAL
# ==========================================
print("\n Passerelle en service")

while True:
    try:
        data_brute = None

        # Lecture Arduino
        if ser and ser.in_waiting > 0:
            ligne = ser.readline().decode('utf-8').strip()
            if ligne.startswith("{"):
                try:
                    data_brute = json.loads(ligne)
                except:
                    pass

        # Simulation
        elif not ser:
            simu = input("\nJSON: ")
            try:
                data_brute = json.loads(simu)
            except:
                print("JSON invalide")

        if data_brute:

            temps_actuel = time.time()
            ok_pour_archive = (temps_actuel - derniere_archive) >= INTERVALLE_HISTORIQUE

            # IMPORTANT : lecture de la pièce UNE SEULE FOIS
            piece = data_brute.get("piece", "salon")

            for cle, valeur in data_brute.items():

                if cle in ["piece", "timestamp"]:
                    continue

                sensor_id = f"{cle}_{piece}"

                sensor_ref = db.collection("users")\
                    .document(USER_ID)\
                    .collection("sensors")\
                    .document(sensor_id)

                # =========================
                # 1. TEMPS REEL
                # =========================
                sensor_ref.set({
                    "valeur": valeur,
                    "nom": cle,
                    "piece": piece,
                    "updatedAt": firestore.SERVER_TIMESTAMP
                }, merge=True)

                # =========================
                # 2. HISTORIQUE
                # =========================
                if ok_pour_archive:
                    sensor_ref.collection("history").add({
                        "valeur": valeur,
                        "timestamp": firestore.SERVER_TIMESTAMP
                    })

                # =========================
                # 3. ALERTES
                # =========================
                if cle == "temperature" and valeur > 30:

                    db.collection("users")\
                        .document(USER_ID)\
                        .collection("alerts")\
                        .add({
                            "sensorId": sensor_id,
                            "message": f"Temp élevée {valeur}°C dans {piece}",
                            "level": "critical",
                            "read": False,
                            "createdAt": firestore.SERVER_TIMESTAMP
                        })

                    print(f" ALERTE {piece}: {valeur}°C")

            if ok_pour_archive:
                derniere_archive = temps_actuel
                print(" Historique sauvegardé")
            else:
                print(f" Update temps réel ({piece})")

    except KeyboardInterrupt:
        print("\n Arrêt")
        break

    except Exception as e:
        print(f" Erreur: {e}")
        time.sleep(2)