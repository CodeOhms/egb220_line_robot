#ifndef  MAIN_H
#define  MAIN_H

#ifdef ENV_ARDUINO
void line_following_robot(void);
#endif //ENV_ARDUINO

#ifdef ENV_AVR
int main(void)
{
    line_following_robot();

    return 0;
}
#endif //ENV_AVR

#endif //MAIN_H