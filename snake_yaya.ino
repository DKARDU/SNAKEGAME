#include "U8glib.h"//Referencing U8G header files
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//Set the device name: I2C-SSD1306-128*64 (OLED)
/***********************************************/
int adc_key_val[5] ={31,100,255,451,600};

int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;
int UP = 1;
int DOWN = 2;
int LEFT = 0;
int RIGHT = 3;
int Enter = 4; 
/***********************************************/
int box_x = 2;//Define variables
int box_y = 0;//Define variables
int box_x_length = 96;//x direction 32 0-31
int box_y_length = 62;//y direction 20 0-19
int snake_max_length = 100; //Maximum body length of snake
int snake_x[100];//Snake body x coordinate
int snake_y[100];//Snake body y coordinate
int snake_body_width = 3; //Snake body width (square)
int food_x;//Food location coordinates x
int food_y;//Food location coordinates y
int snake_length = 3; //Define the initial snake body length
unsigned int game_speed;//Set game speed
/***********************************************/
void setup(void)
{
   pinMode(13, OUTPUT);  //we'll use the debug LED to output a heartbeat
   Serial.begin(9600); // 9600 bps
   welcome();//Welcome Screen
   delay(2000);
   chose_game();//Selection interface
}
/***********************************************/
void welcome()
{
   u8g.firstPage();//The first page shows
   
   do
   {
     u8g.setFont(u8g_font_gdr14r);//Coordinate function
     u8g.setPrintPos(5, 20);
     u8g.print("Snake Game");
     u8g.setPrintPos(5, 38);
     u8g.print("             v2.0");
     u8g.setFont(u8g_font_9x18);
     u8g.setPrintPos(38, 55);
     u8g.print("DKARDU");
   } while (u8g.nextPage());
}
/***********************************************/
int chose_game()//Define the selection interface
{
   int flag = 1;
   int temp = 1;
   while (flag)
   {
     int KEY = read_key();
     if (key == UP )
     {
       u8g.firstPage();
       do
       {
         u8g.setFont(u8g_font_9x18);
         u8g.setPrintPos(5, 20);
         u8g.print("Snake  <");
         u8g.setPrintPos(5, 40);
         u8g.print("Player");
       } while (u8g.nextPage());
       temp = 1;
     }
     if (key == DOWN)
     {
       u8g.firstPage();
       do
       {
         u8g.setFont(u8g_font_9x18);
         u8g.setPrintPos(5, 20);
         u8g.print("Snake");
         u8g.setPrintPos(5, 40);
         u8g.print("Player <");
       } while (u8g.nextPage());
       temp = 2;
     }
     if (key == Enter)
     {
       return temp;
     }
   }
}
/***********************************************/

