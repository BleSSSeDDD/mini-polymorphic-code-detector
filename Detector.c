#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>
#include <stdint.h>
struct REGISTERS
{
    uint16_t AX;
    uint16_t BX;
    uint16_t CX;
    uint16_t DX;
    uint16_t SI;
    uint16_t DI;
    uint16_t BP;
    uint16_t SP;
    uint16_t DS;
    uint16_t ES;
    uint16_t CS;
    uint16_t SS;

};

struct REGISTERS REGISTER = {0, 0, 0, 0, 0, 0, 0, 254, 18653, 18653, 18653, 18653};

uint16_t stack[255] = {0};

uint16_t xor_ax_imm16 = 53;
uint16_t xor_r_r = 51;

uint16_t push = 80;
uint16_t pop = 88;

uint16_t mov_r_r = 139;
uint16_t mov_r_imm16 = 184;
uint16_t mov_r_sr=140;
uint16_t mov_sr_r=142;

uint16_t inc = 64;

uint16_t dec = 72;

uint16_t add_ax = 5;
uint16_t add_r_r = 3;

uint16_t xchg_ax = 144;
uint16_t xchg_r_r = 135;

uint16_t sub_ax =45;
uint16_t add_sub_xor_reg_imm16=129;
uint16_t add_sub_xor_reg_imm8=131;
uint16_t sub_reg_reg=43;

uint16_t stosw = 171;

int MOV_R_R(uint16_t OP1, uint16_t OP2);
int MOV_R_IMM16(uint16_t REG1, uint16_t IMM16);
int MOV_R_SR(uint16_t REG, uint16_t SR);
int MOV_SR_R(uint16_t SR, uint16_t REG);

int PUSH(uint8_t OP1);
int POP(uint8_t OP1);

int INC(uint8_t OP1);
int DEC(uint8_t OP1);

int ADD_AX(uint16_t imm);
int ADD_R_IMM16(uint16_t REG, uint16_t IMM16);
int ADD_R_IMM8(uint16_t REG, uint8_t IMM8);
int ADD_R_R(uint16_t reg1, uint16_t reg2);

int XCHG_AX( uint8_t OP1);
int XCHG_R_R(uint16_t REG1, uint16_t REG2);

int SUB_AX(uint16_t imm);
int SUB_R_IMM16(uint16_t reg, uint16_t IMM16);
int SUB_R_IMM8(uint16_t reg, uint8_t IMM8);
int SUB_R_R(uint16_t reg1, uint16_t reg2);

int XOR_R_R(uint16_t REG1, uint16_t REG2);
int XOR_AX_IMM16(uint16_t IMM16);
int XOR_R_IMM16(uint16_t REG, uint16_t IMM16);
int XOR_R_IMM8(uint16_t REG, uint8_t IMM8);

int STOSW(uint16_t *DATA);

int Size(FILE *f, size_t* size)
{
    if (f == NULL) return 1;
    if (size == NULL) return 2;
    if(fseek(f, 0, SEEK_END)) return 3;
    if (ftell(f) == -1) return 4;
    *size = ftell(f);
    if(fseek(f, 0, SEEK_SET)) return 5;
    return 0;
}

int EmptyStack()
{
  return REGISTER.SP == -1 ? 1 : 0;
}

int FullStack()
{
  return REGISTER.SP == 99 ? 1 : 0;
}
unsigned short reverse(unsigned short x)
{
    x = (x & 0xFF) << 8 | (x & 0xFF00) >>  8;
    return x;
}

int main()
{
    char Str[100];

    char Path[100];

    uint16_t DATA[0xFFFF]={0};
    int64_t i = -1;
    size_t j,k=0;
    FILE *f;

    size_t size;

    uint8_t byte[10000];

    Str[0] = '\0';

    if(printf("\n\n\t\t-----------------------POLYMORPHIC CODE DETECTOR-----------------------\n\n\n\n") != 79) return 1;

    if(printf("This program detects polymorphic code by searching for a signature in a file. "
              "You need to enter the full path to the .com file."
            "Path should look like this: \"C:/.../application.com\" or \"C:\\...\\application.com\". "
             "After that, a message will be displayed in the console indicating whether"
             " a signature has been found in the file.\n\n") != 324) return 2;
    while(1)
    {
        if(printf("Enter the path: ")!=  16) return 3;
        scanf("%99[^\n]s", Path);
        if(fflush(stdin) == EOF) return 4;
        f = fopen(Path, "rb");
        if (f == NULL)
            {
                if(printf("Wrong pass, try again:\n")!= 23) return 5;
                continue;
            }
        break;
    }
    Size(f, &size);
    if (size == 18446744073709551615)
    {
        if(printf("\nfile size is too large, error\n")!= 30) return 6;
        return 7;
    }

    if(fread(byte, 1, size, f) != size) return 8;

    while((i <= size) || (i==-1))
    {
        i++;
        if(byte[i]== 52513) break;
        if (sub_ax==byte[i])
        {
            if (byte[i+1]==0)
            {
                SUB_AX( reverse(byte[i+2]));
                i=i+2;
            }
            else
            {
            SUB_AX( reverse(byte[i+2])+byte[i+1]);
            i=i+2;
            }
            continue;
        }
        if (sub_reg_reg==byte[i])
        {

            SUB_R_R((byte[i+1]%64)/8,(byte[i+1]%8));
            i++;
            continue;
        }
        if (add_ax == byte[i])
        {
            if (byte[i+1]==0)
            {

                ADD_AX( reverse(byte[i+2]));
                i=i+2;

            }
            else
            {
                ADD_AX( reverse(byte[i+2])+byte[i+1]);
                i=i+2;
            }
            continue;
        }
        if (add_sub_xor_reg_imm16 == byte[i])
        {
            if((byte[i+1]%64)/8==0x5)
            {
                SUB_R_IMM16(byte[i+1]%8,reverse(byte[i+3])+byte[i+2]);
                i=i+3;
            }
            else if((byte[i+1]%64)/8==6)
            {
                XOR_R_IMM16(byte[i+1]%8, reverse(byte[i+3])+byte[i+2]);
                i=i+3;
            }
            else
            {
                ADD_R_IMM16(byte[i+1]%8,reverse(byte[i+3])+byte[i+2]);
                i=i+3;
            }
            continue;
        }

        if (add_sub_xor_reg_imm8 == byte[i])
        {

            if((byte[i+1]%64)/8==0x5)
            {
                SUB_R_IMM8(byte[i+1]%8,  reverse(byte[i+2]));
                i=i+2;
            }
            else if((byte[i+1]%64)/8==6)
            {
                XOR_R_IMM8(byte[i+1]%8, reverse(byte[i+2]));
                i=i+2;
            }
            else
            {
                ADD_R_IMM8(byte[i+1]%8, reverse(byte[i+2]));
                i=i+2;
            }
            continue;
        }

        if (add_r_r== byte[i])
        {
            ADD_R_R((byte[i+1]%64)/8,(byte[i+1]%8));
            i++;
        }


        if (mov_r_imm16 == (byte[i] - (byte[i]%8)))
        {
           if (byte[i+1]==0)
            {

                MOV_R_IMM16(byte[i]%8, reverse(byte[i+2]));
                i=i+2;
            }
            else
            {

                MOV_R_IMM16(byte[i]%8, reverse(byte[i+2])+byte[i+1]);
                i=i+2;
            }
            continue;
        }
        if (mov_r_r==byte[i])
        {
            MOV_R_R((byte[i+1]%64)/8,byte[i+1]%8);
            i=i+1;
            continue;
        }
        if (mov_r_sr==byte[i])
        {
            MOV_R_SR(byte[i]%8,(byte[i+1]%64)/8);
            i++;
            continue;
        }
        if (mov_sr_r==byte[i])
        {
            MOV_SR_R((byte[i+1]%64)/8,byte[i]%8);
            i++;
            continue;
        }


        if (push == (byte[i] - (byte[i]%8)))
        {

            PUSH(byte[i] % 8);
            continue;
        }
        if (pop == (byte[i] - (byte[i]%8)))
        {

            POP(byte[i] % 8);
            continue;
        }

        if (inc == (byte[i] - (byte[i]%8)))
        {
            INC(byte[i] % 8);
            continue;
        }

        if (xchg_ax == (byte[i] - (byte[i]%8)))
        {
            XCHG_AX((byte[i] - (byte[i]%8)));
            continue;
        }

        if(xchg_r_r == byte[i])
        {
            XCHG_R_R((byte[i+1]%64)/8,(byte[i+1]%8));
            i++;
            continue;
        }

        if (dec == (byte[i] - (byte[i]%8)))
        {
            DEC(byte[i] % 8);
            continue;
        }

        if(xor_ax_imm16 == byte[i])
        {
            if (byte[i+2]==0)
            {
                XOR_AX_IMM16(reverse(byte[i+3]));
                i=i+3;
            }
            else
            {
            XOR_AX_IMM16(reverse(byte[i+3])+byte[i+2]);
            i=i+3;
            }
            continue;
        }
        if(xor_r_r==byte[i])
        {
            XOR_R_R((byte[i+1]%64)/8,byte[i+1]%8);
            i=i+1;
            continue;
        }
        if (stosw==byte[i])
        {
            STOSW(DATA);
            continue;

        }
    }

    union ex
    {
        uint16_t a;
        uint8_t b[2];
    };
    union ex z;
    while (DATA[j]!=9248)
    {
        z.a=DATA[j];
        if (z.a==0)
        {
            j++;
            continue;
        }
        else
        {
            for (k=0; k<2;k++)
            {
                char f[2];
                f[0] = z.b[k];
                strncat(Str,f, 1);
            }
        }
        j++;
    }
    if(strcmp(Str, "VirusCodeGenerator by RedArc") == 0)
    {
        if(printf("Signature found!\n") != 17) return 9;
    }
    else
    {
        if(printf("Signature not found!\n")!= 21) return 10;
    }
    if(fclose(f)) return 8;

    if(printf("\nInput any symbol to close program\n") != 35) return 11;
    getchar();

    return 0;
}

