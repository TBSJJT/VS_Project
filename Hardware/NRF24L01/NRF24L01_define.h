#ifndef __NRF24L01_DEFINE_H
#define __NRF24L01_DEFINE_H


#define IRQ_Port   GPIOB
#define CE_Port    GPIOB
#define MISO_Port  GPIOA
#define MOSI_Port  GPIOA
#define SCK_Port   GPIOA
#define CSN_Port   GPIOA

#define IRQ_Pin    GPIO_Pin_15
#define CE_Pin     GPIO_Pin_0
#define MOSI_Pin   GPIO_Pin_7
#define MISO_Pin   GPIO_Pin_6
#define SCK_Pin    GPIO_Pin_2
#define CSN_Pin    GPIO_Pin_4

/* =========================
 * nRF24L01 寄存器地址（Register Map）
 * 说明：
 * - 通过 SPI 指令 R_REGISTER / W_REGISTER + 寄存器地址 来读写
 * - 大部分寄存器为 1 字节；地址寄存器（RX_ADDR_Px / TX_ADDR）为 3~5 字节（由 SETUP_AW 决定）
 * ========================= */

/* CONFIG (0x00) 配置寄存器：电源/模式/CRC/中断屏蔽
 * bit7: MASK_RX_DR  1=屏蔽接收中断
 * bit6: MASK_TX_DS  1=屏蔽发送成功中断
 * bit5: MASK_MAX_RT 1=屏蔽达到最大重发次数中断
 * bit4: EN_CRC      1=使能 CRC
 * bit3: CRCO        0=1字节CRC, 1=2字节CRC
 * bit2: PWR_UP      1=上电（进入 Standby-I），0=掉电
 * bit1: PRIM_RX     1=接收模式(PRX)，0=发送模式(PTX)
 * bit0: (保留)
 * 常用配置：
 * - 发送模式：PWR_UP=1, PRIM_RX=0
 * - 接收模式：PWR_UP=1, PRIM_RX=1
 * - 建议：EN_CRC=1 且 CRCO=1（2字节CRC更稳）
 */
#define CONFIG      0x00

/* EN_AA (0x01) 自动应答使能：每个数据管道(P0~P5)独立控制
 * bit0~bit5：ENAA_P0~ENAA_P5，1=该管道启用 Auto-ACK
 * 说明：
 * - 只有接收端开启对应管道的 Auto-ACK，发送端才会收到 ACK
 * - 若你要“无应答”低延迟，可关掉 EN_AA 和 SETUP_RETR
 */
#define EN_AA       0x01

/* EN_RXADDR (0x02) 接收地址使能：开启哪些 RX Pipe
 * bit0~bit5：ERX_P0~ERX_P5，1=使能对应管道接收
 * 常用：
 * - 最常用 P0（接收对端回发/ACK payload 或主通道）
 * - 多接收源可开 P1~P5
 */
#define EN_RXADDR   0x02

/* SETUP_AW (0x03) 地址宽度设置：影响 RX_ADDR_Px / TX_ADDR 长度
 * bit1:0：
 *   0b01 = 3 字节地址
 *   0b10 = 4 字节地址
 *   0b11 = 5 字节地址（最常用/推荐）
 */
#define SETUP_AW    0x03

/* SETUP_RETR (0x04) 自动重发设置（仅在开启 Auto-ACK 时有效）
 * bit7:4 ARD (Auto Retransmit Delay)：重发间隔
 *   0000=250us, 0001=500us, ... , 1111=4000us
 * bit3:0 ARC (Auto Retransmit Count)：最大重发次数 0~15
 * 常用：
 * - 稳定链路：ARD=500~1500us，ARC=5~10
 * - 低延迟：ARC小一点或直接关 Auto-ACK
 */
#define SETUP_RETR  0x04

/* RF_CH (0x05) 射频信道：0~125（2.4GHz 频段）
 * 频率 = 2400MHz + RF_CH (MHz)
 * 常用：
 * - 避开 WiFi：比如 2.412GHz(信道1)、2.437GHz(信道6)、2.462GHz(信道11)
 * - 你可以试 40、60、80 等
 */
#define RF_CH       0x05

/* RF_SETUP (0x06) 射频参数：速率/功率/LNA
 * bit5 RF_DR_LOW + bit3 RF_DR_HIGH 决定空速：
 *   00: 1Mbps
 *   01: 2Mbps (RF_DR_HIGH=1)
 *   10: 250kbps (RF_DR_LOW=1)
 * bit2:1 RF_PWR 发射功率：
 *   00:-18dBm  01:-12dBm  10:-6dBm  11:0dBm
 * bit0 LNA_HCURR：1=高增益LNA（接收灵敏度更好，常开）
 * 常用建议：
 * - 追距离/抗干扰：250kbps + 0dBm
 * - 追低延迟：2Mbps + 0dBm（但距离会近点）
 */
#define RF_SETUP    0x06

/* STATUS (0x07) 状态寄存器：中断标志/当前接收Pipe/发送FIFO状态
 * bit6 RX_DR：接收数据完成中断标志（写1清零）
 * bit5 TX_DS：发送成功中断标志（写1清零）
 * bit4 MAX_RT：达到最大重发次数中断标志（写1清零）
 * bit3:1 RX_P_NO：当前FIFO顶部数据来自哪个Pipe（000~101），111=FIFO空
 * bit0 TX_FULL：TX FIFO满标志
 * 常用处理：
 * - 进入中断或轮询后，读 STATUS 判断原因
 * - 清标志：W_REGISTER|STATUS 写入 (1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT) 对应位
 */
