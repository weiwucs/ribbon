//
// Created by weiwu on 21-4-30.
//

#ifndef RIBBON_RIBBONELEMENTMANAGER_H
#define RIBBON_RIBBONELEMENTMANAGER_H


#include "RibbonElementCreateDelegate.h"

class RibbonElementManager {
protected:
    RibbonElementManager();
public:
    virtual ~RibbonElementManager();
    static RibbonElementManager *instance();
    RibbonElementCreateDelegate *delegate();
    void setupDelegate(RibbonElementCreateDelegate *delegate);

private:
    RibbonElementCreateDelegate *m_delegate;
};

#ifndef RibbonSubElementMgr
#define RibbonSubElementMgr		RibbonElementManager::instance()
#endif
#ifndef RibbonSubElementDelegate
#define RibbonSubElementDelegate	RibbonElementManager::instance()->delegate()
#endif
#ifndef RibbonSubElementStyleOpt
#define RibbonSubElementStyleOpt	RibbonElementManager::instance()->delegate()->getRibbonStyleOption()
#endif

#endif //RIBBON_RIBBONELEMENTMANAGER_H
