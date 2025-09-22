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

*   [**Visual Studio 2022**](https://visualstudio.microsoft.com/downloads/): Make sure to include the **"Desktop development with C++"** workload during installation.
*   [**CMake**](https://cmake.org/download/): Ensure it is added to your system's PATH.

---

### Method 1: Visual Studio Code (Recommended)

This project is pre-configured for easy compilation and debugging within Visual Studio Code.

1.  Open the `DBSrv` folder in VS Code.
2.  Place a breakpoint anywhere in the code.
3.  Press **F5**.

VS Code will automatically run the build task (`build.bat`) and then launch the debugger.

---

### Method 2: Using the Batch Script

A simple batch file is provided to compile the project directly.

Open a terminal (like Command Prompt or PowerShell) and run:

```cmd
.\build.bat
```

This will use CMake to generate the build files and compile the project in the `build/` directory.

---

### Method 3: Manual Compilation with CMake

You can also generate the project files manually and compile them.

1.  Open a terminal (like the Developer Command Prompt for VS 2022).
2.  Navigate to the `DBSrv` directory.
3.  Run the following commands:

    ```sh
    # Create a build directory
    mkdir build
    cd build

    # Generate the Visual Studio solution
    cmake .. -G "Visual Studio 17 2022" -A x64

    # Build the project
    cmake --build . --config Debug
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
