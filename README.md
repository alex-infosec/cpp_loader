# cpp_loader — WebAssembly Image Loader

This project demonstrates C++ to JavaScript interoperability using **Emscripten** and **WebAssembly**.  
It compiles native C++ code into a `.wasm` module and integrates it into an HTML `<canvas>` environment with minimal JavaScript glue.

---

## 🧠 Overview
The program simulates interoperability between a host and guest virtual machine.  
Through **bindings (Embind)**, C++ functions are exposed directly to JavaScript, enabling image loading and rendering inside a browser canvas.

Key features:
- Compiles C++ → WebAssembly using `em++`
- Displays an image generated from a header file (`gator.h`)
- Fetches and decodes an external `.rgba` image file via HTTP
- Handles **endianness conversion** for consistent cross-system rendering
- Fully self-contained: runs locally through a lightweight HTTP server

---

## ⚙️ Build Instructions

### Requirements
- [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html)
- Python 3 (for local server)

### Build Commands
```bash
make            # compiles cpp_loader.cpp → .wasm + .js loader
python3 -m http.server
```

Then open [http://localhost:8000/cpp_loader.html](http://localhost:8000/cpp_loader.html)

---

## 🧩 Core Components

| File | Description |
|------|--------------|
| `src/main.cpp` | Main WebAssembly entry point; exposes C++ functions with Embind |
| `src/gator.h` | Defines width, height, and pixel array for the embedded Gator image |
| `gator.rgba` | Binary RGBA image loaded via HTTP and drawn onto `<canvas>` |
| `cpp_loader.html` | HTML file containing minimal JS glue and target canvas |
| `Makefile` | Automates build process via `em++` |
| `mk.txt` | Compilation notes for reproducibility |

---

## 🧰 Key Functions

### `displayHeaderImage()`
Iterates through the pixel array defined in `gator.h`, converts data into black/transparent pixels,  
and sends them to the HTML canvas using the helper function `copyToCanvas()`.

### `copyToCanvas()`
Finds the target `<canvas>` element, sets its dimensions, and uses JS `putImageData()`  
to draw pixel data at coordinates (0, 0).

### `initializeImageLoad()` and `finalizeImageLoad()`
Fetch an external `.rgba` file, verify its integrity (`"RGBA"` header),  
convert endian order from network to host, and render it onto the canvas.

---

## 🧪 Technical Highlights
- Utilizes **C++17** and **Emscripten’s Embind API**
- Demonstrates **low-level data translation** between C++ and JavaScript
- Manages byte-order conflicts across architectures (big-endian vs little-endian)
- Runs fully offline with no external JS dependencies
- Demonstrates principles of **virtual machine interoperability**

---

## 📸 Result
Once compiled and served locally, the application displays a rendered Gator image on two canvases:
- One loaded from the static C++ header array
- One fetched dynamically from the `.rgba` file

---

## 🧾 Lessons Learned
This project deepened understanding of:
- WebAssembly memory management and byte-order alignment
- Minimal-glue integration with Emscripten
- The translation layer between compiled languages and browser execution

Despite several challenges — especially with file structure, inline scripting,  
and endian handling — the final system demonstrates a reliable and repeatable workflow  
for deploying native C++ logic in a web environment.

---

**Author:** [Alex Fluitt](https://github.com/alex-infosec)  
**Course:** Virtual Machine Bindings (University of Florida)  
**Date:** November 2025
