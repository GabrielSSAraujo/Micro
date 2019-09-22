/*esquema de ligação das portas
  TP1 - Vcc (+5v)
  TP3 - Vss (Gnd)
  P2.0 - D4
  P2.1 - D5
  P2.2 - D6
  P2.3 - D7
  P2.4 - EN
  P2.5 - RS
  Gnd  - RW
  Gnd  - Vee
  */

#include <msp430g2231.h>

#define D4 BIT0
#define D5 BIT1
#define D6 BIT2
#define D7 BIT3

// LCD Registers masks based on pin to which it is connected
#define LCD_EN      BIT4
#define LCD_RS      BIT5

void lcd_reset()
{
  P2DIR = D4 |D5 |D6 |D7 |LCD_EN | LCD_RS; //define como saida os bits de dados enable e registro
  P2OUT = D4 |D5 |D6 |D7 |LCD_EN | LCD_RS; //joga pra nivel alto
  __delay_cycles(20000);
  
}

void lcd_cmd (char cmd)
{
  // Send upper nibble
  P2OUT = ((cmd >> 4) & 0x0F) | LCD_EN; //deslocando para direita 4 bits para pegar o nible mais significativo sem afetar dados
  P2OUT = ((cmd >> 4) & 0x0F);


  // Send lower nibble
  P2OUT = (cmd & 0x0F) | LCD_EN;
  P2OUT = (cmd & 0x0F);

  __delay_cycles(4000);
}

void lcd_init ()
{
  lcd_reset();         // Call LCD reset
  lcd_cmd(0x28);       //Ativando modo de 4 bits e 2 linhas 
  lcd_cmd(0x0C);       // Display no cursor - no blink.
  lcd_cmd(0x06);       // Automatic Increment - No Display shift.
  lcd_cmd(0x80);       // Address DDRAM with 0 offset 80h.
  lcd_cmd(0x01);     // Clear screen
}


void lcd_data (unsigned char dat)
{
  // Send upper nibble
  P2OUT = (((dat >> 4) & 0x0F) | LCD_EN | LCD_RS);
  P2OUT = (((dat >> 4) & 0x0F) | LCD_RS);

  // Send lower nibble
  P2OUT = ((dat & 0x0F) | LCD_EN | LCD_RS);
  P2OUT = ((dat & 0x0F) | LCD_RS);

  __delay_cycles(4000); // a small delay may result in missing char display
}

void display_line(char *line)
{
  while (*line)
    lcd_data(*line++);
}

int main()
{
  // Stop Watch Dog Timer
  WDTCTL = WDTPW + WDTHOLD;
  // Initialize LCD
  lcd_init();
  
  char *temp = "temp      78 C";
  char *ph = "pH      6.8pH";
  
  lcd_cmd(0x80); // select 1st line (0x80 + addr) - here addr = 0x00
  display_line(temp);
  
  lcd_cmd(0xc0); // select 2nd line (0x80 + addr) - here addr = 0x40
  display_line(ph);

  while (1);
}
