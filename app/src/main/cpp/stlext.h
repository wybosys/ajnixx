#ifndef _AJNI_STLEXT_H
#define _AJNI_STLEXT_H

AJNI_BEGIN

template <typename IterT, typename DemT>
inline typename IterT::value_type join(IterT b, IterT e, DemT const& d) {
    typename IterT::value_type r;
    while (true) {
        r += *b++;
        if (b != e)
            r += d;
        else break;
    }
    return r;
}

AJNI_END

#endif