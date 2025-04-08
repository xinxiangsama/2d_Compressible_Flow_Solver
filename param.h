#pragma once
#include <iostream>

/*------------物理常量----------*/
static constexpr double Gamma = 1.4;
/*------------流场信息----------*/
static constexpr double Xmin = 0.0;
static constexpr double Xmax = 1.0;
static constexpr double Tref = 273;
static constexpr double Ma_ref = 1.0;
static constexpr double C = 110.5;
static constexpr double mu_ref = 1.0e-3;

/*------------网格信息----------*/
static constexpr unsigned int N1 = 400;
static constexpr unsigned int N2 = 400;

/*------------求解信息----------*/
static constexpr double T = 1.0;
static constexpr double dt = 5.0e-5;


/*-----------几何参数--------------*/
static constexpr size_t numpoints = 50;

static constexpr double cx = 0.25;
static constexpr double cy = 0.5;
static constexpr double radius = 0.05;


enum class Direction { X, Y };
