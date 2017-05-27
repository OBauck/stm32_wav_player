/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "fatfs.h"

/* USER CODE BEGIN Includes */

#include <string.h>

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim1_ch1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

#define SIN_LOOKUP_TABLE_SIZE 360
	
const float sin_lookup_table[SIN_LOOKUP_TABLE_SIZE] = {
	0.0,0.01745240643728351,0.03489949670250097,0.05233595624294383,0.0697564737441253,0.08715574274765817,0.10452846326765346,0.12186934340514748,0.13917310096006544,0.15643446504023087,
	0.17364817766693033,0.1908089953765448,0.20791169081775931,0.224951054343865,0.24192189559966773,0.25881904510252074,0.27563735581699916,0.29237170472273677,0.3090169943749474,0.32556815445715664,
	0.3420201433256687,0.35836794954530027,0.374606593415912,0.3907311284892737,0.40673664307580015,0.42261826174069944,0.4383711467890774,0.45399049973954675,0.4694715627858908,0.48480962024633706,
	0.49999999999999994,0.5150380749100542,0.5299192642332049,0.5446390350150271,0.5591929034707469,0.573576436351046,0.5877852522924731,0.6018150231520483,0.6156614753256582,0.6293203910498374,
	0.6427876096865393,0.6560590289905072,0.6691306063588582,0.6819983600624985,0.6946583704589973,0.7071067811865475,0.7193398003386511,0.7313537016191705,0.7431448254773941,0.754709580222772,
	0.766044443118978,0.7771459614569708,0.788010753606722,0.7986355100472928,0.8090169943749475,0.8191520442889918,0.8290375725550417,0.8386705679454239,0.848048096156426,0.8571673007021122,
	0.8660254037844386,0.8746197071393957,0.8829475928589269,0.8910065241883678,0.898794046299167,0.9063077870366499,0.9135454576426009,0.9205048534524403,0.9271838545667874,0.9335804264972017,
	0.9396926207859083,0.9455185755993167,0.9510565162951535,0.9563047559630354,0.9612616959383189,0.9659258262890683,0.9702957262759965,0.9743700647852352,0.9781476007338056,0.981627183447664,
	0.984807753012208,0.9876883405951378,0.9902680687415703,0.992546151641322,0.9945218953682733,0.9961946980917455,0.9975640502598242,0.9986295347545738,0.9993908270190958,0.9998476951563913,
	1.0,0.9998476951563913,0.9993908270190958,0.9986295347545738,0.9975640502598242,0.9961946980917455,0.9945218953682734,0.9925461516413221,0.9902680687415704,0.9876883405951377,
	0.984807753012208,0.981627183447664,0.9781476007338057,0.9743700647852352,0.9702957262759965,0.9659258262890683,0.9612616959383189,0.9563047559630355,0.9510565162951536,0.9455185755993168,
	0.9396926207859084,0.9335804264972017,0.9271838545667874,0.9205048534524404,0.913545457642601,0.90630778703665,0.8987940462991669,0.8910065241883679,0.8829475928589271,0.8746197071393959,
	0.8660254037844387,0.8571673007021123,0.8480480961564261,0.8386705679454239,0.8290375725550417,0.819152044288992,0.8090169943749475,0.7986355100472927,0.788010753606722,0.777145961456971,
	0.766044443118978,0.7547095802227718,0.7431448254773942,0.7313537016191706,0.7193398003386514,0.7071067811865476,0.6946583704589971,0.6819983600624986,0.6691306063588583,0.6560590289905073,
	0.6427876096865395,0.6293203910498377,0.6156614753256584,0.6018150231520482,0.5877852522924732,0.5735764363510464,0.5591929034707469,0.544639035015027,0.5299192642332049,0.5150380749100544,
	0.49999999999999994,0.48480962024633717,0.4694715627858911,0.45399049973954686,0.4383711467890773,0.4226182617406995,0.40673664307580043,0.39073112848927416,0.37460659341591224,0.3583679495453002,
	0.3420201433256689,0.32556815445715703,0.3090169943749475,0.29237170472273705,0.27563735581699966,0.258819045102521,0.24192189559966773,0.22495105434386478,0.20791169081775931,0.19080899537654497,
	0.17364817766693028,0.15643446504023098,0.13917310096006574,0.12186934340514755,0.10452846326765373,0.08715574274765864,0.06975647374412552,0.05233595624294381,0.0348994967025007,0.01745240643728344,
	1.2246467991473532e-16,-0.017452406437283192,-0.0348994967025009,-0.05233595624294356,-0.06975647374412483,-0.08715574274765794,-0.10452846326765305,-0.12186934340514774,-0.13917310096006552,-0.15643446504023073,
	-0.17364817766693047,-0.19080899537654472,-0.20791169081775907,-0.22495105434386498,-0.2419218955996675,-0.25881904510252035,-0.275637355816999,-0.2923717047227364,-0.30901699437494773,-0.32556815445715676,
	-0.34202014332566866,-0.35836794954530043,-0.374606593415912,-0.39073112848927355,-0.4067366430757998,-0.4226182617406993,-0.43837114678907707,-0.45399049973954625,-0.46947156278589086,-0.48480962024633695,
	-0.5000000000000001,-0.5150380749100542,-0.5299192642332048,-0.5446390350150271,-0.5591929034707467,-0.5735764363510458,-0.587785252292473,-0.601815023152048,-0.6156614753256578,-0.6293203910498376,
	-0.6427876096865393,-0.6560590289905074,-0.6691306063588582,-0.6819983600624984,-0.6946583704589974,-0.7071067811865475,-0.7193398003386509,-0.7313537016191701,-0.743144825477394,-0.7547095802227717,
	-0.7660444431189779,-0.7771459614569711,-0.7880107536067221,-0.7986355100472928,-0.8090169943749473,-0.8191520442889916,-0.8290375725550414,-0.838670567945424,-0.848048096156426,-0.8571673007021121,
	-0.8660254037844384,-0.874619707139396,-0.882947592858927,-0.8910065241883678,-0.8987940462991668,-0.9063077870366497,-0.913545457642601,-0.9205048534524403,-0.9271838545667873,-0.9335804264972016,
	-0.9396926207859082,-0.9455185755993168,-0.9510565162951535,-0.9563047559630353,-0.961261695938319,-0.9659258262890683,-0.9702957262759965,-0.9743700647852351,-0.9781476007338056,-0.9816271834476639,
	-0.984807753012208,-0.9876883405951377,-0.9902680687415704,-0.9925461516413221,-0.9945218953682734,-0.9961946980917455,-0.9975640502598242,-0.9986295347545738,-0.9993908270190957,-0.9998476951563913,
	-1.0,-0.9998476951563913,-0.9993908270190958,-0.9986295347545738,-0.9975640502598243,-0.9961946980917455,-0.9945218953682734,-0.992546151641322,-0.9902680687415704,-0.9876883405951378,
	-0.9848077530122081,-0.9816271834476641,-0.9781476007338058,-0.9743700647852352,-0.9702957262759966,-0.9659258262890682,-0.9612616959383188,-0.9563047559630354,-0.9510565162951536,-0.945518575599317,
	-0.9396926207859085,-0.9335804264972021,-0.9271838545667874,-0.9205048534524405,-0.9135454576426008,-0.9063077870366499,-0.898794046299167,-0.8910065241883679,-0.8829475928589271,-0.8746197071393961,
	-0.8660254037844386,-0.8571673007021123,-0.8480480961564262,-0.8386705679454243,-0.8290375725550421,-0.8191520442889918,-0.8090169943749476,-0.798635510047293,-0.7880107536067218,-0.7771459614569708,
	-0.7660444431189781,-0.7547095802227722,-0.7431448254773946,-0.731353701619171,-0.7193398003386517,-0.7071067811865477,-0.6946583704589976,-0.6819983600624983,-0.6691306063588581,-0.6560590289905074,
	-0.6427876096865396,-0.6293203910498378,-0.6156614753256588,-0.6018150231520483,-0.5877852522924734,-0.5735764363510465,-0.5591929034707473,-0.544639035015027,-0.5299192642332058,-0.5150380749100545,
	-0.5000000000000004,-0.4848096202463369,-0.4694715627858908,-0.45399049973954697,-0.438371146789077,-0.4226182617407,-0.40673664307580015,-0.3907311284892747,-0.37460659341591235,-0.35836794954530077,
	-0.3420201433256686,-0.32556815445715753,-0.3090169943749476,-0.29237170472273627,-0.2756373558169998,-0.2588190451025207,-0.24192189559966787,-0.22495105434386534,-0.20791169081775987,-0.19080899537654467,
	-0.17364817766693127,-0.15643446504023112,-0.13917310096006588,-0.12186934340514811,-0.10452846326765342,-0.08715574274765832,-0.06975647374412476,-0.05233595624294437,-0.034899496702500823,-0.01745240643728445,
};

