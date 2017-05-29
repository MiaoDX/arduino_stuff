Some lesson learned and configuration stuff when programming the hardwares, such as `raspberry pi`, `arduino`, `steppers` and so on.

I really think that intelligent hardware will have splendid prospect in the coming future. So, a little work on these is worthy.

And one of the most crucial part is the `communication`, let me give you two example:


### Example#1.Communication Speed

**IF** the speed of wireless communication latency between image sensor (camera) and the receiver (calculation machine) can be reduced to the latency of wire connection, great chances are that we **do not have to write embedded programs at all**! 

We can always set up a PC or workstation and keep the robot with sensor not so far away from the PC, and receive sensor results send from the robot, let the PC do the necessary calculation, send back the command to the robot. In such scenario, we can really concentrate on the logical stuff when programming instead of pay so much attention to the limited resources the embedded hardware provided us and do lot of necessary low level trick. 

### Example#2.Communication Protocol

>SIMPLICITY is beauty.

We do not need to and should not create and handwrite communication protocol each and every time we want to communicate between hardwares, no matter what the hardware is, it's a PC, a raspberry pi, an arduino, no matter what communication module we use, it's a serial, a wifi, a Xbee, there should be some level of abstraction we can make use of.

Maybe it can be `RESTFUL` API, or some protocol framework. BUT, please, please, please do not handwrite all the the BYTES and do calculation on the BYTES for validation. -.-


Examples end up here.

## What's the *approximate* ideal methods of hardware programming

Well, I am sure *yet*! :)

But there are so many developments [here](https://github.com/fffaraz/awesome-cpp#robotics) and there.

However,

>Done is better than perfect.

when we do hardware stuff, keep in mind that we want to do better (or even best), but **make the ~~fucking~~ stuff run**[^stuff_run] is what we need to guarantee!

[^stuff_run]: It is a jock inside our lab, the original word is **RUN 起来** in Chinese, often said by us when we want to make a quick trial and error test.


## What's next

I will provide links if I really find out something interesting.

* [protocols.md](protocols.md)
* [reference_and_blogs.md](reference_and_blogs.md)


>Good luck & have fun.