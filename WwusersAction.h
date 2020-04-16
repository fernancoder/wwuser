#ifndef WWUSERSACTION_H_
#define WWUSERSACTION_H_

#include "daework-include.h"
#include "daework-kernel.h"
#include "daework-support.h"

class WwusersAction : public Action
{
  public:
    bool verifyParam(string param, int errorId, string errorMsg);
    void logAction();
    void processSuccess(string result);
    void processError(int errorId, string errorMsg);
};

#endif /* WWUSERSACTION_H_ */