int read_key()//Define keys
{
  digitalWrite(13,LOW);
  adc_key_in = analogRead(1);    // read the value from the sensor 
  
  key = get_key(adc_key_in);  // convert into key press
 
  if (key != oldkey)   // if keypress is detected
   {
    delay(10);  // wait for debounce time
    adc_key_in = analogRead(1);    // read the value from the sensor 
    key = get_key(adc_key_in);  // convert into key press
    int key_temp;
    if (key != oldkey)    
    {   
      oldkey = key;
      if (key >=0){
        digitalWrite(13,HIGH);
        int key_temp;
        if (key == 0) {
          int LEFT=adc_key_in;
         key_temp = LEFT;
         return key_temp;
         }
         if (key==1) {
         int UP=adc_key_in;
         key_temp = UP;
         return key_temp;
         }
         if (key == 2) {
         int DOWN=adc_key_in;
         key_temp = DOWN;
         return key_temp;
         }
         if (key == 3) {
         int RIGHT=adc_key_in;
         key_temp = RIGHT;
         return key_temp;
         }
         if (key == 4) {
         int Enter=adc_key_in;
         key_temp = Enter;
         return key_temp;
         }

         return 0;
      }
    }
   }
}
int get_key(unsigned int input)
{
    int k;
    for (k = 0; k < NUM_KEYS; k++)
    {
      if (input < adc_key_val[k])
     {
            return k;
        }
   }
       if (k >= NUM_KEYS)k = -1;  // No valid key pressed
       return k;
}
/***********************************************/
void game_over()//Define the end of the game
{
   u8g.firstPage();
   do
   {
     u8g.setFont(u8g_font_gdr14r);
     u8g.setPrintPos(0, 40);
     u8g.print(" GAME OVER!");
     }
      
   while (u8g.nextPage());  //
   snake_length=3;
   snake_x[0] = 15; snake_y[0] = 15;//snake starting coordinates
   snake_x[1] = snake_x[0]  - 1; snake_y[1] = snake_y[0];//snake starting coordinates
   snake_x[2] = snake_x[1]  - 1; snake_y[2] = snake_y[1];//snake starting coordinates
}
void food()//Define the physical
{
   int flag = 1;
   while (flag)
   {
     food_x = random(0,(box_x_length-2)/3);
     food_y = random(0,(box_y_length-2)/3);
     for (int i = 0; i < snake_length; i++)
     {
         if((food_x==snake_x[i])&&(food_y==snake_y[i]))
         {
           break;
         }
         flag=0;
     }
   }

}
void snake_frame(int s)
{
   u8g.firstPage();
   do
   {
     u8g.drawFrame(box_x, box_y, box_x_length, box_y_length);
     u8g.setFont(u8g_font_5x8);
     u8g.setPrintPos(box_x_length + 5, 12);
     u8g.print("Score");
     u8g.setPrintPos(box_x_length + 5, 22);
     u8g.print((snake_length - 3) * 5);
     u8g.setPrintPos(box_x_length + 5, 46);
     u8g.print("Speed");
     
     u8g.setPrintPos(box_x_length + 5, 56);
     u8g.print(30-s);
     u8g.drawFrame(food_x*snake_body_width+1, food_y*snake_body_width+1, snake_body_width, snake_body_width);//Show food
     for (int i = 0; i < snake_length; i++)//Show snake
     {
       if(i==0)
       {
         u8g.drawBox(snake_x[i]*snake_body_width+1, snake_y[i]*snake_body_width+1, snake_body_width, snake_body_width);
       }
       else
       {
         u8g.drawFrame(snake_x[i]*snake_body_width+1, snake_y[i]*snake_body_width+1, snake_body_width, snake_body_width);
       }
     }
   } while (u8g.nextPage());
}

void snake()//Define the snake parameters
{
   int flag = 1; //Sign
   snake_x[0] = 15; snake_y[0] = 15;//snake starting coordinates
   snake_x[1] = snake_x[0]  - 1; snake_y[1] = snake_y[0];//snake starting coordinates
   snake_x[2] = snake_x[1]  - 1; snake_y[2] = snake_y[1];//snake starting coordinates
   int snake_dir=RIGHT;//Initial direction right
   game_speed=30;
   int food_flag=1;
   Serial.println("snake!");
   food();
   while (flag)
   {
     snake_frame(game_speed);
     delay(game_speed);
     int KEY = read_key();
       if(key==1 && snake_dir!=DOWN )
       {
         snake_dir=UP;
       }
       if(key==2 && snake_dir!=UP )
       {
         snake_dir=DOWN;
       }
       if(key==0 && snake_dir!=RIGHT )
       {
         snake_dir=LEFT;
       }
       if(key==3 && snake_dir!=LEFT )
       {
         snake_dir=RIGHT;
       }
     if(snake_eat_food(snake_dir)==1)
     {
       food();
       if(snake_length<10){game_speed=30;}
       if(snake_length<30&&snake_length>=10){game_speed=20;}
       if(snake_length<50&&snake_length>=30){game_speed=10;}
       if(snake_length>=50){game_speed=0;}
     }
     else
     {
       if(snake_knock_wall(snake_dir==1))
       {
         game_over();
         flag=0;
       }
       else
       {
         if(snake_eat_body(snake_dir)==1)
         {
           game_over();
           flag=0;
         }
         else
         {
           change_con(snake_dir);
         }
       }
     }
   }
}


