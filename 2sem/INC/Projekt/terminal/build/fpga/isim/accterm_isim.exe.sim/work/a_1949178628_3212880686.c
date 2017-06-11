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
static const char *ng0 = "C:/FitkitSVN/apps/demo/terminal/fpga/sim/tb.vhd";



static void work_a_1949178628_3212880686_p_0(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    int64 t7;
    int64 t8;

LAB0:    t1 = (t0 + 3804U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(122, ng0);
    t2 = (t0 + 4180);
    t3 = (t2 + 32U);
    t4 = *((char **)t3);
    t5 = (t4 + 40U);
    t6 = *((char **)t5);
    *((unsigned char *)t6) = (unsigned char)3;
    xsi_driver_first_trans_fast(t2);
    xsi_set_current_line(123, ng0);
    t2 = (t0 + 3248U);
    t3 = *((char **)t2);
    t7 = *((int64 *)t3);
    t8 = (t7 / 2);
    t2 = (t0 + 3704);
    xsi_process_wait(t2, t8);

LAB6:    *((char **)t1) = &&LAB7;

LAB1:    return;
LAB4:    xsi_set_current_line(124, ng0);
    t2 = (t0 + 4180);
    t3 = (t2 + 32U);
    t4 = *((char **)t3);
    t5 = (t4 + 40U);
    t6 = *((char **)t5);
    *((unsigned char *)t6) = (unsigned char)2;
    xsi_driver_first_trans_fast(t2);
    xsi_set_current_line(125, ng0);
    t2 = (t0 + 3248U);
    t3 = *((char **)t2);
    t7 = *((int64 *)t3);
    t8 = (t7 / 2);
    t2 = (t0 + 3704);
    xsi_process_wait(t2, t8);

LAB10:    *((char **)t1) = &&LAB11;
    goto LAB1;

LAB5:    goto LAB4;

LAB7:    goto LAB5;

LAB8:    goto LAB2;

LAB9:    goto LAB8;

LAB11:    goto LAB9;

}

void work_a_1949178628_3212880686_sub_2769492575_3145898073(char *t0, char *t1, unsigned char t2)
{
    char t4[8];
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;
    char *t10;
    int64 t11;
    char *t12;
    char *t13;
    char *t14;
    char *t15;
    unsigned int t16;
    char *t17;
    char *t18;
    char *t19;

LAB0:    t5 = (t4 + 4U);
    *((unsigned char *)t5) = t2;
    t6 = (t0 + 4216);
    t7 = (t6 + 32U);
    t8 = *((char **)t7);
    t9 = (t8 + 40U);
    t10 = *((char **)t9);
    *((unsigned char *)t10) = t2;
    xsi_driver_first_trans_fast(t6);
    t11 = (7 * 1000000LL);
    xsi_process_wait(t1, t11);

LAB5:    t6 = (t1 + 48U);
    t7 = *((char **)t6);
    t8 = (t7 + 1440U);
    *((unsigned int *)t8) = 1U;
    t9 = (t1 + 48U);
    t10 = *((char **)t9);
    t12 = (t10 + 0U);
    getcontext(t12);
    t13 = (t1 + 48U);
    t14 = *((char **)t13);
    t15 = (t14 + 1440U);
    t16 = *((unsigned int *)t15);
    if (t16 == 1)
        goto LAB6;

LAB7:    t17 = (t1 + 48U);
    t18 = *((char **)t17);
    t19 = (t18 + 1440U);
    *((unsigned int *)t19) = 3U;

LAB3:
LAB4:
LAB2:    t6 = (t0 + 4216);
    t7 = (t6 + 32U);
    t8 = *((char **)t7);
    t9 = (t8 + 40U);
    t10 = *((char **)t9);
    *((unsigned char *)t10) = (unsigned char)32;
    xsi_driver_first_trans_fast(t6);
    t11 = (7 * 1000000LL);
    xsi_process_wait(t1, t11);

LAB11:    t6 = (t1 + 48U);
    t7 = *((char **)t6);
    t8 = (t7 + 1440U);
    *((unsigned int *)t8) = 1U;
    t9 = (t1 + 48U);
    t10 = *((char **)t9);
    t12 = (t10 + 0U);
    getcontext(t12);
    t13 = (t1 + 48U);
    t14 = *((char **)t13);
    t15 = (t14 + 1440U);
    t16 = *((unsigned int *)t15);
    if (t16 == 1)
        goto LAB12;

LAB13:    t17 = (t1 + 48U);
    t18 = *((char **)t17);
    t19 = (t18 + 1440U);
    *((unsigned int *)t19) = 3U;

LAB9:
LAB10:
LAB8:
LAB1:    return;
LAB6:    xsi_saveStackAndSuspend(t1);
    goto LAB7;

LAB12:    xsi_saveStackAndSuspend(t1);
    goto LAB13;

}

static void work_a_1949178628_3212880686_p_1(char *t0)
{
    char *t1;
    char *t2;
    int64 t3;

LAB0:    t1 = (t0 + 3948U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(142, ng0);
    t3 = (40 * 1000000LL);
    t2 = (t0 + 3848);
    xsi_process_wait(t2, t3);

LAB6:    *((char **)t1) = &&LAB7;

LAB1:    return;
LAB4:    xsi_set_current_line(143, ng0);
    t2 = (t0 + 3848);
    work_a_1949178628_3212880686_sub_2769492575_3145898073(t0, t2, (unsigned char)49);
    xsi_set_current_line(144, ng0);
    t2 = (t0 + 3848);
    work_a_1949178628_3212880686_sub_2769492575_3145898073(t0, t2, (unsigned char)50);
    xsi_set_current_line(145, ng0);
    t2 = (t0 + 3848);
    work_a_1949178628_3212880686_sub_2769492575_3145898073(t0, t2, (unsigned char)51);
    xsi_set_current_line(146, ng0);
    t2 = (t0 + 3848);
    work_a_1949178628_3212880686_sub_2769492575_3145898073(t0, t2, (unsigned char)35);
    xsi_set_current_line(148, ng0);

LAB10:    *((char **)t1) = &&LAB11;
    goto LAB1;

LAB5:    goto LAB4;

LAB7:    goto LAB5;

LAB8:    goto LAB2;

LAB9:    goto LAB8;

LAB11:    goto LAB9;

}


extern void work_a_1949178628_3212880686_init()
{
	static char *pe[] = {(void *)work_a_1949178628_3212880686_p_0,(void *)work_a_1949178628_3212880686_p_1};
	static char *se[] = {(void *)work_a_1949178628_3212880686_sub_2769492575_3145898073};
	xsi_register_didat("work_a_1949178628_3212880686", "isim/accterm_isim.exe.sim/work/a_1949178628_3212880686.didat");
	xsi_register_executes(pe);
	xsi_register_subprogram_executes(se);
}
