# void loop()
1) In your if, else/if block where you select your icons you need to make every statement an `if` statement. Remove all the `else if`. This will allow all your icons to display.

2) Also this line is incorrect. Look at the others and try to fix it. `lcd.createChar(2, snowChar);`

# void setup()
3) `getTemperature()` is never called on first boot — tempF starts at 0 and displays 0°F until the timer fires. Call `getTemperature()` at the end of `setup()`.

4) `moonChar` is defined in `icons.h` but never registered or used — `lcd.createChar` is never called for it, and it's not referenced in `loop()`. Either add it or remove it.

# Global Constants
5) `api_url` is a String but only used as `.c_str()` — declare it as `const char*` to avoid a heap allocation.

# Something extra!
6) `printCenter()` doesn't guard against text longer than 16 chars — long weather descriptions will overflow the display. Truncate to `lcdColumns` before printing. Or you can create a way to have longer text scroll for that lcd line.