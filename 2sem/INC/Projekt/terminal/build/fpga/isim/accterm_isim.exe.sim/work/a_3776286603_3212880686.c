/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0x79f3f3a8 */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "C:/FitkitSVN/apps/demo/terminal/fpga/fpga_mcu_dbg.vhd";
extern char *IEEE_P_2592010699;
extern char *IEEE_P_3620187407;

unsigned char ieee_p_2592010699_sub_1605435078_503743352(char *, unsigned char , unsigned char );
unsigned char ieee_p_2592010699_sub_1690584930_503743352(char *, unsigned char );
unsigned char ieee_p_2592010699_sub_2545490612_503743352(char *, unsigned char , unsigned char );
char *ieee_p_3620187407_sub_436279890_3965413181(char *, char *, char *, char *, int );


static void work_a_3776286603_3212880686_p_0(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    int t5;
    unsigned int t6;
    unsigned int t7;
    unsigned int t8;
    unsigned char t9;
    unsigned char t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;
    char *t15;
    char *t16;

LAB0:    xsi_set_current_line(91, ng0);

LAB3:    t1 = (t0 + 2432U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 2156U);
    t4 = *((char **)t1);
    t5 = (0 - 15);
    t6 = (t5 * -1);
    t7 = (1U * t6);
    t8 = (0 + t7);
    t1 = (t4 + t8);
    t9 = *((unsigned char *)t1);
    t10 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t3, t9);
    t11 = (t0 + 5684);
    t12 = (t11 + 32U);
    t13 = *((char **)t12);
    t14 = (t13 + 40U);
    t15 = *((char **)t14);
    *((unsigned char *)t15) = t10;
    xsi_driver_first_trans_fast(t11);

