/*
    LED sample
*/

/*
 PIC24F MCU Peripheral Library 
 MPLAB C のインストールフォルダ内の Microchip PIC24F Peripheral Library.chm 参照
*/
#define USE_AND_OR 
#include<ports.h>
#include<timer.h>
#include<PwrMgnt.h>

/* コンフィグレーション */
 _CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
 _CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
 _CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
 _CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

unsigned short output = 0;

/* タイマー割り込み */
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void)
{
	if(output == 0)
	{
		output |= IOPORT_BIT_15; /* ポートを1に */
	}else{
		output &= ~IOPORT_BIT_15; /* ポートを0に */
	}
	/* PORT出力 RB15 */
	mPORTBWrite(output);
	/* タイマー割り込みクリア */
	T2_Clear_Intr_Status_Bit;
}

int main ( void )
{
	/* 全てのA/D変換ポートをI/Oに使用できるように設定 */
	AD1PCFG = 0xffff;
	/* クロックをPLL prescaler (1:1)で使用する */
    CLKDIV = 0x0000;

	/* PORTB RB15を出力に設定 */
	mPORTBOutputConfig(IOPORT_BIT_15);

	/* PORTBに0を出力 */
	mPORTBWrite(0);

	/* タイマー割り込みを有効に */
	ConfigIntTimer2(T2_INT_ON|T2_INT_PRIOR_1);
	/* タイマーを設定 */
	OpenTimer2(T2_ON | T2_PS_1_64 ,0x9000);

	/* メインループ */
    while (1)
    {
		/* IDLEモードに移行 */
		mPWRMGNT_GotoIdleMode();
	}
}
