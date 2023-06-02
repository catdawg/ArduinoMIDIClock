# ArduinoMIDIClock
A simple Arduino sketch that outputs MIDI Start/Stop and Clock messages, with a BPM pot.
This is meant to be used with a SparkFun MIDI shield. https://www.sparkfun.com/products/12898

I have a guitar looper pedal, RC-500, that I would like to control through MIDI from another device, Novation Circuit Tracks. However, when the Circuit generates the clock, the RC-500's recordings playback with artifacts, as if it was being slowed down very slightly..
I created this project so I could, first of all, learn Arduino, but with the objective of debugging this issue in the looper. Essentially, to see if the problem was actually the other device clock messages being unreliable, or if it was the looper problem.

Turns out the looper works fine with this simple Arduino clock, so mission successful :D

I'm completely new to Arduino and Electronics in general, so I welcome any feedback!
Thanks!