typedef struct
{
	uint16_t freq_hz;	//between 200 and 20khz
	uint16_t duration_ms;
	uint8_t volume;
} tone_t;

#define PWM_DMA_BUFFER_SIZE 1000
__IO uint16_t pwm_dma_buffer[PWM_DMA_BUFFER_SIZE];

#define PWM_FREQ 31250

FATFS fatfs;
FIL MyFile;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

void HAL_TIM_PWM_PulseHalfFinishedCallback(DMA_HandleTypeDef *hdma)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

void play_tone(tone_t tone)
{
	uint16_t buffer_size = PWM_FREQ / tone.freq_hz;
	
	for(uint32_t i = 0; i < buffer_size; i++)
	{
		pwm_dma_buffer[i] = 128 + sin_lookup_table[i * SIN_LOOKUP_TABLE_SIZE / buffer_size] * tone.volume/2;
	}
	
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwm_dma_buffer, buffer_size);

	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	
	HAL_Delay(tone.duration_ms);
	
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		disk_timerproc();
	}
}

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
 
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100);
	
	
	return ch;
}

static
DWORD pn (		/* Pseudo random number generator */
    DWORD pns	/* 0:Initialize, !0:Read */
)
{
    static DWORD lfsr;
    UINT n;


    if (pns) {
        lfsr = pns;
        for (n = 0; n < 32; n++) pn(0);
    }
    if (lfsr & 1) {
        lfsr >>= 1;
        lfsr ^= 0x80200003;
    } else {
        lfsr >>= 1;
    }
    return lfsr;
}


