/* 
 * File:   Control.h
 * Author: matt
 *
 * Created on 08 November 2012, 16:05
 */

#ifndef CONTROL_H
#define	CONTROL_H

class ControlClass
{
public:
    ControlClass();
    ControlClass(const ControlClass& orig);
    virtual ~ControlClass();
    
    void update();
private:

};

#endif	/* CONTROL_H */