int MOV_R_IMM16(uint16_t REG1, uint16_t IMM16)
{
    switch (REG1)
            {
                case 0:
                    REGISTER.AX = IMM16;
                    break;
                case 1:
                    REGISTER.CX = IMM16;
                    break;
                case 2:
                    REGISTER.DX = IMM16;
                    break;
                case 3:
                    REGISTER.BX = IMM16;
                    break;
                case 4:
                    REGISTER.SP = IMM16;
                    break;
                case 5:
                    REGISTER.BP = IMM16;
                    break;
                case 6:
                    REGISTER.SI = IMM16;
                    break;
                case 7:
                    REGISTER.DI = IMM16;
                    break;
                default:
                    return 1;
                    break;
            }
    return 0;
}
int MOV_R_SR(uint16_t REG, uint16_t SR)
{

    switch (REG)
    {
        case 0:
            switch (SR)
            {
                case 0:
                    REGISTER.AX = REGISTER.ES;
                    break;
                case 1:
                    REGISTER.AX = REGISTER.CS;
                    break;
                case 2:
                    REGISTER.AX = REGISTER.SS;
                    break;
                case 3:
                    REGISTER.AX = REGISTER.DS;
                    break;
                default:
                    return 1;
                    break;
            }
        break;
        case 1:
            switch (SR)
            {
                case 0:
                    REGISTER.CX = REGISTER.ES;
                    break;
                case 1:
                    REGISTER.CX = REGISTER.CS;
                    break;
                case 2:
                    REGISTER.CX = REGISTER.SS;
                    break;
                case 3:
                    REGISTER.CX = REGISTER.DS;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case 2:
            switch (SR)
            {
                case 0:
                    REGISTER.DX = REGISTER.ES;
                    break;
                case 1:
                    REGISTER.DX = REGISTER.CS;
                    break;
                case 2:
                    REGISTER.DX = REGISTER.SS;
                    break;
                case 3:
                    REGISTER.DX = REGISTER.DS;
                    break;
                default:
                    return 3;
                    break;
            }
            break;
        case 3:
            switch (SR)
            {
                case 0:
                    REGISTER.BX = REGISTER.ES;
                    break;
                case 1:
                    REGISTER.BX = REGISTER.CS;
                    break;
                case 2:
                    REGISTER.BX = REGISTER.SS;
                    break;
                case 3:
                    REGISTER.BX = REGISTER.DS;
                    break;
                default:
                    return 4;
                    break;
            }
            break;
        case 4:
            switch (SR)
            {
                case 0:
                    REGISTER.SP = REGISTER.ES;
                    break;
                case 1:
                    REGISTER.SP = REGISTER.CS;
                    break;
                case 2:
                    REGISTER.SP = REGISTER.SS;
                    break;
                case 3:
                    REGISTER.SP = REGISTER.DS;
                    break;
                default:
                    return 5;
                    break;
            }
            break;
        case 5:
           switch (SR)
            {
                case 0:
                    REGISTER.BP = REGISTER.ES;
                    break;
                case 1:
                    REGISTER.BP = REGISTER.CS;
                    break;
                case 2:
                    REGISTER.BP = REGISTER.SS;
                    break;
                case 3:
                    REGISTER.BP = REGISTER.DS;
                    break;
                default:
                    return 6;
                    break;
            }
            break;
        case 6:
           switch (SR)
            {
                case 0:
                    REGISTER.SI = REGISTER.ES;
                    break;
                case 1:
                    REGISTER.SI = REGISTER.CS;
                    break;
                case 2:
                    REGISTER.SI = REGISTER.SS;
                    break;
                case 3:
                    REGISTER.SI = REGISTER.DS;
                    break;
                default:
                    return 7;
                    break;
            }
            break;
        case 7:
           switch (SR)
            {
              case 0:
                REGISTER.DI=REGISTER.ES;
                break;
            case 1:
                REGISTER.DI=REGISTER.CS;
                break;
            case 2:
                REGISTER.DI=REGISTER.SS;
                break;
            case 3:
                REGISTER.DI=REGISTER.DS;
                break;
            default:
                return 8;
                break;
            }
            break;
        break;
        default:
        return 9;
        break;
    }
    return 0;
}
int MOV_SR_R(uint16_t SR, uint16_t REG)
{
     switch (SR)
    {
        case 0:
            switch (REG)
            {
                case 0:
                    REGISTER.ES = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.ES = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.ES = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.ES = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.ES = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.ES = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.ES = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.ES = REGISTER.DI;
                    break;
                default:
                    return 1;
                    break;
            }
        break;
        case 1:
            switch (REG)
            {
                case 0:
                    REGISTER.CS = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.CS = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.CS = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.CS = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.CS = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.CS = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.CS = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.CS = REGISTER.DI;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case 2:
            switch (REG)
            {
                case 0:
                    REGISTER.SS = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.SS = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.SS = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.SS = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.SS = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.SS = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.SS = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.SS = REGISTER.DI;
                    break;
                default:
                    return 3;
                    break;
            }
            break;
        case 3:
            switch (REG)
            {
                case 0:
                    REGISTER.DS = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.DS = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.DS = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.DS = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.DS = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.DS = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.DS = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.DS = REGISTER.DI;
                    break;
                default:
                    return 4;
                    break;
            }
            break;
        default:
            return 5;
        break;
    }

    return 0;
}
int MOV_R_R(uint16_t REG1, uint16_t REG2)
{

    switch (REG1)
    {
        case 0:
            switch (REG2)
            {
                case 0:
                    REGISTER.AX = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.AX = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.AX = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.AX = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.AX = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.AX = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.AX = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.AX = REGISTER.DI;
                    break;
                default:
                    return 1;
                    break;
            }
        break;
        case 1:
            switch (REG2)
            {
                case 0:
                    REGISTER.CX = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.CX = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.CX = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.CX = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.CX = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.CX = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.CX = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.CX = REGISTER.DI;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case 2:
            switch (REG2)
            {
                case 0:
                    REGISTER.DX = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.DX = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.DX = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.DX = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.DX = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.DX = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.DX = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.DX = REGISTER.DI;
                    break;
                default:
                    return 3;
                    break;
            }
            break;
        case 3:
            switch (REG2)
            {
                case 0:
                    REGISTER.BX = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.BX = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.BX = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.BX = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.BX = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.BX = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.BX = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.BX = REGISTER.DI;
                    break;
                default:
                    return 4;
                    break;
            }
            break;
        case 4:
            switch (REG2)
            {
                case 0:
                    REGISTER.SP = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.SP = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.SP = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.SP = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.SP = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.SP = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.SP = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.SP = REGISTER.DI;
                    break;
                default:
                    return 5;
                    break;
            }
            break;
        case 5:
           switch (REG2)
            {
                case 0:
                    REGISTER.BP = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.BP = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.BP = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.BP = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.BP = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.BP = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.BP = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.BP = REGISTER.DI;
                    break;
                default:
                    return 6;
                    break;
            }
            break;
        case 6:
           switch (REG2)
            {
                case 0:
                    REGISTER.SI = REGISTER.AX;
                    break;
                case 1:
                    REGISTER.SI = REGISTER.CX;
                    break;
                case 2:
                    REGISTER.SI = REGISTER.DX;
                    break;
                case 3:
                    REGISTER.SI = REGISTER.BX;
                    break;
                case 4:
                    REGISTER.SI = REGISTER.SP;
                    break;
                case 5:
                    REGISTER.SI = REGISTER.BP;
                    break;
                case 6:
                    REGISTER.SI = REGISTER.SI;
                    break;
                case 7:
                    REGISTER.SI = REGISTER.DI;
                    break;
                default:
                    return 7;
                    break;
            }
            break;
        case 7:
           switch (REG2)
            {
              case 0:
                REGISTER.DI=REGISTER.AX;
                break;
            case 1:
                REGISTER.DI=REGISTER.CX;
                break;
            case 2:
                REGISTER.DI=REGISTER.DX;
                break;
            case 3:
                REGISTER.DI=REGISTER.BX;
                break;
            case 4:
                REGISTER.DI=REGISTER.SP;
                break;
            case 5:
                REGISTER.DI=REGISTER.BP;
                break;
            case 6:
                REGISTER.DI=REGISTER.SI;
                break;
            case 7:
                REGISTER.DI=REGISTER.DI;
                break;
            default:
                return 8;
                break;
            }
            break;
        break;
        default:
        return 9;
        break;
    }
    return 0;
}



int XCHG_AX(uint8_t OP1)
{
    char tmp;

    switch (OP1)
            {
                case 0:
                    REGISTER.AX = REGISTER.AX;
                    break;
                case 1:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.CX;
                    REGISTER.CX = tmp;
                    break;
                case 2:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.DX;
                    REGISTER.DX = tmp;
                    break;
                case 3:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.BX;
                    REGISTER.BX = tmp;
                    break;
                case 4:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.SP;
                    REGISTER.SP = tmp;
                    break;
                case 5:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.BP;
                    REGISTER.BP = tmp;
                    break;
                case 6:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.SI;
                    REGISTER.SI = tmp;
                    break;
                case 7:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.DI;
                    REGISTER.DI = tmp;
                    break;
                default:
                    return 1;
                    break;
            }
    return 0;
}

int XCHG_R_R(uint16_t REG1, uint16_t REG2)
{

    uint16_t tmp;
    switch (REG1)
    {
        case 0:
            switch (REG2)
            {
                case 0:
                    REGISTER.AX = REGISTER.AX;
                    break;
                case 1:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.CX;
                    REGISTER.CX = tmp;
                    break;
                case 2:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.DX;
                    REGISTER.DX = tmp;
                    break;
                case 3:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.BX;
                    REGISTER.BX = tmp;
                    break;
                case 4:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.SP;
                    REGISTER.SP = tmp;
                    break;
                case 5:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.BP;
                    REGISTER.BP = tmp;
                    break;
                case 6:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.SI;
                    REGISTER.SI = tmp;
                    break;
                case 7:
                    tmp = REGISTER.AX;
                    REGISTER.AX = REGISTER.DI;
                    REGISTER.DI = tmp;
                    break;
                default:
                    return 1;
                    break;
            }
        break;
        case 1:
            switch (REG2)
            {
                case 0:
                    tmp = REGISTER.CX;
                    REGISTER.CX = REGISTER.AX;
                    REGISTER.CX = tmp;
                    break;
                case 1:
                    REGISTER.CX = REGISTER.CX;
                    break;
                case 2:
                    tmp = REGISTER.CX;
                    REGISTER.CX = REGISTER.DX;
                    REGISTER.DX = tmp;
                    break;
                case 3:
                    tmp = REGISTER.CX;
                    REGISTER.CX = REGISTER.BX;
                    REGISTER.BX = tmp;
                    break;
                case 4:
                    tmp = REGISTER.CX;
                    REGISTER.CX = REGISTER.SP;
                    REGISTER.SP = tmp;
                    break;
                case 5:
                    tmp = REGISTER.CX;
                    REGISTER.CX = REGISTER.BP;
                    REGISTER.BP = tmp;
                    break;
                case 6:
                    tmp = REGISTER.CX;
                    REGISTER.CX = REGISTER.SI;
                    REGISTER.SI = tmp;
                    break;
                case 7:
                    tmp = REGISTER.CX;
                    REGISTER.CX = REGISTER.DI;
                    REGISTER.DI = tmp;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case 2:
            switch (REG2)
            {
                case 0:
                    tmp = REGISTER.DX;
                    REGISTER.DX = REGISTER.AX;
                    REGISTER.AX = tmp;
                    break;
                case 1:
                    tmp = REGISTER.DX;
                    REGISTER.DX = REGISTER.CX;
                    REGISTER.CX = tmp;
                    break;
                case 2:
                    REGISTER.DX = REGISTER.DX;
                    break;
                case 3:
                    tmp = REGISTER.DX;
                    REGISTER.DX = REGISTER.BX;
                    REGISTER.BX = tmp;
                    break;
                case 4:
                    tmp = REGISTER.DX;
                    REGISTER.DX = REGISTER.SP;
                    REGISTER.SP = tmp;
                    break;
                case 5:
                    tmp = REGISTER.DX;
                    REGISTER.DX = REGISTER.BP;
                    REGISTER.BP = tmp;
                    break;
                case 6:
                    tmp = REGISTER.DX;
                    REGISTER.DX = REGISTER.SI;
                    REGISTER.SI = tmp;
                    break;
                case 7:
                    tmp = REGISTER.DX;
                    REGISTER.DX = REGISTER.DI;
                    REGISTER.DI = tmp;
                    break;
                default:
                    return 3;
                    break;
            }
            break;
        case 3:
            switch (REG2)
            {
                case 0:
                    tmp = REGISTER.BX;
                    REGISTER.BX = REGISTER.AX;
                    REGISTER.AX = tmp;
                    break;
                case 1:
                    tmp = REGISTER.BX;
                    REGISTER.BX = REGISTER.CX;
                    REGISTER.CX = tmp;
                    break;
                case 2:
                    tmp = REGISTER.BX;
                    REGISTER.BX = REGISTER.DX;
                    REGISTER.DX = tmp;
                    break;
                case 3:
                    REGISTER.BX = REGISTER.BX;
                    break;
                case 4:
                    tmp = REGISTER.BX;
                    REGISTER.BX = REGISTER.SP;
                    REGISTER.SP = tmp;
                    break;
                case 5:
                    tmp = REGISTER.BX;
                    REGISTER.BX = REGISTER.BP;
                    REGISTER.BP = tmp;
                    break;
                case 6:
                    tmp = REGISTER.BX;
                    REGISTER.BX = REGISTER.SI;
                    REGISTER.SI = tmp;
                    break;
                case 7:
                    tmp = REGISTER.BX;
                    REGISTER.BX = REGISTER.DI;
                    REGISTER.DI = tmp;
                    break;
                default:
                    return 4;
                    break;
            }
            break;
        case 4:
            switch (REG2)
            {
                case 0:
                    tmp = REGISTER.SP;
                    REGISTER.SP = REGISTER.AX;
                    REGISTER.AX = tmp;
                    break;
                case 1:
                    tmp = REGISTER.SP;
                    REGISTER.SP = REGISTER.CX;
                    REGISTER.CX = tmp;
                    break;
                case 2:
                    tmp = REGISTER.SP;
                    REGISTER.SP = REGISTER.DX;
                    REGISTER.DX = tmp;
                    break;
                case 3:tmp = REGISTER.SP;
                    REGISTER.SP = REGISTER.BX;
                    REGISTER.BX = tmp;
                    break;
                case 4:
                    REGISTER.SP = REGISTER.SP;
                    break;
                case 5:
                    tmp = REGISTER.SP;
                    REGISTER.SP = REGISTER.BP;
                    REGISTER.BP = tmp;
                    break;
                case 6:
                    tmp = REGISTER.SP;
                    REGISTER.SP = REGISTER.SI;
                    REGISTER.SI = tmp;
                    break;
                case 7:
                    tmp = REGISTER.SP;
                    REGISTER.SP = REGISTER.DI;
                    REGISTER.DI = tmp;
                    break;
                default:
                    return 5;
                    break;
            }
            break;
        case 5:
           switch (REG2)
            {
                case 0:
                    tmp = REGISTER.BP;
                    REGISTER.BP = REGISTER.AX;
                    REGISTER.AX = tmp;
                    break;
                case 1:
                    tmp = REGISTER.BP;
                    REGISTER.BP = REGISTER.CX;
                    REGISTER.CX = tmp;
                    break;
                case 2:
                    tmp = REGISTER.BP;
                    REGISTER.BP = REGISTER.DX;
                    REGISTER.DX = tmp;
                    break;
                case 3:tmp = REGISTER.BP;
                    REGISTER.BP = REGISTER.BX;
                    REGISTER.BX = tmp;
                    break;
                case 4:
                    tmp = REGISTER.BP;
                    REGISTER.BP = REGISTER.SP;
                    REGISTER.CX = tmp;
                    break;
                case 5:
                    REGISTER.BP = REGISTER.BP;
                    break;
                case 6:
                    tmp = REGISTER.BP;
                    REGISTER.BP = REGISTER.SI;
                    REGISTER.SI = tmp;
                    break;
                case 7:
                    tmp = REGISTER.BP;
                    REGISTER.BP = REGISTER.DI;
                    REGISTER.DI = tmp;
                    break;
                default:
                    return 6;
                    break;
            }
            break;
        case 6:
           switch (REG2)
            {
                case 0:
                    tmp = REGISTER.SI;
                    REGISTER.SI = REGISTER.AX;
                    REGISTER.AX = tmp;
                    break;
                case 1:
                    tmp = REGISTER.SI;
                    REGISTER.SI = REGISTER.CX;
                    REGISTER.CX = tmp;
                    break;
                case 2:
                    tmp = REGISTER.SI;
                    REGISTER.SI = REGISTER.DX;
                    REGISTER.DX = tmp;
                    break;
                case 3:
                    tmp = REGISTER.SI;
                    REGISTER.SI = REGISTER.BX;
                    REGISTER.BX = tmp;
                    break;
                case 4:
                    tmp = REGISTER.SI;
                    REGISTER.SI = REGISTER.SP;
                    REGISTER.SP = tmp;
                    break;
                case 5:
                    tmp = REGISTER.SI;
                    REGISTER.SI = REGISTER.BP;
                    REGISTER.BP = tmp;
                    break;
                case 6:
                    REGISTER.SI = REGISTER.SI;
                    break;
                case 7:
                    tmp = REGISTER.SI;
                    REGISTER.SI = REGISTER.DI;
                    REGISTER.DI = tmp;
                    break;
                default:
                    return 7;
                    break;
            }
            break;
        case 7:
           switch (REG2)
            {
                case 0:
                    tmp = REGISTER.DI;
                    REGISTER.DI = REGISTER.AX;
                    REGISTER.AX = tmp;
                    break;
                case 1:
                    tmp = REGISTER.DI;
                    REGISTER.DI = REGISTER.CX;
                    REGISTER.CX = tmp;
                    break;
                case 2:
                    tmp = REGISTER.DI;
                    REGISTER.DI = REGISTER.DX;
                    REGISTER.DX = tmp;
                    break;
                case 3:
                    tmp = REGISTER.SI;
                    REGISTER.DI = REGISTER.BX;
                    REGISTER.BX = tmp;
                    break;
                case 4:
                    tmp = REGISTER.SI;
                    REGISTER.DI = REGISTER.SP;
                    REGISTER.SP = tmp;
                    break;
                case 5:
                    tmp = REGISTER.SI;
                    REGISTER.DI = REGISTER.BP;
                    REGISTER.BP = tmp;
                    break;
                case 6:
                    tmp = REGISTER.SI;
                    REGISTER.DI = REGISTER.SI;
                    REGISTER.SI = tmp;
                    break;
                case 7:
                    REGISTER.DI = REGISTER.DI;
                    break;
                default:
                    return 8;
                    break;
            }
            break;
        break;
        default:
        return 9;
        break;
    }
    return 0;
}


int PUSH(uint8_t OP1)
{
    if (FullStack()) return 1;
    switch (OP1)
    {
        case 0:
            stack[REGISTER.SP-2] = REGISTER.AX;
            REGISTER.SP=REGISTER.SP-2;
            break;
        case 1:
            stack[REGISTER.SP-2] = REGISTER.CX;
            REGISTER.SP=REGISTER.SP-2;
            break;
        case 2:
            stack[REGISTER.SP-2] = REGISTER.DX;
            REGISTER.SP=REGISTER.SP-2;
            break;
        case 3:
            stack[REGISTER.SP-2] = REGISTER.BX;
            REGISTER.SP=REGISTER.SP-2;
            break;
        case 4:
            stack[REGISTER.SP-2] = REGISTER.SP;
            REGISTER.SP=REGISTER.SP-2;
            break;
        case 5:
            stack[REGISTER.SP-2] = REGISTER.BP;
            REGISTER.SP=REGISTER.SP-2;
            break;
        case 6:
            stack[REGISTER.SP-2] = REGISTER.SI;
            REGISTER.SP=REGISTER.SP-2;
            break;
        case 7:
            stack[REGISTER.SP-2] = REGISTER.DI;
            REGISTER.SP=REGISTER.SP-2;
            break;
        default:
            return 2;
            break;
    }
    return 0;
}
int POP(uint8_t OP1)
{
    if(EmptyStack()) return 1;
    switch (OP1)
    {
        case 0:
            REGISTER.AX = stack[REGISTER.SP];
            REGISTER.SP=REGISTER.SP+2;
            break;
        case 1:
            REGISTER.CX = stack[REGISTER.SP];
            REGISTER.SP=REGISTER.SP+2;
            break;
        case 2:
            REGISTER.DX = stack[REGISTER.SP];
            REGISTER.SP=REGISTER.SP+2;
            break;
        case 3:
            REGISTER.BX = stack[REGISTER.SP];
            REGISTER.SP=REGISTER.SP+2;
            break;
        case 4:
            REGISTER.SP = stack[REGISTER.SP];
            REGISTER.SP=REGISTER.SP+2;
            break;
        case 5:
            REGISTER.BP = stack[REGISTER.SP];
            REGISTER.SP=REGISTER.SP+2;
            break;
        case 6:
            REGISTER.SI = stack[REGISTER.SP];
            REGISTER.SP=REGISTER.SP+2;
            break;
        case 7:
            REGISTER.DI = stack[REGISTER.SP];
            REGISTER.SP=REGISTER.SP+2;
            break;
        default:
            return 2;
            break;
    }
    return 0;
}


int ADD_AX(uint16_t imm)
{
    REGISTER.AX=REGISTER.AX+imm;

    return 0;
}
int ADD_R_IMM16(uint16_t REG, uint16_t IMM16)
{

       switch(REG)
    {
        case 0:
            REGISTER.AX=REGISTER.AX+IMM16;
            break;
        case 1:
            REGISTER.CX=IMM16+REGISTER.CX;
            break;
        case 2:
            REGISTER.DX=REGISTER.DX+IMM16;
            break;
        case 3:
            REGISTER.BX=REGISTER.BX+IMM16;
            break;
        case 4:
            REGISTER.SP=REGISTER.SP+IMM16;
            break;
        case 5:
            REGISTER.BP=REGISTER.BP+IMM16;
            break;
        case 6:
            REGISTER.SI=REGISTER.SI+IMM16;
            break;
        case 7:
            REGISTER.DI=REGISTER.DI+IMM16;
            break;
        default:
            return 1;
            break;
    }
    return 0;
}
int ADD_R_IMM8(uint16_t REG, uint8_t IMM8)
{
    switch(REG)
    {
        case 0:
            REGISTER.AX=REGISTER.AX+IMM8;
            break;
        case 1:
            REGISTER.CX=REGISTER.CX+IMM8;
            break;
        case 2:
            REGISTER.DX=REGISTER.DX+IMM8;
            break;
        case 3:
            REGISTER.BX=REGISTER.BX+IMM8;
            break;
        case 4:
            REGISTER.SP=REGISTER.SP+IMM8;
            break;
        case 5:
            REGISTER.BP=REGISTER.BP+IMM8;
            break;
        case 6:
            REGISTER.SI=REGISTER.SI+IMM8;
            break;
        case 7:
            REGISTER.DI=REGISTER.DI+IMM8;
            break;
        default:
            return 1;
            break;
    }
    return 0;
}
int ADD_R_R(uint16_t reg1, uint16_t reg2)
{


    switch (reg1)
    {
        case 0:
            switch (reg2)
            {
              case 0:
                REGISTER.AX=REGISTER.AX+REGISTER.AX;
                break;
            case 1:
                REGISTER.AX=REGISTER.AX+REGISTER.CX;
                break;
            case 2:
                REGISTER.AX=REGISTER.AX+REGISTER.DX;
                break;
            case 3:
                REGISTER.AX=REGISTER.AX+REGISTER.BX;
                break;
            case 4:
                REGISTER.AX=REGISTER.AX+REGISTER.SP;
                break;
            case 5:
                REGISTER.AX=REGISTER.AX+REGISTER.BP;
                break;
            case 6:
                REGISTER.AX=REGISTER.AX+REGISTER.SI;
                break;
            case 7:
                REGISTER.AX=REGISTER.AX+REGISTER.DI;
                break;
            default:
                return 1;
                break;
            }
        break;
        case 1:
            switch (reg2)
            {
              case 0:
                REGISTER.CX=REGISTER.CX+REGISTER.AX;
                break;
            case 1:
                REGISTER.CX=REGISTER.CX+REGISTER.CX;
                break;
            case 2:
                REGISTER.CX=REGISTER.CX+REGISTER.DX;
                break;
            case 3:
                REGISTER.CX=REGISTER.CX+REGISTER.BX;
                break;
            case 4:
                REGISTER.CX=REGISTER.CX+REGISTER.SP;
                break;
            case 5:
                REGISTER.CX=REGISTER.CX+REGISTER.BP;
                break;
            case 6:
                REGISTER.CX=REGISTER.CX+REGISTER.SI;
                break;
            case 7:
                REGISTER.CX=REGISTER.CX+REGISTER.DI;
                break;
            default:
                return 2;
                break;
            }
        break;
        case 2:
            switch (reg2)
            {
              case 0:
                REGISTER.DX=REGISTER.DX+REGISTER.AX;
                break;
            case 1:
                REGISTER.DX=REGISTER.DX+REGISTER.CX;
                break;
            case 2:
                REGISTER.DX=REGISTER.DX+REGISTER.DX;
                break;
            case 3:
                REGISTER.DX=REGISTER.DX+REGISTER.BX;
                break;
            case 4:
                REGISTER.DX=REGISTER.DX+REGISTER.SP;
                break;
            case 5:
                REGISTER.DX=REGISTER.DX+REGISTER.BP;
                break;
            case 6:
                REGISTER.DX=REGISTER.DX+REGISTER.SI;
                break;
            case 7:
                REGISTER.DX=REGISTER.DX+REGISTER.DI;
                break;
            default:
                return 3;
                break;
            }
        break;
        case 3:
            switch (reg2)
            {
              case 0:
                REGISTER.BX=REGISTER.BX+REGISTER.AX;
                break;
            case 1:
                REGISTER.BX=REGISTER.BX+REGISTER.CX;
                break;
            case 2:
                REGISTER.BX=REGISTER.BX+REGISTER.DX;
                break;
            case 3:
                REGISTER.BX=REGISTER.BX+REGISTER.BX;
                break;
            case 4:
                REGISTER.BX=REGISTER.BX+REGISTER.SP;
                break;
            case 5:
                REGISTER.BX=REGISTER.BX+REGISTER.BP;
                break;
            case 6:
                REGISTER.BX=REGISTER.BX+REGISTER.SI;
                break;
            case 7:
                REGISTER.BX=REGISTER.BX+REGISTER.DI;
                break;
            default:
                return 4;
                break;
            }
        break;
        case 4:
            switch (reg2)
            {
              case 0:
                REGISTER.SP=REGISTER.SP+REGISTER.AX;
                break;
            case 1:
                REGISTER.SP=REGISTER.SP+REGISTER.CX;
                break;
            case 2:
                REGISTER.SP=REGISTER.SP+REGISTER.DX;
                break;
            case 3:
                REGISTER.SP=REGISTER.SP+REGISTER.BX;
                break;
            case 4:
                REGISTER.SP=REGISTER.SP+REGISTER.SP;
                break;
            case 5:
                REGISTER.SP=REGISTER.SP+REGISTER.BP;
                break;
            case 6:
                REGISTER.SP=REGISTER.SP+REGISTER.SI;
                break;
            case 7:
                REGISTER.SP=REGISTER.SP+REGISTER.DI;
                break;
            default:
                return 5;
                break;
            }
        break;
        case 5:
           switch (reg2)
            {
              case 0:
                REGISTER.BP=REGISTER.BP+REGISTER.AX;
                break;
            case 1:
                REGISTER.BP=REGISTER.BP+REGISTER.CX;
                break;
            case 2:
                REGISTER.BP=REGISTER.BP+REGISTER.DX;
                break;
            case 3:
                REGISTER.BP=REGISTER.BP+REGISTER.BX;
                break;
            case 4:
                REGISTER.BP=REGISTER.BP+REGISTER.SP;
                break;
            case 5:
                REGISTER.BP=REGISTER.BP+REGISTER.BP;
                break;
            case 6:
                REGISTER.BP=REGISTER.BP+REGISTER.SI;
                break;
            case 7:
                REGISTER.BP=REGISTER.BP+REGISTER.DI;
                break;
            default:
                return 6;
                break;
            }
            break;
        case 6:
           switch (reg2)
            {
              case 0:
                REGISTER.SI=REGISTER.SI+REGISTER.AX;
                break;
            case 1:
                REGISTER.SI=REGISTER.SI+REGISTER.CX;
                break;
            case 2:
                REGISTER.SI=REGISTER.SI+REGISTER.DX;
                break;
            case 3:
                REGISTER.SI=REGISTER.SI+REGISTER.BX;
                break;
            case 4:
                REGISTER.SI=REGISTER.SI+REGISTER.SP;
                break;
            case 5:
                REGISTER.SI=REGISTER.SI+REGISTER.BP;
                break;
            case 6:
                REGISTER.SI=REGISTER.SI+REGISTER.SI;
                break;
            case 7:
                REGISTER.SI=REGISTER.SI+REGISTER.DI;
                break;
            default:
                return 7;
                break;
            }
        break;
        case 7:
           switch (reg2)
            {
              case 0:
                REGISTER.DI=REGISTER.BP+REGISTER.AX;
                break;
            case 1:
                REGISTER.DI=REGISTER.BP+REGISTER.CX;
                break;
            case 2:
                REGISTER.DI=REGISTER.BP+REGISTER.DX;
                break;
            case 3:
                REGISTER.DI=REGISTER.BP+REGISTER.BX;
                break;
            case 4:
                REGISTER.DI=REGISTER.BP+REGISTER.SP;
                break;
            case 5:
                REGISTER.DI=REGISTER.BP+REGISTER.BP;
                break;
            case 6:
                REGISTER.DI=REGISTER.BP+REGISTER.SI;
                break;
            case 7:
                REGISTER.DI=REGISTER.BP+REGISTER.DI;
                break;
            default:
                return 8;
                break;
            }
            break;
        break;
        default:
        return 9;
        break;
    }
}


int INC(uint8_t OP1)
{

    switch(OP1)
    {
        case 0:
            REGISTER.AX++;
            break;
        case 1:
            REGISTER.CX++;
            break;
        case 2:
            REGISTER.DX++;
            break;
        case 3:
            REGISTER.BX++;
            break;
        case 4:
            REGISTER.SP++;
            break;
        case 5:
            REGISTER.BP++;
            break;
        case 6:
            REGISTER.SI++;
            break;
        case 7:
            REGISTER.DI++;
            break;
        default:
            return 1;
            break;
    }
    return 0;
}
int DEC(uint8_t OP1)
{

    switch(OP1)
    {
        case 0:
            REGISTER.AX--;
            break;
        case 1:
            REGISTER.CX--;
            break;
        case 2:
            REGISTER.DX--;
            break;
        case 3:
            REGISTER.BX--;
            break;
        case 4:
            REGISTER.SP--;
            break;
        case 5:
            REGISTER.BP--;
            break;
        case 6:
            REGISTER.SI--;
            break;
        case 7:
            REGISTER.DI--;
            break;
        default:
            return 1;
            break;
    }

    return 0;
}


int SUB_AX(uint16_t imm)
{
    REGISTER.AX=REGISTER.AX-imm;
    return 0;
}
int SUB_R_IMM16(uint16_t reg, uint16_t IMM16)
{
     switch(reg)
    {
        case 0:
            REGISTER.AX=REGISTER.AX-IMM16;
            break;
        case 1:
            REGISTER.CX=REGISTER.CX-IMM16;
            break;
        case 2:
            REGISTER.DX=REGISTER.DX-IMM16;
            break;
        case 3:
            REGISTER.BX=REGISTER.BX-IMM16;
            break;
        case 4:
            REGISTER.SP=REGISTER.SP-IMM16;
            break;
        case 5:
            REGISTER.BP=REGISTER.BP-IMM16;
            break;
        case 6:
            REGISTER.SI=REGISTER.SI-IMM16;
            break;
        case 7:
            REGISTER.DI=REGISTER.DI-IMM16;
            break;
        default:
            return 1;
            break;


    }
    return 0;
}
int SUB_R_IMM8(uint16_t reg, uint8_t IMM8)
{
    switch(reg)
    {
        case 0:
            REGISTER.AX=REGISTER.AX-IMM8;
            break;
        case 1:
            REGISTER.CX=REGISTER.CX-IMM8;
            break;
        case 2:
            REGISTER.DX=REGISTER.DX-IMM8;
            break;
        case 3:
            REGISTER.BX=REGISTER.BX-IMM8;
            break;
        case 4:
            REGISTER.SP=REGISTER.SP-IMM8;
            break;
        case 5:
            REGISTER.BP=REGISTER.BP-IMM8;
            break;
        case 6:
            REGISTER.SI=REGISTER.SI-IMM8;
            break;
        case 7:
            REGISTER.DI=REGISTER.DI-IMM8;
            break;
        default:
            return 1;
            break;
    }
    return 0;
}
int SUB_R_R(uint16_t reg1, uint16_t reg2)
{
     switch (reg1)
    {
        case 0:
            switch (reg2)
            {
              case 0:
                REGISTER.AX=REGISTER.AX-REGISTER.AX;
                break;
            case 1:
                REGISTER.AX=REGISTER.AX-REGISTER.CX;
                break;
            case 2:
                REGISTER.AX=REGISTER.AX-REGISTER.DX;
                break;
            case 3:
                REGISTER.AX=REGISTER.AX-REGISTER.BX;
                break;
            case 4:
                REGISTER.AX=REGISTER.AX-REGISTER.SP;
                break;
            case 5:
                REGISTER.AX=REGISTER.AX-REGISTER.BP;
                break;
            case 6:
                REGISTER.AX=REGISTER.AX-REGISTER.SI;
                break;
            case 7:
                REGISTER.AX=REGISTER.AX-REGISTER.DI;
                break;
            default:
                return 1;
                break;
            }
        break;
        case 1:
            switch (reg2)
            {
              case 0:
                REGISTER.CX=REGISTER.CX-REGISTER.AX;
                break;
            case 1:
                REGISTER.CX=REGISTER.CX-REGISTER.CX;
                break;
            case 2:
                REGISTER.CX=REGISTER.CX-REGISTER.DX;
                break;
            case 3:
                REGISTER.CX=REGISTER.CX-REGISTER.BX;
                break;
            case 4:
                REGISTER.CX=REGISTER.CX-REGISTER.SP;
                break;
            case 5:
                REGISTER.CX=REGISTER.CX-REGISTER.BP;
                break;
            case 6:
                REGISTER.CX=REGISTER.CX-REGISTER.SI;
                break;
            case 7:
                REGISTER.CX=REGISTER.CX-REGISTER.DI;
                break;
            default:
                return 2;
                break;
            }
        break;
        case 2:
            switch (reg2)
            {
              case 0:
                REGISTER.DX=REGISTER.DX-REGISTER.AX;
                break;
            case 1:
                REGISTER.DX=REGISTER.DX-REGISTER.CX;
                break;
            case 2:
                REGISTER.DX=REGISTER.DX-REGISTER.DX;
                break;
            case 3:
                REGISTER.DX=REGISTER.DX-REGISTER.BX;
                break;
            case 4:
                REGISTER.DX=REGISTER.DX-REGISTER.SP;
                break;
            case 5:
                REGISTER.DX=REGISTER.DX-REGISTER.BP;
                break;
            case 6:
                REGISTER.DX=REGISTER.DX-REGISTER.SI;
                break;
            case 7:
                REGISTER.DX=REGISTER.DX-REGISTER.DI;
                break;
            default:
                return 3;
                break;
            }
        break;
        case 3:
            switch (reg2)
            {
              case 0:
                REGISTER.BX=REGISTER.BX-REGISTER.AX;
                break;
            case 1:
                REGISTER.BX=REGISTER.BX-REGISTER.CX;
                break;
            case 2:
                REGISTER.BX=REGISTER.BX-REGISTER.DX;
                break;
            case 3:
                REGISTER.BX=REGISTER.BX-REGISTER.BX;
                break;
            case 4:
                REGISTER.BX=REGISTER.BX-REGISTER.SP;
                break;
            case 5:
                REGISTER.BX=REGISTER.BX-REGISTER.BP;
                break;
            case 6:
                REGISTER.BX=REGISTER.BX-REGISTER.SI;
                break;
            case 7:
                REGISTER.BX=REGISTER.BX-REGISTER.DI;
                break;
            default:
                return 4;
                break;
            }
        break;
        case 4:
            switch (reg2)
            {
              case 0:
                REGISTER.SP=REGISTER.SP-REGISTER.AX;
                break;
            case 1:
                REGISTER.SP=REGISTER.SP-REGISTER.CX;
                break;
            case 2:
                REGISTER.SP=REGISTER.SP-REGISTER.DX;
                break;
            case 3:
                REGISTER.SP=REGISTER.SP-REGISTER.BX;
                break;
            case 4:
                REGISTER.SP=REGISTER.SP-REGISTER.SP;
                break;
            case 5:
                REGISTER.SP=REGISTER.SP-REGISTER.BP;
                break;
            case 6:
                REGISTER.SP=REGISTER.SP-REGISTER.SI;
                break;
            case 7:
                REGISTER.SP=REGISTER.SP-REGISTER.DI;
                break;
            default:
                return 5;
                break;
            }
        break;
        case 5:
           switch (reg2)
            {
              case 0:
                REGISTER.BP=REGISTER.BP-REGISTER.AX;
                break;
            case 1:
                REGISTER.BP=REGISTER.BP-REGISTER.CX;
                break;
            case 2:
                REGISTER.BP=REGISTER.BP-REGISTER.DX;
                break;
            case 3:
                REGISTER.BP=REGISTER.BP-REGISTER.BX;
                break;
            case 4:
                REGISTER.BP=REGISTER.BP-REGISTER.SP;
                break;
            case 5:
                REGISTER.BP=REGISTER.BP-REGISTER.BP;
                break;
            case 6:
                REGISTER.BP=REGISTER.BP-REGISTER.SI;
                break;
            case 7:
                REGISTER.BP=REGISTER.BP-REGISTER.DI;
                break;
            default:
                return 6;
                break;
            }
            break;
        case 6:
           switch (reg2)
            {
              case 0:
                REGISTER.SI=REGISTER.SI-REGISTER.AX;
                break;
            case 1:
                REGISTER.SI=REGISTER.SI-REGISTER.CX;
                break;
            case 2:
                REGISTER.SI=REGISTER.SI-REGISTER.DX;
                break;
            case 3:
                REGISTER.SI=REGISTER.SI-REGISTER.BX;
                break;
            case 4:
                REGISTER.SI=REGISTER.SI-REGISTER.SP;
                break;
            case 5:
                REGISTER.SI=REGISTER.SI-REGISTER.BP;
                break;
            case 6:
                REGISTER.SI=REGISTER.SI-REGISTER.SI;
                break;
            case 7:
                REGISTER.SI=REGISTER.SI-REGISTER.DI;
                break;
            default:
                return 7;
                break;
            }
        break;
        case 7:
           switch (reg2)
            {
              case 0:
                REGISTER.DI=REGISTER.BP-REGISTER.AX;
                break;
            case 1:
                REGISTER.DI=REGISTER.BP-REGISTER.CX;
                break;
            case 2:
                REGISTER.DI=REGISTER.BP-REGISTER.DX;
                break;
            case 3:
                REGISTER.DI=REGISTER.BP-REGISTER.BX;
                break;
            case 4:
                REGISTER.DI=REGISTER.BP-REGISTER.SP;
                break;
            case 5:
                REGISTER.DI=REGISTER.BP-REGISTER.BP;
                break;
            case 6:
                REGISTER.DI=REGISTER.BP-REGISTER.SI;
                break;
            case 7:
                REGISTER.DI=REGISTER.BP-REGISTER.DI;
                break;
            default:
                return 8;
                break;
            }
            break;
        break;
        default:
        return 9;
        break;
    }
    return 0;
}


int XOR_R_R(uint16_t REG1, uint16_t REG2)
{
    switch (REG1)
    {
        case 0:
            switch (REG2)
            {
                case 0:
                    REGISTER.AX = REGISTER.AX ^ REGISTER.AX;
                    break;
                case 1:
                    REGISTER.AX = REGISTER.AX ^ REGISTER.CX;
                    break;
                case 2:
                    REGISTER.AX = REGISTER.AX ^ REGISTER.DX;
                    break;
                case 3:
                    REGISTER.AX = REGISTER.AX ^ REGISTER.BX;
                    break;
                case 4:
                    REGISTER.AX = REGISTER.AX ^ REGISTER.SP;
                    break;
                case 5:
                    REGISTER.AX = REGISTER.AX ^ REGISTER.BP;
                    break;
                case 6:
                    REGISTER.AX = REGISTER.AX ^REGISTER.SI;
                    break;
                case 7:
                    REGISTER.AX = REGISTER.AX ^ REGISTER.DI;
                    break;
                default:
                    return 1;
                    break;
            }
        break;
        case 1:
            switch (REG2)
            {
                case 0:
                    REGISTER.CX = REGISTER.CX ^ REGISTER.AX;
                    break;
                case 1:
                    REGISTER.CX = REGISTER.CX ^REGISTER.CX;
                    break;
                case 2:
                    REGISTER.CX = REGISTER.CX ^REGISTER.DX;
                    break;
                case 3:
                    REGISTER.CX = REGISTER.CX ^REGISTER.BX;
                    break;
                case 4:
                    REGISTER.CX = REGISTER.CX ^REGISTER.SP;
                    break;
                case 5:
                    REGISTER.CX = REGISTER.CX ^REGISTER.BP;
                    break;
                case 6:
                    REGISTER.CX = REGISTER.CX ^REGISTER.SI;
                    break;
                case 7:
                    REGISTER.CX = REGISTER.CX ^REGISTER.DI;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case 2:
            switch (REG2)
            {
                case 0:
                    REGISTER.DX = REGISTER.DX ^ REGISTER.AX;
                    break;
                case 1:
                    REGISTER.DX = REGISTER.DX ^ REGISTER.CX;
                    break;
                case 2:
                    REGISTER.DX =  REGISTER.DX ^REGISTER.DX;
                    break;
                case 3:
                    REGISTER.DX = REGISTER.DX ^ REGISTER.BX;
                    break;
                case 4:
                    REGISTER.DX =  REGISTER.DX ^REGISTER.SP;
                    break;
                case 5:
                    REGISTER.DX =  REGISTER.DX ^REGISTER.BP;
                    break;
                case 6:
                    REGISTER.DX = REGISTER.DX ^ REGISTER.SI;
                    break;
                case 7:
                    REGISTER.DX = REGISTER.DX ^ REGISTER.DI;
                    break;
                default:
                    return 3;
                    break;
            }
            break;
        case 3:
            switch (REG2)
            {
                case 0:
                    REGISTER.BX = REGISTER.BX ^ REGISTER.AX;
                    break;
                case 1:
                    REGISTER.BX = REGISTER.BX ^ REGISTER.CX;
                    break;
                case 2:
                    REGISTER.BX = REGISTER.BX ^REGISTER.DX;
                    break;
                case 3:
                    REGISTER.BX = REGISTER.BX ^REGISTER.BX;
                    break;
                case 4:
                    REGISTER.BX = REGISTER.BX ^REGISTER.SP;
                    break;
                case 5:
                    REGISTER.BX = REGISTER.BX ^REGISTER.BP;
                    break;
                case 6:
                    REGISTER.BX = REGISTER.BX ^ REGISTER.SI;
                    break;
                case 7:
                    REGISTER.BX = REGISTER.BX ^ REGISTER.DI;
                    break;
                default:
                    return 4;
                    break;
            }
            break;
        case 4:
            switch (REG2)
            {
                case 0:
                    REGISTER.SP = REGISTER.SP ^ REGISTER.AX;
                    break;
                case 1:
                    REGISTER.SP = REGISTER.SP ^ REGISTER.CX;
                    break;
                case 2:
                    REGISTER.SP = REGISTER.SP ^ REGISTER.DX;
                    break;
                case 3:
                    REGISTER.SP = REGISTER.SP ^ REGISTER.BX;
                    break;
                case 4:
                    REGISTER.SP =REGISTER.SP ^ REGISTER.SP;
                    break;
                case 5:
                    REGISTER.SP = REGISTER.SP ^REGISTER.BP;
                    break;
                case 6:
                    REGISTER.SP = REGISTER.SP ^REGISTER.SI;
                    break;
                case 7:
                    REGISTER.SP = REGISTER.SP ^REGISTER.DI;
                    break;
                default:
                    return 5;
                    break;
            }
            break;
        case 5:
           switch (REG2)
            {
                case 0:
                    REGISTER.BP = REGISTER.BP ^ REGISTER.AX;
                    break;
                case 1:
                    REGISTER.BP = REGISTER.BP ^ REGISTER.CX;
                    break;
                case 2:
                    REGISTER.BP = REGISTER.BP ^ REGISTER.DX;
                    break;
                case 3:
                    REGISTER.BP = REGISTER.BP ^ REGISTER.BX;
                    break;
                case 4:
                    REGISTER.BP = REGISTER.BP ^ REGISTER.SP;
                    break;
                case 5:
                    REGISTER.BP = REGISTER.BP ^ REGISTER.BP;
                    break;
                case 6:
                    REGISTER.BP = REGISTER.BP ^ REGISTER.SI;
                    break;
                case 7:
                    REGISTER.BP = REGISTER.BP ^ REGISTER.DI;
                    break;
                default:
                    return 6;
                    break;
            }
            break;
        case 6:
           switch (REG2)
            {
                case 0:
                    REGISTER.SI = REGISTER.SI ^ REGISTER.AX;
                    break;
                case 1:
                    REGISTER.SI = REGISTER.SI ^ REGISTER.CX;
                    break;
                case 2:
                    REGISTER.SI = REGISTER.SI ^ REGISTER.DX;
                    break;
                case 3:
                    REGISTER.SI =REGISTER.SI ^ REGISTER.BX;
                    break;
                case 4:
                    REGISTER.SI =REGISTER.SI ^ REGISTER.SP;
                    break;
                case 5:
                    REGISTER.SI = REGISTER.SI ^REGISTER.BP;
                    break;
                case 6:
                    REGISTER.SI = REGISTER.SI ^REGISTER.SI;
                    break;
                case 7:
                    REGISTER.SI = REGISTER.SI ^REGISTER.DI;
                    break;
                default:
                    return 7;
                    break;
            }
            break;
        case 7:
           switch (REG2)
            {
              case 0:
                REGISTER.DI= REGISTER.DI ^ REGISTER.AX;
                break;
            case 1:
                REGISTER.DI=REGISTER.DI ^REGISTER.CX;
                break;
            case 2:
                REGISTER.DI=REGISTER.DI ^REGISTER.DX;
                break;
            case 3:
                REGISTER.DI=REGISTER.DI ^REGISTER.BX;
                break;
            case 4:
                REGISTER.DI=REGISTER.DI ^REGISTER.SP;
                break;
            case 5:
                REGISTER.DI=REGISTER.DI ^REGISTER.BP;
                break;
            case 6:
                REGISTER.DI=REGISTER.DI ^REGISTER.SI;
                break;
            case 7:
                REGISTER.DI=REGISTER.DI ^ REGISTER.DI;
                break;
            default:
                return 8;
                break;
            }
            break;
        break;
        default:
        return 9;
        break;
    }
    return 0;
}

int XOR_AX_IMM16(uint16_t IMM16)
{
    REGISTER.AX = REGISTER.AX ^ IMM16;
    return 0;
}

int XOR_R_IMM16(uint16_t REG, uint16_t IMM16)
{
    switch (REG)
            {
                case 0:
                    REGISTER.AX ^= IMM16;
                    break;
                case 1:
                    REGISTER.CX ^= IMM16;
                    break;
                case 2:
                    REGISTER.DX ^= IMM16;
                    break;
                case 3:
                    REGISTER.BX ^= IMM16;
                    break;
                case 4:
                    REGISTER.SP ^= IMM16;
                    break;
                case 5:
                    REGISTER.BP ^= IMM16;
                    break;
                case 6:
                    REGISTER.SI ^= IMM16;
                    break;
                case 7:
                    REGISTER.DI ^= IMM16;
                    break;
                default:
                    return 1;
                    break;
            }
    return 0;
}

int XOR_R_IMM8(uint16_t REG, uint8_t IMM8)
{
    switch (REG)
            {
                case 0:
                    REGISTER.AX ^= IMM8;
                    break;
                case 1:
                    REGISTER.CX ^= IMM8;
                    break;
                case 2:
                    REGISTER.DX ^= IMM8;
                    break;
                case 3:
                    REGISTER.BX ^= IMM8;
                    break;
                case 4:
                    REGISTER.SP ^= IMM8;
                    break;
                case 5:
                    REGISTER.BP ^= IMM8;
                    break;
                case 6:
                    REGISTER.SI ^= IMM8;
                    break;
                case 7:
                    REGISTER.DI ^= IMM8;
                    break;
                default:
                    return 1;
                    break;
            }
    return 0;
}

int STOSW(uint16_t *DATA)
{
    if(DATA == NULL) return 1;
    DATA[REGISTER.DI] = (uint16_t)REGISTER.AX;
    REGISTER.DI+=2;
    return 0;
}
