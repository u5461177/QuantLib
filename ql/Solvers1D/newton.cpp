
/*
 Copyright (C) 2000, 2001, 2002 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email ferdinando@ametrano.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file newton.cpp
    \brief Newton 1-D solver

    \fullpath
    ql/Solvers1D/%newton.cpp
*/

// $Id$

/* The implementation of the algorithm was inspired by
   Press, Teukolsky, Vetterling, and Flannery,
   "Numerical Recipes in C", 2nd edition, 
   Cambridge University Press
*/


#include <ql/Solvers1D/newton.hpp>
#include <ql/Solvers1D/newtonsafe.hpp>

namespace QuantLib {

    namespace Solvers1D {

        double Newton::solve_(const ObjectiveFunction& f,
                              double xAccuracy) const {
            double froot, dfroot, dx;

            froot = f(root_);
            dfroot = f.derivative(root_);
            QL_REQUIRE(dfroot != Null<double>(),
                "Newton requires function's derivative");
            evaluationNumber_++;

            while (evaluationNumber_<=maxEvaluations_) {
                dx=froot/dfroot;
                root_ -= dx;
                // jumped out of brackets, switch to NewtonSafe
                if ((xMin_-root_)*(root_-xMax_) < 0.0) {
                    NewtonSafe s;
                    s.setMaxEvaluations(maxEvaluations_-evaluationNumber_);
                    return s.solve(f, xAccuracy, root_+dx, xMin_, xMax_);
                }
                if (QL_FABS(dx) < xAccuracy)
                    return root_;
                froot = f(root_);
                dfroot = f.derivative(root_);
                evaluationNumber_++;
            }

            throw Error("Newton: maximum number of function evaluations (" +
                        IntegerFormatter::toString(maxEvaluations_) +
                        ") exceeded");

            QL_DUMMY_RETURN(0.0);
        }

    }

}
