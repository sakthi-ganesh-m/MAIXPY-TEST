from Maix import GPIO
from Maix import I2S
from Maix import FFT
from fpioa_manager import fm
import image
import lcd

lcd.init()
fm.register(8,  fm.fpioa.GPIO0)
wifi_en=GPIO(GPIO.GPIO0,GPIO.OUT)
wifi_en.value(0)
fm.register(21,fm.fpioa.I2S0_IN_D0)
#fm.register(21,fm.fpioa.I2S0_IN_D1)
#fm.register(22,fm.fpioa.I2S0_IN_D2)
#fm.register(23,fm.fpioa.I2S0_IN_D3)
fm.register(19,fm.fpioa.I2S0_WS)
fm.register(18,fm.fpioa.I2S0_SCLK)
rx = I2S(I2S.DEVICE_0)
rx.channel_config(rx.CHANNEL_0, rx.RECEIVER, align_mode = I2S.STANDARD_MODE)
sample_rate = 38640
rx.set_sample_rate(sample_rate)
img = image.Image()
sample_points = 128
FFT_points = 64
lcd_width = 320
lcd_height = 240
hist_num = FFT_points #changeable
if hist_num > 320:
    hist_num = 320
hist_width = int(320 / hist_num)#changeable
x_shift = 0
while True:
    audio = rx.record(sample_points)
    print(audio)
    FFT_res = FFT.run(audio.to_bytes(),FFT_points)
    FFT_amp = FFT.amplitude(FFT_res)
    #print(FFT_amp)
    img = img.clear()
    x_shift = 0
    for i in range(hist_num):
        if FFT_amp[i] > 240:
            hist_height = 240
        else:
            hist_height = FFT_amp[i]
        img = img.draw_rectangle((x_shift,240-hist_height,hist_width,hist_height),[255,255,255],2,True)
        x_shift = x_shift + hist_width
    lcd.display(img)
