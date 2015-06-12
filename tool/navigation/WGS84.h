/*
 * Copyright (c) 2015, M.Naruoka (fenrir)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the naruoka.org nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __WGS84_H__
#define __WGS84_H__

#include <cmath>

/** @file
 * @brief �n�����f��WGS84
 * 
 * �n���̕������f���̂ЂƂł���WGS84�ɂ��ċL�q�����t�@�C���B
 */

/**
 * @brief �n�����f��WGS84
 * 
 * �n�����f���ł���WGS84��\������N���X�B
 * 
 * �ɗ����a����яd�͂̌v�Z���ł��܂��B
 */
template <class FloatT>
class WGS84Generic{
  public:
    static const FloatT R_e;            ///< �ԓ����a[m]
    static const FloatT F_e;            ///< �n���̝G����
    static const FloatT Omega_Earth;    ///< �n�����]���x
    static const FloatT Omega_Earth_IAU;///< �n�����]���x(IAU, GRS67)
    static const FloatT mu_Earth;       ///< �n���d�͒萔[m^3/s^2]
    static const FloatT epsilon_Earth;  ///< ���ΐS��
    static const FloatT g_WGS0;         ///< �ԓ���d��
    static const FloatT g_WGS1;         ///< �d�͌����萔

#ifndef pow2
#define pow2(x) ((x) * (x))
#else
#define ALREADY_POW2_DEFINED
#endif
    /**
     * ��k����(���Ȃ킿�o����)�̋ɗ����a�����߂܂��B
     * 
     * @param latitude �ܓx[rad]
     * @return (FloatT) ��k�����̋ɗ����a[m]
     */
    static FloatT R_meridian(const FloatT &latitude){
      return R_e * (1. - pow2(epsilon_Earth))
                  / std::pow((1. - pow2(epsilon_Earth) * pow2(std::sin(latitude))), 1.5);
    }
    
    /**
     * ���������̋ɗ����a�����߂܂��B
     * 
     * @param latitude �ܓx[rad]
     * @return (FloatT) ���������̋ɗ����a[m]
     */
    static FloatT R_normal(const FloatT &latitude){
      return R_e / std::sqrt(1. - pow2(epsilon_Earth) * pow2(std::sin(latitude)));  
    }
    
    /**
     * �d�͂����߂܂�
     * 
     * @param latitude �ܓx[rad]
     * @return �d��[m/s^2]
     */
    static FloatT gravity(const FloatT &latitude){
      return g_WGS0 * (1. + g_WGS1 * pow2(std::sin(latitude)))
                / std::sqrt(1. - pow2(epsilon_Earth) * pow2(std::sin(latitude))); 
    }
#ifdef ALREADY_POW2_DEFINED
#undef ALREADY_POW2_DEFINED
#else 
#undef pow2
#endif
};

template <class FloatT>
const FloatT WGS84Generic<FloatT>::R_e = 6378137;
template <class FloatT>
const FloatT WGS84Generic<FloatT>::F_e = (1.0 / 298.257223563);
template <class FloatT>
const FloatT WGS84Generic<FloatT>::Omega_Earth = 7.292115E-5;
template <class FloatT>
const FloatT WGS84Generic<FloatT>::Omega_Earth_IAU = 7.2921151467E-5;
template <class FloatT>
const FloatT WGS84Generic<FloatT>::mu_Earth = 3.986005E14;
template <class FloatT>
const FloatT WGS84Generic<FloatT>::epsilon_Earth = 0.0818191908426;
template <class FloatT>
const FloatT WGS84Generic<FloatT>::g_WGS0 = 9.7803267714;
template <class FloatT>
const FloatT WGS84Generic<FloatT>::g_WGS1 = 0.00193185138639;

typedef WGS84Generic<double> WGS84;

#endif /* __WGS84_H__ */
