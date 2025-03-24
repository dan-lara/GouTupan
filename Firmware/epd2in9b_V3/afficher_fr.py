import re

def hex_to_binary(hex_str):
    # Supprimer les accolades et les virgules
    hex_str = hex_str.strip('{},')
    # Extraire toutes les valeurs hexadécimales
    hex_values = re.findall(r'0x[0-9A-Fa-f]+', hex_str)
    print("\n")
    print(hex_str)
    # Convertir chaque valeur hexadécimale en binaire et l'afficher
    for hex_value in hex_values:
        binary_value = bin(int(hex_value, 16))[2:].zfill(8)  # Conversion en binaire sur 8 bits
        print(binary_value)

    for hex_value in hex_values:
        binary_value = bin(int(hex_value, 16))[2:].zfill(8)  # Conversion en binaire sur 8 bits
        binary_display = binary_value.replace('0', ' ').replace('1', '*')
        print(binary_display)

def binary_to_hex(binary_lines):
    hex_values = []
    for binary_line in binary_lines:
        # Convertir la chaîne binaire en hexadécimal
        hex_value = hex(int(binary_line, 2))
        hex_values.append(hex_value)
    # Affichage formaté
    hex_str = "{" + ", ".join(hex_values) + "}"
    print("\nFormat hexadécimal converti :")
    print(hex_str)
    return hex_str

def multi_line_input():
    print("Veuillez entrer plusieurs lignes de données binaires (8 bits par ligne), tapez 'fin' pour terminer :")
    binary_lines = []
    while True:
        # Lire plusieurs lignes à la fois
        lines = input().strip().split('\n')
        for line in lines:
            line = line.strip()
            if line.lower() == "fin":
                return binary_lines
            if len(line) != 8 or not all(c in '01' for c in line):
                print(f"Erreur : '{line}' n'est pas une donnée binaire valide de 8 bits.")
                continue
            binary_lines.append(line)
        # Si 'fin' n'a pas été détecté, continuer la saisie
        print("Entrez d'autres données binaires ou tapez 'fin' pour terminer :")

if __name__ == "__main__":
    while True:
        user_input = input("Veuillez entrer un tableau (format : {0x02, 0x42, 0xFE, ...}), ou tapez 'b' pour entrer en mode binaire, ou 'q' pour quitter :")
        if user_input.lower() == "q":
            print("Programme terminé.")
            break
        elif user_input.lower() == "b":
            binary_lines = multi_line_input()
            if binary_lines:
                hex_str = binary_to_hex(binary_lines)
                hex_to_binary(hex_str)
        else:
            hex_to_binary(user_input)
