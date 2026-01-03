#include "Motor.h"
#include "ENCODER.h"

/* ===== 方向引脚定义 ===== */
#define AIN1_PORT   GPIOE
#define AIN1_PIN    GPIO_Pin_8

#define AIN2_PORT   GPIOE
#define AIN2_PIN    GPIO_Pin_10

#define BIN1_PORT   GPIOB
#define BIN1_PIN    GPIO_Pin_1

#define BIN2_PORT   GPIOC
#define BIN2_PIN    GPIO_Pin_5
// 这里要和 TIM9 的 Per 保持一致
#define PWM_MAX   99   
#define PWM_MIN  -99


volatile float outA;
volatile float outB;

extern float kp;
extern float ki;
extern float kd;


static inline void PIN_H(GPIO_TypeDef* port, uint16_t pin) { GPIO_SetBits(port, pin); }
static inline void PIN_L(GPIO_TypeDef* port, uint16_t pin) { GPIO_ResetBits(port, pin); }

static inline int iabs_int(int x) { return (x >= 0) ? x : -x; }

static inline void limit_pwm(int *a, int *b)
{
    if (*a > PWM_MAX) *a = PWM_MAX;
    if (*a < PWM_MIN) *a = PWM_MIN;
    if (*b > PWM_MAX) *b = PWM_MAX;
    if (*b < PWM_MIN) *b = PWM_MIN;
}

/* 电机方向IO初始化：PE8/PE10/PB1/PC5 推挽输出 */
void Motor_Init(void)
{
    GPIO_InitTypeDef gpio;

    /* F407: GPIOE/B/C 都在 AHB1 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE |
                           RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    /* PE8 + PE10 */
    gpio.GPIO_Pin = AIN1_PIN | AIN2_PIN;
    GPIO_Init(GPIOE, &gpio);

    /* PB1 */
    gpio.GPIO_Pin = BIN1_PIN;
    GPIO_Init(GPIOB, &gpio);

    /* PC5 */
    gpio.GPIO_Pin = BIN2_PIN;
    GPIO_Init(GPIOC, &gpio);

    /* 默认停机 */
    PIN_L(AIN1_PORT, AIN1_PIN);
    PIN_L(AIN2_PORT, AIN2_PIN);
    PIN_L(BIN1_PORT, BIN1_PIN);
    PIN_L(BIN2_PORT, BIN2_PIN);
}

/* moto1/moto2: 正=正转，负=反转；幅值=PWM */
void Load(int moto1, int moto2)
{
    limit_pwm(&moto1, &moto2);

    /* ---- 电机A方向 ---- */
    if (moto1 >= 0) { PIN_H(AIN1_PORT, AIN1_PIN); PIN_L(AIN2_PORT, AIN2_PIN); }
    else            { PIN_L(AIN1_PORT, AIN1_PIN); PIN_H(AIN2_PORT, AIN2_PIN); }

    /* ---- 电机B方向 ---- */
    if (moto2 >= 0) { PIN_H(BIN1_PORT, BIN1_PIN); PIN_L(BIN2_PORT, BIN2_PIN); }
    else            { PIN_L(BIN1_PORT, BIN1_PIN); PIN_H(BIN2_PORT, BIN2_PIN); }

    /* ---- PWM 输出：TIM9_CH1(PE5)=电机A，TIM9_CH2(PE6)=电机B ---- */
    TIM_SetCompare1(TIM9, (uint16_t)iabs_int(moto1));
    TIM_SetCompare2(TIM9, (uint16_t)iabs_int(moto2));
}

//速度环PID（PI：kd=0）
void Speed_PID(int16_t target_SpeedA,int16_t target_SpeedB,
               int16_t current_SpeedA,int16_t current_SpeedB)
{
    static int16_t lasterra = 0;
    static int32_t suma = 0;
    int16_t erra;
		lasterra = erra;
		erra= target_SpeedA - current_SpeedA;
		suma += erra;
	
    static int16_t lasterrb = 0;
    static int32_t sumb = 0;
    int16_t errb ;
		lasterrb = errb;
		errb = target_SpeedB - current_SpeedB;  
    sumb += errb;

		if (suma > 800)  suma = 800;
    if (suma < -800) suma = -800;
    if (sumb > 800)  sumb = 800;
    if (sumb < -800) sumb = -800;

    //kd=0
    int16_t deltaa = erra - lasterra;
    int16_t deltab = errb - lasterrb;


     outA = kp*erra + ki*(float)suma + kd*deltaa;
     outB = kp*errb + ki*(float)sumb + kd*deltab;
		
    Load((int16_t)outA, (int16_t)outB);
}


