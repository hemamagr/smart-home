import firebase_admin
from firebase_admin import credentials, firestore
import serial
import serial.tools.list_ports
import json
import time
import threading

# ==========================================
# 1. FIREBASE INIT
# ==========================================
try:
    cred = credentials.Certificate(
        r"clé_json"
    )
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
INTERVALLE_HISTORIQUE = 900
derniere_archive = 0


# ==========================================
# 3. ARDUINO / MODE SIMULATION
# ==========================================
ser = None
MODE_SIMULATION = False


def trouver_port_arduino():
    
    try:
        ports = list(serial.tools.list_ports.comports())

    except Exception as e:
        print("Erreur scan ports USB :", e)
        return None

    for p in ports:
        try:
            desc = p.description

            if any(x in desc for x in ["Arduino", "USB", "CH340", "CP210"]):
                return p.device

        except:
            continue

    return None


def connecter_arduino():
    global ser, MODE_SIMULATION

    try:
        port = trouver_port_arduino()

        if port:
            ser = serial.Serial(port, 9600, timeout=1)
            time.sleep(2)

            MODE_SIMULATION = False
            print(f" Arduino connecté sur {port}")

        else:
            MODE_SIMULATION = True
            print(" Aucun Arduino détecté")
            print(" MODE SIMULATION ACTIVÉ")

    except Exception as e:
        MODE_SIMULATION = True
        print(" Erreur Arduino:", e)
        print(" MODE SIMULATION ACTIVÉ")


connecter_arduino()


# ==========================================
# 4. FIRESTORE LISTENER (DEVICES → ARDUINO)
# ==========================================
def on_device_change(col_snapshot, changes, read_time):
    for change in changes:

        if change.type.name in ["ADDED", "MODIFIED"]:

            data = change.document.to_dict()
            device_id = change.document.id
            state = data.get("state", False)

            cmd = {
                "device": device_id,
                "state": state
            }

            if ser:
                try:
                    ser.write((json.dumps(cmd) + "\n").encode())
                    print(" Commande envoyée Arduino:", cmd)
                except:
                    print(" Erreur envoi Arduino")


devices_ref = db.collection("users") \
    .document(USER_ID) \
    .collection("devices")

devices_ref.on_snapshot(on_device_change)


# ==========================================
# 5. ALERTES
# ==========================================
last_alert_time = {}


def handle_alerts(sensor_id, piece, cle, valeur):

    key = f"{sensor_id}_{cle}"

    if cle == "temperature" and valeur > 30:

        now = time.time()

        if key not in last_alert_time or now - last_alert_time[key] > 600:

            db.collection("users") \
                .document(USER_ID) \
                .collection("alerts") \
                .add({
                    "sensorId": sensor_id,
                    "message": f"Temp élevée {valeur}°C dans {piece}",
                    "level": "critical",
                    "read": False,
                    "createdAt": firestore.SERVER_TIMESTAMP
                })

            print(" ALERTE:", piece, valeur)
            last_alert_time[key] = now


# ==========================================
# 6. LOOP CAPTEURS (ARDUINO + SIMULATION)
# ==========================================
def loop_arduino():

    global ser, MODE_SIMULATION

    while True:
        try:

            # =========================
            # MODE SIMULATION
            # =========================
            if MODE_SIMULATION:

                try:
                    line = input("\nJSON capteur > ")

                    if not line.startswith("{"):
                        continue

                    data = json.loads(line)

                except:
                    print(" JSON invalide")
                    continue

            # =========================
            # MODE ARDUINO
            # =========================
            else:

                if ser and ser.in_waiting > 0:
                    line = ser.readline().decode().strip()

                    if not line.startswith("{"):
                        continue

                    try:
                        data = json.loads(line)
                    except:
                        continue
                else:
                    continue

            # =========================
            # TRAITEMENT DONNÉES
            # =========================
            piece = data.get("piece", "salon")

            for key, value in data.items():

                if key == "piece":
                    continue

                sensor_id = f"{key}_{piece}"

                sensor_ref = db.collection("users") \
                    .document(USER_ID) \
                    .collection("sensors") \
                    .document(sensor_id)

                # TEMPS RÉEL
                sensor_ref.set({
                    "value": value,
                    "room": piece,
                    "updatedAt": firestore.SERVER_TIMESTAMP
                }, merge=True)

                # HISTORIQUE (light throttle)
                if int(time.time()) % 900 == 0:
                    sensor_ref.collection("history").add({
                        "value": value,
                        "timestamp": firestore.SERVER_TIMESTAMP
                    })

                # ALERTES
                handle_alerts(sensor_id, piece, key, value)

        except Exception as e:
            print("Loop error:", e)
            time.sleep(2)


# ==========================================
# 7. THREAD CAPTEURS
# ==========================================
threading.Thread(target=loop_arduino, daemon=True).start()


# ==========================================
# 8. MAIN LOOP KEEP ALIVE
# ==========================================
print(" Bridge actif (bidirectionnel + simulation)")

while True:
    time.sleep(5)
