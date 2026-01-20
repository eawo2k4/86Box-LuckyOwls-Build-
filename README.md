# 86Box – LuckyOwl Custom Build

![License](https://img.shields.io/github/license/eawo2k4/86Box-LuckyOwls-Build-?style=flat-square)
![Build](https://img.shields.io/badge/build-Wizard%20Edition-blue?style=flat-square)
![Release](https://img.shields.io/github/v/release/eawo2k4/86Box-LuckyOwls-Build-?style=flat-square)


A customized fork of the 86Box emulator featuring an enhanced New Machine Wizard, improved UI flow, and quality‑of‑life refinements designed to make virtual machine setup smoother, clearer, and more beginner‑friendly.

This project builds on the original 86Box emulator while introducing user‑experience improvements created by **Elijah (LuckyOwl)**.

---

## ✨ What’s New in the LuckyOwl Build

### 🔧 Redesigned New Machine Wizard

A modernized setup flow that guides users through creating a new virtual machine with more clarity and structure.

**Enhancements include:**

- **OS Selection Page**  
  Choose your target operating system before configuring hardware, improving accuracy and recommended defaults.

- **Windows XP Compatibility Warning**  
  A context‑sensitive notice appears when selecting XP‑era configurations that may require specific hardware settings.

- **Improved Page Flow**  
  Cleaner transitions between Intro → OS Select → Name & Location → Summary.

- **UI Layout Polish**  
  Better spacing, alignment, and readability across wizard pages.

- **Updated Internal Logic**  
  Refined `nextId()` behavior and enum handling for smoother navigation.

This fork focuses on user experience while preserving full compatibility with upstream 86Box.

---

## 🖥️ About 86Box (Upstream Project)

86Box is a low‑level emulator for IBM PC systems and compatibles, capable of running classic operating systems and software from early 1980s hardware through late‑1990s PCI‑based designs.  
It emphasizes accuracy, configurability, and support for a wide range of vintage components.

🔗 [Visit the official 86Box repository](https://github.com/86Box/86Box)

---

## 🛠️ Building This Fork

This fork follows the same build process as upstream 86Box.

**Requirements:**

- Qt 6.x  
- MSVC toolchain  
- Standard 86Box dependencies

For complete instructions, refer to the upstream build guide.

---

## 📦 Releases

Compiled binaries of the LuckyOwl build (when available) will be published under the **Releases** section of this repository.  
These builds include all custom wizard enhancements and UI improvements.

---

## 🤝 Credits

- **86Box Team** — Original emulator, architecture, and core development  
- **Elijah (LuckyOwl)** — Custom wizard design, UI improvements, and fork maintenance

This project is made possible thanks to the incredible work of the upstream developers.

---

## 📜 License

This fork is distributed under the **GNU General Public License v2 or later**, identical to the upstream project.  
See the `COPYING` file for full licensing details.


