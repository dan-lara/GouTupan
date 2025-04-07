# EI-Farm

[![en](https://img.shields.io/badge/lang-en-red.svg)](https://github.com/dan-lara/EI-Farm/blob/master/README.md)
[![fr](https://img.shields.io/badge/lang-fr-green.svg)](https://github.com/dan-lara/EI-Farm/blob/master/README.fr.md)

# Guide Utilisateur —— GouTupan  

**Daniel, Vladislav, Xiaochen, Yulin**  

---

## Présentation du dispositif  

Le système est une plateforme intégrée de surveillance environnementale pour serres agricoles, basée sur une carte de développement Arduino MKR WAN 1310.  
Équipé de multiples capteurs spécialisés, il permet la mesure en temps réel des paramètres critiques pour la croissance végétale.  

Les fonctionnalités clés comprennent :  
- Alimentation autonome par panneau solaire avec circuit de gestion d'énergie ;  
- Conception étanche adaptée aux environnements de serre avec arrosage fréquent ;  
- Transmission sans fil des données via LoRaWAN ;  
- Appareil divisé, adapté à des points de mesure plus personnalisés ;  
- Interface utilisateur : site web et écran local pour une consultation à distance ou sur place.  

---

## Installation et instructions d'utilisation  

L'appareil transmet les données via le protocole LoRa. Il est donc impératif de s'assurer que le lieu d'installation bénéficie d'une couverture stable par une passerelle LoRaWAN.  
Sans cela, les fonctionnalités suivantes seront inaccessibles : consultation à distance, et interrogation de l'historique.  

Le système est composé de quatre modules principaux :  
- **Boîtier MCU** : Contenant le microcontrôleur et l'unité de traitement ;  
- **Capteur “champignon”** : Mesurant les conditions ambiantes ;  
- **Ensemble de capteurs de température et d'humidité du sol** ;  
- **Panneau solaire** : Assurant l'alimentation du système.  

### Boîtier MCU  
Intègre une carte MKR WAN 1310, un circuit de gestion d'alimentation, un capteur de luminosité et un écran à encre électronique.  
**Installation** : Positionné dans un endroit non inondable et bien exposé à la lumière.  

### Capteur “champignon”  
Regroupe des capteurs de température, d'humidité et de gaz. Placé aux points de mesure cibles.  
**Attention** : Éviter tout arrosage par le dessous pour prévenir la pénétration d'eau.  

### Capteurs du sol  
- **Capteur compact** : Mesure en surface (10 cm de profondeur) ;  
- **Sonde longue** : Forme cylindrique (50 cm), pour mesures en profondeur.  

### Panneau solaire  
**Installation** : Positionné dans un endroit ensoleillé, en hauteur pour éviter l'accumulation d'eau.  

### Activation  
Activez l'**interrupteur latéral** sur le boîtier MCU. Les données sont transmises toutes les **3 minutes**.  

---

## Informations relatives au réseau et à l'IHM  

### TTN (The Things Network)  
- **Nom d'utilisateur** : Levovitch  
- **Mot de passe** : demo1234  
- **Nom de l'application** : `goutupan-mkr1310-vlad-xioa-dani-yuli`  
- **Device ID** : `goutupan-mkr1310-vlad-xioa-dani-yuli`  
- **AppEUI** : `AA2025AAAA2025AA`  
- **DevEUI** : `A8610A3234268310`  
- **AppKey** : `9qyh4jN6RBh4F@\$`  

### Liens utiles  
- [Page web d'affichage des données](https://goutupan.grafana.net/public-dashboards/d0bf3f14221d4401bc8c1fea2e30d045?var-device_var=goutupan-mkr1310-yulin-4&theme=light&from=now-7d&to=now&timezone=browser)  
- [Page GitHub du projet](https://github.com/dan-lara/GouTupan)  

---

## Description des capteurs  

**Tableau 1 : Spécifications techniques des capteurs environnementaux**  

| Module                     | Paramètres mesurés                                                                 | Caractéristiques techniques                                                                 |
|----------------------------|-----------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|
| **SCD30**                  | Dioxyde de carbone (CO₂),<br>Température,<br>Humidité                             | 400 ppm – 10 000 ppm (±30 ppm ±3%),<br>0–50 °C (±0,4 °C),<br>0–100 %HR (±3 %HR)            |
| **TCS34725/SI1145**        | Composantes spectrales (RVB / UV / IR)                                            | λ = 465, 525, 615 nm,<br>Plage : 400–1000 nm,<br>Détection UV : 280–400 nm (par algorithme) |
| **SEN0605**                | Azote (N), Phosphore (P), Potassium (K)                                           | Plage : 0–2999 mg/kg,<br>Résolution : 1 mg/kg,<br>Précision ≤5%,<br>IP68                   |
| **Baromètre Grove 101020068** | Pression barométrique                                                          | 30–110 kPa (±0,01 hPa)                                                                     |
| **SEN0322**                | Concentration en O₂                                                               | 0–25 %vol (dérive < 2%/mois)                                                              |
| **SGP40**                  | Composés organiques volatils (VOC)                                                | Indice COV : 0–500,<br>Éthanol : 0–1000 ppm                                               |
| **SHT30/SHT20**          | Température et Humidité                                                           | Humidité : 0–100 %HR (±2%RH/±3%RH),<br>Température : -40–125 °C (±0,2 °C/±0.3 °C)                         |
| **Capteur de gaz Grove 101020820** | Ammoniac (NH₃), <br>Monoxyde de carbone (CO), <br>Dioxyde d'azote (NO₂),<br>Propane (C₃H₈), <br>Butane (C₄H₁₀), <br>Méthane (CH₄), <br>Hydrogène (H₂), <br>Éthanol (C₂H₅OH)                      | NH₃ : 1–500 ppm,<br>CO : 5–5000 ppm,<br>NO₂ : 0,05–2 ppm,<br>Hydrocarbures : 500–10 000 ppm,<br>H₂ : 1–1000 ppm,<br>Éthanol : 1–500 ppm    |

---  
