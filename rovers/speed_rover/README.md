# Speed Rover

A rover built for a timed straight run. A servo shifts the drivetrain between three gear ratios during the run: low gear for traction off the line, then progressively faster ratios.

## How a run works

1. Power on. The servo parks in first gear.
2. Press the start button. The sketch waits 1 second, then the run begins.
3. The motor runs at full power. At set times the sketch eases the motor to about half power, moves the servo to the next gear, and returns to full power.
4. At 3 seconds the motor cuts and the sketch halts.

| Gear | Servo angle | Engages at |
|---|---|---|
| 1 | 20° | start of the run |
| 2 | 90° | 0.7 s |
| 3 | 160° | 1.5 s |

```
t = 0 s          0.7 s            1.5 s                3.0 s
|--- gear 1 ---|--- gear 2 -----|------ gear 3 ------|  stop
                ^ ease off, shift ^ ease off, shift
```

Easing off during each shift lets the gears mesh cleanly. The shift times in `SHIFT_AT_MS` are meant to be tuned from timed trials.

## Pins

| Pin | Function |
|---|---|
| D5 | Drive motor (PWM) |
| D9 | Gear-shift servo |
| D2 | Start button (to ground, internal pull-up) |

## Files

- [`speed_rover.ino`](speed_rover.ino)
