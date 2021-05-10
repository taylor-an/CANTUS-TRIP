
#include <sys/syscall.h>
#include "cantus.h"

typedef struct
{
    U32 R1;
    U32 R2;
    U32 R3;
    U32 R4;
    U32 R5;
    U32 R6;
    U32 R7;
    U32 R8;
    U32 R9;
    U32 R10;
    U32 R11;
    U32 R12;
    U32 R13;
    U32 R14;
    U32 R15;
    U32 CR0;
    U32 CR1;
    U32 ML;
    U32 MH;
    U32 ER;
    U32 LR;
    U32 PC;
    U32 SR;
}AE32000_Reg;

static void DumpAllReg(U32* addr)
{

    char* sprname[8]={"CR0","CR1","ML","MH","ER","LR","PC","SR"};
    int i;
    debugstring("\r\n ======== DUMP ALL REGISTER ========\r\n");

    for (i=0;i<16;)
    {
        debugprintf("R%d = 0x%08x \t",i,addr[i]);
        i++;
        if (i % 4 == 0)
        {
            debugstring("\r\n");
        }
    }
    for (;i<24;)
    {
        debugprintf("%s = 0x%08x \t",sprname[i-16],addr[i]);
        i++;
        if (i % 4 == 0)
        {
            debugstring("\r\n");
        }
    }
    debugstring(" ===================================\r\n");
}

#pragma interrupt_handler
static void NOTUSEDISR(void)
{
    U32 INTST = *R_INTPEND;
    debugprintf(" NOT USED ISR : regst:0x%x\r\n",INTST);
    U32* registers = __builtin_frame_address(0);
    DumpAllReg(registers);
    asm("halt 1");
}


#pragma interrupt_handler
static void nmi_autovector(void)
{

    debugstring(" NMI ISR \r\n");
    U32* registers = __builtin_frame_address(0);
    DumpAllReg(registers);
    asm("halt 1");
}

#pragma interrupt_handler
static void double_fault_exception(void)
{
    debugstring(" Double Fault \r\n");
    U32* registers = __builtin_frame_address(0);
    DumpAllReg(registers);
    asm("halt 1");
}
#pragma interrupt_handler
static void bus_error_exception(void)
{

    debugstring(" == Bus Error ==  \r\n");
    U32* registers = __builtin_frame_address(0);
    DumpAllReg(registers);
    asm("halt 1");
}

#pragma interrupt_handler
static void coprocessor0_exception(void)
{
    debugstring(" == CP0 Execption ==\r\n");
    U32* registers = __builtin_frame_address(0);
    int register cp0status;
	
    asm("push %r0");
    asm("mvfc 0,%r15"); // coprocessor 15
    asm("sync");
	asm("lea (%%r0,0), %0" : "=r"(cp0status) :);
    asm("pop %r0");

    cp0status = (cp0status>>2) & 0xf;
    switch (cp0status)
    {
    case 0:
        debugstring("Access violation exception(Instruction)\r\n");
        break;
    case 1:
        debugstring("TLB miss exception(Instruction)\r\n");
        break;
    case 2:
        debugstring("Privilege violation exception\r\n");
        break;
    case 3:
        debugstring("Address alignment error exception(Data)\r\n");
        break;
    case 4:
        debugstring("Access violation exception(Data)\r\n");
        break;
    case 5:
        debugstring("TLB miss exception(Data)\r\n");
        break;
    case 6:
        debugstring("Write to Read-Only region(Data)\r\n");
        break;
    case 7:
        debugstring("Address alignment error exceptioin(Instruction)\r\n");
        break;
    }

    DumpAllReg(registers);

    asm("halt 1");
}

#pragma interrupt_handler
static void undefined_exception(void)
{
    debugstring(" undefined exception and system halted \r\n");
    U32* registers = __builtin_frame_address(0);
    DumpAllReg(registers);
    asm("halt 1");
}

#pragma interrupt_handler
static void unimplemented_exception(void)
{
    debugstring(" unimplemented exception and system halted \r\n");
    U32* registers = __builtin_frame_address(0);
    DumpAllReg(registers);
    asm("halt 1");
}


void swi1isr() __attribute__((weak));
#pragma interrupt_handler
void swi1isr() 
{
	debugstring("swi1 called, system halt\r\n");
	U32* registers = __builtin_frame_address(0);
	DumpAllReg(registers);
	asm("halt 1");
}

//this is system call fucntion..

U32 systemcallback(U32 func,U32 p1,U32 p2,U32 p3)
{
    //	int i;
    U32 ret=0; // 0 is OK

    switch (func)
    {
		case SYS_write:
			if (p1 == 1 || p1 == 2)//stdout, stderr.. p2 is string, p3 is length
			{
				char str[1024] ;
				strncpy(str,(char*)p2,p3);
				str[p3]=0;
				debugstring(str);
				ret = p3;
			}
			else
			{
				debugstring("SYS_write function\r\n");
				debugprintf("not support parameter : 0x%x\r\n",p1);
			}
        break;
		
		default :
			debugprintf("not supported systemcall parameter : func=0x%x, p1=0x%x, p2=0x%x, p3=0x%x,\r\n",func,p1,p2,p3);
        break;
    }
	
    return ret;
}
/* 1st arg : %r8
* 2st arg : %r9
* 3st arg : %sp + 0xc
* 4st arg : %sp + 0x10
All regsiter 16+6(cr0,cr1,ml,mh,%er,%lr) + 2(SR,PC)=24
*/

