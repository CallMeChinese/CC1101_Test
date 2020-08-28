#include "drv_CC1101.h"
#include "drv_delay.h"

//10, 7, 5, 0, -5, -10, -15, -20, dbm output power, 0x12 == -30dbm
const uint8_t PaTabel[] = {0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0};
static const uint8_t CC1101InitData[23][2] =
    {
        {CC1101_IOCFG0, 0x06},
        {CC1101_FIFOTHR, 0x47},
        {CC1101_PKTCTRL0, 0x05},
        {CC1101_CHANNR, 0x96}, //430M
        {CC1101_FSCTRL1, 0x06},
        {CC1101_PKTLEN, 0x40},
        {CC1101_FREQ2, 0x0F},
        {CC1101_FREQ1, 0x62},
        {CC1101_FREQ0, 0x76},
        {CC1101_MDMCFG4, 0xF6},
        {CC1101_MDMCFG3, 0x43},
        {CC1101_MDMCFG2, 0x13},
        {CC1101_DEVIATN, 0x15},
        {CC1101_MCSM0, 0x18},
        {CC1101_FOCCFG, 0x16},
        {CC1101_WORCTRL, 0xFB},
        {CC1101_FSCAL3, 0xE9},
        {CC1101_FSCAL2, 0x2A},
        {CC1101_FSCAL1, 0x00},
        {CC1101_FSCAL0, 0x1F},
        {CC1101_TEST2, 0x81},
        {CC1101_TEST1, 0x35},
        {CC1101_MCSM1, 0x3B},
};

/**
  * @brief :CC1101写锟斤拷锟斤拷
  * @param :
  *	@Command锟斤拷锟斤拷锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Write_Cmd(uint8_t Command)
{
    CC1101_SET_CSN_LOW();
    drv_spi_read_write_byte(Command);
    CC1101_SET_CSN_HIGH();
}

/**
  * @brief :CC1101写锟侥达拷锟斤拷
  * @param :
  *			@Addr锟斤拷锟斤拷址
  *			@WriteValue锟斤拷写锟斤拷锟斤拷锟斤拷锟斤拷纸锟?
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Write_Reg(uint8_t Addr, uint8_t WriteValue)
{
    CC1101_SET_CSN_LOW();

    drv_spi_read_write_byte(Addr);       //写锟斤拷址
    drv_spi_read_write_byte(WriteValue); //写锟斤拷锟斤拷

    CC1101_SET_CSN_HIGH();
}

/**
  * @brief :CC1101锟斤拷锟斤拷写锟侥达拷锟斤拷
  * @param :
  *			@Addr锟斤拷锟斤拷址
  *			@pWriteBuff锟斤拷写锟斤拷锟斤拷锟斤拷荽锟斤拷椎锟街?
  *			@WriteSize锟斤拷写锟斤拷锟斤拷锟斤拷莞锟斤拷锟?
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Write_Multi_Reg(uint8_t Addr, uint8_t *pWriteBuff, uint8_t WriteSize)
{
    uint8_t i;

    CC1101_SET_CSN_LOW();

    drv_spi_read_write_byte(Addr | WRITE_BURST); //锟斤拷锟斤拷写锟斤拷锟斤拷 锟斤拷锟阶碉拷址
    for (i = 0; i < WriteSize; i++)
    {
        drv_spi_read_write_byte(*(pWriteBuff + i)); //锟斤拷锟斤拷写锟斤拷锟斤拷锟斤拷
    }

    CC1101_SET_CSN_HIGH();
}

/**
  * @brief :CC1101锟斤拷锟侥达拷锟斤拷
  * @param :
  *			@Addr锟斤拷锟斤拷址
  * @note  :锟斤拷
  * @retval:锟侥达拷锟斤拷值
  */
uint8_t CC1101_Read_Reg(uint8_t Addr)
{
    uint8_t l_RegValue = 0;

    CC1101_SET_CSN_LOW();

    drv_spi_read_write_byte(Addr | READ_SINGLE); //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷 锟斤拷锟斤拷址
    l_RegValue = drv_spi_read_write_byte(0xFF);  //锟斤拷取锟侥达拷锟斤拷

    CC1101_SET_CSN_HIGH();

    return l_RegValue;
}

/**
  * @brief :CC1101锟斤拷一锟斤拷锟侥达拷锟斤拷状态
  * @param :
  *			@Addr锟斤拷锟斤拷址
  * @note  :锟斤拷
  * @retval:锟侥达拷锟斤拷状态
  */
