# STM32F4xx "Black Pill" USB to I2S DAC Audio Bridge

## Features

* USB Full Speed Class 1 Audio device, no special drivers needed
* Isochronous with endpoint feedback (3bytes, 10.14 format) to synchronize sampling frequency Fs
* Bus powered
* Supports 24-bit audio streams with Fs = 44.1kHz, 48kHz or 96kHz
* USB Audio Volume (0dB to -96dB, 3dB steps) and Mute control 
* I2S master output with I2S Philips standard 24/32 data frame
* Uses inexpensive Aliexpress-sourced STM32F4xx (I saw several models on Taobao around 10+ RMB with 22x2 pins they all should work).
* Uses inexpensive ES9023 DAC bus or self powered (with power board).
* Optional power supply board and amplifier
* Build support (Makefile option) for STM32F401CCU6 and STM32F411CEU6 boards 
* Optional MCLK output generation on STM32F411

I now understand why there is a market for audiophile DACs with higher end headphones. I was given a pair of used Grado SR60 headphones a long time ago and
was unimpressed. With my laptop and smartphone headphone outputs they didn't sound great compared to my budget earbuds. In fact, they were lacking in bass response. And they are bulky, with a heavy cable. So they've been in a cupboard for the past 16-17 years.

I first noticed that my old iPhone SE standard wired earbuds sounded remarkably good with the USB-DAC, much better than my other budget earbuds and
my cheap but comfortable Sennheiser PX60 headphones.

