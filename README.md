# 🎹 ESP32-C3 Hybrid Acid / Happy Hardcore Synth

Synthétiseur hybride **numérique / analogique-virtuel** pour **ESP32-C3**.  
Le dépôt est séparé en deux environnements logiciels distincts :
1. **Firmware C++ (PlatformIO) :** Séquenceur 16 pas, synthèse rythmique (Kick + mélodie) et émulation de basse résonante en temps réel (`22 050 Hz`).
2. **Laboratoire Python (`.venv` + Jupyter) :** Conception des schémas électroniques (`Schemdraw`), vérification électrique (`SKiDL`) et simulations audio (`SciPy`).

---

## 📂 Structure du Projet

```text
mon_synth_esp32c3/
├── platformio.ini               # Configuration C++ / ESP32-C3 (PlatformIO)
├── README.md                    # Documentation de setup et d'utilisation
├── include/                     # En-têtes C++ (.h)
├── src/                         # Code source C++ (.cpp)
└── hardware/                    # Environnement Python & électronique
    ├── .venv/                   # Environnement virtuel Python isolé
    ├── requirements.txt         # Dépendances Python du projet
    ├── synth_circuit.py         # Vérification ERC & export KiCad (SKiDL)
    └── labo_electronique.ipynb  # Notebook interactif (schémas & audio)
```

---

## ⚙️ 1. Configuration des Environnements (Première Installation)

### Prérequis système (Linux / VM SSH)
Installez les outils de base nécessaires à Python et PlatformIO :
```bash
sudo apt update && sudo apt install -y python3-venv python3-pip
```
Dans **Visual Studio Code**, installez les extensions suivantes (sur votre hôte SSH si vous travaillez à distance) :
- **PlatformIO IDE** (pour le C++ et l'ESP32)
- **Python** et **Jupyter** (pour les scripts et notebooks matériels)

---

### A. Configurer l'environnement C++ (PlatformIO)

1. **Vérifier `platformio.ini`** à la racine du projet :
   ```ini
   [env:esp32-c3-devkitm-1]
   platform = espressif32
   board = esp32-c3-devkitm-1
   framework = arduino
   monitor_speed = 115200

   lib_deps =
       adafruit/Adafruit GFX Library
       adafruit/Adafruit SSD1306
   ```

2. **Initialiser l'intégration VS Code** (à faire une seule fois dans le terminal à la racine du projet) :
   ```bash
   ~/.platformio/penv/bin/pio project init --ide vscode
   ```
3. Rechargez la fenêtre VS Code (`Ctrl+Shift+P` → **`Developer: Reload Window`**) pour faire apparaître la barre d'outils PlatformIO (`✓` Build, `→` Upload).

---

### B. Configurer l'environnement Python (`hardware/.venv`)

1. **Créer et activer l'environnement virtuel** dans le dossier `hardware/` :
   ```bash
   mkdir -p hardware && cd hardware
   python3 -m venv .venv
   source .venv/bin/activate
   pip install -r requirements.txt
   ```

2. **Installer les bibliothèques électroniques et audio :**
   ```bash
   pip install --upgrade pip
   pip install skidl schemdraw matplotlib scipy ipykernel ipython
   cd hardware
  source .venv/bin/activate
   ```
   *(Optionnel : sauvegardez vos dépendances avec `pip freeze > requirements.txt`).*

3. **Enregistrer le noyau Jupyter pour VS Code :**
   ```bash
   python -m ipykernel install --user --name=synth_hw --display-name="Python (Synth ESP32)"
   ```
4. **

---

## 🚀 2. Utilisation au Quotidien

### Développement C++ (Firmware ESP32-C3)
- **Compiler le code :** Cliquez sur l'icône **Build (`✓`)** en bas de VS Code (ou exécutez `~/.platformio/penv/bin/pio run` dans le terminal).
- **Téléverser sur la carte :** Branchez l'ESP32-C3 en USB et cliquez sur **Upload (`→`)**.
- **Modifier le firmware :**
  - Les broches sont centralisées dans `include/pins.h`.
  - Les motifs musicaux (`bassPatterns`, `melodyPatterns`) se trouvent dans `src/sequencer.cpp`.

### Développement Python (Schémas & Simulations)
- **En mode interactif (Jupyter Notebook) :**
  1. Ouvrez `hardware/labo_electronique.ipynb` dans VS Code.
  2. Sélectionnez le noyau **`Python (Synth ESP32)`** en haut à droite.
  3. Exécutez vos cellules (`Shift + Entrée`) pour générer les schémas `Schemdraw` et tester vos filtres audio.
- **En ligne de commande (Terminal) :**
  ```bash
  cd hardware
  source .venv/bin/activate
  python synth_circuit.py
  ```


### Utilisation du Synthétiseur
- **Potentiomètres :** `Tempo` (145–195 BPM), `Cutoff` (ouverture du filtre), `Reso` (résonance Acid), `Decay` (claquement de la basse), `Drive` (saturation).
- **Boutons :** `Pattern` (change parmi les 3 morceaux), `Backing` (active/coupe le Kick et la mélodie).
- **Écran OLED :** Affiche le morceau actif, l'état du backing, les valeurs des réglages en direct et un chenillard sur 16 pas.
