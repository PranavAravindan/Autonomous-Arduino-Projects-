# Robotics Projects

A Bluetooth-controlled blimp (with a first autonomous mode) and three purpose-built rovers, all driven by small Arduino sketches. Each project has its own folder with the code, a wiring/pin table, and a diagram of how it behaves.

| Project | What it does | Code |
|---|---|---|
| [**Blimp**](blimp/) | 3-motor Bluetooth RC blimp with a simple autonomous patrol mode | [`blimp.ino`](blimp/blimp.ino) |
| [**Speed rover**](rovers/speed_rover/) | Servo-shifted multi-gear drivetrain built for a timed straight run | [`speed_rover.ino`](rovers/speed_rover/speed_rover.ino) |
| [**Color rover**](rovers/color_rover/) | Classifies the ground color and picks a behavior for each color | [`color_rover.ino`](rovers/color_rover/color_rover.ino) |
| [**Terrain rover**](rovers/terrain_rover/) | Avoids obstacles and lowers a soil-moisture probe on an arm | [`terrain_rover.ino`](rovers/terrain_rover/terrain_rover.ino) |

## Repository layout

```
.
├── blimp/
│   ├── blimp.ino
│   ├── docs/chassis-cad.png
│   └── README.md
└── rovers/
    ├── README.md
    ├── speed_rover/    (.ino + README)
    ├── color_rover/    (.ino + README)
    └── terrain_rover/  (.ino + README)
```

## Building and uploading

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) (or `arduino-cli`).
2. Open the `.ino` file of the project you want. Each sketch sits in a folder of the same name, which the IDE requires.
3. Install any library listed below, pick your board and port, and upload.

| Library | Used by | How to get it |
|---|---|---|
| `SoftwareSerial` | Blimp | Bundled with the Arduino IDE |
| `Servo` | Speed rover, Terrain rover | Bundled with the Arduino IDE |
| `Adafruit TCS34725` | Color rover | Library Manager (also pulls in Adafruit BusIO) |

## Safety

These are hobby builds with exposed motors and propellers. Test with the vehicle held or restrained before the first free run, and keep a hand on the power switch.
