# DBSrv - WYD Server

This repository contains the `DBSrv` (Database Server) component for the C++ WYD Server project.

## ⚠️ Disclaimer

This project is based on a decompilation of a previously available server release and is intended for educational and study purposes only. All copyrights and intellectual property rights for the game "With Your Destiny" belong to Hanbitsoft. We do not claim ownership of the original work.

---

## About This Service

The `DBSrv` is a critical part of the server's distributed architecture. It acts as the central authority for all data persistence.

*   **Responsibilities:** Handles saving and loading all persistent data, including accounts, characters, items, skills, and guild information.
*   **Architecture:** It uses a file-based database system, where each account and its characters are stored in individual binary files.
*   **Communication:** It listens for connections from game servers (`TMSrv`) and processes their requests to fetch or update data. It also provides a separate interface for administrative connections.

---

## Compilation

### Prerequisites

Before you begin, ensure you have the following installed:

*   **A C++ compiler:**
    *   **Windows:** [Visual Studio](https://visualstudio.microsoft.com/downloads/) with the "Desktop development with C++" workload.
    *   **Linux:** A modern C++ compiler like GCC or Clang (`sudo apt-get install build-essential`).
*   [**CMake**](https://cmake.org/download/): Ensure it is added to your system's PATH.

### Build Instructions (Recommended)

This project includes convenient scripts to automate the build process on both Windows and Linux.

1.  **Open a terminal** in the project's root directory (`DBSrv`).

2.  **Run the build script for your system:**
    *   **On Windows:**
        ```cmd
        .\build.bat
        ```
    *   **On Linux:**
        ```sh
        ./build.sh
        ```

The compiled executable, `DBSrv` (or `DBSrv.exe` on Windows), will be created in the `build` directory.

The scripts support the following commands:
*   `build`: (Default) Configures and compiles the project.
*   `rebuild`: Cleans the build directory and then compiles the project from scratch.
*   `clean`: Deletes the build directory.

For example, to rebuild the project on Linux, you would run: `./build.sh rebuild`.

### Manual Compilation

If you prefer to build manually, you can use CMake directly:

1.  **Create a build directory:**
    ```sh
    mkdir build
    cd build
    ```

2.  **Configure the project:**
    ```sh
    cmake ..
    ```

3.  **Compile the project:**
    ```sh
    cmake --build .
    ```

---

## Documentation

Detailed technical documentation about the server's architecture, execution flow, and data processing can be found in the [`./doc`](./doc) directory. The main summary is available at [`doc/sumario.md`](./doc/sumario.md).

---

## Contributing

Pull requests are welcome! To maintain consistency, please try to follow the existing coding patterns and style.

## Contributors

*   **Project modernization**
    *   Amatsukan
*   **Original Decompilation & Development:**
    *   Klafke
    *   TheHouse
## License

This source code is released under the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.html).
