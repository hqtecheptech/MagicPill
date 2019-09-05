



#include "../../../dev_app_conf.h"

#if defined HQ_HLXT && defined GSQY1_PROJECT

#include "GSQY1_HLXT_App.h"
#include "../HLXT_Fun.h"


//**** 控制皮带机运行 *****/
//void mHD_PDJ1_Ctr(void)
//{
//    static mHD_PDJ_F  pd1;
//    static mHD_PDJ_R pdctr;
//    static uint8_t remstart =0;
//    static uint8_t remstop =0;
//    int i;
//    static int tcnt;

//    //开始上升沿检测
//    if(remstart !=  mHD_Rem433Data.BT_11)  {
//        if(mHD_Rem433Data.BT_11 ==1)
//        {
//            pdctr.sf =1;
//        }
//        remstart = mHD_Rem433Data.BT_11;
//    }
//    //停止上升沿检测
//    if(remstop != mHD_Rem433Data.BT_21)  {
//        if(mHD_Rem433Data.BT_21 ==1)
//        {
//            pdctr.sf =0;
//        }
//        remstop = mHD_Rem433Data.BT_21;
//    }

//    pdctr.fk_mgn =1;
//    pd1 = mHD_HLXT_SXPD_Ctr(pdctr);

//    Dev_data.MData[1].DOutData[0] = pd1.r_mf;
//}

extern  mHD_DevRun_F mHD_Hydro_Ctr(mHD_DevRun_R buf);

void mHD_Valve_Test(void)
{
    static mHD_DevRun_R in;
    static mHD_DevRun_F out;
    static uint8_t remsf =0;
    static uint8_t remsr =0;
    static uint8_t remstop =0;
    static int tcnt;


    //正向上升沿检测
    if(remsf !=  mHD_Rem433Data.BT_24)  {
        if(mHD_Rem433Data.BT_24 ==1)
        {
            in.vf =1;
        }
        remsf = mHD_Rem433Data.BT_24;
    }

    //反向上升沿检测
    if(remsr !=  mHD_Rem433Data.BT_34)  {
        if(mHD_Rem433Data.BT_34 ==1)
        {
            in.vr =1;
        }
        remsr = mHD_Rem433Data.BT_34;
    }

    //停止上升沿检测
    if(remstop != mHD_Rem433Data.BT_44)  {
        if(mHD_Rem433Data.BT_44 ==1) {
             in.vf = in.vr =0;
             tcnt =0;
        }
        remstop = mHD_Rem433Data.BT_44;
    }

    in.pt = mHD_System_Clock;
    in.fk_mgn =1;
    in.fk_sf =1;

    if(tcnt ==0) {
        in.spt = in.pt+50;
        tcnt = 1;
    }
    out = mHD_Hydro_Ctr(in);

    Dev_data.MData[1].DOutData[0] = out.r_mf;
    Dev_data.MData[1].DOutData[1] = out.fvalve;
    Dev_data.MData[1].DOutData[2] = out.rvalve;
    Dev_data.MData[2].DOutData[3] = out.hvalve;
    Dev_data.MData[2].DOutData[4] = out.uvalve;
}

//mHD_DevRun_F mHD_Hydro_Ctr(mHD_DevRun_R buf)
//{
//    mHD_DevRun_F out;

//    if(((buf.vf) ||(buf.vr) )&& (buf.fk_mgn)&&(buf.fk_sr==0) )  out.r_mf =1;
//    else out.r_mf =0;

//     if((buf.fk_sf !=0)&&(buf.pt >=buf.spt))
//     {
//         if(buf.vf !=0) {
//             out.rvalve =0;  out.fvalve =1;
//             out.hvalve =1;  out.uvalve =1;
//         }else if(buf.vr !=0) {
//             out.rvalve =1;  out.fvalve =0;
//             out.hvalve =1;  out.uvalve =1;
//         }
//     } else {
//            out.rvalve =0;  out.fvalve =0;
//            out.hvalve =0;  out.uvalve =0;
//     }
//    return out;
//}







#endif