int test_diskio (
    BYTE pdrv,      /* Physical drive number to be checked (all data on the drive will be lost) */
    UINT ncyc,      /* Number of test cycles */
    DWORD* buff,    /* Pointer to the working buffer */
    UINT sz_buff    /* Size of the working buffer in unit of byte */
)
{
    UINT n, cc, ns;
    DWORD sz_drv, lba, lba2, sz_eblk, pns = 1;
    WORD sz_sect;
    BYTE *pbuff = (BYTE*)buff;
    DSTATUS ds;
    DRESULT dr;


    printf("test_diskio(%u, %u, 0x%08X, 0x%08X)\n", pdrv, ncyc, (UINT)buff, sz_buff);

    if (sz_buff < _MAX_SS + 4) {
        printf("Insufficient work area to run program.\n");
        return 1;
    }

    for (cc = 1; cc <= ncyc; cc++) {
        printf("**** Test cycle %u of %u start ****\n", cc, ncyc);

        /* Initialization */
        printf(" disk_initalize(%u)", pdrv);
        ds = disk_initialize(pdrv);
        if (ds & STA_NOINIT) {
            printf(" - failed.\n");
            return 2;
        } else {
            printf(" - ok.\n");
        }

        /* Get drive size */
        printf("**** Get drive size ****\n");
        printf(" disk_ioctl(%u, GET_SECTOR_COUNT, 0x%08X)", pdrv, (UINT)&sz_drv);
        sz_drv = 0;
        dr = disk_ioctl(pdrv, GET_SECTOR_COUNT, &sz_drv);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 3;
        }
        if (sz_drv < 128) {
            printf("Failed: Insufficient drive size to test.\n");
            return 4;
        }
        printf(" Number of sectors on the drive %u is %lu.\n", pdrv, sz_drv);

#if _MAX_SS != _MIN_SS
        /* Get sector size */
        printf("**** Get sector size ****\n");
        printf(" disk_ioctl(%u, GET_SECTOR_SIZE, 0x%X)", pdrv, (UINT)&sz_sect);
        sz_sect = 0;
        dr = disk_ioctl(pdrv, GET_SECTOR_SIZE, &sz_sect);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 5;
        }
        printf(" Size of sector is %u bytes.\n", sz_sect);
#else
        sz_sect = _MAX_SS;
