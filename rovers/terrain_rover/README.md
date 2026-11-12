# Terrain Rover

A rover that roams on its own, avoids obstacles with an ultrasonic sensor, and periodically lowers a soil-moisture probe on a rack-and-pinion arm to take a reading.

## Behavior

```mermaid
flowchart TD
    S[Loop] --> T{8 s since last<br/>soil test?}
    T -- yes --> P[Stop, lower probe, read moisture,<br/>print soil_raw, raise probe]
    T -- no --> O
    P --> O{Obstacle closer<br/>than 20 cm?}
    O -- yes --> R[Back up 0.4 s, then spin<br/>a random 0.3 to 0.7 s]
    O -- no --> F[Drive forward]
    R --> S
    F --> S
```

- **Obstacle avoidance:** if something is within 20 cm, the rover reverses briefly and then turns by a random amount to pick a new heading.
- **Soil sampling:** every 8 seconds the rover stops, the servo drives the rack-and-pinion arm down, the moisture sensor is read, and the arm goes back up. Readings print on the serial port as `soil_raw=<value>` at 9600 baud.
- **Stream-bed detection:** this ran on a separate camera-based system and is not part of this sketch. Here it is replaced by the timer trigger, which can be swapped for a real detector.

## Hardware and pins

| Part | Pin |
|---|---|
| Ultrasonic sensor TRIG / ECHO | D7 / D8 |
| Left motor forward / reverse | D5 / D6 |
| Right motor forward / reverse | D9 / D10 |
| Probe arm servo | D11 |
| Soil moisture sensor | A1 |

Requires only the bundled `Servo` library.

## Files

- [`terrain_rover.ino`](terrain_rover.ino)
