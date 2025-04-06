#ifndef NPK_SENSOR_HPP
#define NPK_SENSOR_HPP

#include <Arduino.h>

// Commandes pour interroger le capteur
uint8_t Com[8] = { 0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C };  // Azote (N)
uint8_t Com1[8] = { 0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC }; // Phosphore (P)
uint8_t Com2[8] = { 0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0 }; // Potassium (K)

// Déclaration des variables globales pour stocker les valeurs
float N, P, K;

// Prototypes des fonctions
void begin_NPK(int attempts);
float read_NPK_N_Nitrogen();
float read_NPK_P_Phosphorus();
float read_NPK_K_Potassium();
uint8_t readN(uint8_t *buf, size_t len);
unsigned int CRC16_2(unsigned char *buf, int len);

// Initialisation
void begin_NPK(int attempts) {
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("Initialisation du capteur NPK...");
}

// Lecture de l'azote (N)
float read_NPK_N_Nitrogen() {
    uint8_t Data[10] = { 0 };
    uint8_t ch = 0;
    bool flag = true;
    float N = -1;//91.0 + (random(-5, 6) * 0.1);  // Simulation de la valeur lue

    while (flag) {
        delay(100);
        Serial1.write(Com, 8);
        delay(10);
        if (readN(&ch, 1) == 1 && ch == 0x01) {
            Data[0] = ch;
            if (readN(&ch, 1) == 1 && ch == 0x03) {
                Data[1] = ch;
                if (readN(&ch, 1) == 1 && ch == 0x02) {
                    Data[2] = ch;
                    if (readN(&Data[3], 4) == 4 && CRC16_2(Data, 5) == (Data[5] * 256 + Data[6])) {
                        N = Data[3] * 256 + Data[4];
                        flag = false;
                    }
                }
            }
        }
        Serial1.flush();
    }
    return N;
}

// Lecture du phosphore (P)
float read_NPK_P_Phosphorus() {
    uint8_t Data1[10] = { 0 };
    uint8_t ch1 = 0;
    bool flag1 = true;
    float P = -1;//259.0 + (random(-5, 6) * 0.1);  // Simulation de la valeur lue

    while (flag1) {
        delay(100);
        Serial1.write(Com1, 8);
        delay(10);
        if (readN(&ch1, 1) == 1 && ch1 == 0x01) {
            Data1[0] = ch1;
            if (readN(&ch1, 1) == 1 && ch1 == 0x03) {
                Data1[1] = ch1;
                if (readN(&ch1, 1) == 1 && ch1 == 0x02) {
                    Data1[2] = ch1;
                    if (readN(&Data1[3], 4) == 4 && CRC16_2(Data1, 5) == (Data1[5] * 256 + Data1[6])) {
                        P = Data1[3] * 256 + Data1[4];
                        flag1 = false;
                    }
                }
            }
        }
        Serial1.flush();
    }
    return P;
}

// Lecture du potassium (K)
float read_NPK_K_Potassium() {
    uint8_t Data2[10] = { 0 };
    uint8_t ch2 = 0;
    bool flag2 = true;
    float K = -1;// 252.0 + (random(-5, 6) * 0.1);  // Simulation de la valeur lue

    while (flag2) {
        delay(100);
        Serial1.write(Com2, 8);
        delay(10);
        if (readN(&ch2, 1) == 1 && ch2 == 0x01) {
            Data2[0] = ch2;
            if (readN(&ch2, 1) == 1 && ch2 == 0x03) {
                Data2[1] = ch2;
                if (readN(&ch2, 1) == 1 && ch2 == 0x02) {
                    Data2[2] = ch2;
                    if (readN(&Data2[3], 4) == 4 && CRC16_2(Data2, 5) == (Data2[5] * 256 + Data2[6])) {
                        K = Data2[3] * 256 + Data2[4];
                        flag2 = false;
                    }
                }
            }
        }
        Serial1.flush();
    }
    return K;
}

// Fonction de lecture série
uint8_t readN(uint8_t *buf, size_t len) {
    size_t offset = 0, left = len;
    int16_t Timeout = 500;
    long curr = millis();

    while (left) {
        if (Serial1.available()) {
            buf[offset++] = Serial1.read();
            left--;
        }
        if (millis() - curr > Timeout) {
            break;
        }
    }
    return offset;
}

// Fonction de calcul du CRC16
unsigned int CRC16_2(unsigned char *buf, int len) {
    unsigned int crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (unsigned int)buf[pos];
        for (int i = 8; i != 0; i--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
}
#endif