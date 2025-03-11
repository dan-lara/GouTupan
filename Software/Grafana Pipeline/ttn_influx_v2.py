import json
from dotenv import load_dotenv
import os
import os, time
from influxdb_client_3 import InfluxDBClient3, Point
import paho.mqtt.client as mqtt

# Configurations de TTN
load_dotenv()

TTN_BROKER = os.getenv("TTN_MQTT_URI",)  # Ajustez selon votre région (ex: nam1, eu1, etc.)
TTN_PORT = int(os.getenv("TTN_MQTT_PORT", 1883))  # Port par défaut de MQTT
TTN_USERNAME = os.getenv("TTN_MQTT_USERNAME", "seu_app_id")  # Remplacez par l'ID de l'application TTN
TTN_PASSWORD = os.getenv("TTN_MQTT_PASSWORD", "seu_api_key")  # Remplacez par la clé API de TTN
TTN_TOPIC = "#"  # Le '#' s'abonne à tous les sujets disponibles
# Configuration de INFLUXDB

INFLUXDB_TOKEN = os.environ.get("INFLUXDB_TOKEN")
INFLUXDB_BUCKET = os.environ.get("INFLUXDB_BUCKET")+"v2"
INFLUXDB_ORG = os.environ.get("INFLUXDB_ORG")
INFLUXDB_HOST = os.environ.get("INFLUXDB_HOST")
influx_client = InfluxDBClient3(host=INFLUXDB_HOST, token=INFLUXDB_TOKEN, org=INFLUXDB_ORG)


def manage_device(data, database):    
    # Extraction des informations de l'appareil
    end_device_ids = data.get("end_device_ids", {})
    device_id = end_device_ids.get("device_id", "unknown")
    table = None
    if device_id == "unknown":
        return
    try:
        
        query = """SELECT *
        FROM "devices"
        WHERE "device_id" IN ('{device_id}')"""
        
        query = query.format(device_id=device_id)

        # Exécuter la requête
        table = influx_client.query(query, database=INFLUXDB_BUCKET, language='sql')
        # print(f"Requête retournée {(table)} résultats")
        
    except Exception as e:
        table = False
        print(f"Erreur: {e}")
        
    if not table:
        print("Appareil non trouvé, création d'une nouvelle entrée")
        # Extraction des informations de l'appareil
        application = end_device_ids.get("application_ids", {}).get("application_id", "unknown")
        dev_eui = end_device_ids.get("dev_eui", "unknown")
        join_eui = end_device_ids.get("join_eui", "")
        dev_addr = end_device_ids.get("dev_addr", "")

        # Extraction des informations de version_ids (métadonnées de l'appareil)
        version_ids = data.get("uplink_message", {}).get("version_ids", {})
        brand_id = version_ids.get("brand_id", "")
        model_id = version_ids.get("model_id", "")
        band_id = version_ids.get("band_id", "")

        # Création du point pour la mesure "devices"
        point_devices = (
            Point("devices")
            .tag("device_id", device_id)
            .field("application", application)
            .field("dev_eui", dev_eui)
            .field("join_eui", join_eui)
            .field("dev_addr", dev_addr)
            .field("brand_id", brand_id)
            .field("model_id", model_id)
            .field("band_id", band_id)
        )
        influx_client.write(database=database, record=point_devices)
        return
    # print(f"Appareil {device_id} trouvé dans la base de données")