uint8_t CC1101_Read_Status(uint8_t Addr)
{
    uint8_t l_RegStatus = 0;

    CC1101_SET_CSN_LOW();

    drv_spi_read_write_byte(Addr | READ_BURST);  //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷 锟斤拷锟斤拷址
    l_RegStatus = drv_spi_read_write_byte(0xFF); //锟斤拷取状态

    CC1101_SET_CSN_HIGH();

    return l_RegStatus;
}

/**
  * @brief :CC1101锟斤拷锟斤拷锟斤拷锟侥达拷锟斤拷
  * @param :
  *			@Addr锟斤拷锟斤拷址
  *			@pReadBuff锟斤拷锟斤拷取锟斤拷锟捷达拷锟斤拷椎锟街?
  *			@ReadSize锟斤拷锟斤拷取锟斤拷锟捷的革拷锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Read_Multi_Reg(uint8_t Addr, uint8_t *pReadBuff, uint8_t ReadSize)
{
    uint8_t i = 0, j = 0;

    CC1101_SET_CSN_LOW();

    drv_spi_read_write_byte(Addr | READ_BURST); //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷 锟斤拷锟阶碉拷址
    for (i = 0; i < ReadSize; i++)
    {
        for (j = 0; j < 20; j++)
            ;
        *(pReadBuff + i) = drv_spi_read_write_byte(0xFF); //锟斤拷锟斤拷锟斤拷取锟斤拷锟斤拷
    }

    CC1101_SET_CSN_HIGH();
}

/**
  * @brief :CC1101锟斤拷锟酵斤拷锟斤拷模式锟斤拷锟斤拷
  * @param :
  *	@Mode 锟斤拷TX_MODE锟斤拷锟斤拷锟斤拷模式 RX_MODE锟斤拷锟斤拷锟斤拷模式
  * @note  :锟斤拷
  * @retval:锟侥达拷锟斤拷状态
  */
void CC1101_Set_Mode(CC1101_ModeType Mode)
{
    uint8_t WaitTimeOut = 0;

    if (Mode == TX_MODE) //锟斤拷锟斤拷模式
    {
        CC1101_Write_Reg(CC1101_IOCFG0, 0x46);
        CC1101_Write_Cmd(CC1101_STX);
    }
    else if (Mode == RX_MODE) //锟斤拷锟斤拷模式
    {
        CC1101_Write_Reg(CC1101_IOCFG0, 0x46);
        CC1101_Write_Cmd(CC1101_SRX);
    }

    while (0 != CC1101_GET_GDO0_STATUS())
        ; //锟饺达拷锟斤拷锟斤拷 锟斤拷 锟斤拷锟秸匡拷始
}

