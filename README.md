Lesson learned about Arduino

## Environment

The official Arduino IDE is not so good for developers, luckily, there are pretty nice tools out there -- [visualmicro](http://www.visualmicro.com/), see the document for more info, it's pretty nice.

### Tips about visualmicro

Use `{@ReportDigital}` action when debug to give a more easy way to show the status of digital pins.

## Boards

### RAMPS 1.4

* The enable pin means `enable` when `LOW`.

## Lesson learned

### `long` values

>If doing math with integers, at least one of the numbers must be followed by an L, forcing it to be a `long`.
-- [official document about long](https://www.arduino.cc/en/Reference/Long)

So, unlike ordinary C/Cpp, to declare a long type value, one can just type:

`long l = 1000000000;`

in arduino, we must append `L` after the value, like `long l = 1000000000L;`, this is really annoying. So if we want to do a multiply operation, we should do something like:

`long testLong = 2000 * long(32);`

I really think it is a bug, even the document declared it. 