#ifndef CARDFRAME_H
#define CARDFRAME_H

#include <QFrame>
#include <QObject>

class I_CardFrame : public QFrame
{
    Q_OBJECT
public:
    I_CardFrame();

    virtual void createInterface() = 0;
    virtual QVariant getValue() = 0;
    virtual void setValue(QVariant value) = 0;
};

#endif // CARDFRAME_H
