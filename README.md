# GouTupan
[![en](https://img.shields.io/badge/lang-en-red.svg)](https://github.com/dan-lara/EI-Farm/blob/master/README.md)
[![fr](https://img.shields.io/badge/lang-fr-green.svg)](https://github.com/dan-lara/EI-Farm/blob/master/README.fr.md)

# User Guide —— GouTupan  

**Daniel, Vladislav, Xiaochen, Yulin**  

---

## Device Overview  

The system is an integrated environmental monitoring platform for agricultural greenhouses, based on an Arduino MKR WAN 1310 development board.  
Equipped with multiple specialized sensors, it enables real-time measurement of critical parameters for plant growth.  

Key features include:  
- Self-powered via solar panel with energy management circuit;  
- Waterproof design suitable for greenhouse environments with frequent irrigation;  
- Wireless data transmission via LoRaWAN;  
- Modular architecture for customized measurement points;  
- User interface: Web platform and local e-ink display for remote or on-site monitoring.  

---

## Installation and Usage Instructions  

The device transmits data using the LoRa protocol. Ensure the installation site has stable coverage from a LoRaWAN gateway.  
Without this, the following features will be unavailable: remote monitoring and historical data access.  

The system comprises four main modules:  
- **MCU Enclosure**: Contains the microcontroller and processing unit;  
- **"Mushroom" Sensor**: Measures ambient conditions;  
- **Soil Temperature & Humidity Sensor Array**;  
- **Solar Panel**: Powers the system.  

### MCU Enclosure  
Integrates an MKR WAN 1310 board, power management circuit, light sensor, and e-ink display.  
**Installation**: Position in a non-floodable area with direct ambient light exposure.  

### "Mushroom" Sensor  
Combines temperature, humidity, and gas sensors. Deploy at target measurement points.  
**Caution**: Avoid watering from below to prevent water ingress.  

### Soil Sensors  
- **Compact Probe**: Surface measurement (10 cm depth);  
- **Long Cylindrical Probe**: 50 cm length for deep soil measurements.  

### Solar Panel  
**Installation**: Place in a sunlit, elevated location to avoid water accumulation.  

### Activation  
Toggle the **side switch** on the MCU enclosure. Data transmission occurs every **3 minutes** (preconfigured).  

---

## Network and HMI Information  

### TTN (The Things Network)  
- **Username**: Levovitch  
- **Password**: demo1234  
- **Application Name**: `goutupan-mkr1310-vlad-xioa-dani-yuli`  
- **Device ID**: `goutupan-mkr1310-vlad-xioa-dani-yuli`  
- **AppEUI**: `AA2025AAAA2025AA`  
- **DevEUI**: `A8610A3234268310`  
- **AppKey**: `9qyh4jN6RBh4F@\$`  

### Useful Links  
- [Data Visualization Dashboard](https://goutupan.grafana.net/public-dashboards/d0bf3f14221d4401bc8c1fea2e30d045?var-device_var=goutupan-mkr1310-yulin-4&theme=light&from=now-7d&to=now&timezone=browser)  
- [Project GitHub Repository](https://github.com/dan-lara/GouTupan)  

---

## Sensor Specifications  

**Table 1: Technical Specifications of Environmental Sensors**  

| Module                     | Measured Parameters                                                                 | Technical Specifications                                                                 |
|----------------------------|-------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------|
| **SCD30**                  | Carbon Dioxide (CO₂),<br>Temperature,<br>Humidity                                   | 400 ppm – 10,000 ppm (±30 ppm ±3%),<br>0–50 °C (±0.4 °C),<br>0–100% RH (±3% RH)         |
| **TCS34725/SI1145**        | Spectral Components (RGB/UV/IR)                                                    | λ = 465, 525, 615 nm,<br>Range: 400–1000 nm,<br>UV Detection: 280–400 nm (algorithmic)  |
| **SEN0605**                | Nitrogen (N), Phosphorus (P), Potassium (K)                                        | Range: 0–2999 mg/kg,<br>Resolution: 1 mg/kg,<br>Accuracy ≤5%,<br>IP68                   |
| **Grove Barometer 101020068** | Barometric Pressure                                                             | 30–110 kPa (±0.01 hPa)                                                                 |
| **SEN0322**                | Oxygen (O₂) Concentration                                                          | 0–25% vol (drift < 2%/month)                                                          |
| **SGP40**                  | Volatile Organic Compounds (VOC)                                                   | VOC Index: 0–500,<br>Ethanol: 0–1000 ppm                                              |
| **SHT35**                  | Temperature & Humidity                                                             | Humidity: 0–100% RH (±2%),<br>Temperature: -40–125 °C (±0.2 °C)                      |
| **Grove Gas Sensor 101020820** | Ammonia (NH₃), <br>Carbon Monoxide (CO), <br>Nitrogen Dioxide (NO₂),<br>Propane (C₃H₈), <br>Butane (C₄H₁₀), <br>Methane (CH₄), <br>Hydrogen (H₂), <br>Ethanol (C₂H₅OH) | NH₃: 1–500 ppm,<br>CO: 5–5000 ppm,<br>NO₂: 0.05–2 ppm,<br>Hydrocarbons: 500–10,000 ppm,<br>H₂: 1–1000 ppm,<br>Ethanol: 1–500 ppm |

---