LAB2:    t16 = (t0 + 5584);
    *((int *)t16) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_3776286603_3212880686_p_1(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    int t5;
    unsigned int t6;
    unsigned int t7;
    unsigned int t8;
    unsigned char t9;
    unsigned char t10;
    unsigned char t11;
    char *t12;
    char *t13;
    char *t14;
    char *t15;
    char *t16;
    char *t17;

LAB0:    xsi_set_current_line(92, ng0);

LAB3:    t1 = (t0 + 2432U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 2156U);
    t4 = *((char **)t1);
    t5 = (0 - 15);
    t6 = (t5 * -1);
    t7 = (1U * t6);
    t8 = (0 + t7);
    t1 = (t4 + t8);
    t9 = *((unsigned char *)t1);
    t10 = ieee_p_2592010699_sub_1690584930_503743352(IEEE_P_2592010699, t9);
    t11 = ieee_p_2592010699_sub_1605435078_503743352(IEEE_P_2592010699, t3, t10);
    t12 = (t0 + 5720);
    t13 = (t12 + 32U);
    t14 = *((char **)t13);
    t15 = (t14 + 40U);
    t16 = *((char **)t15);
    *((unsigned char *)t16) = t11;
    xsi_driver_first_trans_fast(t12);

LAB2:    t17 = (t0 + 5592);
    *((int *)t17) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_3776286603_3212880686_p_2(char *t0)
{
    char *t1;
    char *t2;
    unsigned int t3;
    unsigned int t4;
    unsigned int t5;
    char *t6;
    char *t7;
    int t8;
    char *t9;
    char *t10;
    int t11;
    char *t12;
    int t14;
    char *t15;
    int t17;
    char *t18;
    int t20;
    char *t21;
    int t23;
    char *t24;
    int t26;
    char *t27;
    int t29;
    char *t30;
    int t32;
    char *t33;
    int t35;
    char *t36;
    int t38;
    char *t39;
    int t41;
    char *t42;
    int t44;
    char *t45;
    int t47;
    char *t48;
    int t50;
    char *t51;
    int t53;
    char *t54;
    char *t56;
    char *t57;
    char *t58;
    char *t59;
    char *t60;

LAB0:    xsi_set_current_line(96, ng0);
    t1 = (t0 + 2248U);
    t2 = *((char **)t1);
    t3 = (7 - 3);
    t4 = (t3 * 1U);
    t5 = (0 + t4);
    t1 = (t2 + t5);
    t6 = (t0 + 10768);
    t8 = xsi_mem_cmp(t6, t1, 4U);
    if (t8 == 1)
        goto LAB3;

LAB20:    t9 = (t0 + 10772);
    t11 = xsi_mem_cmp(t9, t1, 4U);
    if (t11 == 1)
        goto LAB4;

LAB21:    t12 = (t0 + 10776);
    t14 = xsi_mem_cmp(t12, t1, 4U);
    if (t14 == 1)
        goto LAB5;

LAB22:    t15 = (t0 + 10780);
    t17 = xsi_mem_cmp(t15, t1, 4U);
    if (t17 == 1)
        goto LAB6;

LAB23:    t18 = (t0 + 10784);
    t20 = xsi_mem_cmp(t18, t1, 4U);
    if (t20 == 1)
        goto LAB7;

LAB24:    t21 = (t0 + 10788);
    t23 = xsi_mem_cmp(t21, t1, 4U);
    if (t23 == 1)
        goto LAB8;

LAB25:    t24 = (t0 + 10792);
    t26 = xsi_mem_cmp(t24, t1, 4U);
    if (t26 == 1)
        goto LAB9;

LAB26:    t27 = (t0 + 10796);
    t29 = xsi_mem_cmp(t27, t1, 4U);
    if (t29 == 1)
        goto LAB10;

LAB27:    t30 = (t0 + 10800);
    t32 = xsi_mem_cmp(t30, t1, 4U);
    if (t32 == 1)
        goto LAB11;

LAB28:    t33 = (t0 + 10804);
    t35 = xsi_mem_cmp(t33, t1, 4U);
    if (t35 == 1)
        goto LAB12;

LAB29:    t36 = (t0 + 10808);
    t38 = xsi_mem_cmp(t36, t1, 4U);
    if (t38 == 1)
        goto LAB13;

LAB30:    t39 = (t0 + 10812);
    t41 = xsi_mem_cmp(t39, t1, 4U);
    if (t41 == 1)
        goto LAB14;

LAB31:    t42 = (t0 + 10816);
    t44 = xsi_mem_cmp(t42, t1, 4U);
    if (t44 == 1)
        goto LAB15;

LAB32:    t45 = (t0 + 10820);
    t47 = xsi_mem_cmp(t45, t1, 4U);
    if (t47 == 1)
        goto LAB16;

LAB33:    t48 = (t0 + 10824);
    t50 = xsi_mem_cmp(t48, t1, 4U);
    if (t50 == 1)
        goto LAB17;

LAB34:    t51 = (t0 + 10828);
    t53 = xsi_mem_cmp(t51, t1, 4U);
    if (t53 == 1)
        goto LAB18;

LAB35:
LAB19:    xsi_set_current_line(113, ng0);
    t1 = xsi_get_transient_memory(16U);
    memset(t1, 0, 16U);
    t2 = t1;
    memset(t2, (unsigned char)2, 16U);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);

LAB2:    t1 = (t0 + 5600);
    *((int *)t1) = 1;

LAB1:    return;
LAB3:    xsi_set_current_line(97, ng0);
    t54 = (t0 + 10832);
    t56 = (t0 + 5756);
    t57 = (t56 + 32U);
    t58 = *((char **)t57);
    t59 = (t58 + 40U);
    t60 = *((char **)t59);
    memcpy(t60, t54, 16U);
    xsi_driver_first_trans_fast_port(t56);
    goto LAB2;

LAB4:    xsi_set_current_line(98, ng0);
    t1 = (t0 + 10848);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB5:    xsi_set_current_line(99, ng0);
    t1 = (t0 + 10864);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB6:    xsi_set_current_line(100, ng0);
    t1 = (t0 + 10880);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB7:    xsi_set_current_line(101, ng0);
    t1 = (t0 + 10896);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB8:    xsi_set_current_line(102, ng0);
    t1 = (t0 + 10912);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB9:    xsi_set_current_line(103, ng0);
    t1 = (t0 + 10928);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB10:    xsi_set_current_line(104, ng0);
    t1 = (t0 + 10944);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB11:    xsi_set_current_line(105, ng0);
    t1 = (t0 + 10960);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB12:    xsi_set_current_line(106, ng0);
    t1 = (t0 + 10976);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB13:    xsi_set_current_line(107, ng0);
    t1 = (t0 + 10992);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB14:    xsi_set_current_line(108, ng0);
    t1 = (t0 + 11008);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB15:    xsi_set_current_line(109, ng0);
    t1 = (t0 + 11024);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB16:    xsi_set_current_line(110, ng0);
    t1 = (t0 + 11040);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB17:    xsi_set_current_line(111, ng0);
    t1 = (t0 + 11056);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB18:    xsi_set_current_line(112, ng0);
    t1 = (t0 + 11072);
    t6 = (t0 + 5756);
    t7 = (t6 + 32U);
    t9 = *((char **)t7);
    t10 = (t9 + 40U);
    t12 = *((char **)t10);
    memcpy(t12, t1, 16U);
    xsi_driver_first_trans_fast_port(t6);
    goto LAB2;

LAB36:;
}