#endif

        /* Get erase block size */
        printf("**** Get block size ****\n");
        printf(" disk_ioctl(%u, GET_BLOCK_SIZE, 0x%X)", pdrv, (UINT)&sz_eblk);
        sz_eblk = 0;
        dr = disk_ioctl(pdrv, GET_BLOCK_SIZE, &sz_eblk);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
        }
        if (dr == RES_OK || sz_eblk >= 2) {
            printf(" Size of the erase block is %lu sectors.\n", sz_eblk);
        } else {
            printf(" Size of the erase block is unknown.\n");
        }

        /* Single sector write test */
        printf("**** Single sector write test 1 ****\n");
        lba = 0;
        for (n = 0, pn(pns); n < sz_sect; n++) pbuff[n] = (BYTE)pn(0);
        printf(" disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
        dr = disk_write(pdrv, pbuff, lba, 1);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 6;
        }
        printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 7;
        }
        memset(pbuff, 0, sz_sect);
        printf(" disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
        dr = disk_read(pdrv, pbuff, lba, 1);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 8;
        }
        for (n = 0, pn(pns); n < sz_sect && pbuff[n] == (BYTE)pn(0); n++) ;
        if (n == sz_sect) {
            printf(" Data matched.\n");
        } else {
            printf("Failed: Read data differs from the data written.\n");
            return 10;
        }
        pns++;

        /* Multiple sector write test */
        printf("**** Multiple sector write test ****\n");
        lba = 1; ns = sz_buff / sz_sect;
        if (ns > 4) ns = 4;
        for (n = 0, pn(pns); n < (UINT)(sz_sect * ns); n++) pbuff[n] = (BYTE)pn(0);
        printf(" disk_write(%u, 0x%X, %lu, %u)", pdrv, (UINT)pbuff, lba, ns);
        dr = disk_write(pdrv, pbuff, lba, ns);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 11;
        }
        printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 12;
        }
        memset(pbuff, 0, sz_sect * ns);
        printf(" disk_read(%u, 0x%X, %lu, %u)", pdrv, (UINT)pbuff, lba, ns);
        dr = disk_read(pdrv, pbuff, lba, ns);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 13;
        }
        for (n = 0, pn(pns); n < (UINT)(sz_sect * ns) && pbuff[n] == (BYTE)pn(0); n++) ;
        if (n == (UINT)(sz_sect * ns)) {
            printf(" Data matched.\n");
        } else {
            printf("Failed: Read data differs from the data written.\n");
            return 14;
        }
        pns++;

        /* Single sector write test (misaligned memory address) */
        printf("**** Single sector write test (misaligned address) ****\n");
        lba = 5;
        for (n = 0, pn(pns); n < sz_sect; n++) pbuff[n+3] = (BYTE)pn(0);
        printf(" disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+3), lba);
        dr = disk_write(pdrv, pbuff+3, lba, 1);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 15;
        }
        printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 16;
        }
        memset(pbuff+5, 0, sz_sect);
        printf(" disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+5), lba);
        dr = disk_read(pdrv, pbuff+5, lba, 1);
        if (dr == RES_OK) {
            printf(" - ok.\n");
        } else {
            printf(" - failed.\n");
            return 17;
        }
        for (n = 0, pn(pns); n < sz_sect && pbuff[n+5] == (BYTE)pn(0); n++) ;
        if (n == sz_sect) {
            printf(" Data matched.\n");
        } else {
            printf("Failed: Read data differs from the data written.\n");
            return 18;
        }
        pns++;

        /* 4GB barrier test */
        printf("**** 4GB barrier test ****\n");
        if (sz_drv >= 128 + 0x80000000 / (sz_sect / 2)) {
            lba = 6; lba2 = lba + 0x80000000 / (sz_sect / 2);
            for (n = 0, pn(pns); n < (UINT)(sz_sect * 2); n++) pbuff[n] = (BYTE)pn(0);
            printf(" disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
            dr = disk_write(pdrv, pbuff, lba, 1);
            if (dr == RES_OK) {
                printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 19;
            }
            printf(" disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+sz_sect), lba2);
            dr = disk_write(pdrv, pbuff+sz_sect, lba2, 1);
            if (dr == RES_OK) {
                printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 20;
            }
            printf(" disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
            dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
            if (dr == RES_OK) {
            printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 21;
            }
            memset(pbuff, 0, sz_sect * 2);
            printf(" disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
            dr = disk_read(pdrv, pbuff, lba, 1);
            if (dr == RES_OK) {
                printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 22;
            }
            printf(" disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+sz_sect), lba2);
            dr = disk_read(pdrv, pbuff+sz_sect, lba2, 1);
            if (dr == RES_OK) {
                printf(" - ok.\n");
            } else {
                printf(" - failed.\n");
                return 23;
            }
            for (n = 0, pn(pns); pbuff[n] == (BYTE)pn(0) && n < (UINT)(sz_sect * 2); n++) ;
            if (n == (UINT)(sz_sect * 2)) {
                printf(" Data matched.\n");
            } else {
                printf("Failed: Read data differs from the data written.\n");
                return 24;
            }
        } else {
            printf(" Test skipped.\n");
        }
        pns++;

        printf("**** Test cycle %u of %u completed ****\n\n", cc, ncyc);
    }

    return 0;
}

//DWORD buff[1024];  /* 4096 byte working buffer */

void read_file(void)
{
	if(f_open(&MyFile, "test.txt", FA_OPEN_EXISTING | FA_READ) != FR_OK)
	{
		printf("failed to open file\n");
	}
	else
	{
		printf("File opened\n");
		
		uint32_t bytesread;
		uint8_t rtext[100];
		
		FRESULT res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);
		if((bytesread == 0) || (res != FR_OK))
		{
			printf("Failed to read file\n");
		}
		else
		{
			printf("Read: %s\n", rtext);
			if(f_close(&MyFile) != FR_OK)
			{
				printf("failed to close file\n");
			}
			else
			{
				printf("file closed\n");
			}
		}
	}
}

void playback(void)
{
	if(f_open(&MyFile, "test.wav", FA_OPEN_EXISTING | FA_READ) != FR_OK)
	{
		printf("failed to open file\n");
	}
	else
	{
		printf("File opened\n");
		
		uint32_t bytesread;
		uint8_t buffer[44];
		
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		
		FRESULT res = f_read(&MyFile, buffer, sizeof(buffer), (UINT*)&bytesread);
		
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		
		if((bytesread < sizeof(buffer)) || (res != FR_OK))
		{
			printf("Failed to read file\n");
		}
		else
		{
			/*
			for(uint8_t i = 0; i < sizeof(buffer); i++)
			{
				printf("%x\t", buffer[i]);
				if((i % 16) == 15)
				{
					printf("\n");
				}
			}
			printf("\n");
			*/
			
			uint16_t channels = buffer[22] + (buffer[23]<<8);
			uint32_t sample_rate = buffer[24] + (buffer[25]<<8) + (buffer[26]<<16) + (buffer[27]<<24);
			uint16_t bits_per_sample = buffer[34] + (buffer[35]<<8);
			
			printf("channels: %d\n", channels);
			printf("sample rate: %d\n", sample_rate);
			printf("bits per sample: %d\n", bits_per_sample);
			
			/*
			if(f_close(&MyFile) != FR_OK)
			{
				printf("failed to close file\n");
			}
			else
			{
				printf("file closed\n");
			}
			*/
		}
	}
}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_FATFS_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
	
	printf("Up and running\r\n");
	
	htim1.hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = HAL_TIM_PWM_PulseHalfFinishedCallback;
	HAL_TIM_Base_Start_IT(&htim2);

	if (f_mount(&fatfs, USER_Path, 0) != FR_OK)
	{
		printf("error in mounting disk\n");
	}
	else
	{
		printf("disk mounted\n");
		//read_file();
		playback();
	}

	
	/*
	int rc;

	// Check function/compatibility of the physical drive #0
	rc = test_diskio(0, 3, buff, sizeof buff);
	if (rc) {
			printf("Sorry the function/compatibility test failed. (rc=%d)\nFatFs will not work on this disk driver.\n", rc);
	} else {
			printf("Congratulations! The disk driver works well.\n");
	}
	*/
	
	//play_tone((tone_t){880, 1000, 64});
	//play_tone((tone_t){1000, 1000, 64});
	
	/*
	pwm_dma_buffer[0] = 128+64;
	pwm_dma_buffer[1] = 128-64;

	
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwm_dma_buffer, 2);

	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	
	HAL_Delay(1000);
	
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	*/
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		HAL_Delay(500);
		
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 8;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 255;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim1);

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 64000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
