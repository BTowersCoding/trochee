import board
from kmk.keys import KC
from kmk.kmk_keyboard import KMKKeyboard
from kmk.matrix import DiodeOrientation
from kmk.hid import HIDModes

envkb = KMKKeyboard()
envkb.col_pins = (board.GP18, board.GP19, board.GP20, board.GP21, board.GP22, board.GP26, board.GP27, board.GP28)
envkb.row_pins = (board.GP2, board.GP3, board.GP4, board.GP5, board.GP6, board.GP7, board.GP8, board.GP9)
envkb.diode_orientation = DiodeOrientation.COLUMNS
envkb.debug_enabled = False
FN = KC.MO(1)

envkb.keymap = [
    [
        KC.ESC, KC.N1, KC.N2, KC.N3, FN, KC.LCTL, KC.LALT, KC.DEL,
        KC.GRAVE, KC.N4, KC.N5, KC.N6, KC.LBRC, KC.RBRC, KC.MINUS, KC.EQUAL,
        KC.TAB, KC.N7, KC.N8, KC.N9, KC.N0, KC.SLSH, KC.BSLASH, KC.QUOT,
        KC.Q, KC.J, KC.L, KC.M, KC.F, KC.Y, KC.P, KC.SCLN,
        KC.Z, KC.DOT, KC.O, KC.R, KC.S, KC.U, KC.C, KC.B,
        KC.X, KC.A, KC.E, KC.H, KC.T, KC.D, KC.G, KC.K,
        KC.LSFT, KC.COMM, KC.I, KC.N, KC.W, KC.V, KC.UP, KC.RSHIFT,
        KC.BSPC, KC.SPC, KC.SPC, KC.TAB, KC.ENT, KC.LEFT, KC.DOWN, KC.RIGHT,
    ],
    [
        KC.ESC, KC.F1, KC.F2, KC.F3, FN, KC.LCTL, KC.LALT, KC.DEL,
        KC.GRAVE, KC.F4, KC.F5, KC.F6, KC.LBRC, KC.RBRC, KC.MINUS, KC.EQUAL,
        KC.TAB, KC.F7, KC.F8, KC.F9, KC.F10, KC.F11, KC.F12, KC.QUOT,
        KC.Q, KC.J, KC.L, KC.M, KC.F, KC.Y, KC.P, KC.SCLN,
        KC.Z, KC.DOT, KC.O, KC.R, KC.S, KC.U, KC.C, KC.B,
        KC.X, KC.A, KC.E, KC.H, KC.T, KC.D, KC.G, KC.K,
        KC.LSFT, KC.COMM, KC.I, KC.N, KC.W, KC.V, KC.UP, KC.RSHIFT,
        KC.BSPC, KC.SPC, KC.SPC, KC.TAB, KC.ENT, KC.LEFT, KC.DOWN, KC.RIGHT,
    ],
]

# Enable LED on pi once this script is executed
import board
import digitalio
led = digitalio.DigitalInOut(board.GP25)
led.direction = digitalio.Direction.OUTPUT
led.value = True
#At this point once the LED is enabled the keyboard should be usable
#This might be a problem if you are trying to get into the bios of your PC
if __name__ == '__main__':
    envkb.go(hid_type=HIDModes.USB) #Wired USB enables