def generate_measurement(data, database):
    # Extraction des informations de l'appareil
    end_device_ids = data.get("end_device_ids", {})
    device_id = end_device_ids.get("device_id", "unknown")
    application = end_device_ids.get("application_ids", {}).get("application_id", "unknown")    
    # Extraction des informations de uplink_message (données de capteur)
    uplink_message = data.get("uplink_message", {})
    received_at = uplink_message.get("received_at", "")
    # tags
    f_port = int(uplink_message.get("f_port", ""))
    # fields
    f_cnt = int(uplink_message.get("f_cnt", 0))
    decoded_payload = uplink_message.get("decoded_payload", {})
    battery_level = decoded_payload.get("Battery_Level", 0)   
    
    light_infrared = decoded_payload.get("Light_Infrared", 0)
    light_intensity = decoded_payload.get("Light_Intensity", 0)
    light_ultraviolet = decoded_payload.get("Light_Ultraviolet", 0)
    rgb_b = decoded_payload.get("RGB_B", 0)
    rgb_g = decoded_payload.get("RGB_G", 0)
    rgb_r = decoded_payload.get("RGB_R", 0)
    
    air_co2 = decoded_payload.get("Outside_CO2", 0)
    air_humidity = decoded_payload.get("Outside_Humidity", 0)
    air_temperature = decoded_payload.get("Outside_Temperature", 0)
    air_quality = decoded_payload.get("Outside_Air_Quality", 0)
    air_c2h5oh = decoded_payload.get("Outside_C2H50H", 0)
    air_c3h8 = decoded_payload.get("Outside_C3H8", 0)
    air_c4h10 = decoded_payload.get("Outside_C4H10", 0)
    air_ch4 = decoded_payload.get("Outside_CH4", 0)
    air_co = decoded_payload.get("Outside_CO", 0)
    air_h2 = decoded_payload.get("Outside_H2", 0)
    air_nh3 = decoded_payload.get("Outside_NH3", 0)
    air_no2 = decoded_payload.get("Outside_NO2", 0)
    air_o2 = decoded_payload.get("Outside_O2", 0)
    
    pressure = decoded_payload.get("Pressure", 0)
    
    soil_nutrients_k_potassium = decoded_payload.get("Soil_Nutrients_K_Potassium", 0)
    soil_nutrients_n_nitrogen = decoded_payload.get("Soil_Nutrients_N_Nitrogen", 0)
    soil_nutrients_p_phosphorus = decoded_payload.get("Soil_Nutrients_P_Phosphorus", 0)
    surface_humidity = decoded_payload.get("Surface_Humidity", 0)
    surface_temperature = decoded_payload.get("Surface_Temperature", 0)
    deep_humidity = decoded_payload.get("Deep_Humidity", 0)
    deep_temperature = decoded_payload.get("Deep_Temperature", 0)
    
    point_measures = (
        Point("measures")
        .tag("device_id", device_id)
        .tag("application", application)
        .tag("f_port", int(f_port))
        
        .time(received_at)
        
        .field("f_cnt", int(f_cnt))
        .field("battery_level", int(battery_level))
        
        .field("light_infrared", int(light_infrared))
        .field("light_intensity", int(light_intensity))
        .field("light_ultraviolet", int(light_ultraviolet))
        .field("rgb_b", int(rgb_b))
        .field("rgb_g", int(rgb_g))
        .field("rgb_r", int(rgb_r))
        
        .field("air_co2", int(air_co2))
        .field("air_humidity", float(air_humidity))
        .field("air_temperature", float(air_temperature))
        .field("air_quality", float(air_quality))
        .field("air_c2h5oh", float(air_c2h5oh))
        .field("air_c3h8", float(air_c3h8))
        .field("air_c4h10", float(air_c4h10))
        .field("air_ch4", float(air_ch4))
        .field("air_co", float(air_co))
        .field("air_h2", float(air_h2))
        .field("air_nh3", float(air_nh3))
        .field("air_no2", float(air_no2))
        .field("air_o2", float(air_o2))
        
        
        .field("pressure", float(pressure))
        
        .field("soil_nutrients_k_potassium", int(soil_nutrients_k_potassium))
        .field("soil_nutrients_n_nitrogen", int(soil_nutrients_n_nitrogen))
        .field("soil_nutrients_p_phosphorus", int(soil_nutrients_p_phosphorus))
        .field("surface_humidity", float(surface_humidity))
        .field("surface_temperature", float(surface_temperature))
        .field("deep_humidity", float(deep_humidity))
        .field("deep_temperature", float(deep_temperature))
    )

    influx_client.write(database=database, record=point_measures)
    