I retrieved the Grado headphones and tried them out, the difference is astonishing.  I'm no golden-ears audiophile, but 
the amount of detail and frequency response is remarkable. I'm kind of surprised that the DACs on brand name (Lenovo & Vaio) laptops are
so poor in quality, but have a look at this [Cambridge Audio website](https://www.cambridgeaudio.com/row/en/blog/our-guide-usb-audio-why-should-i-use-it?fbclid=IwAR33SS0e_jNiQ1tBSOj29KdEOi1mhHn1r87bMg-VyAMmR2NeSmKETod-JkY#:~:text=Class%201%20will%20give%20you,step%20up%20to%20Class%202) comparing a dedicated USB Audio Class 1 DAC to a laptop headphone output. I have to agree with them.

I normally re-cycle my prototype modules for new projects, but I am now using this setup as a permanent headphone driver, and my Grado phones
are back in service.

When the USB Audio DAC device is enumerated on plug-in, it reports its capabilities (audio class, sampling frequency options, bit depth). If you configure the audio device driver optimally, a native 96kHZ 24bit audio file will play unmodified, while a 44.1kHz or 48kHz 16bit stream will be resized to 24bits and resampled to 96kHz. And yes, even 44.1kHz/16bit MP3 files sound much better when played back via the USB DAC. I'm not sure why - the PCM5102A isn't marketed as an "audiophile" component, but it obviously can drive high-quality headphones much better than standard laptop/smartphone DAC components.

## Credits
* [Dragonman USB Audio project](https://github.com/dragonman225/stm32f469-usbaudio)
* [Endpoint feedback](https://www.microchip.com/forums/m547546.aspx)

## Software Development Environment
* Ubuntu 20.04 AMDx64
* STM32CubeIDE v1.6.0
* STM32 F4 library v1.26.1
* Makefile project. Edit makefile flags to
  * Select STM32F411 or STM32F401
  * Enable MCLK output generation (STM32F411 only)
  * Enable diagnostic printout on serial UART port 

## Hardware

* WeAct STM32F411CEU6 or STM32F401CCU6 "Black Pill" development board
	* I2S_2 peripheral interface generates WS, BCK, SDO and optionally MCK
	* external R,G,B LEDs indicate sampling frequency
	* On-board LED for diagnostic status
	* UART2 serial interface for diagnostic information
* ES9023 I2S DAC module
	* asynchronized mode with 50MHz MCK
```
F4xx    ES9023      Description
--------------------------------------------------------------------
GND     GND
B13     BCK         I2S_BCK (Bit Clock)
B15     SDI         I2S_SDI (Data Input)
B12     LRCK        I2S_WS (LR Clock)
GND     DIF         Format = I2S
A8      MUTE_B      Mute control
A6      -           I2S_MCK (not used)


LED 7/8 bits for frequence display
4 = 44.1kHz
8 = 48kHz
9 = 96kHz
F4xx    LED pin
--------------------------------------------------------------------
B0
B1
B3
B4
B5
B6
B7

C13     on-board    Diagnostic
--------------------------------------------------------------------
A11                        USB_D-   USB data
A12                        USB_D+
A2                         TX       Serial debug
A3                         RX
GND                        GND
PA0                                 KEY button. Triggers endpoint  
                                    feedback printout if enabled with  
                                    DEBUG_FEEDBACK_ENDPOINT
--------------------------------------------------------------------
```    

<img src="docs/prototype.jpg" />

## Checking USB Audio device on Ubuntu 20.04

* Execute `lsusb` with and without the USB-Audio DAC plugged in, you should see the 
  new USB device
  
<img src="docs/lsusb.png" />
  
* Execute `aplay -L` and look for `PCM5102 DAC`

<img src="docs/aplay_output.png" />

* Run the `Sound` application without the USB-Audio DAC plugged in and check the
  Speaker/Headphone output options
* Plug in the USB-Audio DAC and check again, you should see at least one new option.
  Select this for playing sound output via the USB-Audio DAC
* Execute `cat /proc/asound/DAC/stream0` when a song is playing

<img src="docs/stream.png" />

## Optimizing Pulseaudio on Ubuntu 20.04 for USB-Audio DAC

* Edit `/etc/pulse/daemon.conf` as root
* Force re-sampling to 96kHz
* Resize to 24bits
* Use highest quality re-sampling algorithm
* Save file, log out and log in again for the changes to take effect

<img src="docs/pulseaudio_config.png" />

## Optimizing Windows 10 for USB-Audio DAC

* Use the Control Panel Sound playback device properties dialog

<img src="docs/win10_96kHz_24bit.png" />


## Endpoint Feedback mechanism

<img src="docs/feedback_endpoint_spec.png" />

Unfortunately, we do not have any means of measuring the actual Fs (accurate to 10.14 resolution)
generated by the PLLI2S peripheral on an SOF resolution interval of 1mS. So we calculate
a nominal Fs value by assuming the HSE crystal has 0ppm accuracy (no error), and use the PLLI2S N,R,
I2SDIV and ODD register values to compute the generated Fs value. For example when MCLK output
is disabled, the optimal register settings result in a value of 96.0144kHz.

<img src="docs/i2s_pll_settings.png" />

Since the USB host is asynchronous to the PLLI2S Fs clock generator, the incoming Fs rate of audio packets will be slightly different. We use
a circular buffer of audio packets to accommodate the difference in incoming and outgoing Fs. 

The USB driver writes incoming audio packets to this buffer while the I2S transmit DMA reads from this buffer. We start I2S playback when the buffer is half full, and then try to maintain this position, i.e. the difference between the write pointer and read pointer should optimally be half of the buffer size.

Any change to this pointer distance implies the USB host and I2S playback Fs values are not in sync.
To correct this, we implement a PID style feedback mechanism where we report an ideal Fs feedback frequency
based on the deviation from the nominal pointer distance. We want to avoid the write process overwriting the unread packets, and we also want to minimize the oscillation in Fs due to unnecessarily large corrections.

This is a debug log of changes in Fs due to the implemented mechanism. The first datum is the SOF frame counter, the second is the pointer distance in samples, the third is the feedback Fs. As you can see, the feedback is able to minimize changes in pointer distance AND oscillations in Fs frequency.

<img src="docs/endpoint_feedback.png" />