#pragma interrupt_handler
static void SystemCall()
{
    U32 ret;
    volatile U32 *registers;
    int func,p1,p2,p3;
	
    registers = __builtin_frame_address(0);

#if 1
    // ae32000-elf-gcc 3.x.x
	
    // _vgDumpAllReg(registers);
    func = registers[8];
    p1 = registers[9];
    p2 = registers[24+3];
    p3 = registers[24+4];

#else
    // ae32000-elf-gcc 4.x.x

    func = registers[8];
    p1 = registers[9];
    p2 = registers[10];
    p3 = registers[11];
#endif

    ret = systemcallback(func,p1,p2,p3);

    registers[8] = ret;
}

static void (*UserVector_table[32])(void) =
{
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
    NOTUSEDISR,
};


/*
#define ISRHOOK(A)	void SDK_ExternISR##A(void)\
{void (*fp)();fp = UserVector_table[A];fp();*R_PENDCLR = 0x20 + A;}
*/
#if 1
    // ae32000-elf-gcc 3.x.x

#define ISRHOOK(A)	void SDK_ExternISR##A(void) __attribute__((weak));\
	asm(".section .text");\
	asm(".type _SDK_ExternISR"#A", @function");\
	asm("_SDK_ExternISR"#A":");\
	asm("push	%cr0,%cr1,%ml,%mh,%er,%lr");\
	asm("push 	%r7");\
	asm("push 	%r8,%r9");\
	asm("ldi	0x20 + "#A",%r8");\
	asm("ldi	0x80020800,%r9");\
	asm("st		%r8,(%r9,0)");\
	asm("ldi	_UserVector_table +(4 * "#A") ,%r8");\
	asm("ld		(%r8,0),%r9");\
	asm("jalr	%r9");\
	asm("pop	%r8,%r9");\
	asm("pop	%r7");\
	asm("pop	%cr0,%cr1,%ml,%mh,%er,%lr,%pc,%sr");

#else
    // ae32000-elf-gcc 4.x.x

#define ISRHOOK(A)	void SDK_ExternISR##A(void) __attribute__((weak));\
	asm(".section .text");\
	asm(".type _SDK_ExternISR"#A", @function");\
	asm("_SDK_ExternISR"#A":");\
	asm("push	%cr0,%cr1,%ml,%mh,%er,%lr");\
	asm("push 	%r8,%r9,%r10,%r11");\
	asm("ldi	0x20 + "#A",%r8");\
	asm("ldi	0x80020800,%r9");\
	asm("st		%r8,(%r9,0)");\
	asm("ldi	_UserVector_table +(4 * "#A") ,%r8");\
	asm("ld		(%r8,0),%r9");\
	asm("jalr	%r9");\
	asm("pop	%r8,%r9,%r10,%r11");\
	asm("pop	%cr0,%cr1,%ml,%mh,%er,%lr,%pc,%sr");
#endif

ISRHOOK(0)

ISRHOOK(1)

ISRHOOK(2)

ISRHOOK(3)

ISRHOOK(4)

ISRHOOK(5)

ISRHOOK(6)

ISRHOOK(7)

ISRHOOK(8)

ISRHOOK(9)

ISRHOOK(10)

ISRHOOK(11)

ISRHOOK(12)

ISRHOOK(13)

ISRHOOK(14)

ISRHOOK(15)

ISRHOOK(16)

ISRHOOK(17)

ISRHOOK(18)

ISRHOOK(19)

ISRHOOK(20)

ISRHOOK(21)

ISRHOOK(22)

ISRHOOK(23)

ISRHOOK(24)

ISRHOOK(25)

ISRHOOK(26)

ISRHOOK(27)

ISRHOOK(28)

ISRHOOK(29)

ISRHOOK(30)

ISRHOOK(31)

// #define uC_OSIII

#ifdef uC_OSIII
extern void OSCtxSw();
extern void OSTickISR();
#endif

extern void Reset_Start();
typedef void (*fp)(void) ;
//void (*vector_table[])(void) __attribute__((section (".vects")))= {
fp vector_table[] __attribute__((section (".vects")))=
{
    Reset_Start                , /* V00 : Reset Vector          */
    nmi_autovector              , /* V01 : NMI Vector            */
    NOTUSEDISR        , /* V02 : Interrupt Auto Vector */
    double_fault_exception      , /* V03 : Double fault Vector   */
    bus_error_exception         , /* V04 : Bus Error Exception   */
    NOTUSEDISR                  , /* V05 : Reserved              */
    NOTUSEDISR                  , /* V06 : Reserved              */
    NOTUSEDISR                  , /* V07 : Reserved              */
    coprocessor0_exception      , /* V08 : Coprocessor0 Exception*/
    NOTUSEDISR      , /* V09 : Coprocessor1 Exception*/
    NOTUSEDISR     , /* V0A : Coprocessor2 Exception*/
    NOTUSEDISR      , /* V0B : Coprocessor3 Exception*/
    NOTUSEDISR            , /* V0C : OSI reset vector      */
    NOTUSEDISR         , /* V0D : OSI break exception   */
    undefined_exception         , /* V0E : Reserved              */
    unimplemented_exception     , /* V0F : Reserved              */
    NOTUSEDISR          , /* V10 : SWI 00 Vector         */
#ifdef uC_OSIII	
    OSCtxSw          , /* V11 : SWI 01 Vector         */
#else
    swi1isr          , /* V11 : SWI 01 Vector         */
#endif
    NOTUSEDISR          , /* V12 : SWI 02 Vector         */
    NOTUSEDISR          , /* V13 : SWI 03 Vector         */
    NOTUSEDISR          , /* V14 : SWI 04 Vector         */
    NOTUSEDISR          , /* V15 : SWI 05 Vector         */
    SystemCall          , /* V16 : SWI 06 Vector         */
    NOTUSEDISR          , /* V17 : SWI 07 Vector         */
    NOTUSEDISR         , /* V18 : SWI 08 Vector         */
    NOTUSEDISR         , /* V19 : SWI 09 Vector         */
    NOTUSEDISR          , /* V1A : SWI 0A Vector         */
    NOTUSEDISR          , /* V1B : SWI 0B Vector         */
    NOTUSEDISR          , /* V1C : SWI 0C Vector         */
    NOTUSEDISR          , /* V1D : SWI 0D Vector         */
    NOTUSEDISR          , /* V1E : SWI 0E Vector         */
    NOTUSEDISR         , /* V1F : SWI 0F Vector         */
    SDK_ExternISR0          , /* V20 : User Vector 0         */
#ifdef uC_OSIII
    OSTickISR          , /* V21 : User Vector 1         */
#else
    SDK_ExternISR1          , /* V21 : User Vector 1         */
#endif
    SDK_ExternISR2          , /* V22 : User Vector 2         */
    SDK_ExternISR3          , /* V23 : User Vector 3         */
    SDK_ExternISR4          , /* V24 : User Vector 4         */
    SDK_ExternISR5          , /* V25 : User Vector 5         */
    SDK_ExternISR6          , /* V26 : User Vector 6         */
    SDK_ExternISR7         , /* V27 : User Vector 7         */
    SDK_ExternISR8          , /* V28 : User Vector 8         */
    SDK_ExternISR9          , /* V29 : User Vector 9         */
    SDK_ExternISR10          , /* V2A : User Vector 10        */
    SDK_ExternISR11          , /* V2B : User Vector 11        */
    SDK_ExternISR12          , /* V2C : User Vector 12        */
    SDK_ExternISR13         , /* V2D : User Vector 13        */
    SDK_ExternISR14          , /* V2E : User Vector 14        */
    SDK_ExternISR15         , /* V2F : User Vector 15        */
    SDK_ExternISR16          , /* V30 : User Vector 16            */
    SDK_ExternISR17          , /* V31 : User Vector 17           */
    SDK_ExternISR18          , /* V32 : User Vector 18           */
    SDK_ExternISR19          , /* V33 : User Vector 19           */
    SDK_ExternISR20          , /* V34 : User Vector 20           */
    SDK_ExternISR21          , /* V35 : User Vector 21           */
    SDK_ExternISR22          , /* V36 : User Vector 22           */
    SDK_ExternISR23          , /* V37 : User Vector 23           */
    SDK_ExternISR24          , /* V38 : User Vector 24           */
    SDK_ExternISR25          , /* V39 : User Vector 25           */
    SDK_ExternISR26          , /* V3A : User Vector 26           */
    SDK_ExternISR27          , /* V3B : User Vector 27           */
    SDK_ExternISR28          , /* V3C : User Vector 28           */
    SDK_ExternISR29          , /* V3D : User Vector 29           */
    SDK_ExternISR30          , /* V3E : User Vector 30           */
    SDK_ExternISR31          , /* V3F : User Vector 31           */
};


void InitInterrupt()
{
    SETVECTORED;
    INTERRUPT_ENABLE;

    *R_INTEN = 0; //all disable
    *R_INTMASKCLR = 0xffffffff;// all interrupt disable
    //set vector base register
    asm("push %r0");
    asm("ldi	_vector_table,%r0");//program load address
    asm("mvtc	0,%r12");
    asm("sync");
    asm("pop %r0");

}


BOOL setinterrupt(INTERRUPT_TYPE intnum,void (*fp)())
{
    if (intnum >= INTNUM_MAX)
        return FALSE;

    UserVector_table[intnum]=fp;
    return TRUE;

}

void EnableInterrupt(INTERRUPT_TYPE num,BOOL b)
{
    CRITICAL_ENTER();
    if (!b) //disable
    {
        *R_INTEN &= (~(1 << num));
        *R_INTMASKCLR |= ( 1 << num);
    }
    else
    {
        *R_INTMASKSET = (1 << num);
        *R_INTEN |= ( 1 << num);
    }
    CRITICAL_EXIT();
}
