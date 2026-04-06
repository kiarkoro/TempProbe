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

# New Stuff
1) I don't think you are using `#include <WiFiClientSecure.h>` you can probably remove it.

2) Rename your `timestamp` variable to something else. Timestamp has a very specific meaning and is a bit misleading. Maybe something like `dayTime` or `day_night`. 

3) You could make your variables `lcdColumns` and `lcdRows` constant by using the `const` keyword. They never change.

4) `getTemperatue()` function now does way more than just get temp. You should probably rename it to something more descriptive. Maybe `getWeather()` or something like that.