/**
  * @brief :CC1101锟斤拷锟斤拷锟斤拷锟侥Ｊ?
  * @param :锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Set_Idle_Mode(void)
{
    CC1101_Write_Cmd(CC1101_SIDLE);
}

/**
  * @brief :CC1101锟斤拷始锟斤拷WOR锟斤拷锟斤拷
  * @param :锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void C1101_WOR_Init(void)
{
    CC1101_Write_Reg(CC1101_MCSM0, 0x18);
    CC1101_Write_Reg(CC1101_WORCTRL, 0x78);
    CC1101_Write_Reg(CC1101_MCSM2, 0x00);
    CC1101_Write_Reg(CC1101_WOREVT1, 0x8C);
    CC1101_Write_Reg(CC1101_WOREVT0, 0xA0);
    CC1101_Write_Cmd(CC1101_SWORRST); //写锟斤拷WOR锟斤拷锟斤拷
}

/**
  * @brief :CC1101锟斤拷锟矫碉拷址
  * @param :
  *			@Address锟斤拷锟斤拷锟矫碉拷锟借备锟斤拷址值
  *			@AddressMode锟斤拷锟斤拷址锟斤拷锟侥Ｊ?
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Set_Address(uint8_t Address, CC1101_AddrModeType AddressMode)
{
    uint8_t btmp = 0;

    btmp = CC1101_Read_Reg(CC1101_PKTCTRL1) & ~0x03; //锟斤拷取CC1101_PKTCTRL1锟侥达拷锟斤拷锟斤拷始值
    CC1101_Write_Reg(CC1101_ADDR, Address);          //锟斤拷锟斤拷锟借备锟斤拷址

    if (AddressMode == BROAD_ALL)
    {
    } //锟斤拷锟斤拷锟斤拷址
    else if (AddressMode == BROAD_NO)
    {
        btmp |= 0x01; //锟斤拷锟斤拷址 锟斤拷锟角诧拷锟斤拷锟姐播
    }
    else if (AddressMode == BROAD_0)
    {
        btmp |= 0x02; //0x00为锟姐播
    }
    else if (AddressMode == BROAD_0AND255)
    {
        btmp |= 0x03; //0x00 0xFF为锟姐播
    }

    CC1101_Write_Reg(CC1101_PKTCTRL1, btmp); //写锟斤拷锟街纺Ｊ?
}

/**
  * @brief :CC1101锟斤拷锟斤拷同锟斤拷锟街讹拷
  * @param :锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Set_Sync(uint16_t Sync)
{
    CC1101_Write_Reg(CC1101_SYNC1, 0xFF & (Sync >> 8));
    CC1101_Write_Reg(CC1101_SYNC0, 0xFF & Sync); //写锟斤拷同锟斤拷锟街讹拷 16Bit
}

/**
  * @brief :CC1101锟斤拷辗锟斤拷突锟斤拷锟斤拷锟?
  * @param :锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Clear_TxBuffer(void)
{
    CC1101_Set_Idle_Mode();        //锟斤拷锟饺斤拷锟斤拷IDLE模式
    CC1101_Write_Cmd(CC1101_SFTX); //写锟斤拷锟藉发锟酵伙拷锟斤拷锟斤拷锟斤拷锟斤拷
}

/**
  * @brief :CC1101锟斤拷战锟斤拷栈锟斤拷锟斤拷锟?
  * @param :锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Clear_RxBuffer(void)
{
    CC1101_Set_Idle_Mode();        //锟斤拷锟饺斤拷锟斤拷IDLE模式
    CC1101_Write_Cmd(CC1101_SFRX); //写锟斤拷锟斤拷锟斤拷栈锟斤拷锟斤拷锟斤拷锟斤拷锟?
}

/**
  * @brief :CC1101锟斤拷锟斤拷锟斤拷锟捷帮拷
  * @param :
  *			@pTxBuff锟斤拷锟斤拷锟斤拷锟斤拷锟捷伙拷锟斤拷锟斤拷
  *			@TxSize锟斤拷锟斤拷锟斤拷锟斤拷锟捷筹拷锟斤拷
  *			@DataMode锟斤拷锟斤拷锟斤拷模式
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Tx_Packet(uint8_t *pTxBuff, uint8_t TxSize, CC1101_TxDataModeType DataMode)
{
    uint8_t Address;
    uint16_t l_RxWaitTimeout = 0;

    if (DataMode == BROADCAST)
    {
        Address = 0;
    }
    else if (DataMode == ADDRESS_CHECK)
    {
        Address = CC1101_Read_Reg(CC1101_ADDR);
    }

    CC1101_Clear_TxBuffer();

    if ((CC1101_Read_Reg(CC1101_PKTCTRL1) & 0x03) != 0)
    {
        CC1101_Write_Reg(CC1101_TXFIFO, TxSize + 1);
        CC1101_Write_Reg(CC1101_TXFIFO, Address); //写锟诫长锟饺和碉拷址 锟斤拷锟节讹拷一锟斤拷锟街节碉拷址锟斤拷时锟斤拷锟斤拷应锟矫硷拷1
    }
    else
    {
        CC1101_Write_Reg(CC1101_TXFIFO, TxSize); //只写锟斤拷锟斤拷 锟斤拷锟斤拷锟斤拷址
    }

    CC1101_Write_Multi_Reg(CC1101_TXFIFO, pTxBuff, TxSize); //写锟斤拷锟斤拷锟斤拷
    CC1101_Set_Mode(TX_MODE);                               //锟斤拷锟斤拷模式

    while (0 == CC1101_GET_GDO0_STATUS()) //锟饺达拷锟斤拷锟斤拷锟斤拷锟?
    {
        drv_delay_ms(1);
        if (1000 == l_RxWaitTimeout++)
        {
            l_RxWaitTimeout = 0;
            CC1101_Init();
            break;
        }
    }
}

/**
  * @brief :CC1101锟斤拷取锟斤拷锟秸碉拷锟斤拷锟街斤拷锟斤拷
  * @param :锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷锟秸碉拷锟斤拷锟斤拷锟捷革拷锟斤拷
  */
uint8_t CC1101_Get_RxCounter(void)
{
    return (CC1101_Read_Status(CC1101_RXBYTES) & BYTES_IN_RXFIFO);
}

