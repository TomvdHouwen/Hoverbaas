#include "regelaar23.h"
#include <algorithm>
#include <cmath>

regelaar::regelaar(double Kp, double Kd, double Ki)
    : Kp(Kp), Kd(Kd), Ki(Ki), error_last(0.0), error_sum(0.0), basis_kracht(0.3), max_krachtverschil(0.3) {}

regelaar23::regelaar23(double Kp, double Kd, double Ki)
    : regelaar(Kp, Kd, Ki) {}

void regelaar23::step(float theta, float setpoint, float dt, float& m1_force, float& m2_force) { // theta, m1 en m2  van state, m1&m2 by reference
    double pi = 3.14159265358979323846; // TODO ergens anders definiëren
    double error_new = std::fmod((setpoint - theta + pi), (2 * pi)) - pi;
    double d_error = (error_new - this->error_last) / dt;

    float m1 = error_new * this->Kp + d_error * this->Kd + this->error_sum * this->Ki;
    float m2 = (error_new * this->Kp + d_error * this->Kd + this->error_sum * this->Ki) * -1;
    m1 = std::max(0.0, std::min(m1, static_cast<double>(this->basis_kracht + this->max_krachtverschil)));
    m2 = std::max(0.0, std::min(m2, static_cast<double>(this->basis_kracht + this->max_krachtverschil)));

    // Anti integrator-windup for m1
    if (m1 < ((this->basis_kracht + this->max_krachtverschil) / 2.0)) {
        this->error_sum += error_new * dt;
    }
    // Anti integrator-windup for m2
    if (m2 < ((this->basis_kracht + this->max_krachtverschil) / 2.0)) {
        this->error_sum += error_new * dt;
    }

    // oude anti-windup
    // if (!(m1 >= (this->basis_kracht + this->max_krachtverschil))) { // anti integrator-windup
    //     this->error_sum += error_new * dt;
    // }
    // this->error_last = error_new;

    m1_force = m1;
    m2_force = m2;
}