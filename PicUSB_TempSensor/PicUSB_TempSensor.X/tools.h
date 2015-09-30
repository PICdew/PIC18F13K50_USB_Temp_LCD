/* 
 * File:   tools.h
 * Author: imp
 *
 * Created on September 26, 2015, 3:18 PM
 */

#ifndef TOOLS_H
#define	TOOLS_H

#ifdef	__cplusplus
extern "C" {
#endif

char* myitoa(uint16_t i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    uint16_t shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}


#ifdef	__cplusplus
}
#endif

#endif	/* TOOLS_H */

