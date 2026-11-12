# Corneroe

ZMK config for my Corne-style 3x6 wireless split with an encoder and a 128x32 OLED on each half.

- **Controller:** nice!nano v2 (left half is central)
- **Firmware:** ZMK v0.3 + [zmk-nice-oled](https://github.com/mctechnology17/zmk-nice-oled)
- **Keys:** 44 — 3x6 grid, 3 thumbs per side, plus an encoder push button per side
- **Encoders:** left = volume, right = smooth scroll (on every layer)
- **ZMK Studio:** enabled on the left half over USB (unlocked)

## Layout

`▽` = transparent (falls through to the layer below). The bottom line of each grid is the encoder push buttons.

### Base

```
Esc  Q  W  E  R  T     │  Y      U      I  O  P  Bksp
Tab  A  S  D  F  G     │  H      J      K  L  ;  '
Alt  Z  X  C  V  B     │  N      M      ,  .  /  Enter
       Win Lower Shift │  Space  Raise  Ctrl
              [Mute]   │  [Middle click]
```

### Lower (hold left middle thumb)

```
Esc  1      2     3     4     5       │ 6      7     8   9     0     Bksp
Tab  BT0    BT1   BT2   BT3   BT4     │ Home   ←     ↑   →     End   Enter
Alt  BTclr  Prev  Play  Next  USB/BT  │ Vol-   Vol+  ↓   PgUp  PgDn  Game on/off
                  Win   ▽     Shift   │ Space  ▽     Ctrl
```

### Raise (hold right middle thumb)

```
Tab   !  @  #  $  %    │ ^     &       *       (       )    Bksp
Ctrl  -  =  [  ]  \    │ Del   Mouse←  Mouse↑  Mouse→  MB5  Scroll↑
Lock  `  +  _  {  }    │ |     LClick  Mouse↓  RClick  MB4  Scroll↓
         Win  ▽  Shift │ Ctrl  ▽       RAlt
```

Lock = Win+L.

### Adjust (hold Lower + Raise)

```
F1    F2     F3   F4  F5  F6  │ F7  F8  F9  F10  F11    F12
Caps  PrtSc  Ins  ▽   ▽   ▽   │ ▽   ▽   ▽   ▽    ▽      ▽
Boot  Reset  ▽    ▽   ▽   ▽   │ ▽   ▽   ▽   ▽    Reset  Boot
```

Boot/Reset act on the half they're pressed on: use the left pair for the left half, the right pair for the right.

### Game (toggle with Lower + bottom-right key)

Only the left half changes; the right half stays on Base so chat still works.

```
Tab    Q  W  E  R  T
Shift  A  S  D  F  G
Ctrl   Z  X  C  V  B
     Esc GameFn Space
```

### Game Fn (hold left middle thumb in Game mode)

```
`    1   2   3   4   5    │ ▽ ▽ ▽ ▽ ▽ Game off
Alt  6   7   8   9   0    │ ▽ ▽ ▽ ▽ ▽ ▽
▽    F1  F2  F3  F4  F5   │ ▽ ▽ ▽ ▽ ▽ Game off
```

## Building and flashing

Every push builds firmware via GitHub Actions. Download the `firmware` artifact from the latest run:

| File | Flash to |
|---|---|
| `corneroe_left_with_studio` | left half |
| `corneroe_right` | right half |
| `settings_reset` | either half, to clear pairings / Studio changes |

To flash: press Boot on the Adjust layer (or double-tap the reset button), then drag the `.uf2` onto the `NICENANO` drive.

Keymap changes saved in ZMK Studio override `config/corneroe.keymap`. If an edit to the file doesn't show up after flashing, flash `settings_reset` to both halves and re-pair.

## Files

- `config/corneroe.keymap` — layers, encoders, behaviors
- `config/corneroe.conf` — display, power, Bluetooth, pointing settings
- `build.yaml` — which firmware images CI builds
- `boards/shields/corneroe/` — shield definition (matrix pins, encoders, OLED, physical layout)
