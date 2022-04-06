const five = require("johnny-five");

const board = new five.Board();

board.on("ready", () => {
  const lcd = new five.LCD({
    pins: [7, 6, 5, 4, 3, 2],
    // backlight: 6,
    rows: 2,
    cols: 16,
  });

  lcd.clear().print("rmurphey, hgstrp");
  lcd.cursor(1, 0);

//   lcd.autoscroll();
//   lcd.print('Hi there');

//   lcd.cursor(1, 7);

//   lcd.blink().print("Bleep Bloop");

lcd.autoscroll().print("Bloop").print("Bleeeeeeeeeep");

lcd.clear();

lcd.cursor(0, 0).print("10".repeat(8));

//   lcd.blink();

//   lcd.blink();
//   lcd.clear()

//   this.repl.inject({
//     lcd,
//   });
});
