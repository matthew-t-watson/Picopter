/* 
 * File:   Kalman.h
 * Author: matt
 *
 * Created on 26 November 2012, 10:04
 */

#ifndef KALMAN_H
#define	KALMAN_H

class KalmanClass
{
public:
    KalmanClass();
    KalmanClass(const KalmanClass& orig);
    virtual ~KalmanClass();
    
    void predict(double* u, double* x, float* dt);
    void update(double* z, double* x);
    
private:
    double bias_;
    double P_[2][2];
    double y_;
    double S_;
    double K_[2];
    
    double Q_angle_;
    double Q_bias_;
    double R_angle_;

};

#endif	/* KALMAN_H */

