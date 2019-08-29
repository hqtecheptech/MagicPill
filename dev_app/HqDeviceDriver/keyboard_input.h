#ifndef __KEYBOARD_INPUT_H
#define __KEYBOARD_INPUT_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

extern char mHD_keyboard_InBuf[] ;
extern int mHD_Keyboard_Finish;

void keyboard_prumsg_poll(char mtfg[20][50],int msgtype);
void keyboard_prudata(char (*mtf)[128]);
void  keyboard_moduleconfig(char (*mtf)[128]);
void keyboard_moduledata(char (*mtf)[128]);
void keyboard_prumsg(char (*mtf)[128],int msgtype);//处理键盘输入的pru msg 信息

int  mHD_Debug_KeyboardInput_Init(void); //键盘输入 调试



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
