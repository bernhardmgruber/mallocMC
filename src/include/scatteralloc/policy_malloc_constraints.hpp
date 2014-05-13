/*
  ScatterAlloc: Massively Parallel Dynamic Memory Allocation for the GPU.
  https://www.hzdr.de/crp

  Copyright 2014 Institute of Radiation Physics,
                 Helmholtz-Zentrum Dresden - Rossendorf

  Author(s):  Carlchristian Eckert - c.eckert ( at ) hzdr.de

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#pragma once

#include "creationPolicies/Scatter.hpp"
#include "distributionPolicies/XMallocSIMD.hpp"
#include <boost/mpl/assert.hpp>

namespace PolicyMalloc{

  /** The default PolicyChecker (does always succeed)
   */
  template<typename Policy1, typename Policy2>
  struct PolicyChecker{};


  /** Enforces constraints on policies or combinations of polices
   * 
   * Uses template specialization of PolicyChecker
   */
  template < 
     typename T_CreationPolicy, 
     typename T_DistributionPolicy, 
     typename T_OOMPolicy, 
     typename T_GetHeapPolicy,
     typename T_AlignmentPolicy
       >
  class PolicyConstraints{
      PolicyChecker<T_CreationPolicy, T_DistributionPolicy> c;
  };


  /** Scatter and XMallocSIMD need the same pagesize!
   *
   * This constraint ensures that if the CreationPolicy "Scatter" and the
   * DistributionPolicy "XMallocSIMD" are selected, they are configured to use
   * the same value for their "pagesize"-parameter.
   */
  template<typename x, typename y, typename z >
  struct PolicyChecker<
    typename CreationPolicies::Scatter<x,y>,
    typename DistributionPolicies::XMallocSIMD<z> 
  >{
    BOOST_MPL_ASSERT_MSG(x::pagesize::value == z::pagesize::value,
        Pagesize_must_be_the_same_when_combining_Scatter_and_XMallocSIMD, () );
  };

}//namespace PolicyMalloc
