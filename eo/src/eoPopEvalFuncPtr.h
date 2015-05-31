/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

-----------------------------------------------------------------------------

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */
//-----------------------------------------------------------------------------
//

#ifndef eoPopEvalFuncPtr_H
#define eoPopEvalFuncPtr_H

#include <vector>
#include <eoEvalFunc.h>
#include <eoPop.h>

template< class EOT, class FitT = typename EOT::Fitness, class FunctionArg = const std::vector<EOT const*>& >
struct eoPopEvalFuncPtr: public eoEvalFunc<eoPop<EOT> >
{
    public:
        eoPopEvalFuncPtr( std::vector<FitT> (* _eval)( FunctionArg ) )
          : eoEvalFunc<eoPop<EOT> >(), evalFunc( _eval ) {}

        virtual void operator() ( eoPop<EOT> & _pop )
        {
            std::vector<EOT const*> workload;
            for (typename eoPop<EOT>::iterator it=_pop.begin(),itEnd=_pop.end(); it!=itEnd; ++it)
            {
                EOT& _eo = *it;
                if (_eo.invalid())
                    workload.push_back(&_eo);
            }

            const std::vector<FitT>& fitnessValues = evalFunc(workload);

            // const_cast is ok here because the pointed-to object is originally writable by us
            typename std::vector<FitT>::const_iterator fitIt = fitnessValues.begin();
            for (typename std::vector<EOT const*>::iterator it=workload.begin(),itEnd=workload.end(); it!=itEnd; ++it, ++fitIt)
                const_cast<EOT*>(*it)->fitness(*fitIt);
        }

    private:
        std::vector<FitT> (* evalFunc )( FunctionArg );
};

#endif
