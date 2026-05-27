# Starfield Configuration & Mod Setup

A curated Starfield mod configuration managed through **linutil** for easy installation and setup.

## 🚀 Quick Start Installation

### Prerequisites
- Starfield installed on your system (MUST HAVE LAUNCHED AT LEAST ONCE!)
- [linutil](https://github.com/ChrisTitusTech/linutil) installed and configured

### Installation Steps

1. **Open linutil** and navigate to the Starfield configuration installation option
2. **Follow the linutil prompt** to select this repository
3. linutil will **automatically**:
   - Install and configure SFSE (Starfield Script Extender)
   - Download and install all required mods
   - Copy mod files to the correct locations
   - Generate necessary configuration files

![linutil install guide](linutil%20install.webp)

### Post-Installation: Enable ESM Files

After linutil completes the installation, you need to enable the ESM files through Creations:

**Steps:**
1. Launch Starfield
2. Go to **Main Menu → Creations**
3. Enable the following ESM files:
   - ✅ Easy Digipick.esm
   - ✅ KeepYourTrash.esm
   - ✅ NoXPKillSteal.esm
4. Start your game and enjoy!

---

## 📦 What's Included

### SFSE Mods
- **SFSE** - Starfield Script Extender core
- **SFSE Address Libraries** - Required dependency for SFSE mods
- **SFSE BAKA Achievement Unlock** - Unlock achievements with mods enabled
- **SFSE Seamless Grav Jumps** - Smooth gravity jump transitions
- **SFSE Starware Search Engine** - Enhanced search functionality
- **Dynamic HUD** - Context-aware HUD that adapts to situations

### UI Enhancements
- **StarUI Inventory** - Completely redesigned inventory interface
- **StarUI Workbench** - Improved crafting interface
- **StarUI Outpost** - Better outpost management
- **StarUI HUD** - Enhanced heads-up display
- **StarUI Ship Builder** - Streamlined ship customization
- **Compact Mission Menu** - Space-efficient mission tracker
- **60 FPS - Smooth UI** - UI animations at 60 FPS for smooth performance

### Quality of Life
- **HUD Info Widget** - Display useful information on screen
- **Instant Workbench** - Skip loading screens for workbenches
- **Clean Reflex Sight Lenses** - Visual improvements for scopes

### ESM Files
- **Easy Digipick.esm** - Simplifies the digipick minigame
- **KeepYourTrash.esm** - Prevents item deletion
- **NoXPKillSteal.esm** - Prevents XP penalty for kill stealing

---

## 🔧 Configuration

The `StarfieldCustom.ini` file contains custom game settings. Modify as needed for your preferences.

All mod files are located in the `Data/` directory with proper folder structure for Starfield.

---

## 📝 File Structure

```
starfield-config/
├── README.md                    # This file
├── Mod-list.txt                # Complete list of mods
├── StarfieldCustom.ini         # Game configuration
├── sfse_loader.exe             # SFSE loader (auto-installed by linutil)
├── sfse_1_16_242.dll           # SFSE core (auto-installed by linutil)
├── Data/                        # Game mod files
│   ├── Easy Digipick.esm
│   ├── KeepYourTrash.esm
│   ├── NoXPKillSteal.esm
│   ├── Interface/              # UI modification files
│   ├── Materials/              # Texture and material files
│   ├── Meshes/                 # 3D model files
│   ├── SFSE/                   # SFSE plugin files
│   ├── Strings/                # Text localization files
│   └── Video/                  # Video/cinematic files
└── linutil install.webp        # Installation guide image
```

---

## ⚡ What linutil Handles

✅ Automatic SFSE installation and configuration  
✅ All mod file downloads and placement  
✅ Dependency resolution  
✅ File structure setup  
✅ Configuration file generation  

**You only need to:** Enable the ESM files in the Creations menu!

---

## 🐛 Troubleshooting

### ESM Files Not Showing in Creations
- Verify that `Data/` directory contains the `.esm` files
- Restart Starfield completely
- Check that the `StarfieldCustom.ini` is in your Starfield installation directory

### Mods Not Loading
- Ensure SFSE is running (you should see "Starfield Script Extender" in the main menu)
- Verify all files are in the correct `Data/` subdirectories
- Re-run linutil to verify all files are properly installed

### Questions or Issues
- Check the original linutil repository: [ChrisTitusTech/linutil](https://github.com/ChrisTitusTech/linutil)
- Review your Starfield load order and mod conflicts
- Consult individual mod documentation for specific mod issues

---

## 📄 License

This configuration is provided as-is for personal use. All included mods maintain their original licenses and belong to their respective creators.

---

**Enjoy your enhanced Starfield experience!** 🎮✨