void change_con(int DIR)//Moving coordinate function
{
   int temp_x[snake_length+2];
   int temp_y[snake_length+2];
   for(int i=0;i<snake_length-1;i++)//Store the old coordinate data in the temp array
   {
     temp_x[i]=snake_x[i];
     temp_y[i]=snake_y[i];
   }
   switch(DIR)
   {
     case 3: {snake_x[0]+=1;break;}
     case 0: {snake_x[0]-=1;break;}
     case 1: {snake_y[0]-=1;break;}
     case 2: {snake_y[0]+=1;break;}
   }
   for(int i=1;i<snake_length;i++)
   {
     snake_x[i]=temp_x[i-1];
     snake_y[i]=temp_y[i-1];
   }
}

void change_pos()//Move function after eating
{
   int temp_x[snake_length+2];
   int temp_y[snake_length+2];
   for(int i=0;i<snake_length-1;i++)//Store the old coordinate data in the temp array
   {
     temp_x[i]=snake_x[i];
     temp_y[i]=snake_y[i];
   }
   snake_x[0]=food_x;
   snake_y[0]=food_y;
   for(int i=1;i<snake_length;i++)
   {
     snake_x[i]=temp_x[i-1];
     snake_y[i]=temp_y[i-1];
   }
}
//Determine whether you have eaten food 0-not eaten 1-eated 2-ineffective
int snake_eat_food(int dir)
{
   int x_temp=snake_x[0];
   int y_temp=snake_y[0];
   switch(dir)
   {
     case 1 :y_temp-=1;break;
     case 2 :y_temp+=1;break;
     case 0 :x_temp-=1;break;
     case 3 :x_temp+=1;break;
   }
   if((x_temp==food_x)&&(y_temp==food_y))
   {
       snake_length+=1;
       change_pos();
       return 1;
   }
   else
   {
     return 0;
   }
}
//Judge whether to hit the wall 0-not hit 1-hit 2-invalid
int snake_knock_wall(int dir)
{
   int x_temp=snake_x[0];
   int y_temp=snake_y[0];
   switch(dir)
   {
     case 1 :y_temp-=1;break;
     case 2 :y_temp+=1;break;
     case 0 :x_temp-=1;break;
     case 3 :x_temp+=1;break;
   }
   if(x_temp<0||x_temp>31||y_temp<0||y_temp>19)
   {
     return 1;
   }
   else
   {
     return 0;
   }
}
//Determine whether you have eaten yourself 0-not eaten 1-eated 2-ineffective
int snake_eat_body(int dir)
{
   int x_temp=snake_x[0];
   int y_temp=snake_y[0];
   switch(dir)
   {
     case 1 :y_temp-=1;break;
     case 2 :y_temp+=1;break;
     case 0 :x_temp-=1;break;
     case 3 :x_temp+=1;break;
   }
   for(int i=1;i<snake_length;i++)
   {
     if((snake_x[0]==snake_x[i])&&(snake_y[0]==snake_y[i]))//if((snake_x[0]==snake_x[i])&&(snake_y[0]==snake_y[i]))
     {
       return 1;
     }
   }
   return 0;
}
/***********************************************/
//Instructions for use (player) main function
void tetrs()
{
   int flag = 1;
   while (flag)
   {
       u8g.firstPage();
       do
       {
            u8g.setFont(u8g_font_6x10r);
            u8g.setPrintPos(1, 15);
            u8g.print("1.Press UP: Return");
            u8g.setPrintPos(1, 35);
            u8g.print("2:Press Enter: Start "); 
            u8g.setPrintPos(1, 55);
            u8g.print("the game");
            
       } while (u8g.nextPage());
       delay(1000);
       flag=0;
   }
}
/***********************************************/
void loop(void)//Main loop function
{
   while (1)
   {
     switch (chose_game())
     {
       case 1: snake(); break;
       case 2: tetrs(); break;
       default:break;
     }
   }
}
