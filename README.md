# STM32_HAL_VS1053

VS1053 library for STM32 HAL Driver<br>
1. LCSoft VS1053 module project.[STM32F4_HAL_SPI_VS1053]<br>
   - Only MP3 support.<br>
   - Add GPIO configuration to start as MP3 mode. (https://www.stm32duino.com/viewtopic.php?t=3989)
   - 3.0x CLKI = 36MHz clock operation.<br>
   - 36MHz / 7 = Max 5.1MHz SPI speed.
   
      
2. Adafruit music maker project.[STM32F4_HAL_ADAFRUIT_MUSICMAKER]<br>
   - MP3 + FLAC supporting.<br>
   - Flac feature was tested with only 3 flac files, so I won't be able to guarantee its sound quality.<br>
   - (5.0 + 2.0)x CLKI = 84MHz clock operation.<br>
   - 84 / 7 = Max 12MHz SPI speed.   
      
Tutorial : <br>
https://blog.naver.com/eziya76/221584296747



References :<br>
https://www.sparkfun.com/datasheets/Components/SMD/vs1053.pdf
http://www.toughdev.com/content/2013/10/interfacing-vs1053-audio-encoderdecoder-module-with-pic-using-spi/
https://github.com/adafruit/Adafruit_VS1053_Library
https://github.com/MikroElektronika/MP3_click/
https://www.stm32duino.com/viewtopic.php?t=3989

Video : <br>
[mp3] https://youtu.be/eXfEtPJAg7A<br>
[control] https://youtu.be/T6InNaj4nKY
[flac]
