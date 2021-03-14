#ifndef  MAIN_H
#define  MAIN_H


#ifdef   ENV_ARDUINO
void line_following_robot(void);
#endif //ENV_ARDUINO


#if ENV_AVR == 1
int main(void)
{
    line_following_robot();

    return 0;
}
#endif //ENV_AVR


#endif //MAIN_H