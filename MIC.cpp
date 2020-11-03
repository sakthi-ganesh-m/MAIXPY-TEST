#include <Arduino.h>
#include "Maix_Speech_Recognition.h"

uint16_t VcBuf[atap_len];
atap_tag    atap_arg;
valid_tag   valid_voice[max_vc_con];
v_ftr_tag   ftr;
v_ftr_tag   ftr_temp;
v_ftr_tag   ftr_mdl_temp[10];
v_ftr_tag   *pftr_mdl_temp[10];

#define save_ok     0
#define VAD_fail    1
#define MFCC_fail   2
#define Flash_fail  3

#define FFT_N 512

uint16_t rx_buf[FRAME_LEN];
uint32_t g_rx_dma_buf[FRAME_LEN * 2];
uint64_t fft_out_data[FFT_N / 2];

volatile uint32_t g_index;
volatile uint8_t uart_rec_flag;
volatile uint32_t receive_char;
volatile uint8_t i2s_rec_flag;
volatile uint8_t i2s_start_flag = 0;


int i2s_dma_irq(void *ctx)
{
    uint32_t i;
    if(i2s_start_flag)
    {
        int16_t s_tmp;
        if(g_index)
        {
            i2s_receive_data_dma(I2S_DEVICE_0, &g_rx_dma_buf[g_index], frame_mov * 2, DMAC_CHANNEL3);
            g_index = 0;
            for(i = 0; i < frame_mov; i++)
            {
                s_tmp = (int16_t)(g_rx_dma_buf[2 * i] & 0xffff); //g_rx_dma_buf[2 * i + 1] Low left
                rx_buf[i] = s_tmp + 32768;
            }
            i2s_rec_flag = 1;
        }
        else
        {
            i2s_receive_data_dma(I2S_DEVICE_0, &g_rx_dma_buf[0], frame_mov * 2, DMAC_CHANNEL3);
            g_index = frame_mov * 2;
            for(i = frame_mov; i < frame_mov * 2; i++)
            {
                s_tmp = (int16_t)(g_rx_dma_buf[2 * i] & 0xffff);//g_rx_dma_buf[2 * i + 1] Low left
                rx_buf[i] = s_tmp + 32768;
            }
            i2s_rec_flag = 2;
        }
    }
    else
    {
        i2s_receive_data_dma(I2S_DEVICE_0, &g_rx_dma_buf[0], frame_mov * 2, DMAC_CHANNEL3);
        g_index = frame_mov * 2;
    }
    return 0;
}

void setup()
{ 
    fpioa_set_function(20, FUNC_I2S0_IN_D0);
    fpioa_set_function(18, FUNC_I2S0_SCLK);
    fpioa_set_function(19, FUNC_I2S0_WS);

    //i2s init
    i2s_init(I2S_DEVICE_0, I2S_RECEIVER, 0x3);

    i2s_rx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_0,
            RESOLUTION_24_BIT, SCLK_CYCLES_32,
            TRIGGER_LEVEL_1, STANDARD_MODE);

    i2s_set_sample_rate(I2S_DEVICE_0, 8000);

    dmac_init();
    dmac_set_irq(DMAC_CHANNEL3, i2s_dma_irq, NULL, 3);
    i2s_receive_data_dma(I2S_DEVICE_0, &g_rx_dma_buf[0], frame_mov * 2, DMAC_CHANNEL3);
    
    Serial.begin(115200);
    Serial.println("start rec...");
/*
    if( rec.record(0, 0) == 0) //keyword_num, model_num 
    {    
      rec.print_model(0, 0);
    }
    else 
        Serial.println("rec failed");*/
}

void loop()
{
  i2s_receive_data_dma(I2S_DEVICE_0, &g_rx_dma_buf[0], frame_mov * 2, DMAC_CHANNEL3);
  for(int i = 0; i<FRAME_LEN;i++ )
   {
       Serial.print(g_rx_dma_buf[i]);
       Serial.print(" , ");
   }
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  delay(1000);
}
