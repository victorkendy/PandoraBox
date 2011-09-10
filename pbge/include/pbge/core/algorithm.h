#ifndef PBGE_CORE_ALGORITHM_H
#define PBGE_CORE_ALGORITHM_H

#include <algorithm>

namespace pbge {
    namespace alg_internal {
        template<typename E, typename F, typename P>
        class CondicionalExec {
        public:
            inline CondicionalExec(F f, P p) : func(f), pred(p) {}
            inline void operator () (E e) {
                if(pred(e)) func(e);
            }
        private:
            F func;
            P pred;
        };
    }

    template <typename Iter, typename Fun, typename Pred>
    inline void cond_for_each(Iter begin, Iter end, Fun f, Pred p) {
        std::for_each(
            begin, 
            end, 
            alg_internal::CondicionalExec<typename Iter::value_type, Fun, Pred>(f, p));
    }
}

#endif