//
// Created by weiwu on 21-4-30.
//

#include "RibbonElementManager.h"

RibbonElementManager::RibbonElementManager() : m_delegate(nullptr) {
    m_delegate = new RibbonElementCreateDelegate();
}

RibbonElementManager::~RibbonElementManager() {
    if (m_delegate) {
        delete m_delegate;
    }
    m_delegate = nullptr;
}

RibbonElementManager *RibbonElementManager::instance() {
    static RibbonElementManager s_instance;

    return (&(s_instance));
}


RibbonElementCreateDelegate *RibbonElementManager::delegate() {
    return (m_delegate);
}


void RibbonElementManager::setupDelegate(RibbonElementCreateDelegate *delegate) {
    if (m_delegate) {
        delete m_delegate;
    }
    m_delegate = delegate;
}
