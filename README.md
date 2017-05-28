A wav player using NUCLEO-F103RB, SD card, motor driver and a speaker element.

SD card is read using SPI2 and FATFS module (included in cubeMx). A SD card module with pullup on all data lines was used.

Motor driver used was L9110s from ebay or similar. CH1 and CH1N (complementary output) was connected to IA and IB inputs of motor driver. The motor driver had to be driven at 3.3V to avoid sound loss when driving a 4 ohm 3 Watt speaker, may be due to too much current/temperature. Might try to use a different motor driver with higher current capabilities like the L298N.

Wav files are encoded with 1 channel, 32000 samples per second and 16 bit signed values. No other encoding is supported at this moment.

Check cubeMX file for pin connections. This page is useful for the pinouts of the headers: https://developer.mbed.org/platforms/ST-Nucleo-F103RB/