/**
  * @brief :CC1101锟斤拷锟斤拷锟斤拷锟捷帮拷
  * @param :
  *			@RxBuff锟斤拷锟斤拷锟斤拷锟斤拷锟捷伙拷锟斤拷锟斤拷
  * @note  :锟斤拷
  * @retval锟斤拷锟斤拷锟秸碉拷锟斤拷锟街斤拷锟斤拷锟斤拷0锟斤拷示锟斤拷锟斤拷锟斤拷
  */
uint8_t CC1101_Rx_Packet(uint8_t *RxBuff)
{
    uint8_t l_PktLen = 0;
    uint8_t l_Status[2] = {0};
    uint16_t l_RxWaitTimeout = 0;

    while (0 == CC1101_GET_GDO0_STATUS()) //锟饺达拷锟斤拷锟斤拷锟斤拷锟?
    {
        drv_delay_ms(1);
        if (3000 == l_RxWaitTimeout++)
        {
            l_RxWaitTimeout = 0;
            CC1101_Init();
            break;
        }
    }

    if (0 != CC1101_Get_RxCounter())
    {
        l_PktLen = CC1101_Read_Reg(CC1101_RXFIFO); // 锟斤拷取锟斤拷锟斤拷锟斤拷息

        if ((CC1101_Read_Reg(CC1101_PKTCTRL1) & 0x03) != 0)
        {
            CC1101_Read_Reg(CC1101_RXFIFO); //锟斤拷锟斤拷锟斤拷莅锟斤拷邪锟斤拷锟斤拷锟街凤拷锟较? 锟斤拷锟斤拷锟饺★拷锟街凤拷锟较?
        }
        if (l_PktLen == 0)
        {
            return 0; //锟斤拷锟斤拷锟斤拷
        }
        else
        {
            l_PktLen--; //锟斤拷去一锟斤拷锟斤拷址锟街斤拷
        }
        CC1101_Read_Multi_Reg(CC1101_RXFIFO, RxBuff, l_PktLen); //锟斤拷取锟斤拷锟斤拷
        CC1101_Read_Multi_Reg(CC1101_RXFIFO, l_Status, 2);      //锟斤拷取锟斤拷锟捷帮拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷纸冢锟斤拷锟揭伙拷锟轿狢RC锟斤拷志位

        CC1101_Clear_RxBuffer();

        if (l_Status[1] & CRC_OK)
        {
            return l_PktLen;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

/**
  * @brief :CC1101锟斤拷位
  * @param :锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Reset(void)
{
    CC1101_SET_CSN_HIGH();
    CC1101_SET_CSN_LOW();
    CC1101_SET_CSN_HIGH();
    drv_delay_ms(1);
    CC1101_Write_Cmd(CC1101_SRES);
}

/**
  * @brief :CC1101锟斤拷锟脚筹拷始锟斤拷
  * @param :锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
static void CC1101_Gpio_Init(void)
{
    //GDO0 GDO2锟斤拷锟斤拷为锟斤拷锟斤拷锟斤拷锟斤拷
    GPIO_Init(CC1101_GDO0_GPIO_PORT, CC1101_GDO0_GPIO_PIN, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(CC1101_GDO2_GPIO_PORT, CC1101_GDO2_GPIO_PIN, GPIO_Mode_In_PU_No_IT);
}

/**
  * @brief :CC1101锟斤拷始锟斤拷
  * @param :锟斤拷
  * @note  :锟斤拷
  * @retval:锟斤拷
  */
void CC1101_Init(void)
{
    uint8_t i = 0;

    CC1101_Gpio_Init();
    CC1101_Reset();

    for (i = 0; i < 22; i++)
    {
        CC1101_Write_Reg(CC1101InitData[i][0], CC1101InitData[i][1]);
    }
    CC1101_Set_Address(0x05, BROAD_0AND255); //锟借备锟斤拷址 锟斤拷 锟斤拷址锟斤拷锟侥Ｊ斤拷锟斤拷锟?
    CC1101_Set_Sync(0x8799);                 //同锟斤拷锟街讹拷锟斤拷锟斤拷
    CC1101_Write_Reg(CC1101_MDMCFG1, 0x72);  //锟斤拷锟狡斤拷锟斤拷锟斤拷锟斤拷锟?
		CC1101_Write_Reg(CC1101_MDMCFG3, 0x22);
		CC1101_Write_Reg(CC1101_MDMCFG4, 0x8c);

    CC1101_Write_Multi_Reg(CC1101_PATABLE, (uint8_t *)PaTabel, 8);
}
