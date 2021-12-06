# Structure of a PlatformIO project (like this repo)

* `.vscode/` contains settings read by your VSCode to be compatible with the PlatformIO project
* `include/` contains the header files
* `lib/`contains private libraries (not used for this project)
* `src/` contains `main.cpp`
* `test/` contains PlatformIO project tests (not used for this project)
* `platformio.ini` is a setting file for PlatformIO building

# Cables labeling

## Pin ordering for pincher's PCB

For any `servo_i`:
* *Brown* goes to left (stands for ground)
* *Orange* goes to center (stands for signal)
* *Red* goes right (stands for voltage input)

If you want to use the force sensor: plug into `FF1`

## Pin ordering for UART communication between to ESP32

By default the *Green* wire of the ethernet cable goes to `GND` on the two ESP32 (chose the one between ports `19` and `22` for convinience). For communication on a first ESP32, assign *White* and *Orange* wires to `16` and `17`. For reception on a second ESP32, flip those two last wires.
* Brown goes to left (stands for ground)
* Orange goes to center (stands for signal)
* Red goes right (stands for voltage input)

If you want to use the force sensor: plug into `FF1`

# To have both ESP32 communicating
The sender is pluged on power outlet and flashed by pushing the ENG button (this program is flashed, it will remain permanent after upload)
The reciever is pluged to the computer on microUSB. Monitoring will be assigned on this platform. 
