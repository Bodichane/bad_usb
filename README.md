# Simulation d'Injection HID Sécurisée (Outil d'Audit BadUSB)

Un projet de preuve de concept (PoC) démontrant comment les systèmes d'exploitation font nativement confiance aux périphériques HID (Human Interface Device). Cet outil simule un clavier USB à frappe rapide à l'aide d'un Arduino Leonardo (ATmega32u4) afin d'automatiser une tâche de reconnaissance système sur Linux (Ubuntu/GNOME), tout en utilisant des mesures de furtivité et de sécurité matérielle.

⚠️ **Avertissement :** Ce projet est créé strictement à des fins éducatives, défensives et d'audit de sécurité autorisé. N'utilisez pas cet outil sur des systèmes sans autorisation explicite préalable.

## Fonctionnalités

- **Interrupteur de Sécurité Matériel (Fail-Safe) :** Prévention immédiate des boucles d'exécution accidentelles. Le script s'exécute *uniquement* si la broche physique 2 est reliée au GND.
- **Architecture C++ Moderne :** Entièrement développé avec VS Code et PlatformIO, supprimant les objets lourds de type `String` pour optimiser l'empreinte mémoire RAM et Flash.
- **Moteur de Frappe Réaliste :** Injection caractère par caractère via une fonction personnalisée `typeCommand` pour garantir la stabilité du tampon d'entrée sur l'OS cible.
- **Techniques de Furtivité :** Utilisation de la suppression de l'écho visuel du terminal (`stty -echo`) et de raccourcis de gestion des fenêtres pour exécuter l'audit discrètement.

## Configuration Matérielle

- **Microcontrôleur :** Arduino Leonardo (ou toute carte basée sur un ATmega32u4 capable de simuler nativement un périphérique HID USB).
- **Câblage :** 1x fil de connexion Dupont Mâle-Mâle (faisant office d'interrupteur physique de sécurité).

### Installation Matérielle
Connectez une extrémité du fil de connexion à la broche numérique **Pin 2** et l'autre extrémité à n'importe quelle broche **GND** de la carte Arduino pour autoriser l'exécution de la charge utile (payload).

```text
  [ Arduino Leonardo ]
   Pin 2 <---> Fil de connexion <---> GND
```

## Configuration les Permissions Linux (Obligatoire)

Sur les distributions Linux modernes comme Ubuntu, le système de sécurité bloque par défaut l'injection brute de caractères par un périphérique HID tiers non autorisé. Avant de brancher la carte, vous devez accorder les permissions nécessaires via une règle `udev`.

1. Ouvrez un terminal sur votre machine Linux et créez le fichier de règles :
   ```bash
   sudo nano /etc/udev/rules.d/99-arduino.rules
   ```

2. Collez la ligne suivante à l'intérieur (elle autorise l'accès au Vendor ID et Product ID de l'Arduino Leonardo) :
   ```text
   SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="8036", MODE="0666"
   ```

3. Sauvegardez (`Ctrl+O` puis `Entrée`) et quittez (`Ctrl+X`).

4. Rechargez le gestionnaire de périphériques pour appliquer les modifications :
   ```bash
   sudo udevadm control --reload-rules && sudo udevadm trigger
   ```

## Logiciel & Environnement Cible

- **Environnement de Développement :** VS Code avec l'extension **PlatformIO IDE**.
- **OS Cible :** Linux Ubuntu (testé sur des environnements de bureau GNOME modernes utilisant Wayland et l'agent de terminal Ptyxis).
- **Disposition Clavier Cible :** QWERTY (Standard US).

## Configuration du Projet (`platformio.ini`)

Assurez-vous que la configuration de votre environnement inclut explicitement la dépendance à la bibliothèque native du clavier :

```ini
[env:leonardo]
platform = atmelavr
board = leonardo
framework = arduino
monitor_speed = 9600
lib_deps = 
    arduino-libraries/Keyboard @ ^1.0.6
```

## Fonctionnement du Script

1. **Vérification de Sécurité :** L'appareil démarre et évalue l'état de la broche 2. Si elle est à l'état `LOW` (fil connecté), il continue après une fenêtre de sécurité de 3 secondes.
2. **Déclenchement du Terminal :** Il injecte le raccourci `Ctrl + Alt + T` pour ouvrir le shell cible.
3. **Suppression Visuelle :** Il envoie une combinaison rapide d'ancrage de fenêtre, suivie immédiatement de `stty -echo` pour empêcher l'affichage des futures frappes sur le moniteur.
4. **Charge Utile de Reconnaissance :** Il crée un fichier texte sur le Bureau (`system_info.txt`) contenant les télémétries du système (`uname -a`).
5. **Fermeture Propre :** Il restaure l'état de l'écho du terminal et ferme l'instance en toute sécurité (`stty echo && exit`).

## Licence

Ce projet est sous licence MIT - voir le fichier LICENSE pour plus de détails.
