/* 
 * File:   DLPF.h
 * Author: matt
 *
 * Created on 18 November 2012, 11:43
 */

#ifndef DLPF_H
#define	DLPF_H

#define NZEROS 3
#define NPOLES 3
#define GAIN   55.251875

class DLPF
{
public:
    DLPF();
    DLPF(const DLPF& orig);
    virtual ~DLPF();
    void iterate(double* tofilter, double* output);
private:
    double xv_[NZEROS+1], yv_[NPOLES+1];


};

#endif	/* DLPF_H */

