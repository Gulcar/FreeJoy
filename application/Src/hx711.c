#include "hx711.h"

int8_t next_requested_source = 0;
GPIO_TypeDef* dt_port = NULL;
uint16_t dt_pin = 0;
GPIO_TypeDef* sck_port = NULL;
uint16_t sck_pin = 0;

void HX711_SetDTPin(GPIO_TypeDef* port, uint16_t pin)
{
    dt_port = port;
    dt_pin = pin;
    next_requested_source = 0;
}

void HX711_SetSCKPin(GPIO_TypeDef* port, uint16_t pin)
{
    sck_port = port;
    sck_pin = pin;
    next_requested_source = 0;

    GPIO_WriteBit(sck_port, sck_pin, Bit_RESET);
}

bool HX711_DataReady(int8_t source)
{
    return (GPIO_ReadInputDataBit(dt_port, dt_pin) == 0) &&
        (source == next_requested_source || next_requested_source == 0);
}

int32_t HX711_ReadValue32(int8_t next_source)
{
    int32_t value = 0;
    for (int i = 0; i < 24; i++)
    {
        GPIO_WriteBit(sck_port, sck_pin, Bit_SET);
        Delay_us(1);

        int32_t bit = GPIO_ReadInputDataBit(dt_port, dt_pin);
        value = (value << 1) | bit;

        GPIO_WriteBit(sck_port, sck_pin, Bit_RESET);
        Delay_us(1);
    }

    // sign extension (ker je predznak trenutno na bitu 23)
    // chatgpt-ja mi ni uspelo prepricat da tega ne rabim ker na koncu uporabim samo 16 bitov
    // tako da me je dovolj dolgo prepriceval da matematika vmes ni prov zato bom zdaj dodal sign extension
    // tako da dela tudi ce uporabljas 32 bitne vrednosti
    if (value & (1 << 23))
    {
        value |= 0xFF000000;
    }

    int num_pulses = 25;
    switch (next_source)
    {
    case SOURCE_HX711_A_128: num_pulses = 25; break;
    case SOURCE_HX711_B_32:  num_pulses = 26; break;
    case SOURCE_HX711_A_64:  num_pulses = 27; break;
    }

    for (int i = 24; i < num_pulses; i++)
    {
        GPIO_WriteBit(sck_port, sck_pin, Bit_SET);
        Delay_us(1);
        GPIO_WriteBit(sck_port, sck_pin, Bit_RESET);
        Delay_us(1);
    }

    next_requested_source = next_source;

    return value;
}

analog_data_t HX711_ReadValue(int8_t next_source)
{
    int32_t value = HX711_ReadValue32(next_source);
    return (int16_t)(value >> 8);
}
