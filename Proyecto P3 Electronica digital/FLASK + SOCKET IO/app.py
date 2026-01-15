import json
from flask import Flask, render_template
from flask_socketio import SocketIO
import paho.mqtt.client as mqtt

# ---------------- CONFIG ----------------
BROKER_IP = "192.168.100.18"
MQTT_PORT = 1883
MQTT_TOPIC = "parking/estado"

app = Flask(__name__)



socketio = SocketIO(
    app,
    cors_allowed_origins="*",
    async_mode="threading"
)

estado_parqueos = {}

# ---------- MQTT CALLBACKS ----------
def on_connect(client, userdata, flags, rc, properties=None): # Actualizado para VERSION2
    print("✅ MQTT conectado exitosamente")
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    global estado_parqueos
    try:
        payload = msg.payload.decode()
        estado_parqueos = json.loads(payload)
        # Emitimos directamente
        socketio.emit("estado_parqueos", estado_parqueos)
    except Exception as e:
        print(f"❌ Error en mensaje: {e}")

# ---------- BACKGROUND TASK ----------
  
def run_mqtt():
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(BROKER_IP, MQTT_PORT, 60)

    # Loop NO bloqueante
    client.loop_start()

@app.route("/")
def index():
    return render_template("index.html")



if __name__ == "__main__":
    print("🚀 Iniciando tareas de fondo...")
    socketio.start_background_task(run_mqtt)

    socketio.run(
        app,
        host="0.0.0.0",
        port=5000,
        debug=False
    )