static void work_a_3776286603_3212880686_p_3(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;

LAB0:    xsi_set_current_line(116, ng0);

LAB3:    t1 = (t0 + 2524U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 5792);
    t4 = (t1 + 32U);
    t5 = *((char **)t4);
    t6 = (t5 + 40U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast_port(t1);

LAB2:    t8 = (t0 + 5608);
    *((int *)t8) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_3776286603_3212880686_p_4(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;

LAB0:    xsi_set_current_line(117, ng0);

LAB3:    t1 = (t0 + 2616U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 5828);
    t4 = (t1 + 32U);
    t5 = *((char **)t4);
    t6 = (t5 + 40U);
    t7 = *((char **)t6);
    *((unsigned char *)t7) = t3;
    xsi_driver_first_trans_fast_port(t1);

LAB2:    t8 = (t0 + 5616);
    *((int *)t8) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_3776286603_3212880686_p_5(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    unsigned char t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;
    char *t10;
    char *t12;
    char *t13;
    char *t14;
    char *t15;
    char *t16;
    char *t17;

LAB0:    xsi_set_current_line(140, ng0);
    t1 = (t0 + 1512U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t4 = (t3 == (unsigned char)3);
    if (t4 != 0)
        goto LAB3;

LAB4:
LAB5:    t10 = (t0 + 11088);
    t12 = (t0 + 5864);
    t13 = (t12 + 32U);
    t14 = *((char **)t13);
    t15 = (t14 + 40U);
    t16 = *((char **)t15);
    memcpy(t16, t10, 8U);
    xsi_driver_first_trans_fast(t12);

LAB2:    t17 = (t0 + 5624);
    *((int *)t17) = 1;

LAB1:    return;
LAB3:    t1 = (t0 + 1420U);
    t5 = *((char **)t1);
    t1 = (t0 + 5864);
    t6 = (t1 + 32U);
    t7 = *((char **)t6);
    t8 = (t7 + 40U);
    t9 = *((char **)t8);
    memcpy(t9, t5, 8U);
    xsi_driver_first_trans_fast(t1);
    goto LAB2;

LAB6:    goto LAB2;

}

static void work_a_3776286603_3212880686_p_6(char *t0)
{
    char *t1;
    char *t2;
    unsigned char t3;
    char *t4;
    unsigned char t5;
    unsigned char t6;
    char *t7;
    char *t8;
    char *t9;
    char *t10;
    char *t11;

LAB0:    xsi_set_current_line(141, ng0);

LAB3:    t1 = (t0 + 1512U);
    t2 = *((char **)t1);
    t3 = *((unsigned char *)t2);
    t1 = (t0 + 1604U);
    t4 = *((char **)t1);
    t5 = *((unsigned char *)t4);
    t6 = ieee_p_2592010699_sub_2545490612_503743352(IEEE_P_2592010699, t3, t5);
    t1 = (t0 + 5900);
    t7 = (t1 + 32U);
    t8 = *((char **)t7);
    t9 = (t8 + 40U);
    t10 = *((char **)t9);
    *((unsigned char *)t10) = t6;
    xsi_driver_first_trans_fast(t1);

LAB2:    t11 = (t0 + 5632);
    *((int *)t11) = 1;

LAB1:    return;
LAB4:    goto LAB2;

}

static void work_a_3776286603_3212880686_p_7(char *t0)
{
    char t17[16];
    unsigned char t1;
    char *t2;
    unsigned char t3;
    char *t4;
    char *t5;
    unsigned char t6;
    unsigned char t7;
    char *t8;
    unsigned char t9;
    unsigned char t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;
    char *t15;
    char *t16;
    unsigned int t18;
    unsigned int t19;
    unsigned int t20;

LAB0:    xsi_set_current_line(145, ng0);
    t2 = (t0 + 568U);
    t3 = xsi_signal_has_event(t2);
    if (t3 == 1)
        goto LAB5;

LAB6:    t1 = (unsigned char)0;

LAB7:    if (t1 != 0)
        goto LAB2;

LAB4:
LAB3:    t2 = (t0 + 5640);
    *((int *)t2) = 1;

LAB1:    return;
LAB2:    xsi_set_current_line(146, ng0);
    t4 = (t0 + 2616U);
    t8 = *((char **)t4);
    t9 = *((unsigned char *)t8);
    t10 = (t9 == (unsigned char)3);
    if (t10 != 0)
        goto LAB8;

LAB10:    t2 = (t0 + 2800U);
    t4 = *((char **)t2);
    t1 = *((unsigned char *)t4);
    t3 = (t1 == (unsigned char)3);
    if (t3 != 0)
        goto LAB11;

LAB12:
LAB9:    goto LAB3;

LAB5:    t4 = (t0 + 592U);
    t5 = *((char **)t4);
    t6 = *((unsigned char *)t5);
    t7 = (t6 == (unsigned char)3);
    t1 = t7;
    goto LAB7;

LAB8:    xsi_set_current_line(147, ng0);
    t4 = xsi_get_transient_memory(6U);
    memset(t4, 0, 6U);
    t11 = t4;
    memset(t11, (unsigned char)2, 6U);
    t12 = (t0 + 5936);
    t13 = (t12 + 32U);
    t14 = *((char **)t13);
    t15 = (t14 + 40U);
    t16 = *((char **)t15);
    memcpy(t16, t4, 6U);
    xsi_driver_first_trans_delta(t12, 10U, 6U, 0LL);
    goto LAB9;

LAB11:    xsi_set_current_line(149, ng0);
    t2 = (t0 + 2984U);
    t5 = *((char **)t2);
    t18 = (15 - 5);
    t19 = (t18 * 1U);
    t20 = (0 + t19);
    t2 = (t5 + t20);
    t8 = (t0 + 10600U);
    t11 = ieee_p_3620187407_sub_436279890_3965413181(IEEE_P_3620187407, t17, t2, t8, 1);
    t12 = (t0 + 5936);
    t13 = (t12 + 32U);
    t14 = *((char **)t13);
    t15 = (t14 + 40U);
    t16 = *((char **)t15);
    memcpy(t16, t11, 6U);
    xsi_driver_first_trans_delta(t12, 10U, 6U, 0LL);
    goto LAB9;

}


extern void work_a_3776286603_3212880686_init()
{
	static char *pe[] = {(void *)work_a_3776286603_3212880686_p_0,(void *)work_a_3776286603_3212880686_p_1,(void *)work_a_3776286603_3212880686_p_2,(void *)work_a_3776286603_3212880686_p_3,(void *)work_a_3776286603_3212880686_p_4,(void *)work_a_3776286603_3212880686_p_5,(void *)work_a_3776286603_3212880686_p_6,(void *)work_a_3776286603_3212880686_p_7};
	xsi_register_didat("work_a_3776286603_3212880686", "isim/accterm_isim.exe.sim/work/a_3776286603_3212880686.didat");
	xsi_register_executes(pe);
}
