#ifndef PBGE_CORE_PAIRCOMPARE_H_
#define PBGE_CORE_PAIRCOMPARE_H_

namespace pbge {
    template <typename T1, typename T2>
    class PairCompare {
    public:
        inline static bool less(const T1 & t11, const T1 & t12, const T2 & t21, const T2 & t22) {
            return (t11 < t12 || (t11 == t12 && t21 < t22));
        }
    };
}

#endif