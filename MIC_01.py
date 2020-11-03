from Maix import GPIO
from Maix import I2S
from Maix import FFT
from fpioa_manager import fm

sample_rate = 38640
sample_points = 1024
FFT_points = 512

fm.register(8,  fm.fpioa.GPIO0)
wifi_en=GPIO(GPIO.GPIO0,GPIO.OUT)
wifi_en.value(0)

fm.register(23,fm.fpioa.I2S0_IN_D0)
fm.register(22,fm.fpioa.I2S0_IN_D1)
fm.register(21,fm.fpioa.I2S0_IN_D2)
fm.register(20,fm.fpioa.I2S0_IN_D3)
fm.register(19,fm.fpioa.I2S0_WS)
fm.register(18,fm.fpioa.I2S0_SCLK)

rx = I2S(I2S.DEVICE_0)
rx.channel_config(rx.CHANNEL_2, rx.RECEIVER, align_mode = I2S.STANDARD_MODE)
rx.set_sample_rate(sample_rate)


while 1:
    audio = rx.record(sample_points)
    print(audio)
    audio_data = audio.to_bytes ()
    FFT_res = FFT.run(audio.to_bytes(),FFT_points)
    FFT_amp = FFT.amplitude(FFT_res)
    print(FFT_amp)
    #print(audio_data)
    print("\n")