#define STATUS      0x07

/* OBSERVE_TX (0x08) 发送观察寄存器：丢包/重发统计
 * bit7:4 PLOS_CNT：丢包计数（只增不减，写 RF_CH 可清零）
 * bit3:0 ARC_CNT：当前包已自动重发次数（本次发送的即时值）
 * 用途：
 * - 调参时判断链路质量（ARC_CNT大说明经常重发）
 */
#define OBSERVE_TX  0x08

/* CD (0x09) 载波检测（老版本叫 CD，新版常叫 RPD）
 * nRF24L01：CD=1 表示检测到载波（信道上可能有人在发）
 * 用途：
 * - 简易信道探测/避让（不精确，但能用来挑干净信道）
 */
#define CD          0x09

/* RX_ADDR_P0 ~ RX_ADDR_P5 (0x0A~0x0F) 各接收管道地址
 * - P0 / P1：完整地址（3~5字节）
 * - P2~P5：只设置最低 1 字节，高字节与 P1 共用（硬件规则）
 * 常用：
 * - 单对单：只用 P0（或 P1），地址与对端 TX_ADDR 配对
 * - 多对一：用 P1 做“基地址”，P2~P5 用不同低字节区分来源
 */
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F

/* TX_ADDR (0x10) 发送地址（3~5字节）
 * 说明：
 * - 在启用 Auto-ACK 时：TX_ADDR 必须等于接收端某个 RX_ADDR_Px（通常用 P0）
 * - 经典配置：TX_ADDR = RX_ADDR_P0（本机）用于 ACK 匹配
 */
#define TX_ADDR     0x10

/* RX_PW_P0 ~ RX_PW_P5 (0x11~0x16) 各 Pipe 固定负载长度（1~32字节）
 * 说明：
 * - 不使用动态负载(DPL)时必须设置
 * - 你现在代码 R_Buf(..., 32) 这种，通常就把对应 Pipe 设为 32
 * - 若启用 DPL（FEATURE/DYNPD），这些可忽略
 */
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16

/* FIFO_STATUS (0x17) FIFO 状态：TX/RX FIFO 空/满等
 * bit6 TX_REUSE：1=正在重复发送上一包（配合 REUSE_TX_PL 指令）
 * bit5 TX_FULL：TX FIFO满
 * bit4 TX_EMPTY：TX FIFO空
 * bit1 RX_FULL：RX FIFO满
 * bit0 RX_EMPTY：RX FIFO空
 * 用途：
 * - 轮询方式下判断是否有包可读（RX_EMPTY=0）
 */
#define FIFO_STATUS 0x17

/* DYNPD (0x1C) 动态负载使能：每个 Pipe 是否启用 DPL
 * bit0~bit5：DPL_P0~DPL_P5
 * 说明：
 * - 需要同时在 FEATURE 中打开 EN_DPL
 */
#define DYNPD       0x1C

/* FEATURE (0x1D) 特性开关：动态负载/ACK Payload/无ACK发送
 * bit2 EN_DPL：动态负载(DPL)使能
 * bit1 EN_ACK_PAY：ACK Payload 使能（接收端可在 ACK 中夹带数据返回）
 * bit0 EN_DYN_ACK：W_TX_PAYLOAD_NOACK 指令使能（单包禁ACK）
 * 注意：
 * - 某些模块上电后 FEATURE 写不进去，需要先发送 ACTIVATE(0x50,0x73) 解锁
 */
#define FEATURE     0x1D


/* =========================
 * SPI 指令（Commands）
 * ========================= */

/* R_REGISTER (0x00) 读寄存器指令前缀
 * 用法：SPI 发送 (R_REGISTER | reg) 再读回 1~N 字节
 */
#define R_REGISTER    0x00

/* W_REGISTER (0x20) 写寄存器指令前缀
 * 用法：SPI 发送 (W_REGISTER | reg) 再写入 1~N 字节
 */
#define W_REGISTER    0x20

/* R_RX_PAYLOAD (0x61) 读 RX FIFO 顶部负载
 * 用法：读出 1~32 字节（固定长度或动态长度）
 */
#define R_RX_PAYLOAD  0x61

/* W_TX_PAYLOAD (0xA0) 写 TX FIFO 负载（准备发送）
 * 用法：写入 1~32 字节；随后 CE 拉高 >=10us 触发发送（PTX模式）
 */
#define W_TX_PAYLOAD  0xA0

/* FLUSH_TX (0xE1) 清空 TX FIFO（丢弃未发送/待发送数据） */
#define FLUSH_TX      0xE1

/* FLUSH_RX (0xE2) 清空 RX FIFO（丢弃已接收未读数据） */
#define FLUSH_RX      0xE2

/* NOP (0xFF) 空操作：常用于“只读 STATUS”——发送 NOP 返回的第一字节就是 STATUS */
#define NOP           0xFF


/* =========================
 * STATUS 寄存器中的常用中断标志位（bit mask）
 * ========================= */

/* RX_OK：对应 STATUS.RX_DR（bit6）=1 表示收到数据（需写1清零） */
#define RX_OK       0x40

/* TX_OK：对应 STATUS.TX_DS（bit5）=1 表示发送成功（收到ACK或无ACK模式成功） */
#define TX_OK       0x20

/* MAX_TX：对应 STATUS.MAX_RT（bit4）=1 表示达到最大重发次数（需清零并FLUSH_TX） */
#define MAX_TX      0x10


#endif
