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

#include "xsi.h"

struct XSI_INFO xsi_info;

char *IEEE_P_2592010699;
char *STD_STANDARD;
char *IEEE_P_3499444699;
char *UNISIM_P_0947159679;
char *IEEE_P_1242562249;
char *IEEE_P_3620187407;
char *WORK_P_3986149543;
char *IEEE_P_3564397177;
char *STD_TEXTIO;
char *UNISIM_P_3222816464;
char *IEEE_P_2717149903;
char *IEEE_P_1367372525;


int main(int argc, char **argv)
{
    xsi_init_design(argc, argv);
    xsi_register_info(&xsi_info);

    xsi_register_min_prec_unit(-12);
    ieee_p_2592010699_init();
    ieee_p_3499444699_init();
    ieee_p_3620187407_init();
    std_textio_init();
    ieee_p_3564397177_init();
    ieee_p_1242562249_init();
    ieee_p_2717149903_init();
    ieee_p_1367372525_init();
    unisim_p_0947159679_init();
    work_a_3764953092_0478353704_init();
    work_a_1613306508_3212880686_init();
    work_a_4275660748_3212880686_init();
    work_a_4091158095_3212880686_init();
    work_a_1480518215_3212880686_init();
    work_a_3702425192_3212880686_init();
    work_a_1026520565_3212880686_init();
    work_a_2795304073_3212880686_init();
    work_a_3643194780_3212880686_init();
    unisim_p_3222816464_init();
    work_a_2974611363_0394840199_init();
    work_p_3986149543_init();
    work_a_0593422272_4114004269_init();
    unisim_a_3658712214_1768247982_init();
    work_a_3776286603_3212880686_init();
    work_a_1599429066_0286164271_init();
    work_a_0336150939_3212880686_init();
    work_a_2961494699_3212880686_init();
    work_a_1949178628_3212880686_init();


    xsi_register_tops("work_a_1949178628_3212880686");

    IEEE_P_2592010699 = xsi_get_engine_memory("ieee_p_2592010699");
    xsi_register_ieee_std_logic_1164(IEEE_P_2592010699);
    STD_STANDARD = xsi_get_engine_memory("std_standard");
    IEEE_P_3499444699 = xsi_get_engine_memory("ieee_p_3499444699");
    UNISIM_P_0947159679 = xsi_get_engine_memory("unisim_p_0947159679");
    IEEE_P_1242562249 = xsi_get_engine_memory("ieee_p_1242562249");
    IEEE_P_3620187407 = xsi_get_engine_memory("ieee_p_3620187407");
    WORK_P_3986149543 = xsi_get_engine_memory("work_p_3986149543");
    IEEE_P_3564397177 = xsi_get_engine_memory("ieee_p_3564397177");
    STD_TEXTIO = xsi_get_engine_memory("std_textio");
    UNISIM_P_3222816464 = xsi_get_engine_memory("unisim_p_3222816464");
    IEEE_P_2717149903 = xsi_get_engine_memory("ieee_p_2717149903");
    IEEE_P_1367372525 = xsi_get_engine_memory("ieee_p_1367372525");

    return xsi_run_simulation(argc, argv);

}
