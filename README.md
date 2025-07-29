# LeviosaSQL

**LeviosaSQL** is a modern Qt 6 application for managing **SQLite databases remotely via SSH**. It leverages [`libssh`](https://www.libssh.org/) for secure connections and uses the `sqlite3` binary on the target machine to execute queries. Designed for speed and ease of use, LeviosaSQL brings powerful remote database management with an intuitive, multi-tab SQL editor interface.

---

## ✨ Features

- 🔐 Secure **SSH-based access** to remote SQLite databases (`libssh`)
- ⚙️ Executes SQL commands using `sqlite3` **on the remote machine**
- 🧠 Syntax-aware SQL editor with autocompletion and keyword hints
- 🗂️ Session manager for saving and organizing remote connections
- 🪄 Multi-tab query editing with real-time syntax highlighting
- ⚡ Lightweight and performant, with a modern Qt 6 GUI

---

## 🧰 Requirements

- **Qt 6.5+**
- **CMake ≥ 3.19**
- **C++17-compatible compiler**
- **libssh development library**
- Remote machine with `sqlite3` installed and accessible via SSH

---

## 📦 Installing Dependencies

### Debian / Ubuntu

```bash
sudo apt install qt6-base-dev cmake libssh-dev pkg-config build-essential
```

### Arch Linux

```bash
sudo pacman -S qt6-base cmake libssh pkgconf
```

---

## 🛠️ Build Instructions

### 1. Clone the repository

```bash
git clone https://github.com/wellingtonctm/leviosasql.git
cd leviosasql
```

### 2. Configure and build using CMake

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 3. Install (optional)

```bash
sudo make install
```

### 4. Run the application

```bash
./LeviosaSQL
```

---

## 📂 Project Structure

```
LeviosaSQL/
├── build/                     # CMake build directory
├── CMakeLists.txt             # Build configuration
├── *.cpp / *.h                # Source and header files
├── resources/
│   ├── sqlite_commands.txt    # Autocomplete keywords
│   ├── sqlite_functions.txt
│   └── sqlite_keywords.txt
├── resources.qrc              # Qt resource manifest
```

### Key Modules

- `sshcore.*`, `sshworker.*` – SSH connection and command execution (via libssh)
- `sessionmanagercore.*` – Core logic for remote session handling
- `multitabcodeeditor.*` – Tabbed SQL editing
- `databaseloader.*` – Loader for the database schema

---

## 📝 License

This project is licensed under the **MIT License**. See [LICENSE](LICENSE) for details.

---

## 🤝 Contributing

Found a bug? Have a feature idea?  
Feel free to open an issue or submit a pull request.
