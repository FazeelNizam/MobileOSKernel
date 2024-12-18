# EEX5335 Mini Project - Mobile Operating System Kernal

This project contains a C program demonstrate the working of Mobile Operating System Kernal.

## Prerequisites

- GCC (GNU Compiler Collection)
  > I used VSCode Windows 10 x64 with Complete `C/C++ Extention Pack` inatalled and `mingw` installed. you have to installe `mingw` before compiling using `gcc` command. You can download it from this repository 
- Linux or Windows with WSL (Windows Subsystem for Linux)
- Basic understanding of command-line operations

## Getting Started

### Cloning the Repository

- Clone the repository to your local machine using Git:

```sh
git clone https://github.com/FazeelNizam/MobileOSKernel
```

### Compiling the Code

#### In WSL

- Open the Ubuntu terminal (on Windows with WSL) `Ubuntu 18.04 LTS` and open the home folder:

```sh
explorer.exe .
```

- Move the `mobile_os_kernel.c` file into your Linux directory or Navigate to the directory where you cloned the repository.

- Compile the program using GCC:

```sh
gcc mobile_os_kernel.c
```

#### In VSCode

- Open the VSCode and open the cloned folder:

- Compile the program using GCC:
  > View results in the terminal
    ```sh
    gcc mobile_os_kernel.c
    ```
  > Create a executable file
    ```sh
    gcc mobile_os_kernel.c -o mobile_os_kernel.exe
    ```


### Running the Program

- After compilation, you can run the program using bellow command

```sh
./a.out
```

- You should see output similar to the following or an executable file named `mobile_os_kernel.exe` in the same directory as the code:

```ruby
  Processes created:
  PID: 1, Name: NavigationApp, Priority: 8
  PID: 2, Name: CameraApp, Priority: 5
  PID: 3, Name: BackgroundTask, Priority: 2
  Sensor Type 0 - Simulated Data: 71 38 69 12 67 99 35 94 3 11  
  Sensor Type 4 - Simulated Data: 22 33 73 64 41 11 53 68 47 44 
  Sensor Type 2 - Simulated Data: 62 57 37 59 23 41 29 78 16 35 
  Simulating process scheduler...
  Running Process PID: 1, Name: NavigationApp, Priority: 8
  Running Process PID: 2, Name: CameraApp, Priority: 5
  Running Process PID: 3, Name: BackgroundTask, Priority: 2
  Current Power Mode: 0

  Switching to POWER_BATTERY_SAVE...
  Sensor Type 0 - New Sampling Rate: 25 Hz
  Sensor Type 4 - New Sampling Rate: 10 Hz
  Sensor Type 2 - New Sampling Rate: 0 Hz

  Switching to POWER_ULTRA_BATTERY_SAVE...
  Process Name: NavigationApp, Power State: 0
  Process Name: CameraApp, Power State: 0
  Process Name: BackgroundTask, Power State: 0
  ```

### Notes

- I have not tested the GUI version of this code in Linux environment or `gcc mobile_os_kernel.c -o mobile_os_kernel.exe` command create a executable file.

### Troubleshooting

- If you encounter a "Permission denied" error, ensure the compiled program has execute permissions:

  ```sh
  chmod +x mobile_os_kernel.c
  ```

- If the `gcc` compiler is not installed, you can install it using:

  ```sh
  sudo apt update
  sudo apt install gcc
  ```

### Contributing

Feel free to open issues or submit pull requests if you have any suggestions or improvements.

### Acknowledgments

- Based on examples from:
  > "Operating System Concepts" by Abraham Silberschatz, Peter B. Galvin, and Greg Gagne.