def generate_rx(data, database):
    # Extraction des informations de l'appareil
    end_device_ids = data.get("end_device_ids", {})
    device_id = end_device_ids.get("device_id", "unknown")
    application = end_device_ids.get("application_ids", {}).get("application_id", "unknown")
    
    # Extraction des informations de rx_metadata (données de réception)
    rx_metadata = data.get("uplink_message", {}).get("rx_metadata", [])
    
    for metadata in rx_metadata:
        gateway_id = metadata.get("gateway_ids", {}).get("gateway_id", "unknown")
        eui = metadata.get("gateway_ids", {}).get("eui", "unknown")
        timestamp = metadata.get("timestamp", 0)
        rssi = metadata.get("rssi", 0)
        channel_rssi = metadata.get("channel_rssi", 0)
        snr = metadata.get("snr", 0)
        location = metadata.get("location", {})
        latitude = location.get("latitude", 0)
        longitude = location.get("longitude", 0)
        altitude = location.get("altitude", 0)
        received_at = metadata.get("received_at", "")
        
        point_rx = (
            Point("rx_metadata")
            .tag("device_id", device_id)
            .tag("application", application)
            .tag("gateway_id", gateway_id)
            .tag("eui", eui)
                        
            .time(received_at)
            
            .field("timestamp", timestamp)
            .field("rssi", rssi)
            .field("channel_rssi", channel_rssi)
            .field("snr", int(snr))
            .field("latitude", latitude)
            .field("longitude", longitude)
            .field("altitude", altitude)
        )
        
        influx_client.write(database=database, record=point_rx)
    

def persistir(data):
    
    database=INFLUXDB_BUCKET
    # print(f"INFLUXDB_BUCKET: {database}")
    # print(f"InfluxDB Token: {INFLUXDB_TOKEN}")
    
    # Sélectionne les données dans la première clé du JSON
    first_key = next(iter(data))
    data = data[first_key]
    device_id = data.get("end_device_ids", {}).get("device_id", "unknown")
    f_cnt = data.get("uplink_message", {}).get("f_cnt", 0)
    if data.get("uplink_message", {}).get("decoded_payload", {}).get("Mux_Code") == "Reading_1_data":
        
        manage_device(data, database)
        time.sleep(1) # séparer les points par 1 seconde

        generate_measurement(data, database)
        time.sleep(1) # séparer les points par 1 seconde
        
        generate_rx(data, database)
        time.sleep(1) # séparer les points par 1 seconde
        print(f"Données persistées pour l'appareil {device_id} avec f_cnt {f_cnt}")
    else:
        print("Message de {}n'est pas du type Reading_1_data, ignoré...".format(device_id))
        return


# Callback pour lorsque la connexion est établie
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Connecté au The Things Network MQTT")
        client.subscribe(TTN_TOPIC)  # S'abonne à tous les sujets
    else:
        print(f"❌ Échec de la connexion, code d'erreur: {rc}")

# Callback pour lorsque un message MQTT est reçu
def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode("utf-8"))  # Convertit le message en JSON
        data = {msg.topic: payload}
        with open("mqtt_messages.json", "w") as f:
            json.dump(data, f, indent=4)  # Sauvegarde dans le fichier JSON
        # print(f"Message sauvegardé dans le fichier mqtt_messages.json")
        try:
            persistir(data)
        except Exception as e:
            print("Erreur : ", e)
        # print(f"Données persistées dans la base de données")
    except json.JSONDecodeError:
        print(f"Erreur de décodage JSON du sujet {msg.topic}")
    

# Configuration du client MQTT
client = mqtt.Client()
client.username_pw_set(TTN_USERNAME, TTN_PASSWORD)
client.on_connect = on_connect
client.on_message = on_message

# Se connecter à TTN et attendre les messages
client.connect(TTN_BROKER, TTN_PORT, 60)
client.loop_forever()
