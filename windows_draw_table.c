#include <stdio.h>
#include <conio.h>
void draw_table(void)
{
    static char ch = 0;
    ch = getch();
    switch(ch)
    {
        case 27 : return;//esc
        case 8  : printf("\b \b");break;//backspace
        case 13 : printf("\n");break;
        case '.': printf("│");break;
        case '0': printf("─");break;
        case '7': printf("┌");break;
        case '8': printf("┬");break;
        case '9': printf("┐");break;
        case '4': printf("├");break;
        case '5': printf("┼");break;
        case '6': printf("┤");break;
        case '1': printf("└");break;
        case '2': printf("┴");break;
        case '3': printf("┘");break;
        default : break;
    }
    draw_table();
}
void main(void)
{
	draw_table();
	return ;
} 
