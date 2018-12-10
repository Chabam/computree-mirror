#ifndef CT_COUNTER_H
#define CT_COUNTER_H

#include "ctlibstructureaddon_global.h"

#include <QString>

class CTLIBSTRUCTUREADDON_EXPORT CT_Counter
{
public:
    CT_Counter(const size_t &nTurns);

    inline void setNTurns(size_t n) {_nTurns = n;}
    inline void setCurrentTurn(size_t n) {_currentTurn = n;}
    inline void setTurnName(QString turnName) {_turnName = turnName;}

    inline size_t nTurns() const {return _nTurns;}
    inline size_t currentTurn() const {return _currentTurn;}
    inline QString turnName() const {return _turnName;}
    inline bool hasNextTurn() {return _currentTurn < _nTurns;}
    inline bool isFinished() {return _currentTurn > _nTurns;}

    inline void beginNextTurn() {++_currentTurn;}

private:
    size_t       _currentTurn;
    size_t       _nTurns;
    QString      _turnName;

};

#endif // CT_COUNTER_H
