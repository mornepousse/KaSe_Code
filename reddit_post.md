# 🇫🇷 Présentation du projet KaSe - Clavier mécanique custom ESP32-S3

Salut à tous ! 👋

Je suis ravi de vous présenter **KaSe**, mon projet de clavier mécanique custom complètement open-source, basé sur l'**ESP32-S3** !

---

## 🎯 Le projet en bref

KaSe est un clavier mécanique DIY qui combine **hardware**, **firmware** et **logiciel de configuration** dans un écosystème entièrement ouvert. Le projet est divisé en 3 dépôts GitHub complémentaires :

### 🔧 **KaSe_Code** - Le firmware embarqué
**👉 https://github.com/mornepousse/KaSe_Code**

Le cerveau du clavier ! Firmware ESP32-S3 avec :
- ✅ **USB HID** (clavier USB via TinyUSB)
- ✅ **Bluetooth HID** (mode sans fil)
- ✅ **Écran OLED I²C** (affichage du layer actif et infos)
- ✅ **Keymaps multi-couches** configurables et persistants (NVS/LittleFS)
- ✅ **Interface CDC ACM** pour la configuration en temps réel
- ✅ Scan de matrice de touches optimisé

### ⚡ **KaSe_PCB** - Le hardware
**👉 https://github.com/mornepousse/KaSe_PCB**

Tout le design électronique et mécanique :
- 📐 Schémas et layout PCB
- 🔌 Pinout et connectique
- 🛠️ Design mécanique (switches, boîtier, etc.)
- 📋 BOM et références des composants

### 💻 **KaSe_Soft** - L'outil de configuration
**👉 https://github.com/mornepousse/KaSe_soft**

Application desktop pour configurer le clavier facilement :
- 🎨 Interface graphique pour remapper les touches
- 📊 Gestion des layers (couches de keymaps)
- ⚙️ Configuration et personnalisation avancée
- 🔄 Communication USB CDC ACM avec le firmware

---

## ✨ Pourquoi KaSe ?

- **100% Open Source** : GPL-3.0, tout est disponible et hackable
- **Moderne** : ESP32-S3 avec WiFi, Bluetooth et USB natif
- **Flexible** : Dual-mode USB/Bluetooth, layers configurables
- **Complet** : Projet clé en main (firmware + PCB + soft de config)
- **Orienté maker** : Conçu pour être modifiable et personnalisable

---

## 🚀 État du projet

Le projet est **fonctionnel** mais toujours **en développement actif** ! Les features de base marchent bien (scan matrice, USB HID, BLE HID, OLED, stockage persistent), et je continue d'ajouter des améliorations.

**Prochaines étapes** :
- Optimisation de la consommation en mode BLE
- Support macros avancées
- Peut-être RGB ? 🌈
- Documentation plus détaillée

---

## 🤝 Contributions bienvenues !

C'est un projet communautaire et ouvert. Si ça vous intéresse :
- N'hésitez pas à **explorer le code** et à poser des questions
- Les **issues, PR et suggestions** sont les bienvenues sur les 3 repos
- Idéal pour apprendre l'ESP32, USB/BLE HID, et les claviers custom

---

## 📚 Pour aller plus loin

**Documentation principale** : dans chaque dépôt (README complets)

**Setup rapide** :
```bash
# Firmware (ESP-IDF requis)
git clone https://github.com/mornepousse/KaSe_Code
cd KaSe_Code
idf.py set-target esp32s3
idf.py build
idf.py flash
```

---

Voilà ! N'hésitez pas si vous avez des questions ou si vous voulez contribuer. Je suis ouvert aux retours et aux idées pour améliorer le projet ! 😊

**Liens directs** :
- 🔧 Firmware : https://github.com/mornepousse/KaSe_Code
- ⚡ PCB/Hardware : https://github.com/mornepousse/KaSe_PCB
- 💻 Config soft : https://github.com/mornepousse/KaSe_soft

---

*Projet sous licence GPL-3.0 - Libre et ouvert !*
