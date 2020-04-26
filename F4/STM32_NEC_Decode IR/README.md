# STM32_NEC_Decode
A library for decoding NEC IR protocol with STM32

Instructions:
=============
- configure HW
- initialize the library(provide the necessary information)

Configuring HW
==============
The library requires a specific HW setup
Requirements
- GPIO pin that is a timer input capture channel
- that timer can generate DMA requests
- that timer can measure up to 20ms
- that timer can be configured in ResetSlave mode to reset on every falling edge of the capture input

Initialize the library
======================
To start receiving data the library needs the following:
- timerHandle of the timer being used for the time measurements
- timerChannel and timerChannelActive of the timer channel being used for the input capture
- type of the NEC protocol (either extended or not)
- timingBitBoundary is the boundary value of the timer value for individual bits (if timer value is lower bit=0, if greater bit=1)
- timingAgcBoundary is the boundary value of the timer value for the AGC pulse (if timer value is lower the frame is a RepeatFrame, if greater frame is a DataFrame)
- 3 callbacks to handle decoded/error/repeat events

And then you can just call NEC_Read() to start listening for a packet
